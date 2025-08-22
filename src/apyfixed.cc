/*
 * APyFixed: Dynamic arbitrary fixed-point data type.
 */

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfloat.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// Python object access through Pybind
#include <nanobind/nanobind.h>
namespace nb = nanobind;

// Standard header includes
#include <algorithm>        // std::copy, std::max, std::transform, etc...
#include <cassert>          // assert()
#include <cmath>            // std::isinf, std::isnan
#include <cstddef>          // std::size_t
#include <cstring>          // std::memcpy
#include <functional>       // std::bit_not
#include <initializer_list> // initializer_list
#include <iterator>         // std::back_inserter
#include <optional>         // std::optional
#include <sstream>          // std::stringstream
#include <string>           // std::string
#include <vector>           // std::vector, std::swap

#include <fmt/format.h>

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

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

    // Copy data into resulting vector
    std::size_t it_elements = std::distance(begin, end);
    std::copy_n(begin, std::min(vector_size(), it_elements), std::begin(_data));

    // Two's-complements overflow bits outside of the range
    _overflow_twos_complement(std::begin(_data), std::end(_data), _bits, _int_bits);
}

APyFixed::APyFixed(int bits, int int_bits, const std::vector<apy_limb_t>& vec)
    : APyFixed(bits, int_bits, std::begin(vec), std::end(vec))
{
}

APyFixed::APyFixed(int bits, int int_bits, std::initializer_list<apy_limb_t> list)
    : APyFixed(bits, int_bits, std::begin(list), std::end(list))
{
}

APyFixed::APyFixed(int bits, int int_bits, const ScratchVector<apy_limb_t>& data)
    : APyFixed(bits, int_bits, std::begin(data), std::end(data))
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
    auto lhs_shift_amount = unsigned(res_frac_bits - frac_bits());
    auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());

    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native operation
        result._data[0] = base_op {}(
            _data[0] << lhs_shift_amount, rhs._data[0] << rhs_shift_amount
        );
    } else {
        // Resulting number of bits is more than one limb. Use ripple-carry operation
        ScratchVector<apy_limb_t, 8> operand(bits_to_limbs(res_bits));
        _cast_no_quantize_no_overflow(
            std::begin(_data),
            std::end(_data),
            std::begin(result._data),
            std::end(result._data),
            lhs_shift_amount
        );
        _cast_no_quantize_no_overflow(
            std::begin(rhs._data),
            std::end(rhs._data),
            std::begin(operand),
            std::end(operand),
            rhs_shift_amount
        );
        ripple_carry_op {}(
            &result._data[0],    // dst
            &result._data[0],    // src1
            &operand[0],         // src2
            result.vector_size() // limb vector length
        );
    }

    return result;
}

APyFixed APyFixed::operator+(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<std::plus<>, apy_add_n_functor<>>(rhs);
}

APyFixed APyFixed::operator-(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<std::minus<>, apy_sub_n_functor<>>(rhs);
}

APyFixed APyFixed::operator*(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_bits = bits() + rhs.bits();

    // Result fixed-point number
    APyFixed result(res_bits, res_int_bits);

    // Single-limb result specialization
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        result._data[0] = _data[0] * rhs._data[0];
        return result; // early exit
    }

    // Both arguments are single limb, result two limbs
    if (unsigned(bits()) <= APY_LIMB_SIZE_BITS
        && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
        auto [high, low] = long_signed_mult(_data[0], rhs._data[0]);
        result._data[1] = high;
        result._data[0] = low;
    }

    // Scratch data:
    // * abs_op1:   _data.size()
    // * abs_op2:   rhs._data.size()
    // * prod_abs:  _data.size() + rhs._data.size()
    std::size_t scratch_size = 2 * (_data.size() + rhs._data.size());
    ScratchVector<apy_limb_t, 16> scratch(scratch_size);

    // Perform the product
    fixed_point_product(
        std::begin(_data),                                    // src1
        std::begin(rhs._data),                                // src2
        std::begin(result._data),                             // dst
        vector_size(),                                        // src1_limbs
        rhs.vector_size(),                                    // src2_limbs
        bits_to_limbs(res_bits),                              // dst_limbs
        std::begin(scratch),                                  // op1_abs
        std::begin(scratch) + _data.size(),                   // op2_abs
        std::begin(scratch) + _data.size() + rhs._data.size() // prod_abs
    );

    return result;
}

