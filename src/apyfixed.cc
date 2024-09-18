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
#include <cassert>    // assert()
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

#include "apybuffer.h"
#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfloat.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

APyFixed::APyFixed(const APyFixed& other)
    : _bits { other._bits }
    , _int_bits { other._int_bits }
    , _data(other._data.begin(), other._data.end())
{
}

APyFixed::APyFixed(
    const nb::int_& python_long_int_bit_pattern,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyFixed(int_bits, frac_bits, bits)
{
    _data = python_long_to_limb_vec(python_long_int_bit_pattern, _data.size());
    _overflow_twos_complement(_data.begin(), _data.end(), _bits, _int_bits);
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyFixed::APyFixed(
    std::optional<int> int_bits, std::optional<int> frac_bits, std::optional<int> bits
)
    : _bits { bits_from_optional(bits, int_bits, frac_bits) }
    , _int_bits { int_bits.has_value() ? *int_bits : *bits - *frac_bits }
    , _data(bits_to_limbs(_bits), 0)
{
}

APyFixed::APyFixed(int bits, int int_bits)
    : _bits { bits }
    , _int_bits { int_bits }
    , _data(bits_to_limbs(_bits), 0)
{
}

template <typename _IT>
APyFixed::APyFixed(int bits, int int_bits, _IT begin, _IT end)
    : APyFixed(bits, int_bits)
{
    assert(std::distance(begin, end) >= 0);
    assert(std::distance(begin, end) <= ptrdiff_t(bits_to_limbs(bits)));

    // Copy data into resulting vector
    std::copy(begin, end, _data.begin());

    // Two's-complements overflow bits outside of the range
    _overflow_twos_complement(_data.begin(), _data.end(), _bits, _int_bits);
}

APyFixed::APyFixed(int bits, int int_bits, const std::vector<mp_limb_t>& vec)
    : APyFixed(bits, int_bits, vec.begin(), vec.end())
{
}

/* ********************************************************************************** *
 * *                            Arithmetic member functions                         * *
 * ********************************************************************************** */

template <class base_op, class ripple_carry_op>
inline APyFixed APyFixed::_apyfixed_base_add_sub(const APyFixed& rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    APyFixed result(res_bits, res_int_bits);
    auto left_shift_amount = unsigned(res_frac_bits - frac_bits());
    auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());

    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native operation
        result._data[0] = base_op {}(
            _data[0] << left_shift_amount, rhs._data[0] << rhs_shift_amount
        );
    } else {
        _cast_correct_wl(result._data.begin(), result._data.end(), left_shift_amount);
        // Resulting number of bits is more than one limb. Use ripple-carry operation
        APyFixed operand(res_bits, res_int_bits);
        rhs._cast_correct_wl(
            operand._data.begin(), operand._data.end(), rhs_shift_amount
        );
        ripple_carry_op {}(
            &result._data[0],    // dst
            &result._data[0],    // src1
            &operand._data[0],   // src2
            result.vector_size() // limb vector length
        );
    }

    return result;
}

APyFixed APyFixed::operator+(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<std::plus<>, mpn_add_n_functor<>>(rhs);
}

APyFixed APyFixed::operator-(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<std::minus<>, mpn_sub_n_functor<>>(rhs);
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

    ScratchVector<mp_limb_t> abs_op1(_data.size());
    ScratchVector<mp_limb_t> abs_op2(rhs._data.size());
    limb_vector_abs(_data.begin(), _data.end(), abs_op1.begin());
    limb_vector_abs(rhs._data.begin(), rhs._data.end(), abs_op2.begin());

    // `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the
    // product’s most significant limb is zero."
    result._data.resize(abs_op1.size() + abs_op2.size());

    // `mpn_mul` requires the limb vector length of the first operand to be
    // greater than, or equally long as, the limb vector length of the second
    // operand. Simply swap the operands (essentially a free operation in C++)
    // if this is not the case.
    if (abs_op1.size() < abs_op2.size()) {
        std::swap(abs_op1, abs_op2);
    }

    mpn_mul(
        &result._data[0], // dst
        &abs_op1[0],      // src1
        abs_op1.size(),   // src1 limb vector length
        &abs_op2[0],      // src2
        abs_op2.size()    // src2 limb vector length
    );

    // Shape the rsult vector back to the number of significant limbs
    result._data.resize(bits_to_limbs(res_bits));

    // Handle sign
    if (is_negative() ^ rhs.is_negative()) {
        limb_vector_negate(
            result._data.begin(), result._data.end(), result._data.begin()
        );
    }
    return result;
}

