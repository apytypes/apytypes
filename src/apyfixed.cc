/*
 * APyFixed: Dynamic arbitrary fixed-point data type.
 */

// Python object access through Pybind
#include <pybind11/pybind11.h>
namespace py = pybind11;

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

// Standard header includes
#include <algorithm>  // std::copy, std::max, std::transform, etc...
#include <cstddef>    // std::size_t
#include <cstring>    // std::memcpy
#include <functional> // std::bit_not
#include <iostream>
#include <iterator>  // std::back_inserter
#include <optional>  // std::optional
#include <sstream>   // std::stringstream
#include <stdexcept> // std::domain_error
#include <string>    // std::string
#include <vector>    // std::vector, std::swap

#include "apyfixed.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// GMP should be included after all other includes
#include <gmp.h>

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

//! Constructor: specify size and initialize from another APyFixed number
APyFixed::APyFixed(
    const APyFixed& other,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
    : _bits {}
    , _int_bits {}
    , _data {}
{
    if (bits.has_value() || int_bits.has_value() || frac_bits.has_value()) {
        // One or more bit-specifiers set, initialize from the specifier arguments
        _set_bit_specifier_from_optional(bits, int_bits, frac_bits);
        _bit_specifier_sanitize_bits();
        _data = std::vector<mp_limb_t>(bits_to_limbs(_bits), 0);
    } else {
        // No bit-specifiers set, use bit-specifiers from `other`
        _bits = other._bits;
        _int_bits = other._int_bits;
        _data = std::vector<mp_limb_t>(bits_to_limbs(_bits), 0);
    }

    // Copy the limb data from `other`
    set_from_apyfixed(other);
}

//! Constructor: construct from a Python arbitrary long integer object
APyFixed::APyFixed(
    py::int_ python_long_int_bit_pattern,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
    : APyFixed(bits, int_bits, frac_bits)
{
    /*
     * Beware when making changes to this function: This function is not (yet) tested by
     * any C++ unit tests, as it's difficuly to construct `py::int_` or `PyLongObject`
     * objects from C++.
     *
     * Update 2024-01-17: This function is now tested in the Python test-environment
     */
    PyLongObject* py_long = (PyLongObject*)python_long_int_bit_pattern.ptr();
    long py_long_digits = PyLong_DigitCount(py_long);
    bool py_long_is_negative = PyLong_IsNegative(py_long);

    if (py_long_digits == 0) {
        // Python integer is zero
        _data[0] = 0;
    } else if (py_long_digits == 1) {
        // Python integer fits in one Python digit
        _data[0] = mp_limb_t(GET_OB_DIGIT(py_long)[0]);
        if (py_long_is_negative) {
            _data[0] = -_data[0];
            std::fill(_data.begin() + 1, _data.end(), mp_limb_t(-1));
        }
    } else {
        // Python integer is stored using multiple Python digits. Import data
        // from multi-digit Python long integer.
        mpz_t mpz_from_py_long;
        mpz_init(mpz_from_py_long);
        mpz_import(
            mpz_from_py_long,                    // Destination operand
            py_long_digits,                      // Words to read
            -1,                                  // LSWord first
            sizeof(GET_OB_DIGIT(py_long)[0]),    // Word size in bytes
            0,                                   // Machine endianness
            sizeof(GET_OB_DIGIT(py_long)[0]) * 8 // Nail bits
                - PYLONG_BITS_IN_DIGIT,          //
            GET_OB_DIGIT(py_long)                // Source operand
        );

        // Export data to limb vector
        auto limb_copy_count = std::min(_data.size(), mpz_size(mpz_from_py_long));
        std::memcpy(
            &_data[0],                        // dst
            mpz_limbs_read(mpz_from_py_long), // src
            limb_copy_count * _LIMB_SIZE_BYTES
        );

        // Negate limb vector if negative
        if (py_long_is_negative) {
            std::transform(_data.begin(), _data.end(), _data.begin(), std::bit_not {});
            increment_lsb();
        }

        // Clear resources and return
        mpz_clear(mpz_from_py_long);
    }

    // Perform two's complement overflowing
    _twos_complement_overflow();
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

//! Constructor: specify only size and zero data on construction
APyFixed::APyFixed(
    std::optional<int> bits, std::optional<int> int_bits, std::optional<int> frac_bits
)
    : _bits { 0 }
    , _int_bits { 0 }
    , _data {}
{
    _set_bit_specifier_from_optional(bits, int_bits, frac_bits);
    _bit_specifier_sanitize_bits();
    _data = std::vector<mp_limb_t>(bits_to_limbs(_bits), 0);
}

//! Constructor: specify only size and zero data on construction
APyFixed::APyFixed(int bits, int int_bits)
    : _bits { bits }
    , _int_bits { int_bits }
    , _data(bits_to_limbs(bits))
{
    _bit_specifier_sanitize_bits();
}

//! Constructor: specify size and initialize from a `double`
APyFixed::APyFixed(double value, int bits, int int_bits)
    : APyFixed(bits, int_bits)
{
    set_from_double(value);
}

//! Constructor: specify size and initialize from string
APyFixed::APyFixed(int bits, int int_bits, const char* str, int base)
    : APyFixed(bits, int_bits)
{
    switch (base) {
    case 8:
        set_from_string_oct(str);
        break;
    case 10:
        set_from_string_dec(str);
        break;
    case 16:
        set_from_string_hex(str);
        break;
    default:
        throw std::domain_error("Unsupported numeric base. Valid bases are: 8, 10, 16");
    }
}

//! Underlying vector iterator-based constructor
template <typename _ITER>
APyFixed::APyFixed(int bits, int int_bits, _ITER begin, _ITER end)
    : _bits { bits }
    , _int_bits { int_bits }
    , _data(begin, end)
{
    _bit_specifier_sanitize_bits();

    if (std::distance(begin, end) <= 0) {
        throw std::domain_error(
            "APyInt vector initialization needs propriate vector size"
        );
    } else if (std::size_t(std::distance(begin, end)) != bits_to_limbs(bits)) {
        throw std::domain_error(
            "APyInt vector initialization needs propriate vector size"
        );
    }

    // Two's-complements overflow bits outside of the range
    _twos_complement_overflow();
}

//! Construction from std::vector<mp_limb_t>
APyFixed::APyFixed(int bits, int int_bits, const std::vector<mp_limb_t>& vec)
    : APyFixed(bits, int_bits, vec.begin(), vec.end())
{
}

//! Construction from std::vector<mp_limb_signed_t>
APyFixed::APyFixed(int bits, int int_bits, const std::vector<mp_limb_signed_t>& vec)
    : APyFixed(bits, int_bits, vec.begin(), vec.end())
{
}

/* ********************************************************************************** *
 * *                            Arithmetic member functions                         * *
 * ********************************************************************************** */

APyFixed APyFixed::operator+(const APyFixed& rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    APyFixed result(res_int_bits + res_frac_bits, res_int_bits);
    std::vector<mp_limb_t> operand_shifted;

    if (frac_bits() < rhs.frac_bits()) {
        // Right-hand side (rhs) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, rhs, *this);
    } else { // frac_bits() >= rhs.frac_bits()
        // Left-hand side (*this) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, *this, rhs);
    }

    // Add with carry and return
    mpn_add_n(
        &result._data[0],    // dst
        &result._data[0],    // src1
        &operand_shifted[0], // src2
        result.vector_size() // limb vector length
    );
    return result;
}

APyFixed APyFixed::operator-(const APyFixed& rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    APyFixed result(res_int_bits + res_frac_bits, res_int_bits);
    std::vector<mp_limb_t> operand_shifted;

    bool swap_operand;
    if (frac_bits() < rhs.frac_bits()) {
        // Right-hand side (rhs) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, rhs, *this);
        swap_operand = false;
    } else { // frac_bits() >= rhs.frac_bits()
        // Left-hand side (*this) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, *this, rhs);
        swap_operand = true;
    }

    // Add with carry and return
    mpn_sub_n(
        &result._data[0],                                      // dst
        swap_operand ? &result._data[0] : &operand_shifted[0], // src1
        swap_operand ? &operand_shifted[0] : &result._data[0], // src2
        result.vector_size()                                   // limb vector length
    );
    return result;
}

APyFixed APyFixed::operator*(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    std::vector<mp_limb_t> abs_operand1 = _unsigned_abs();
    std::vector<mp_limb_t> abs_operand2 = rhs._unsigned_abs();
    APyFixed result(res_int_bits + res_frac_bits, res_int_bits);
    bool sign_product = is_negative() ^ rhs.is_negative();

    // `mpn_mul` requires the limb vector length of the first operand to be
    // greater than, or equally long as, the limb vector length of the second
    // operand. Simply swap the operands (essentially a free operation in C++)
    // if this is not the case.
    if (abs_operand1.size() < abs_operand2.size()) {
        std::swap(abs_operand1, abs_operand2);
    }

    mpn_mul(
        &result._data[0],    // dst
        &abs_operand1[0],    // src1
        abs_operand1.size(), // src1 limb vector length
        &abs_operand2[0],    // src2
        abs_operand2.size()  // src2 limb vector length
    );

    // Handle sign
    if (sign_product) {
        result._data = result._non_extending_negate();
    }
    return result;
}

APyFixed APyFixed::operator/(const APyFixed& rhs) const
{
    const int res_bits = bits() + std::max(rhs.bits() - rhs.int_bits(), 0) + 1;
    const int res_int_bits = int_bits() + rhs.bits() - rhs.int_bits() + 1;
    APyFixed result(res_bits, res_int_bits);

    // Absolute value numerator and denominator
    bool sign_product = is_negative() ^ rhs.is_negative();
    std::vector<mp_limb_t> abs_den = rhs._unsigned_abs();
    std::vector<mp_limb_t> abs_num;
    if (is_negative()) {
        abs_num = (-*this)._data_asl(rhs.frac_bits());
    } else {
        abs_num = (*this)._data_asl(rhs.frac_bits());
    }

    // `mpn_tdiv_rq` requires that the number of significant limbs in the
    // numerator is greater than or equal to that of the denominator.
    std::size_t num_significant_limbs = significant_limbs(abs_num);
    std::size_t den_significant_limbs = significant_limbs(abs_den);
    if (num_significant_limbs < den_significant_limbs) {
        // Early return zero
        return result;
    } else {
        mpn_tdiv_qr(
            &result._data[0],      // Quotient
            &abs_num[0],           // Remainder (unused -- discarded into numerator)
            0,                     // QXn (must be zero according to documentation)
            &abs_num[0],           // Numerator
            num_significant_limbs, // Numerator significant limbs
            &abs_den[0],           // Denominator
            den_significant_limbs  // Denominator significant limbs
        );
        if (sign_product) {
            result._data = result._non_extending_negate();
        }
        return result;
    }
}

APyFixed APyFixed::operator<<(int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixed result = *this;
    result._int_bits += shift_val;
    return result;
}

APyFixed APyFixed::operator>>(int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixed result = *this;
    result._int_bits -= shift_val;
    return result;
}

bool APyFixed::operator==(const APyFixed& rhs) const { return (*this - rhs).is_zero(); }

bool APyFixed::operator!=(const APyFixed& rhs) const { return !(*this == rhs); }

bool APyFixed::operator<(const APyFixed& rhs) const
{
    return (*this - rhs).is_negative();
}

bool APyFixed::operator<=(const APyFixed& rhs) const
{
    return (*this < rhs) || (*this == rhs);
}

bool APyFixed::operator>(const APyFixed& rhs) const
{
    return (rhs - *this).is_negative();
}

bool APyFixed::operator>=(const APyFixed& rhs) const
{
    return (*this > rhs) || (*this == rhs);
}

APyFixed APyFixed::operator-() const
{
    // Invert all bits of *this, possibly append sign to new limb, and increment lsb
    APyFixed result(_bits + 1, _int_bits + 1);
    std::transform(_data.cbegin(), _data.cend(), result._data.begin(), std::bit_not {});
    if (result.vector_size() > vector_size()) {
        mp_limb_t sign = _data.back() & (mp_limb_t(1) << (_LIMB_SIZE_BITS - 1));
        result._data.back() = sign ? 0 : mp_limb_signed_t(-1);
    }
    result.increment_lsb();
    return result;
}

APyFixed APyFixed::abs() const
{
    if (is_negative()) {
        return -*this;
    } else {
        return *this;
    }
}

/* ********************************************************************************** *
 * *                           Public member functions                              * *
 * ********************************************************************************** */

bool APyFixed::is_negative() const noexcept
{
    return mp_limb_signed_t(_data.back()) < 0;
}

bool APyFixed::is_zero() const noexcept
{
    auto limb_non_zero = [](mp_limb_t limb) { return limb != 0; };
    return std::find_if(_data.begin(), _data.end(), limb_non_zero) == _data.end();
}

// Increment the LSB without making the fixed-point number wider. Returns carry out
mp_limb_t APyFixed::increment_lsb() noexcept
{
    return mpn_add_1(
        &_data[0],     // dst
        &_data[0],     // src1
        vector_size(), // limb vector length
        1              // src2
    );
}

void APyFixed::from_vector(const std::vector<mp_limb_t>& new_vector)
{
    if (new_vector.size() != this->vector_size()) {
        throw std::domain_error("Vector size miss-match");
    }
    _data = std::vector<mp_limb_t>(new_vector.begin(), new_vector.end());
    _twos_complement_overflow();
}

std::string APyFixed::to_string_dec() const
{
    // Construct a string from the absolute value of number, and conditionally append a
    // minus sign to the string if negative
    APyFixed abs_val(is_negative() ? -(*this) : *this, _bits + 1, _int_bits + 1);

    // Convert this number to BCD with the double-dabble algorithm
    std::vector<mp_limb_t> bcd_limb_list = double_dabble(abs_val._data);
    std::size_t bcd_limb_list_start_size = bcd_limb_list.size();

    // Divide BCD limb list by two, one time per fractional bit (if any)
    long decimal_point = 0;
    for (int i = 0; i < frac_bits(); i++) {
        bcd_limb_vec_div2(bcd_limb_list);
        decimal_point += bcd_limb_list.size() > bcd_limb_list_start_size ? 1 : 0;
    }
    long rjust = ((_LIMB_SIZE_BITS / 4) - decimal_point) % (_LIMB_SIZE_BITS / 4);

    // Multiply BCD list by two, one time per for each missing fractional bit (if any)
    for (int i = 0; i < -frac_bits(); i++) {
        bcd_limb_vec_mul2(bcd_limb_list);
    }

    // Convert BCD limb list to regular BCD list (`std::vector<uint8_t>`)
    auto bcd_list = to_nibble_list(bcd_limb_list, decimal_point + rjust + 1);

    // Convert BCDs to ASCII
    std::string result = is_negative() ? "-" : "";
    for (long i = bcd_list.size() - 1; i >= rjust; i--) {
        result.push_back(bcd_list[i] + 0x30);
        if (decimal_point && i == rjust + long(decimal_point)) {
            result.push_back('.');
        }
    }

    return result;
}

std::string APyFixed::to_string_hex() const { throw NotImplementedException(); }

std::string APyFixed::to_string_oct() const { throw NotImplementedException(); }

std::string APyFixed::to_string(int base) const
{
    switch (base) {
    case 8:
        return to_string_oct();
        break;
    case 16:
        return to_string_hex();
        break;
    case 10:
        return to_string_dec();
        break;
    default:
        throw NotImplementedException();
        break;
    }
}

void APyFixed::set_from_string_dec(const std::string& str)
{
    // Trim the string from leading and trailing whitespace
    std::string str_trimmed = string_trim_whitespace(str);

    // Check the validity as a decimal string
    if (!is_valid_decimal_numeric_string(str_trimmed)) {
        throw std::domain_error("Not a valid decimal numeric string");
    }

    // Test if negative. If so, remove the negative sign from the string.
    // `is_valid_decimal_numeric_string()` makes sure that str_trimmed[0] is valid.
    bool is_negative = str_trimmed.front() == '-';
    if (is_negative) {
        str_trimmed.erase(0, 1);
    }

    // Trim leading and trailing zeros that don't affect the numeric value of the
    // decimal number
    str_trimmed = string_trim_zeros(str_trimmed);

    // Find the binary point of the trimmed string and remove it from the string
    std::size_t binary_point_dec = str_trimmed.find('.');
    binary_point_dec = (binary_point_dec == std::string::npos) ? 0 : binary_point_dec;
    str_trimmed.erase(
        std::remove(str_trimmed.begin(), str_trimmed.end(), '.'), str_trimmed.cend()
    );

    // Copy characters (from back) of the trimmed string into a BCD list
    std::vector<uint8_t> bcd_list;
    std::for_each(str_trimmed.crbegin(), str_trimmed.crend(), [&](char c) {
        bcd_list.push_back(c - 0x30);
    });

    // Multiply BCD number by 2^(frac_bits() + 1) (extra bit for rounding)
    auto bcd_list_size_prev = bcd_list.size();
    for (int i = 0; i < frac_bits() + 1; i++) {
        bcd_mul2(bcd_list);
    }

    // Remove elements after decimal dot
    if (binary_point_dec) {
        bcd_list.erase(
            bcd_list.begin(), bcd_list.begin() + bcd_list_size_prev - binary_point_dec
        );
    }

    // Reverse double-dabble algorithm (BCD -> binary)
    std::vector<mp_limb_t> data = reverse_double_dabble(bcd_list);

    // Round the data
    mpn_add_1(
        &data[0],    // dst
        &data[0],    // src1
        data.size(), // limb vector length
        1            // src2
    );
    mpn_rshift(
        &data[0],    // dst
        &data[0],    // src
        data.size(), // limb vector length
        1            // shift amount
    );

    // Adjust limb vector if negative fractional bits are present
    if (frac_bits() + 1 < 0) {
        limb_vector_asr(data, -(frac_bits() + 1));
    }

    // Copy the data into the result vector
    _data = data;
    _data.resize(bits_to_limbs(bits()));
    if (is_negative) {
        _data = _non_extending_negate();
    }

    // Two's complement overflow and we're done
    _twos_complement_overflow();
}

void APyFixed::set_from_string_hex(const std::string& str)
{
    (void)str;
    throw NotImplementedException();
}

void APyFixed::set_from_string_oct(const std::string& str)
{
    (void)str;
    throw NotImplementedException("Not implemented: APyFixed::from_string_oct()");
}

void APyFixed::set_from_string(const std::string& str, int base)
{
    switch (base) {
    case 8:
        set_from_string_oct(str);
        break;
    case 10:
        set_from_string_dec(str);
        break;
    case 16:
        set_from_string_hex(str);
        break;
    default:
        throw NotImplementedException();
        break;
    }
}

void APyFixed::set_from_double(double value)
{
    if constexpr (_LIMB_SIZE_BITS == 64) {
        mp_limb_signed_t exp = exp_of_double(value);
        mp_limb_t man = man_of_double(value);

        // Append mantissa hidden one
        if (exp) {
            man |= mp_limb_t(1) << 52;
        }
        _data[0] = man;

        // Adjust the actual exponent
        exp -= 1023;

        // Shift the data into its correct position
        auto left_shift_amnt = exp + frac_bits() - 52;
        if (left_shift_amnt >= 0) {
            limb_vector_lsl(_data, left_shift_amnt);
        } else {
            if (-left_shift_amnt - 1 < 64) {
                // Round the value
                _data[0] += mp_limb_t(1) << (-left_shift_amnt - 1);
            }
            limb_vector_lsr(_data, -left_shift_amnt);
        }

        // Adjust result from sign
        if (sign_of_double(value)) {
            _data = _non_extending_negate();
        }
        _twos_complement_overflow();
    } else {
        throw NotImplementedException(
            "Not implemented: APyFixed::set_from_double() for 32-bit systems"
        );
    }
}

double APyFixed::to_double() const
{
    if constexpr (_LIMB_SIZE_BITS == 64) {
        // Early exit if zero
        if (is_zero()) {
            return 0.0;
        }

        mp_limb_t man {};
        mp_limb_signed_t exp {};
        bool sign = is_negative();

        std::vector<mp_limb_t> man_vec = _unsigned_abs();
        unsigned man_leading_zeros = limb_vector_leading_zeros(man_vec);

        // Shift the mantissa into position and set the mantissa and exponent part
        int left_shift_amnt = 53 - _LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
        if (left_shift_amnt > 0) {
            limb_vector_lsl(man_vec, left_shift_amnt);
        } else {
            limb_vector_lsr(man_vec, -left_shift_amnt);
        }
        man = man_vec[0];
        exp = 52 - left_shift_amnt - frac_bits();
        exp += 1023;

        // Return the result
        double result {};
        set_sign_of_double(result, sign);
        set_exp_of_double(result, exp);
        set_man_of_double(result, man);
        return result;
    } else {
        // Not implemented for 32-bit system yet...
        throw NotImplementedException(
            "Not implemented: APyFixed::to_double() for 32-bit systems"
        );
    }
}

void APyFixed::set_from_apyfixed(const APyFixed& other)
{
    // Copy data from `other` limb vector shift binary point into position
    std::vector<mp_limb_t> other_data_copy { other._data };
    if (frac_bits() <= other.frac_bits()) {
        limb_vector_asr(other_data_copy, other.frac_bits() - frac_bits());
    } else {
        limb_vector_lsl(other_data_copy, frac_bits() - other.frac_bits());
    }

    // Copy binary point-adjusted data
    if (vector_size() <= other_data_copy.size()) {
        std::copy(
            other_data_copy.cbegin(),
            other_data_copy.cbegin() + vector_size(),
            _data.begin()
        );
    } else {
        std::copy(other_data_copy.cbegin(), other_data_copy.cend(), _data.begin());
        std::fill(
            _data.begin() + other_data_copy.size(),
            _data.end(),
            other.is_negative() ? -1 : 0
        );
    }

    _twos_complement_overflow();
}

py::int_ APyFixed::bit_pattern_to_int(bool allow_negative_return_value) const
{
    // Required conversion information
    bool sign = allow_negative_return_value ? is_negative() : false;
    std::vector<mp_limb_t> limb_vec
        = allow_negative_return_value ? _unsigned_abs() : _data;

    // Make sure to zero bits outside of fixed-point range if printing as non-negative
    if (!allow_negative_return_value && (bits() % _LIMB_SIZE_BITS)) {
        limb_vec.back() &= (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
    }

    // Number of significant bits in the limb vector
    std::size_t significant_bits
        = _LIMB_SIZE_BITS * limb_vec.size() - limb_vector_leading_zeros(limb_vec);

    // Number of resulting limbs (Python nomencalture: `digits`) in the Python long
    // integer result
    std::size_t python_digits
        = (significant_bits + PYLONG_BITS_IN_DIGIT - 1) / PYLONG_BITS_IN_DIGIT;

    // Intermediate GMP `mpz` variable for import and export
    mpz_t mpz_to_py_long;
    mpz_init(mpz_to_py_long);
    mpz_import(
        mpz_to_py_long,    // Destination operand
        vector_size(),     // Words to read
        -1,                // LSWord first
        sizeof(mp_limb_t), // Word size in bytes
        0,                 // Machine endianness
        0,                 // Number of nail bits
        &limb_vec[0]       // Source operand
    );

    PyLongObject* result = PyLong_New(python_digits);
    if (!result) {
        throw std::runtime_error("Could not allocate memory for Python long integer");
    }

    // Export the intermediate data to the python integer
    std::size_t words_written = 0;
    mpz_export(
        &GET_OB_DIGIT(result)[0],           // Destination operand
        &words_written,                     // Number of words written
        -1,                                 // LSWord first
        sizeof(GET_OB_DIGIT(result)[0]),    // Word size in bytes
        0,                                  // Machine endianness
        sizeof(GET_OB_DIGIT(result)[0]) * 8 // Nail bits
            - PYLONG_BITS_IN_DIGIT,         //
        mpz_to_py_long                      // Source operand
    );
    if (!words_written) {
        GET_OB_DIGIT(result)[0] = 0;
    }

    // Clear the GMP `mpz` intermediate and finalize the Python long integer
    mpz_clear(mpz_to_py_long);
    while (python_digits > 0 && (GET_OB_DIGIT(result)[python_digits - 1] == 0)) {
        python_digits--;
    }
    PyLong_SetSignAndDigitCount(result, sign, python_digits);

    // Do a PyBind11 steal of the object and return
    return py::reinterpret_steal<py::int_>((PyObject*)result);
}

std::string APyFixed::bit_pattern_to_string_dec() const
{
    std::stringstream ss {};

    std::vector<mp_limb_t> data = _data;
    if (bits() % _LIMB_SIZE_BITS) {
        mp_limb_t and_mask = (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
        data.back() &= and_mask;
    }

    // Double-dabble for binary-to-BCD conversion
    std::vector<mp_limb_t> v = double_dabble(data);

    // Setup hex printing which will properly display the BCD characters
    ss << std::hex;

    // The remaining limbs can be printed normally
    for (auto limb_it = v.crbegin()++; limb_it != v.crend(); ++limb_it) {
        ss << *limb_it;
    }
    return ss.str();
}

std::string APyFixed::repr() const
{
    std::stringstream ss {};
    ss << "APyFixed(";
    ss << bit_pattern_to_string_dec() << ", ";
    ss << "bits=" << bits() << ", ";
    ss << "int_bits=" << int_bits() << ")";
    return ss.str();
}

APyFixed APyFixed::resize(
    std::optional<int> bits,
    std::optional<int> int_bits,
    APyFixedRoundingMode rounding_mode,
    APyFixedOverflowMode overflow_mode,
    std::optional<int> frac_bits
) const
{
    APyFixed result { *this };
    int old_bits = result._bits;
    int old_int_bits = result._int_bits;
    result._set_bit_specifier_from_optional(bits, int_bits, frac_bits);
    result._bit_specifier_sanitize_bits();
    if (bits_to_limbs(result._bits) > vector_size()) {
        std::fill_n( // Vector sign-extend the result limb vector
            std::back_inserter(result._data),
            bits_to_limbs(result._bits) - vector_size(),
            is_negative() ? mp_limb_t(-1) : 0
        );
    }

    // First perform the rounding
    result._round(rounding_mode, old_bits, old_int_bits);

    // And than handle possible overflowing
    result._overflow(overflow_mode);

    return result;
}

/* ********************************************************************************** *
 * *                           Static member functions                              * *
 * ********************************************************************************** */

APyFixed APyFixed::from_double(
    double value,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
{
    APyFixed result(0, bits, int_bits, frac_bits);
    result.set_from_double(value);
    return result;
}

APyFixed APyFixed::from_string(
    std::string string_value,
    int base,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
{
    APyFixed result(0, bits, int_bits, frac_bits);
    result.set_from_string(string_value, base);
    return result;
}

/* ********************************************************************************** *
 * *                          Private member functions                              * *
 * ********************************************************************************** */

// Perform rounding of fixed-point numbers
void APyFixed::_round(
    APyFixedRoundingMode rounding_mode, int old_bits, int old_int_bits
)
{
    switch (rounding_mode) {
    case APyFixedRoundingMode::TRN:
        _round_trn(old_bits, old_int_bits); // Truncation
        break;
    case APyFixedRoundingMode::RND:
        _round_rnd(old_bits, old_int_bits); // Rounding, ties to plus infinity
        break;
    case APyFixedRoundingMode::RND_TO_ZERO:
        throw NotImplementedException("Rounding: RND_TO_ZERO not implemented yet");
    case APyFixedRoundingMode::RND_AWAY_ZERO:
        throw NotImplementedException("Rounding: RND_AWAY_ZERO not implemented yet");
    case APyFixedRoundingMode::RND_CONV:
        throw NotImplementedException("Rounding: RND_CONV not implemented yet");
    case APyFixedRoundingMode::RND_CONV_ODD:
        throw NotImplementedException("Rounding: RND_CONV_ODD not implemented yet");
    default:
        throw std::domain_error("APyFixed::_round(): unregistered rounding mode");
    }
}

// Perform overflowing of fixed-point numbers
void APyFixed::_overflow(APyFixedOverflowMode overflow_mode)
{
    switch (overflow_mode) {
    case APyFixedOverflowMode::OVERFLOW:
        _twos_complement_overflow();
        break;
    case APyFixedOverflowMode::SATURATE:
        throw NotImplementedException("Overflow: SATURATE not implemented yet");
    default:
        throw std::domain_error("APyFixed::_overflow(): unregistered rounding mode");
    }
}

// Truncation rounding
void APyFixed::_round_trn(int old_bits, int old_int_bits)
{
    int old_frac_bits = old_bits - old_int_bits;
    if (old_frac_bits <= frac_bits()) {
        limb_vector_lsl(_data, frac_bits() - old_frac_bits);
    } else { // frac_bits() < old_frac_bits
        limb_vector_asr(_data, old_frac_bits - frac_bits());
    }
}

// Round towards plus infinity
void APyFixed::_round_rnd(int old_bits, int old_int_bits)
{
    int old_frac_bits = old_bits - old_int_bits;
    if (old_frac_bits <= frac_bits()) {
        limb_vector_lsl(_data, frac_bits() - old_frac_bits);
    } else { // frac_bits() < old_frac_bits
        limb_vector_add_pow2(_data, old_frac_bits - frac_bits() - 1);
        limb_vector_asr(_data, old_frac_bits - frac_bits());
    }
}

// Perform two's complement overflowing
void APyFixed::_twos_complement_overflow() noexcept
{
    unsigned bits_last_word = _bits & (_LIMB_SIZE_BITS - 1);
    if (bits_last_word) {
        // Exploit signed arithmetic right-shift to perform two's complement overflow
        unsigned shft_amnt = _LIMB_SIZE_BITS - bits_last_word;
        _data.back() = mp_limb_signed_t(_data.back() << (shft_amnt)) >> (shft_amnt);
    }
}

// Sanitize the _bits and _int_bits parameters
void APyFixed::_set_bit_specifier_from_optional(
    std::optional<int> bits, std::optional<int> int_bits, std::optional<int> frac_bits
)
{
    int num_bit_spec = bits.has_value() + int_bits.has_value() + frac_bits.has_value();
    if (num_bit_spec != 2) {
        throw std::domain_error(
            "APyInt needs exactly two of three bit specifiers (bits, int_bits, "
            "frac_bits) set when specifying bits."
        );
    }

    // Set the internal `_bits` and `_int_bits` fields from two out of the three bit
    // specifier fields
    if (bits.has_value()) {
        if (int_bits.has_value()) {
            _bits = *bits;
            _int_bits = *int_bits;
            return;
        } else {
            // `bits` set and `int_bits` unset so `frac_bits` is set
            _bits = *bits;
            _int_bits = *bits - *frac_bits;
        }
    } else {
        // `bits` unset, so `int_bits` and `frac_bits` is set
        _bits = *int_bits + *frac_bits;
        _int_bits = *int_bits;
    }
}

// Sanitize the _bits and _int_bits parameters
void APyFixed::_bit_specifier_sanitize_bits() const
{
    if (_bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of at-least 1 bit"
        );
    }
}

// Sign preserving automatic size extending arithmetic left shift. Returns a new
// limb vector with the shifted content.
std::vector<mp_limb_t> APyFixed::_data_asl(unsigned shift_val) const
{
    if (shift_val == 0) {
        return _data;
    }

    // Perform the left-shift
    unsigned vec_skip_val = shift_val / _LIMB_SIZE_BITS;
    unsigned bit_shift_val = shift_val % _LIMB_SIZE_BITS;
    std::vector<mp_limb_t> result(bits_to_limbs(bits() + shift_val), 0);
    std::copy(_data.cbegin(), _data.cend(), result.begin() + vec_skip_val);
    mpn_lshift(
        &result[0],    // dst
        &result[0],    // src
        result.size(), // limb vector length
        bit_shift_val  // shift amount
    );

    // Append sign bits for "arithmetic" shift
    if ((bits() + shift_val) % _LIMB_SIZE_BITS > 0) {
        // Arithmetic right-shift
        mp_limb_t sign_int = mp_limb_signed_t(_data.back()) >> (_LIMB_SIZE_BITS - 1);
        mp_limb_t or_mask
            = ~((mp_limb_t(1) << (bits() + shift_val) % _LIMB_SIZE_BITS) - 1);
        result.back() |= or_mask & sign_int;
    }

    return result;
}

// Prepare for binary arithmetic by aligning the binary points of two limb vectors. The
// limbs of the first operand (`operand1`) are copied into the limb vector of `result`
// and vector sign extended. The limbs of the second operand (`operand2`) are
// vector-shifted by `operand1.frac_bits()` - `operand2.frac_bits()` bits to the left
// and copied into the `operand_shifted` limb vector. Assumptions when calling this
// method:
//   * `result` is already initialized with a propriate vector limb size
//   * `operand_shifted` is *not* initialized, i.e., it's an empty (zero element) vector
//   * the number of fractional bits in `operand1` is greater than that of `operand2`
void APyFixed::_normalize_binary_points(
    APyFixed& result,
    std::vector<mp_limb_t>& operand_shifted,
    const APyFixed& operand1,
    const APyFixed& operand2
) const
{
    std::copy(operand1._data.cbegin(), operand1._data.cend(), result._data.begin());
    operand_shifted = operand2._data_asl(operand1.frac_bits() - operand2.frac_bits());

    // Vector sign-extend the `result` limb vector
    mp_limb_t s_ext = mp_limb_signed_t(operand1._data.back()) >> (_LIMB_SIZE_BITS - 1);
    std::fill(result._data.begin() + operand1.vector_size(), result._data.end(), s_ext);

    // Vector sign-extend the `operand_shifted` limb vector
    std::fill_n(
        std::back_inserter(operand_shifted),
        result.vector_size() - operand_shifted.size(),
        mp_limb_signed_t(operand_shifted.back()) >> (_LIMB_SIZE_BITS - 1)
    );
}

// Retrieve a limb vector with the negated value from this APyFixed type. This member
// function does not extend the size of the result, unlike `APyFixed::operator-()`, that
// extends the result with one bit.
std::vector<mp_limb_t> APyFixed::_non_extending_negate() const
{
    // Invert all bits and increment the lsb
    APyFixed result(bits(), 0);
    std::transform(_data.begin(), _data.end(), result._data.begin(), std::bit_not {});
    result.increment_lsb();
    return result._data;
}

// Get the absolute value of the number in the limb vector. This method does not extend
// the resulting limb vector to make place for an additional bit. Instead, it relies on
// the user knowing that the number in the vector is now unsigned.
std::vector<mp_limb_t> APyFixed::_unsigned_abs() const
{
    return is_negative() ? _non_extending_negate() : _data;
}