APyFixed APyFixed::operator/(const APyFixed& rhs) const
{
    if (rhs.is_zero()) {
        PyErr_SetString(PyExc_ZeroDivisionError, "fixed-point division by zero");
        throw nb::python_error();
    }

    const int res_int_bits = 1 + int_bits() + rhs.frac_bits();
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyFixed result(res_bits, res_int_bits);

    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        apy_limb_signed_t numerator = _data[0] << rhs.bits();
        apy_limb_signed_t denominator = rhs._data[0];
        result._data[0] = numerator / denominator;
        return result; // early exit
    }

    // Scratch data (size):
    // * abs_num:   bits_to_limbs(res_bits)
    // * abs_den:   rhs._data.size()
    std::size_t scratch_size = bits_to_limbs(res_bits) + rhs._data.size();
    ScratchVector<apy_limb_t, 16> scratch(scratch_size);

    // Absolute value left-shifted numerator
    auto abs_num_begin = std::begin(scratch);
    auto abs_num_end = abs_num_begin + bits_to_limbs(res_bits);
    bool sign_num = limb_vector_abs(_data.begin(), _data.end(), abs_num_begin);
    limb_vector_lsl(abs_num_begin, abs_num_end, rhs.bits());

    // Absolute value denominator
    auto abs_den_begin = abs_num_end;
    auto abs_den_end = abs_den_begin + rhs._data.size();
    bool sign_den
        = limb_vector_abs(rhs._data.cbegin(), rhs._data.cend(), abs_den_begin);

    // `apy_unsigned_division` requires the number of *significant* limbs in denominator
    std::size_t den_significant_limbs = significant_limbs(abs_den_begin, abs_den_end);
    apy_unsigned_division(
        &result._data[0],                          // Quotient
        &*abs_num_begin,                           // Numerator
        std::distance(abs_num_begin, abs_num_end), // Numerator limbs
        &*abs_den_begin,                           // Denominator
        den_significant_limbs                      // Denominator significant limbs
    );

    // Negate result if negative
    if (sign_num ^ sign_den) {
        limb_vector_negate_inplace(result._data.begin(), result._data.end());
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
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
    );
    return *this == rhs_fixed;
}

bool APyFixed::operator!=(const nb::int_& rhs) const
{
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
    );
    return *this != rhs_fixed;
}

bool APyFixed::operator<(const nb::int_& rhs) const
{
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
    );
    return *this < rhs_fixed;
}

bool APyFixed::operator<=(const nb::int_& rhs) const
{
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
    );
    return *this <= rhs_fixed;
}

bool APyFixed::operator>(const nb::int_& rhs) const
{
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
    );
    return *this > rhs_fixed;
}

bool APyFixed::operator>=(const nb::int_& rhs) const
{
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native negation
        result._data[0] = -_data[0];
    } else {

        // Invert all bits of *this, possibly append sign to new limb, and increment lsb
        std::transform(
            _data.cbegin(), _data.cend(), result._data.begin(), std::bit_not {}
        );
        if (result.vector_size() > vector_size()) {
            apy_limb_t sign
                = _data.back() & (apy_limb_t(1) << (APY_LIMB_SIZE_BITS - 1));
            result._data.back() = sign ? 0 : apy_limb_signed_t(-1);
        }
        result.increment_lsb();
    }
    return result;
}

APyFixed APyFixed::operator~() const
{
    APyFixed result(_bits, _int_bits);
    std::transform(_data.cbegin(), _data.cend(), result._data.begin(), std::bit_not {});
    return result;
}