APyFixed APyFixed::operator/(const APyFixed& rhs) const
{
    if (rhs.is_zero()) {
        PyErr_SetString(PyExc_ZeroDivisionError, "fixed-point division by zero");
        throw nb::python_error();
    }

    const int res_int_bits = int_bits() + rhs.frac_bits() + 1;
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyFixed result(res_bits, res_int_bits);

    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        mp_limb_signed_t numerator = _data[0] << rhs.bits();
        mp_limb_signed_t denominator = rhs._data[0];
        result._data[0] = numerator / denominator;
        return result; // early exit
    }

    // Absolute value denominator
    ScratchVector<mp_limb_t> abs_den(rhs._data.size());
    limb_vector_abs(rhs._data.cbegin(), rhs._data.cend(), abs_den.begin());

    // Absolute value left-shifted numerator
    ScratchVector<mp_limb_t> abs_num(bits_to_limbs(res_bits));
    limb_vector_abs(_data.begin(), _data.end(), abs_num.begin());
    limb_vector_lsl(abs_num.begin(), abs_num.end(), rhs.bits());

    // `mpn_tdiv_qr` requires the number of *significant* limbs in denominator
    std::size_t den_significant_limbs
        = significant_limbs(abs_den.begin(), abs_den.end());
    mpn_div_qr(
        &result._data[0],     // Quotient
        &abs_num[0],          // Numerator
        abs_num.size(),       // Numerator limbs
        &abs_den[0],          // Denominator
        den_significant_limbs // Denominator significant limbs
    );

    // Negate result if negative
    if (is_negative() ^ rhs.is_negative()) {
        limb_vector_negate(
            result._data.begin(), result._data.end(), result._data.begin()
        );
    }
    return result;
}

APyFixed APyFixed::operator+(const nb::int_& rhs) const
{
    return *this + APyFixed::from_integer(rhs, _int_bits, frac_bits());
}

APyFixed APyFixed::operator-(const nb::int_& rhs) const
{
    return *this - APyFixed::from_integer(rhs, _int_bits, frac_bits());
}

APyFixed APyFixed::operator*(const nb::int_& rhs) const
{
    return *this * APyFixed::from_integer(rhs, _int_bits, frac_bits());
}

APyFixed APyFixed::operator/(const nb::int_& rhs) const
{
    return *this / APyFixed::from_integer(rhs, _int_bits, frac_bits());
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

bool APyFixed::operator==(const nb::int_& rhs) const
{
    const std::vector<mp_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        _LIMB_SIZE_BITS * limb_vec.size(), _LIMB_SIZE_BITS * limb_vec.size(), limb_vec
    );
    return *this == rhs_fixed;
}

bool APyFixed::operator!=(const nb::int_& rhs) const
{
    const std::vector<mp_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        _LIMB_SIZE_BITS * limb_vec.size(), _LIMB_SIZE_BITS * limb_vec.size(), limb_vec
    );
    return *this != rhs_fixed;
}

bool APyFixed::operator<(const nb::int_& rhs) const
{
    const std::vector<mp_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        _LIMB_SIZE_BITS * limb_vec.size(), _LIMB_SIZE_BITS * limb_vec.size(), limb_vec
    );
    return *this < rhs_fixed;
}

bool APyFixed::operator<=(const nb::int_& rhs) const
{
    const std::vector<mp_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        _LIMB_SIZE_BITS * limb_vec.size(), _LIMB_SIZE_BITS * limb_vec.size(), limb_vec
    );
    return *this <= rhs_fixed;
}

