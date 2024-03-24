/*
 * APyFixed: Dynamic arbitrary fixed-point data type.
 */

// Python object access through Pybind
#include <nanobind/nanobind.h>
namespace nb = nanobind;

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

// Standard header includes
#include <algorithm>  // std::copy, std::max, std::transform, etc...
#include <cmath>      // std::isinf, std::isnan
#include <cstddef>    // std::size_t
#include <cstring>    // std::memcpy
#include <functional> // std::bit_not
#include <iterator>   // std::back_inserter
#include <optional>   // std::optional
#include <sstream>    // std::stringstream
#include <string>     // std::string
#include <vector>     // std::vector, std::swap

#include <fmt/format.h>

#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

//! Constructor: specify size and initialize from another APyFixed number
APyFixed::APyFixed(const APyFixed& other)
    : _bits { other._bits }
    , _int_bits { other._int_bits }
    , _data(other._data.begin(), other._data.end())
{
}

//! Constructor: construct from a Python arbitrary long integer object
APyFixed::APyFixed(
    nb::int_ python_long_int_bit_pattern,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
    : APyFixed(bits, int_bits, frac_bits)
{
    _data = python_long_to_limb_vec(python_long_int_bit_pattern, _data.size());
    _twos_complement_overflow(_data.begin(), _data.end(), _bits, _int_bits);
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

//! Constructor: specify only size and zero data on construction
APyFixed::APyFixed(
    std::optional<int> bits, std::optional<int> int_bits, std::optional<int> frac_bits
)
    : _bits { bits_from_optional(bits, int_bits, frac_bits) }
    , _int_bits { int_bits_from_optional(bits, int_bits, frac_bits) }
    , _data(bits_to_limbs(_bits), 0)
{
}

//! Constructor: specify only size and zero data on construction
APyFixed::APyFixed(int bits, int int_bits)
    : _bits { bits }
    , _int_bits { int_bits }
    , _data(bits_to_limbs(_bits), 0)
{
}

//! Underlying vector iterator-based constructor
template <typename _IT>
APyFixed::APyFixed(int bits, int int_bits, _IT begin, _IT end)
    : APyFixed(bits, int_bits)
{
    if (std::distance(begin, end) <= 0) {
        throw nb::value_error(
            "APyFixed::APyFixed(int bits, int int_bits, _IT begin, _IT end): "
            "`end` iterator points before `begin` iterator"
        );
    } else if (std::size_t(std::distance(begin, end)) > bits_to_limbs(bits)) {
        throw nb::value_error(
            fmt::format(
                "APyFixed::APyFixed(int bits, int int_bits, _IT begin, _IT end): "
                "[std::distance(`begin`, `end`)=={}] > [bits_to_limbs(`bits`)=={}], "
                "[bits={}], [int_bits={}]",
                std::distance(begin, end),
                bits_to_limbs(bits),
                bits,
                int_bits
            )
                .c_str()
        );
    }

    // Copy data into resulting vector
    std::copy(begin, end, _data.begin());

    // Sign-extend if necessary
    if (std::size_t(std::distance(begin, end)) < bits_to_limbs(bits)) {
        mp_limb_t sign_limb = mp_limb_signed_t(*--end) < 0 ? -1 : 0;
        std::fill_n(
            _data.begin() + std::distance(begin, end),
            bits_to_limbs(bits) - std::distance(begin, end),
            sign_limb
        );
    }

    // Two's-complements overflow bits outside of the range
    _twos_complement_overflow(_data.begin(), _data.end(), _bits, _int_bits);
}

//! Construction from std::vector<mp_limb_t>
APyFixed::APyFixed(int bits, int int_bits, const std::vector<mp_limb_t>& vec)
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
    const int res_bits = res_int_bits + res_frac_bits;

    APyFixed result(res_bits, res_int_bits);
    auto left_shift_amount = unsigned(res_frac_bits - frac_bits());
    auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());

    _cast_correct_wl(result._data.begin(), result._data.end(), left_shift_amount);

    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native addition
        mp_limb_t operand = rhs._data[0];
        operand <<= rhs_shift_amount;
        result._data[0] += operand;
    } else {
        // Result bits is more than one limb. Add with carry
        APyFixed operand(res_bits, res_int_bits);
        rhs._cast_correct_wl(
            operand._data.begin(), operand._data.end(), rhs_shift_amount
        );
        mpn_add_n(
            &result._data[0],    // dst
            &result._data[0],    // src1
            &operand._data[0],   // src2
            result.vector_size() // limb vector length
        );
    }

    return result;
}