APyFixed APyFixed::abs() const
{
    const int res_bits = _bits + 1;
    APyFixed result(res_bits, _int_bits + 1);
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        result._data[0] = std::abs(apy_limb_signed_t(_data[0]));
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
apy_limb_t APyFixed::increment_lsb() noexcept
{
    return apy_inplace_add_one_lsb(
        &_data[0],    // dst
        vector_size() // limb vector length
    );
}

std::string APyFixed::to_string_dec() const
{
    return fixed_point_to_string_dec(
        std::begin(_data), std::end(_data), _bits, _int_bits
    );
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
    case 10:
        return to_string_dec();
    default:
        auto msg = fmt::format("APyFixed.__str__: base={} is not supported", base);
        throw nb::value_error(msg.c_str());
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
    std::vector<apy_limb_t> data = reverse_double_dabble(bcd_list);

    // Round the data
    apy_inplace_add_one_lsb(
        &data[0],   // dst
        data.size() // limb vector length
    );
    apy_inplace_right_shift(
        &data[0],    // dst/src
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
        limb_vector_negate_inplace(_data.begin(), _data.end());
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

double APyFixed::to_double() const
{
    return fixed_point_to_double(std::begin(_data), std::end(_data), frac_bits());
}

nb::int_ APyFixed::to_bits() const
{
    return python_limb_vec_to_long(
        _data.begin(), _data.end(), false, bits() % APY_LIMB_SIZE_BITS
    );
}

std::string APyFixed::bit_pattern_to_string_dec() const
{
    std::stringstream ss {};

    ScratchVector<apy_limb_t> data = _data;
    if (bits() % APY_LIMB_SIZE_BITS) {
        apy_limb_t and_mask = (apy_limb_t(1) << (bits() % APY_LIMB_SIZE_BITS)) - 1;
        data.back() &= and_mask;
    }

    // Double-dabble for binary-to-BCD conversion
    ss << bcds_to_string(double_dabble(static_cast<std::vector<apy_limb_t>>(data)));

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
    std::string value_str = fixed_point_to_string_dec(
        std::begin(_data), std::end(_data), _bits, _int_bits
    );
    std::string str;
    if (this->is_negative()) {
        APyFixed absval = abs();
        str = "$-\\frac{" + absval.bit_pattern_to_string_dec() + "}{2^{"
            + std::to_string(frac_bits()) + "}} = " + value_str + "$";
    } else {
        str = "$\\frac{" + bit_pattern_to_string_dec() + "}{2^{"
            + std::to_string(frac_bits()) + "}} = " + value_str + "$";
    }

    return str;
}

bool APyFixed::is_identical(
    const std::variant<const APyFixed*, const APyFixedArray*>& other
) const
{
    if (!std::holds_alternative<const APyFixed*>(other)) {
        return false;
    } else {
        auto&& other_scalar = *std::get<const APyFixed*>(other);
        return bits() == other_scalar.bits() && int_bits() == other_scalar.int_bits()
            && *this == other_scalar;
    }
}

std::size_t APyFixed::leading_zeros() const
{
    std::size_t leading_zeros = limb_vector_leading_zeros(_data.begin(), _data.end());
    if (leading_zeros == 0) {
        return 0;
    } else {
        std::size_t utilized_bits_last_limb = ((bits() - 1) % APY_LIMB_SIZE_BITS) + 1;
        return leading_zeros - (APY_LIMB_SIZE_BITS - utilized_bits_last_limb);
    }
}

std::size_t APyFixed::leading_ones() const
{
    std::size_t leading_ones = limb_vector_leading_ones(_data.begin(), _data.end());
    if (leading_ones == 0) {
        return 0;
    } else {
        std::size_t utilized_bits_last_limb = (bits() - 1) % APY_LIMB_SIZE_BITS + 1;
        return leading_ones - (APY_LIMB_SIZE_BITS - utilized_bits_last_limb);
    }
}

std::size_t APyFixed::leading_fractional_zeros() const
{
    int frac_bits = bits() - int_bits();
    if (frac_bits <= 0) {
        return 0; // early return
    }

    std::size_t utilized_full_frac_limbs = frac_bits / APY_LIMB_SIZE_BITS;
    std::size_t utilized_frac_bits_last_limb = frac_bits % APY_LIMB_SIZE_BITS;

    std::size_t leading_frac_bits_full_limbs = limb_vector_leading_zeros(
        _data.begin(), _data.begin() + utilized_full_frac_limbs
    );

    std::size_t leading_frac_bits_last_limb = 0;
    if (utilized_frac_bits_last_limb) {
        apy_limb_t mask = (apy_limb_t(1) << utilized_frac_bits_last_limb) - 1;
        apy_limb_t limb = _data[utilized_full_frac_limbs];
        limb &= mask;
        leading_frac_bits_last_limb = ::leading_zeros(limb)
            - (APY_LIMB_SIZE_BITS - utilized_frac_bits_last_limb);
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

void APyFixed::set_bit_pow2(int n, bool bit)
{
    const unsigned binary_point = std::max(0, frac_bits() + n);
    limb_vector_set_bit(_data.begin(), _data.end(), binary_point, bit);
}

APyFixed APyFixed::pown(int n) const
{
    if (n < 0) {
        throw NotImplementedException("Not implemented: power with negative integers.");
    }

    if (n == 1) {
        return *this;
    }

    if (n == 0) {
        if (frac_bits() < 0) {
            return APyFixed(int_bits(), int_bits(), { 1 });
        } else {
            return APyFixed(
                bits(), int_bits(), { apy_limb_t(1) << (unsigned int)frac_bits() }
            );
        }
    }

    // Early exit for one of the most common cases
    if (n == 2) {
        return *this * *this;
    }

    auto result = ipow(n);
    // Remove two integer bit as the starting value has two bits
    return result.cast_no_overflow(result.bits() - 2, result.int_bits() - 2);
}

APyFixed APyFixed::ipow(unsigned int n) const
{
    // Because how pown is written, we know n will be >= 3,
    // this fact can probably be used to optimize this code further.
    APyFixed base = *this;
    APyFixed result = APyFixed(2, 2, { 1 });
    for (;;) {
        if (n & 1) {
            result = result * base; // Until *= is implemented
        }
        n >>= 1;
        if (!n) {
            break;
        }
        base = base * base; // Until *= is implemented
    }
    return result;
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
    } else if (nb::isinstance<APyFixed>(py_obj)) {
        const auto d = static_cast<APyFixed>(nb::cast<APyFixed>(py_obj));
        return d.cast(
            int_bits, frac_bits, QuantizationMode::RND_INF, OverflowMode::WRAP, bits
        );
    } else if (nb::isinstance<APyFloat>(py_obj)) {
        const auto d = static_cast<APyFloat>(nb::cast<APyFloat>(py_obj));
        return d.to_fixed().cast(
            int_bits, frac_bits, QuantizationMode::RND_INF, OverflowMode::WRAP, bits
        );
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
    if (result._data.size() == 1) {
        unsigned shift_amount = 64 - (result._bits & (64 - 1));
        result._data[0] = fixed_point_from_double_single_limb(
            value, result.frac_bits(), shift_amount
        );
    } else {
        assert(result._data.size() > 1);
        fixed_point_from_double(
            value,
            std::begin(result._data),
            std::end(result._data),
            result._bits,
            result._int_bits
        );
    }
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
    std::vector<apy_limb_t> limbs {};
    if constexpr (APY_LIMB_SIZE_BITS == 32) {
        limbs = { apy_limb_t(man & 0xFFFFFFFF), apy_limb_t(man >> 32) };
    } else { /* APY_LIMB_SIZE_BITS == 64 */
        limbs = { apy_limb_t(man) };
    }

    // Calculate required number of bits
    const int bits = 1 + bit_width(man); // +1 For the sign
    const int int_bits = exp + bits;

    // Adjust result from sign
    if (sign_of_double(value)) {
        limb_vector_negate_inplace(limbs.begin(), limbs.end());
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
    APyFixed result(int_bits, frac_bits, bits);
    fixed_point_from_py_integer(
        value,
        std::begin(result._data),
        std::end(result._data),
        result._bits,
        result._int_bits
    );
    return result;
}

APyFixed APyFixed::from_unspecified_integer(const nb::int_& value)
{
    std::vector<apy_limb_t> limbs = python_long_to_limb_vec(value);

    // If the value is positive but the MSB is set, adding a zero limb
    // will prevent it from representing a negative value.
    if (!python_long_is_negative(value)
        && limb_vector_is_negative(std::begin(limbs), std::end(limbs))) {
        limbs.push_back(0);
    }
    const std::size_t res_bits = limbs.size() * APY_LIMB_SIZE_BITS;
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

//! Get bit pattern for the value one
APyFixed APyFixed::one(int bits, int int_bits)
{
    std::size_t limb_bit = (bits - int_bits) % APY_LIMB_SIZE_BITS;
    std::size_t limb_index = (bits - int_bits) / APY_LIMB_SIZE_BITS;
    ScratchVector<apy_limb_t> data(limb_index + 1, apy_limb_t(0));
    data[limb_index] |= apy_limb_t(1) << limb_bit;
    return APyFixed(bits, int_bits, data);
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
    fixed_point_cast_unsafe(
        std::begin(_data),
        std::end(_data),
        std::begin(result._data),
        std::end(result._data),
        _bits,
        _int_bits,
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
    // Result that temporarily can hold all the necessary bits
    APyFixed result(std::max(bits, _bits), std::max(int_bits, _int_bits));

    auto src_begin = std::begin(_data);
    auto src_end = std::end(_data);
    auto dst_begin = std::begin(result._data);
    auto dst_end = std::end(result._data);

    // Copy data into the result and sign extend
    limb_vector_copy_sign_extend(src_begin, src_end, dst_begin, dst_end);

    // Perform quantization
    quantize(
        dst_begin, dst_end, _bits, _int_bits, bits, int_bits, quantization, rnd64_fx
    );

    // Set the result bit-specifiers correctly
    result._bits = bits;
    result._int_bits = int_bits;
    result._data.resize(bits_to_limbs(bits));
    return result;
}