bool APyFixed::operator>(const nb::int_& rhs) const
{
    const std::vector<mp_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        _LIMB_SIZE_BITS * limb_vec.size(), _LIMB_SIZE_BITS * limb_vec.size(), limb_vec
    );
    return *this > rhs_fixed;
}

bool APyFixed::operator>=(const nb::int_& rhs) const
{
    const std::vector<mp_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        _LIMB_SIZE_BITS * limb_vec.size(), _LIMB_SIZE_BITS * limb_vec.size(), limb_vec
    );
    return *this >= rhs_fixed;
}

bool APyFixed::operator==(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this == rhs_fp.to_fixed());
}

bool APyFixed::operator!=(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this != rhs_fp.to_fixed());
}

bool APyFixed::operator<=(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this <= rhs_fp.to_fixed());
}

bool APyFixed::operator<(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this < rhs_fp.to_fixed());
}

bool APyFixed::operator>=(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this >= rhs_fp.to_fixed());
}

bool APyFixed::operator>(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this > rhs_fp.to_fixed());
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
    APyFixed result(res_bits, _int_bits + 1);
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        result._data[0] = std::abs(mp_limb_signed_t(_data[0]));
    } else {
        limb_vector_abs(_data.begin(), _data.end(), result._data.begin());
    }
    return result;
}

/* ********************************************************************************** *
 * *                           Public member functions                              * *
 * ********************************************************************************** */

bool APyFixed::is_negative() const noexcept
{
    return limb_vector_is_negative(_data.begin(), _data.end());
}

bool APyFixed::is_positive() const noexcept { return !is_negative() && !is_zero(); }

bool APyFixed::is_zero() const noexcept
{
    return limb_vector_is_zero(std::begin(_data), std::end(_data));
}

// Increment the LSB without making the fixed-point number wider. Return carry out
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

std::string APyFixed::to_string_hex() const
{
    throw NotImplementedException("APyFixed::to_string_hex()");
}

std::string APyFixed::to_string_oct() const
{
    throw NotImplementedException("APyFixed::to_string_oct()");
}

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
        throw nb::value_error(
            fmt::format("APyFixed::to_string(base={}): base is not supported", base)
                .c_str()
        );
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
        std::remove(str_trimmed.begin(), str_trimmed.end(), '.'), str_trimmed.end()
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
        limb_vector_asr(data.begin(), data.end(), -(frac_bits() + 1));
    }

    // Copy the data into the result vector
    _data = data;
    _data.resize(bits_to_limbs(bits()));
    if (is_negative) {
        limb_vector_negate(_data.begin(), _data.end(), _data.begin());
    }

    // Two's complement overflow and we're done
    _overflow_twos_complement(_data.begin(), _data.end(), bits(), int_bits());
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

    // Zero limb vector data
    std::fill(_data.begin(), _data.end(), 0);

    int exp = exp_of_double(value);
    uint64_t man = man_of_double(value);

    // Left-shift amount needed to align floating binary point with fixed binary point
    int left_shift_amnt;
    if (exp) {
        // Append mantissa hidden one
        man |= uint64_t(1) << 52;
        left_shift_amnt = exp + frac_bits() - 52 - 1023;
    } else {
        left_shift_amnt = exp + frac_bits() - 52 - 1023 + 1;
    }

    if constexpr (_LIMB_SIZE_BITS == 64) {

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
        _overflow_twos_complement(_data.begin(), _data.end(), bits(), int_bits());

    } else { /* _LIMB_SIZE_BITS == 32 */

        bool pop_back = false;
        _data[0] = man & 0xFFFFFFFF;
        if (_data.size() == 1) {
            _data.push_back(mp_limb_t(man >> 32));
            pop_back = true;
        } else {
            _data[1] = mp_limb_t(man >> 32);
        }

        if (left_shift_amnt >= 0) {
            limb_vector_lsl(_data.begin(), _data.end(), left_shift_amnt);
        } else {
            auto right_shift_amount = -left_shift_amnt;
            if (right_shift_amount - 1 < 64) {
                // Round the value
                if (right_shift_amount - 1 >= 32) {
                    _data[1] += mp_limb_t(1) << (right_shift_amount - 1 - 32);
                } else {
                    // The mantissa is at most 53 bits (two limbs)
                    mpn_add_1(
                        &_data[0],                               // dst
                        &_data[0],                               // src
                        2,                                       // limb vector len
                        mp_limb_t(1) << (right_shift_amount - 1) // single limb
                    );
                }
            }
            limb_vector_lsr(_data.begin(), _data.end(), right_shift_amount);
        }

        // Adjust result from sign
        if (sign_of_double(value)) {
            limb_vector_negate(_data.begin(), _data.end(), _data.begin());
        }
        _overflow_twos_complement(_data.begin(), _data.end(), bits(), int_bits());

        // Pop of temporary limb
        if (pop_back) {
            _data.pop_back();
        }
    }
}