APyFixed APyFixed::operator-(const APyFixed& rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    APyFixed result(res_bits, res_int_bits);
    auto left_shift_amount = unsigned(res_frac_bits - frac_bits());
    auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());

    _cast_correct_wl(result._data.begin(), result._data.end(), left_shift_amount);

    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native subtraction
        mp_limb_t operand = rhs._data[0];
        operand <<= rhs_shift_amount;
        result._data[0] -= operand;
    } else {
        // Result bits is more than one limb. Add with carry
        APyFixed operand(res_bits, res_int_bits);
        rhs._cast_correct_wl(
            operand._data.begin(), operand._data.end(), rhs_shift_amount
        );
        mpn_sub_n(
            &result._data[0],    // dst
            &result._data[0],    // src1
            &operand._data[0],   // src2
            result.vector_size() // limb vector length
        );
    }
    return result;
}

APyFixed APyFixed::operator*(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    // Result fixed-point number
    APyFixed result(res_bits, res_int_bits);

    // Single-limb result specialization
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        result._data[0] = _data[0] * rhs._data[0];
        return result; // early exit
    }

    const bool sign_product = is_negative() ^ rhs.is_negative();
    auto abs_operand1 = limb_vector_abs(_data.cbegin(), _data.cend());
    auto abs_operand2 = limb_vector_abs(rhs._data.cbegin(), rhs._data.cend());

    // `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the
    // product’s most significant limb is zero."
    result._data.resize(abs_operand1.size() + abs_operand2.size());

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

    // Shape the rsult vector back to the number of significant limbs
    result._data.resize(bits_to_limbs(res_bits));

    // Handle sign
    if (sign_product) {
        limb_vector_negate(
            result._data.begin(), result._data.end(), result._data.begin()
        );
    }
    return result;
}

APyFixed APyFixed::operator/(const APyFixed& rhs) const
{
    const int res_bits = bits() + std::max(rhs.bits() - rhs.int_bits(), 0) + 1;
    const int res_int_bits = int_bits() + rhs.bits() - rhs.int_bits() + 1;
    APyFixed result(res_bits, res_int_bits);

    // Single-limb result specialization
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        mp_limb_signed_t numerator = _data[0] << rhs.frac_bits();
        mp_limb_signed_t denominator = rhs._data[0];
        result._data[0] = numerator / denominator;
        return result; // early exit
    }

    // Resulting sign
    const bool sign_product = is_negative() ^ rhs.is_negative();

    // Absolute value denominator
    ScratchVector<mp_limb_t> abs_den(rhs._data.size());
    limb_vector_abs(rhs._data.cbegin(), rhs._data.cend(), abs_den.begin());

    // Absolute value left-shifted numerator
    ScratchVector<mp_limb_t> abs_num(1 + bits_to_limbs(bits() + rhs.frac_bits()));
    abs()._data_asl(abs_num.begin(), abs_num.end(), rhs.frac_bits());

    // `mpn_tdiv_qr` requires that the number of significant limbs in denominator
    std::size_t den_significant_limbs
        = significant_limbs(abs_den.begin(), abs_den.end());
    mpn_div_qr(
        &result._data[0],     // Quotient
        &abs_num[0],          // Numerator
        abs_num.size(),       // Numerator significant limbs
        &abs_den[0],          // Denominator
        den_significant_limbs // Denominator significant limbs
    );
    if (sign_product) {
        limb_vector_negate(
            result._data.begin(), result._data.end(), result._data.begin()
        );
    }
    return result;
}