double APyFixed::to_double() const
{
    // Early exit if zero
    if (is_zero()) {
        return 0.0;
    }

    uint64_t man {};
    int exp {};
    bool sign = is_negative();

    if constexpr (_LIMB_SIZE_BITS == 64) {

        ScratchVector<mp_limb_t> man_vec(_data.size());
        limb_vector_abs(_data.cbegin(), _data.cend(), man_vec.begin());
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Compute the shift amount and exponent value
        int left_shift_n = 53 - _LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
        exp = 1023 + 52 - left_shift_n - frac_bits();
        if (exp < 1) {
            // Handle IEEE subnormals
            left_shift_n += exp - 1;
            exp = 0;
        }

        // Shift the mantissa into position and set the mantissa and exponent part
        if (left_shift_n >= 0) {
            limb_vector_lsl(std::begin(man_vec), std::end(man_vec), left_shift_n);
        } else {
            int right_shift_n = -left_shift_n;
            int rnd_pow2 = right_shift_n - 1;
            limb_vector_add_pow2(std::begin(man_vec), std::end(man_vec), rnd_pow2);
            limb_vector_lsr(std::begin(man_vec), std::end(man_vec), right_shift_n);
        }
        man = man_vec[0];

    } else { /* _LIMB_SIZE_BITS == 32 */

        ScratchVector<mp_limb_t> man_vec(std::max(_data.size(), std::size_t(2)));
        if (_data.size() == 1) {
            man_vec[0] = _data[0];
            man_vec[1] = mp_limb_signed_t(_data[0]) < 0 ? -1 : 0;
        } else {
            std::copy(_data.begin(), _data.end(), man_vec.begin());
        }
        limb_vector_abs(man_vec.cbegin(), man_vec.cend(), man_vec.begin());
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Compute the shift amount and exponent value
        int left_shift_n = 53 - _LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
        exp = 1023 + 52 - left_shift_n - frac_bits();
        if (exp < 1) {
            // Handle IEEE subnormals
            left_shift_n += exp - 1;
            exp = 0;
        }

        // Shift the mantissa into position and set the mantissa and exponent part
        if (left_shift_n >= 0) {
            limb_vector_lsl(std::begin(man_vec), std::end(man_vec), left_shift_n);
        } else {
            int right_shift_n = -left_shift_n;
            int rnd_pow2 = right_shift_n - 1;
            limb_vector_add_pow2(std::begin(man_vec), std::end(man_vec), rnd_pow2);
            limb_vector_lsr(std::begin(man_vec), std::end(man_vec), right_shift_n);
        }

        man = uint64_t(man_vec[0]);
        man |= uint64_t(man_vec[1]) << 32;
    }

    // Return the result
    double result {};
    set_sign_of_double(result, sign);
    set_exp_of_double(result, exp);
    set_man_of_double(result, man);
    return result;
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
    std::string str;
    if (this->is_negative()) {
        str = "$-\\frac{" + bit_pattern_to_string_dec() + "}{2^{"
            + std::to_string(frac_bits()) + "}} = " + to_string_dec() + "$";
    } else {
        str = "$\\frac{" + bit_pattern_to_string_dec() + "}{2^{"
            + std::to_string(frac_bits()) + "}} = " + to_string_dec() + "$";
    }

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
        mp_limb_t mask = (mp_limb_t(1) << utilized_frac_bits_last_limb) - 1;
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
    return is_negative() ? leading_ones() : leading_zeros();
}

bool APyFixed::positive_greater_than_equal_pow2(int n) const
{
    unsigned test_binary_point = std::max(0, frac_bits() + n);
    return limb_vector_gte_pow2(_data.begin(), _data.end(), test_binary_point);
}

/* ********************************************************************************** *
 * *                           Static member functions                              * *
 * ********************************************************************************** */

APyFixed APyFixed::from_number(
    const nb::object& py_obj,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    if (nb::isinstance<nb::int_>(py_obj)) {
        return APyFixed::from_integer(
            nb::cast<nb::int_>(py_obj), int_bits, frac_bits, bits
        );
    } else if (nb::isinstance<nb::float_>(py_obj)) {
        const auto d = static_cast<double>(nb::cast<nb::float_>(py_obj));
        return APyFixed::from_double(d, int_bits, frac_bits, bits);
    } else {
        const nb::type_object type = nb::cast<nb::type_object>(py_obj.type());
        const nb::str type_string = nb::str(type);
        throw std::domain_error(
            std::string("Non supported type: ") + type_string.c_str()
        );
    }
}

APyFixed APyFixed::from_double(
    double value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    APyFixed result(int_bits, frac_bits, bits);
    result.set_from_double(value);
    return result;
}

APyFixed APyFixed::from_unspecified_double(double value)
{
    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }

    int exp = exp_of_double(value);
    uint64_t man = man_of_double(value);

    // Append mantissa hidden one
    if (exp) {
        man |= uint64_t(1) << 52;
    } else {
        exp = 1;
    }

    const int zeros_to_trim = trailing_zeros(man);
    man >>= zeros_to_trim;
    exp = exp + zeros_to_trim - 52 - 1023;

    // Create limb vector
    std::vector<mp_limb_t> limbs {};
    if constexpr (_LIMB_SIZE_BITS == 32) {
        limbs = { mp_limb_t(man & 0xFFFFFFFF), mp_limb_t(man >> 32) };
    } else { /* _LIMB_SIZE_BITS == 64 */
        limbs = { mp_limb_t(man) };
    }

    // Calculate required number of bits
    const int bits = 1 + bit_width(man); // +1 For the sign
    const int int_bits = exp + bits;

    // Adjust result from sign
    if (sign_of_double(value)) {
        limb_vector_negate(limbs.begin(), limbs.end(), limbs.begin());
    }

    return APyFixed(bits, int_bits, limbs);
}

APyFixed APyFixed::from_integer(
    const nb::int_& value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    // Extract things bit widths
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;

    APyFixed result(res_bits, res_int_bits);
    result._data = python_long_to_limb_vec(value, result._data.size());

    // Adjust the number
    if (result.frac_bits() > 0) {
        limb_vector_lsl(result._data.begin(), result._data.end(), result.frac_bits());
    } else { /* result.frac_bits() <= 0 */
        limb_vector_asr(result._data.begin(), result._data.end(), -result.frac_bits());
    }

    // Two's-complements overflow bits outside of the range
    _overflow_twos_complement(
        result._data.begin(), result._data.end(), res_bits, res_int_bits
    );

    return result;
}

APyFixed APyFixed::from_unspecified_integer(const nb::int_& value)
{
    std::vector<mp_limb_t> limbs = python_long_to_limb_vec(value);

    // If the value is positive but the MSB is set, adding a zero limb
    // will prevent it from representing a negative value.
    if (!python_long_is_negative(value)
        && limb_vector_is_negative(std::begin(limbs), std::end(limbs))) {
        limbs.push_back(0);
    }
    const std::size_t res_bits = limbs.size() * _LIMB_SIZE_BITS;
    return APyFixed(res_bits, res_bits, limbs.begin(), limbs.end());
}