APyFixed APyFixed::operator+(int rhs) const
{
    // TODO
    if (rhs == 0) {
        return *this;
    }
    throw NotImplementedException("Not implemented: APyFixed.__add__(int)");
}

APyFixed APyFixed::operator-(int rhs) const
{
    // TODO
    if (rhs == 0) {
        return *this;
    }
    throw NotImplementedException("Not implemented: APyFixed.__sub__(int)");
}

APyFixed APyFixed::operator*(int rhs) const
{
    if (rhs == 1) {
        return *this;
    }
    throw NotImplementedException("Not implemented: APyFixed.__mul__(int)");
}

APyFixed APyFixed::operator/(int rhs) const
{
    if (rhs == 1) {
        return *this;
    }
    throw NotImplementedException("Not implemented: APyFixed.__mul__(int)");
}

APyFixed APyFixed::operator<<(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixed result = *this;
    result._int_bits += shift_val;
    return result;
}

APyFixed APyFixed::operator>>(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixed result = *this;
    result._int_bits -= shift_val;
    return result;
}

APyFixed& APyFixed::operator<<=(const int shift_val)
{
    // Left-shift in place
    _int_bits += shift_val;
    return *this;
}

APyFixed& APyFixed::operator>>=(const int shift_val)
{
    // Left-shift in place
    _int_bits -= shift_val;
    return *this;
}

bool APyFixed::operator==(const APyFixed& rhs) const { return (*this - rhs).is_zero(); }

bool APyFixed::operator!=(const APyFixed& rhs) const { return !(*this == rhs); }

bool APyFixed::operator<(const APyFixed& rhs) const
{
    return (*this - rhs).is_negative();
}

bool APyFixed::operator<=(const APyFixed& rhs) const
{
    auto diff = *this - rhs;
    return diff.is_negative() || diff.is_zero();
}

bool APyFixed::operator>(const APyFixed& rhs) const
{
    return (rhs - *this).is_negative();
}

bool APyFixed::operator>=(const APyFixed& rhs) const
{
    auto diff = rhs - *this;
    return diff.is_negative() || diff.is_zero();
}

APyFixed APyFixed::operator-() const
{
    const int res_bits = _bits + 1;
    APyFixed result(res_bits, _int_bits + 1);
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native negation
        result._data[0] = -_data[0];
    } else {

        // Invert all bits of *this, possibly append sign to new limb, and increment lsb
        std::transform(
            _data.cbegin(), _data.cend(), result._data.begin(), std::bit_not {}
        );
        if (result.vector_size() > vector_size()) {
            mp_limb_t sign = _data.back() & (mp_limb_t(1) << (_LIMB_SIZE_BITS - 1));
            result._data.back() = sign ? 0 : mp_limb_signed_t(-1);
        }
        result.increment_lsb();
    }
    return result;
}

APyFixed APyFixed::abs() const
{
    const int res_bits = _bits + 1;
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        APyFixed result(res_bits, _int_bits + 1);
        result._data[0] = std::abs(mp_limb_signed_t(_data[0]));
        return result;
    } else {
        if (is_negative()) {
            // Unary `operator-()` increases word length by one
            return -*this;
        } else {
            // Incrase word length by one and return copy (extra bit guaranteed to be
            // zero)
            APyFixed result(res_bits, _int_bits + 1);
            std::copy(_data.cbegin(), _data.cend(), result._data.begin());
            return result;
        }
    }
}

/* ********************************************************************************** *
 * *                           Public member functions                              * *
 * ********************************************************************************** */

bool APyFixed::is_negative() const noexcept
{
    return limb_vector_is_negative(_data.begin(), _data.end());
}