APyFixed APyFixed::from_string(
    std::string string_value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    int base,
    std::optional<int> bits
)
{
    APyFixed result(int_bits, frac_bits, bits);
    result.set_from_string(string_value, base);
    return result;
}

/* ********************************************************************************** *
 * *                        Resize and quantization methods                         * *
 * ********************************************************************************** */

APyFixed APyFixed::cast(
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<int> bits
) const
{
    // Sanitize the input (bit-specifier validity tested in `bits_from_optional_cast()`)
    const auto [new_bits, new_int_bits]
        = bits_from_optional_cast(bits, int_bits, frac_bits, _bits, _int_bits);

    const APyFixedCastOption cast_option = get_fixed_cast_mode();
    const auto quantization_mode = quantization.value_or(cast_option.quantization);
    const auto overflow_mode = overflow.value_or(cast_option.overflow);

    // Result that temporarily can hold all the necessary bits
    APyFixed result(std::max(new_bits, _bits), new_int_bits);
    _cast(
        result._data.begin(), // output start
        result._data.end(),   // output sentinel
        new_bits,
        new_int_bits,
        quantization_mode,
        overflow_mode
    );

    result._bits = new_bits;
    result._data.resize(bits_to_limbs(new_bits));
    return result;
}

APyFixed
APyFixed::cast_no_overflow(int bits, int int_bits, QuantizationMode quantization) const
{
    // Sanitize the input
    int new_bits = bits;
    int new_int_bits = int_bits;

    // Result that temporarily can hold all the necessary bits
    APyFixed result(std::max(new_bits, _bits), std::max(new_int_bits, _int_bits));
    _cast_no_overflow(
        result._data.begin(), // output start
        result._data.end(),   // output sentinel
        new_bits,
        new_int_bits,
        quantization
    );

    result._bits = new_bits;
    result._int_bits = new_int_bits;
    result._data.resize(bits_to_limbs(new_bits));
    return result;
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_cast(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits,
    QuantizationMode q_mode,
    OverflowMode v_mode
) const
{
    // Copy data into the result and sign extend
    _copy_and_sign_extend(it_begin, it_end);

    // First perform quantization
    quantize(it_begin, it_end, bits(), int_bits(), new_bits, new_int_bits, q_mode);

    // Then perform overflowing
    overflow(it_begin, it_end, new_bits, new_int_bits, v_mode);
}

template <class RANDOM_ACCESS_ITERATOR>
void APyFixed::_cast_no_overflow(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits,
    QuantizationMode quantization
) const
{
    // Copy data into the result and sign extend
    _copy_and_sign_extend(it_begin, it_end);

    // Perform quantization
    quantize(
        it_begin, it_end, bits(), int_bits(), new_bits, new_int_bits, quantization
    );
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
    limb_vector_lsl(it_begin, it_end, shift_amount);
}

/* ********************************************************************************** *
 * *                          Private member functions                              * *
 * ********************************************************************************** */

template <typename RANDOM_ACCESS_ITERATOR>
void APyFixed::_copy_and_sign_extend(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end
) const
{
    std::size_t result_vector_size = std::distance(it_begin, it_end);
    std::copy_n(_data.begin(), std::min(vector_size(), result_vector_size), it_begin);
    if (vector_size() < result_vector_size) {
        std::fill(it_begin + vector_size(), it_end, is_negative() ? mp_limb_t(-1) : 0);
    }
}

/* ********************************************************************************** *
 * *                       Explicit template instantiations                         * *
 * ********************************************************************************** */

template void APyFixed::_cast(
    APyBuffer<mp_limb_t>::vector_type::iterator,
    APyBuffer<mp_limb_t>::vector_type::iterator,
    int,
    int,
    QuantizationMode,
    OverflowMode
) const;