bool APyFixed::is_positive() const noexcept
{
    return mp_limb_signed_t(_data.back()) > 0;
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

std::string APyFixed::to_string_dec() const
{
    // Construct a string from the absolute value of number, and conditionally append a
    // minus sign to the string if negative
    APyFixed abs_val = abs();

    // Convert this number to BCD with the double-dabble algorithm
    std::vector<mp_limb_t> bcd_limb_list
        = double_dabble(static_cast<std::vector<mp_limb_t>>(abs_val._data));
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

APyFixed::operator double() const { return to_double(); }

void APyFixed::set_from_string_dec(const std::string& str)
{
    // Trim the string from leading and trailing whitespace
    std::string str_trimmed = string_trim_whitespace(str);

    // Check the validity as a decimal string
    if (!is_valid_decimal_numeric_string(str_trimmed)) {
        throw nb::value_error("Not a valid decimal numeric string");
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

    // Multiply BCD number by 2^(frac_bits() + 1) (extra bit for quantization)
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
        limb_vector_negate(_data.begin(), _data.end(), _data.begin());
    }

    // Two's complement overflow and we're done
    _twos_complement_overflow(_data.begin(), _data.end(), bits(), int_bits());
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
    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }

    if constexpr (_LIMB_SIZE_BITS == 64) {
        mp_limb_signed_t exp = exp_of_double(value);
        mp_limb_t man = man_of_double(value);

        // Append mantissa hidden one
        if (exp) {
            man |= mp_limb_t(1) << 52;
        }

        // Adjust the actual exponent with bias (-1023) and
        // shift the data into its correct position
        auto left_shift_amnt = exp + frac_bits() - 52 - 1023;
        if (unsigned(_bits) <= _LIMB_SIZE_BITS) {
            _data[0] = man;
            if (left_shift_amnt >= 0) {
                _data[0] <<= left_shift_amnt;
            } else {
                auto right_shift_amount = -left_shift_amnt;
                if (right_shift_amount - 1 < 64) {
                    // Round the value
                    _data[0] += mp_limb_t(1) << (right_shift_amount - 1);
                }
                _data[0] >>= right_shift_amount;
            }
            // Adjust result from sign
            if (sign_of_double(value)) {
                _data[0] = -_data[0];
            }
            _data[0] = twos_complement_overflow(_data[0], bits());
        } else {
            std::fill(_data.begin(), _data.end(), 0);
            _data[0] = man;
            if (left_shift_amnt >= 0) {
                limb_vector_lsl(_data.begin(), _data.end(), left_shift_amnt);
            } else {
                auto right_shift_amount = -left_shift_amnt;
                if (right_shift_amount - 1 < 64) {
                    // Round the value
                    _data[0] += mp_limb_t(1) << (right_shift_amount - 1);
                }
                limb_vector_lsr(_data.begin(), _data.end(), right_shift_amount);
            }
            // Adjust result from sign
            if (sign_of_double(value)) {
                limb_vector_negate(_data.begin(), _data.end(), _data.begin());
            }
            _twos_complement_overflow(_data.begin(), _data.end(), bits(), int_bits());
        }
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

        ScratchVector<mp_limb_t> man_vec(std::distance(_data.cbegin(), _data.cend()));
        limb_vector_abs(_data.cbegin(), _data.cend(), man_vec.begin());
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Shift the mantissa into position and set the mantissa and exponent part
        int left_shift_amnt = 53 - _LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
        if (left_shift_amnt > 0) {
            limb_vector_lsl(man_vec.begin(), man_vec.end(), left_shift_amnt);
        } else {
            limb_vector_lsr(man_vec.begin(), man_vec.end(), -left_shift_amnt);
        }
        man = man_vec[0];
        exp = 1023 + 52 - left_shift_amnt - frac_bits();

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
    ScratchVector<mp_limb_t> other_data_copy { other._data };
    if (frac_bits() <= other.frac_bits()) {
        limb_vector_asr(
            other_data_copy.begin(),
            other_data_copy.end(),
            other.frac_bits() - frac_bits()
        );
    } else {
        limb_vector_lsl(
            other_data_copy.begin(),
            other_data_copy.end(),
            frac_bits() - other.frac_bits()
        );
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

    _twos_complement_overflow(_data.begin(), _data.end(), bits(), int_bits());
}

nb::int_ APyFixed::to_bits() const
{
    return python_limb_vec_to_long(
        _data.begin(), _data.end(), false, bits() % _LIMB_SIZE_BITS
    );
}

std::string APyFixed::bit_pattern_to_string_dec() const
{
    std::stringstream ss {};

    ScratchVector<mp_limb_t> data = _data;
    if (bits() % _LIMB_SIZE_BITS) {
        mp_limb_t and_mask = (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
        data.back() &= and_mask;
    }

    // Double-dabble for binary-to-BCD conversion
    ss << bcds_to_string(double_dabble(static_cast<std::vector<mp_limb_t>>(data)));

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

std::string APyFixed::latex() const
{
    std::string str = (this->is_negative() ? "$-" : "$");
    str += "\\frac{" + bit_pattern_to_string_dec() + "}{2^{"
        + std::to_string(frac_bits()) + "}} = " + fmt::format("{:g}", float(*this))
        + "$";

    return str;
}

bool APyFixed::is_identical(const APyFixed& other) const
{
    return bits() == other.bits() && int_bits() == other.int_bits() && *this == other;
}

std::size_t APyFixed::leading_zeros() const
{
    std::size_t leading_zeros = limb_vector_leading_zeros(_data.begin(), _data.end());
    if (leading_zeros == 0) {
        return 0;
    } else {
        std::size_t utilized_bits_last_limb = ((bits() - 1) % _LIMB_SIZE_BITS) + 1;
        return leading_zeros - (_LIMB_SIZE_BITS - utilized_bits_last_limb);
    }
}

std::size_t APyFixed::leading_ones() const
{
    std::size_t leading_ones = limb_vector_leading_ones(_data.begin(), _data.end());
    if (leading_ones == 0) {
        return 0;
    } else {
        std::size_t utilized_bits_last_limb = (bits() - 1) % _LIMB_SIZE_BITS + 1;
        return leading_ones - (_LIMB_SIZE_BITS - utilized_bits_last_limb);
    }
}

std::size_t APyFixed::leading_fractional_zeros() const
{
    int frac_bits = bits() - int_bits();
    if (frac_bits <= 0) {
        return 0; // early return
    }

    std::size_t utilized_full_frac_limbs = frac_bits / _LIMB_SIZE_BITS;
    std::size_t utilized_frac_bits_last_limb = frac_bits % _LIMB_SIZE_BITS;

    std::size_t leading_frac_bits_full_limbs = limb_vector_leading_zeros(
        _data.begin(), _data.begin() + utilized_full_frac_limbs
    );

    std::size_t leading_frac_bits_last_limb = 0;
    if (utilized_frac_bits_last_limb) {
        mp_limb_t mask = (1 << utilized_frac_bits_last_limb) - 1;
        mp_limb_t limb = _data[utilized_full_frac_limbs];
        limb &= mask;
        leading_frac_bits_last_limb
            = ::leading_zeros(limb) - (_LIMB_SIZE_BITS - utilized_frac_bits_last_limb);
    }

    if (leading_frac_bits_last_limb != utilized_frac_bits_last_limb) {
        return leading_frac_bits_last_limb;
    } else {
        return leading_frac_bits_last_limb + leading_frac_bits_full_limbs;
    }
}

std::size_t APyFixed::leading_signs() const
{
    if (is_negative()) {
        return leading_ones();
    } else {
        return leading_zeros();
    }
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
    APyFixed result(bits, int_bits, frac_bits);
    result.set_from_double(value);
    return result;
}

APyFixed APyFixed::from_string(
    std::string string_value,
    std::optional<int> bits,
    std::optional<int> int_bits,
    int base,
    std::optional<int> frac_bits
)
{
    APyFixed result(bits, int_bits, frac_bits);
    result.set_from_string(string_value, base);
    return result;
}

/* ********************************************************************************** *
 * *                        Resize and quantization methods                         * *
 * ********************************************************************************** */

APyFixed APyFixed::cast(
    std::optional<int> bits,
    std::optional<int> int_bits,
    QuantizationMode quantization,
    OverflowMode overflow,
    std::optional<int> frac_bits
) const
{
    // Sanitize the input
    int new_bits = bits_from_optional(bits, int_bits, frac_bits);
    int new_int_bits = int_bits_from_optional(bits, int_bits, frac_bits);

    // Result that temporarily can hold all the necessary bits
    APyFixed result(std::max(new_bits, _bits), std::max(new_int_bits, _int_bits));
    _cast(
        result._data.begin(), // output start
        result._data.end(),   // output sentinel
        new_bits,
        new_int_bits,
        quantization,
        overflow
    );

    result._bits = new_bits;
    result._int_bits = new_int_bits;
    result._data.resize(bits_to_limbs(new_bits));
    return result;
}

APyFixed APyFixed::resize(
    std::optional<int> bits,
    std::optional<int> int_bits,
    QuantizationMode quantization,
    OverflowMode overflow,
    std::optional<int> frac_bits
) const
{
    PyErr_WarnEx(
        PyExc_DeprecationWarning, "resize() is deprecated, use cast() instead.", 1
    );
    return cast(bits, int_bits, quantization, overflow, frac_bits);
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_cast(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits,
    QuantizationMode quantization,
    OverflowMode overflow
) const
{
    if (it_end <= it_begin) {
        return; // Early exit
    }

    // Copy data into the result and sign extend
    std::size_t result_vector_size = std::distance(it_begin, it_end);
    std::copy_n(_data.begin(), std::min(vector_size(), result_vector_size), it_begin);
    if (vector_size() < result_vector_size) {
        std::fill(it_begin + vector_size(), it_end, is_negative() ? mp_limb_t(-1) : 0);
    }

    // First perform quantization
    _quantize(it_begin, it_end, new_bits, new_int_bits, quantization);

    // Then perform overflowing
    _overflow(it_begin, it_end, new_bits, new_int_bits, overflow);
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_cast_correct_wl(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    unsigned int shift_amount
) const
{
    // Copy data into the result and sign extend
    std::size_t result_vector_size = std::distance(it_begin, it_end);
    std::copy_n(_data.begin(), std::min(result_vector_size, vector_size()), it_begin);
    if (vector_size() < result_vector_size) {
        std::fill(it_begin + vector_size(), it_end, is_negative() ? mp_limb_t(-1) : 0);
    }
    if (shift_amount > 0) {
        limb_vector_lsl(it_begin, it_end, shift_amount);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits,
    QuantizationMode quantization
) const
{
    switch (quantization) {
    case QuantizationMode::TRN:
        _quantize_trn(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::TRN_INF:
        _quantize_trn_inf(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::TRN_ZERO:
        _quantize_trn_zero(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND:
        _quantize_rnd(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_ZERO:
        _quantize_rnd_zero(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_INF:
        _quantize_rnd_inf(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_MIN_INF:
        _quantize_rnd_min_inf(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_CONV:
        _quantize_rnd_conv(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_CONV_ODD:
        _quantize_rnd_conv_odd(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::JAM:
        _quantize_jam(it_begin, it_end, new_bits, new_int_bits);
        break;
    case QuantizationMode::JAM_UNBIASED:
        _quantize_jam_unbiased(it_begin, it_end, new_bits, new_int_bits);
        break;
    default:
        throw NotImplementedException(fmt::format(
            "Not implemented: APyFixed.cast(): with quantization mode: {}",
            quantization_mode_to_string(quantization)
        ));
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_trn(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        limb_vector_asr(it_begin, it_end, frac_bits() - new_frac_bits);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_trn_inf(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_trn_zero(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (limb_vector_is_negative(it_begin, it_end)) {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_rnd(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_rnd_zero(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
        } else {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_rnd_inf(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
        } else {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_rnd_min_inf(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
        } else {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_rnd_conv(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
            limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
        } else {
            if (limb_vector_test_bit(it_begin, it_end, start_idx)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_rnd_conv_odd(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
            limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
        } else {
            if (!limb_vector_test_bit(it_begin, it_end, start_idx)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_jam(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        limb_vector_asr(it_begin, it_end, frac_bits() - new_frac_bits);
    }
    limb_vector_set_bit(it_begin, it_end, 0, true);
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_quantize_jam_unbiased(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits
) const
{
    int new_frac_bits = new_bits - new_int_bits;
    if (frac_bits() <= new_frac_bits) {
        limb_vector_lsl(it_begin, it_end, new_frac_bits - frac_bits());
    } else {
        unsigned start_idx = frac_bits() - new_frac_bits;
        if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
            limb_vector_set_bit(it_begin, it_end, start_idx, true);
        }
        limb_vector_asr(it_begin, it_end, start_idx);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_overflow(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits,
    OverflowMode overflow
) const
{
    switch (overflow) {
    case OverflowMode::WRAP:
        _twos_complement_overflow(it_begin, it_end, new_bits, new_int_bits);
        break;
    default:
        throw NotImplementedException(fmt::format(
            "Not implemented: APyFixed.cast(): with overflow mode: {}",
            overflow_mode_to_string(overflow)
        ));
    }
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_twos_complement_overflow(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int bits,
    int int_bits
) const
{
    (void)int_bits;
    unsigned limb_shift_val = bits & (_LIMB_SIZE_BITS - 1);
    std::size_t n_out_limbs = std::distance(it_begin, it_end);
    std::size_t significant_limbs = bits_to_limbs(bits);
    auto limb_it = it_begin + significant_limbs - 1;

    if (limb_shift_val) {
        auto shft_amnt = _LIMB_SIZE_BITS - limb_shift_val;
        auto signed_limb = mp_limb_signed_t(*limb_it << shft_amnt) >> shft_amnt;
        *limb_it = mp_limb_t(signed_limb);
    }

    if (n_out_limbs > significant_limbs) {
        std::fill_n(
            it_begin + significant_limbs,
            n_out_limbs - significant_limbs,
            mp_limb_signed_t(*limb_it) < 0 ? -1 : 0
        );
    }
}

/* ********************************************************************************** *
 * *                          Private member functions                              * *
 * ********************************************************************************** */

template <typename RANDOM_ACCESS_ITERATOR>
void APyFixed::_data_asl(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_val
) const
{
    if (shift_val == 0) {
        std::copy(_data.cbegin(), _data.cend(), it_begin);
    }

    // Perform the left-shift
    unsigned vec_skip_val = shift_val / _LIMB_SIZE_BITS;
    unsigned bit_shift_val = shift_val % _LIMB_SIZE_BITS;
    std::copy(_data.cbegin(), _data.cend(), it_begin + vec_skip_val);
    mpn_lshift(
        &*it_begin,                      // dst
        &*it_begin,                      // src
        std::distance(it_begin, it_end), // limb vector length
        bit_shift_val                    // shift amount
    );

    // Append sign bits for "arithmetic" shift
    if ((bits() + shift_val) % _LIMB_SIZE_BITS > 0) {
        // Arithmetic right-shift
        mp_limb_t sign_int = mp_limb_signed_t(_data.back()) >> (_LIMB_SIZE_BITS - 1);
        mp_limb_t or_mask
            = ~((mp_limb_t(1) << (bits() + shift_val) % _LIMB_SIZE_BITS) - 1);
        *std::prev(it_end) |= or_mask & sign_int;
    }
}

/* ********************************************************************************** *
 * *                       Explicit template instantiations                         * *
 * ********************************************************************************** */

template void APyFixed::_cast(
    std::vector<mp_limb_t>::iterator,
    std::vector<mp_limb_t>::iterator,
    int,
    int,
    QuantizationMode,
    OverflowMode
) const;

template void APyFixed::_twos_complement_overflow(
    std::vector<mp_limb_t>::iterator, std::vector<mp_limb_t>::iterator, int, int
) const;

template void APyFixed::_twos_complement_overflow(
    ScratchVector<mp_limb_t>::iterator, ScratchVector<mp_limb_t>::iterator, int, int
) const;
