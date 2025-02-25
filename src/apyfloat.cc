#include "apyfloat.h"
#include "apyfixed.h"
#include "apyfloat_util.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
namespace nb = nanobind;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fmt/format.h>

/* **********************************************************************************
 * * Constructors                                                                   *
 * **********************************************************************************
 */

void APyFloat::create_in_place(
    APyFloat* apyfloat,
    int sign,
    exp_t exp,
    man_t man,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    new (apyfloat) APyFloat(sign, exp, man, exp_bits, man_bits, bias);
}

APyFloat::APyFloat(
    bool sign,
    exp_t exp,
    man_t man,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias.value_or(ieee_bias()))
    , sign(sign)
    , exp(exp)
    , man(man)
{
}

APyFloat::APyFloat(
    bool sign,
    exp_t exp,
    man_t man,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    exp_t bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , sign(sign)
    , exp(exp)
    , man(man)
{
}

APyFloat::APyFloat(
    std::uint8_t exp_bits, std::uint8_t man_bits, std::optional<exp_t> bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias.value_or(ieee_bias()))
    , sign(false)
    , exp(0)
    , man(0)
{
}

APyFloat::APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , sign(false)
    , exp(0)
    , man(0)
{
}

APyFloat::APyFloat(
    const APyFloatData& data, std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , sign(data.sign)
    , exp(data.exp)
    , man(data.man)
{
}

/* **********************************************************************************
 * * Methods for conversions                                                        *
 * **********************************************************************************
 */

APyFloat APyFloat::from_number(
    const nb::object& py_obj, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    if (nb::isinstance<nb::int_>(py_obj)) {
        return APyFloat::from_integer(
            nb::cast<nb::int_>(py_obj), exp_bits, man_bits, bias
        );
    } else if (nb::isinstance<nb::float_>(py_obj)) {
        const auto d = static_cast<double>(nb::cast<nb::float_>(py_obj));
        return APyFloat::from_double(d, exp_bits, man_bits, bias);
    } else if (nb::isinstance<APyFixed>(py_obj)) {
        const auto d = static_cast<APyFixed>(nb::cast<APyFixed>(py_obj));
        return APyFloat::from_fixed(d, exp_bits, man_bits, bias);
    } else if (nb::isinstance<APyFloat>(py_obj
               )) { // One should really use `cast` instead
        const auto d = static_cast<APyFloat>(nb::cast<APyFloat>(py_obj));
        return d.cast(
            exp_bits, man_bits, bias, QuantizationMode::RND_CONV
        ); // TIES_EVEN
    } else {
        const nb::type_object type = nb::cast<nb::type_object>(py_obj.type());
        const nb::str type_string = nb::str(type);
        throw std::domain_error(
            std::string("Non supported type: ") + type_string.c_str()
        );
    }
}

APyFloat APyFloat::from_double(
    double value, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    APyFloat apytypes_double(
        sign_of_double(value), exp_of_double(value), man_of_double(value), 11, 52, 1023
    );
    return apytypes_double.cast_from_double(
        exp_bits, man_bits, bias.value_or(APyFloat::ieee_bias(exp_bits))
    );
}

APyFloat APyFloat::from_integer(
    const nb::int_ value, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    APyFixed apyfixed = APyFixed::from_unspecified_integer(value);
    // Custom version of 'from_fixed' since we know certain properties

    const exp_t actual_bias = bias.value_or(APyFloat::ieee_bias(exp_bits));

    if (apyfixed.is_zero()) {
        return APyFloat(0, 0, 0, exp_bits, man_bits, actual_bias);
    }

    APyFloat res(exp_bits, man_bits, actual_bias);

    // Get sign
    if (apyfixed.is_negative()) {
        res.sign = true;
        apyfixed = apyfixed.abs();
    } else {
        res.sign = false;
    }

    // Calculate exponent
    const exp_t target_exp
        = apyfixed.bits() - apyfixed.leading_zeros() - 1; // The bit width minus 1
    std::uint64_t tmp_exp = target_exp + res.bias;

    // Calculate mantissa
    apyfixed >>= target_exp;
    apyfixed = apyfixed.cast(3, man_bits, QuantizationMode::RND_CONV);

    // Check for overflow
    if (apyfixed.positive_greater_than_equal_pow2(1)) {
        ++tmp_exp;
        apyfixed >>= 1;
    }

    // Check for overflow
    if (tmp_exp >= res.max_exponent()) {
        res.exp = res.max_exponent();
        res.man = 0;
    } else {
        res.exp = tmp_exp;
        // Remove leading one
        apyfixed = apyfixed - APyFixed(2, 2, { 1 });
        res.man = apyfixed.get_lsbs();
    }

    return res;
}

APyFloat APyFloat::from_fixed(
    APyFixed apyfixed, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    const exp_t actual_bias = bias.value_or(APyFloat::ieee_bias(exp_bits));

    if (apyfixed.is_zero()) {
        return APyFloat(0, 0, 0, exp_bits, man_bits, actual_bias);
    }

    APyFloat res(exp_bits, man_bits, actual_bias);

    // Get sign
    if (apyfixed.is_negative()) {
        res.sign = true;
        apyfixed = apyfixed.abs();
    } else {
        res.sign = false;
    }

    // Calculate exponent
    const std::int64_t target_exp = apyfixed.int_bits() - apyfixed.leading_zeros() - 1;
    std::int64_t tmp_exp = target_exp + res.bias;

    // Calculate mantissa
    if (target_exp >= 0) {
        apyfixed >>= target_exp;
    } else {
        apyfixed <<= -target_exp;
    }

    if (tmp_exp <= 0) {
        apyfixed >>= -tmp_exp + 1;
        tmp_exp = 0;
    }

    apyfixed = apyfixed.cast(3, man_bits, QuantizationMode::RND_CONV);

    // Check for overflow
    if (apyfixed.positive_greater_than_equal_pow2(1)) {
        ++tmp_exp;
        apyfixed >>= 1;
    }

    // Check for overflow
    if (tmp_exp >= res.max_exponent()) {
        res.exp = res.max_exponent();
        res.man = 0;
    } else {
        res.exp = tmp_exp;
        // Remove leading one
        if (apyfixed.positive_greater_than_equal_pow2(0)) {
            apyfixed = apyfixed - APyFixed(2, 2, { 1 });
        }
        res.man = apyfixed.get_lsbs();
    }

    return res;
}

APyFloat APyFloat::cast(
    std::optional<int> new_exp_bits,
    std::optional<int> new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    const auto actual_exp_bits = new_exp_bits.value_or(exp_bits);
    const auto actual_man_bits = new_man_bits.value_or(man_bits);

    check_exponent_format(actual_exp_bits);
    check_mantissa_format(actual_man_bits);

    return _cast(
        actual_exp_bits,
        actual_man_bits,
        new_bias.value_or(APyFloat::ieee_bias(actual_exp_bits)),
        quantization.value_or(get_float_quantization_mode())
    );
}

APyFloat APyFloat::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    return _cast(
        new_exp_bits,
        new_man_bits,
        new_bias,
        quantization.value_or(get_float_quantization_mode())
    );
}

APyFloat APyFloat::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    QuantizationMode quantization
) const
{
    if (new_exp_bits == exp_bits && new_man_bits == man_bits && new_bias == bias) {
        return *this;
    }
    return _checked_cast(new_exp_bits, new_man_bits, new_bias, quantization);
}

APyFloat APyFloat::_checked_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    QuantizationMode quantization
) const
{
    APyFloat res(new_exp_bits, new_man_bits, new_bias);

    res.sign = sign;

    // Handle special values first
    if (is_max_exponent()) {
        if (man) {
            res.set_to_nan();
            return res;
        }
        res.set_to_inf();
        return res;
    }
    if (is_zero()) {
        res.set_to_zero();
        return res;
    }

    // Initial value for exponent
    std::int64_t new_exp = true_exp() + (std::int64_t)res.bias;

    // Normalize the exponent and mantissa if converting from a subnormal
    man_t prev_man = man;
    if (is_zero_exponent()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = new_exp - man_bits + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    }

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        if (new_exp < -static_cast<std::int64_t>(res.man_bits)) {
            // Exponent too small after rounding
            res.man = quantize_close_to_zero(sign, quantization);
            res.exp = 0;
            return res;
        }
        prev_man |= leading_one();
        res.exp = 0;
        // Cast mantissa
        res.man = prev_man;
        const int man_bits_delta = 1 - new_exp + (man_bits - new_man_bits);
        if (man_bits_delta > 0) {
            res.cast_mantissa_subnormal(man_bits_delta, quantization);
        } else {
            res.man <<= -man_bits_delta;
        }
        return res;
    }

    // Cast mantissa
    res.exp = new_exp;
    res.man = prev_man;
    res.man_bits = man_bits;
    res.cast_mantissa(new_man_bits, quantization);
    return res;
}

void APyFloat::cast_mantissa(std::uint8_t new_man_bits, QuantizationMode quantization)
{
    auto man_bits_delta = man_bits - new_man_bits;
    man_bits = new_man_bits;
    const auto max_exp = max_exponent();

    // Check if only zeros should be added
    if (man_bits_delta <= 0) {
        if (exp >= max_exp) {
            if (do_infinity(quantization, sign)) {
                exp = max_exp;
                man = 0;
            } else {
                exp = max_exp - 1;
                man = leading_one() - 1;
            }
        } else {
            man <<= -man_bits_delta;
        }
        return;
    }

    auto qntz_func = get_qntz_func(quantization);
    auto sticky = (1ULL << (man_bits_delta - 1)) - 1;
    qntz_func(man, exp, max_exp, man_bits_delta, sign, leading_one(), sticky);
}

// Simplified version of cast_mantissa when it is known that new_man_bits is shorter
// than man_bits
void APyFloat::cast_mantissa_shorter(
    std::uint8_t new_man_bits, QuantizationMode quantization
)
{
    auto man_bits_delta = man_bits - new_man_bits;
    man_bits = new_man_bits;
    const auto max_exp = max_exponent();
    auto qntz_func = get_qntz_func(quantization);
    auto sticky = (1ULL << (man_bits_delta - 1)) - 1;
    qntz_func(man, exp, max_exp, man_bits_delta, sign, leading_one(), sticky);
}

// Simplified version of cast_mantissa with exp = 0
void APyFloat::cast_mantissa_subnormal(
    std::uint8_t man_bits_delta, QuantizationMode quantization
)
{
    // The overflow check with max_exponent() is not needed here, but send it anyway
    auto qntz_func = get_qntz_func(quantization);
    auto sticky = (1ULL << (man_bits_delta - 1)) - 1;
    qntz_func(man, exp, max_exponent(), man_bits_delta, sign, leading_one(), sticky);
}

APyFloat APyFloat::_cast_to_double() const
{
    APyFloat res(11, 52, 1023);

    res.sign = sign;
    // Handle special values first
    if (is_max_exponent()) {
        if (man) {
            res.set_to_nan();
            return res;
        }
        res.set_to_inf();
        return res;
    }

    if (is_zero()) {
        res.set_to_zero();
        return res;
    }

    // Initial value for exponent
    std::int64_t new_exp = true_exp() + 1023;

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man = man;
    if (is_zero_exponent()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = new_exp - man_bits + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    }

    auto man_bits_delta = 52 - man_bits;

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        if (new_exp <= -52) { // Exponent too small
            res.set_to_zero();
            return res;
        }
        prev_man |= leading_one();
        // Prepare for right shift to adjust the mantissa
        man_bits_delta += new_exp - 1;
        new_exp = 0;
    }

    // Initial value for mantissa
    man_t new_man;

    if (man_bits_delta < 0) { // Quantization of mantissa needed
        const man_t bits_to_discard = -man_bits_delta;
        new_man = (prev_man >> bits_to_discard);
        // Calculate quantization bit
        man_t G, // Guard (bit after LSB)
            T,   // Sticky bit, logical OR of all the bits after the guard bit
            B;   // Quantization bit to add to LSB

        G = (prev_man >> (bits_to_discard - 1)) & 1;
        T = (prev_man & ((1ULL << (bits_to_discard - 1)) - 1)) != 0;

        // Using 'new_man' directly here is fine since G can only be '0' or '1',
        // thus calculating the LSB of 'new_man' is not needed.
        B = G & (new_man | T);

        new_man += B;
        if (static_cast<std::uint64_t>(new_man) > (1ULL << 52) - 1) {
            ++new_exp;
            new_man = 0;
        }
    } else {
        new_man = (prev_man << man_bits_delta);
    }

    if (new_exp >= 2047) {
        res.set_to_inf();
        return res;
    }

    res.man = new_man;
    res.exp = new_exp;
    return res;
}

APyFloat APyFloat::cast_from_double(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, exp_t new_bias
) const
{
    if (new_exp_bits == 11 && new_man_bits == 52 && new_bias == 1023) {
        return *this;
    }
    APyFloat res(new_exp_bits, new_man_bits, new_bias);

    res.sign = sign;
    // Handle special values first
    if (exp == 2047) {
        if (man) {
            res.set_to_nan();
            return res;
        }
        res.set_to_inf();
        return res;
    }
    if (is_zero()) {
        res.set_to_zero();
        return res;
    }

    // Initial value for exponent
    std::int64_t new_exp;

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man;
    if (is_zero_exponent()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = (std::int64_t)exp - 1074 + (std::int64_t)res.bias + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    } else {
        new_exp = (std::int64_t)exp - 1023 + (std::int64_t)res.bias;
        prev_man = man;
    }

    std::int8_t man_bits_delta = res.man_bits - 52;

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        if (new_exp < -static_cast<std::int64_t>(res.man_bits)) {
            // Exponent too small after rounding
            res.set_to_zero();
            return res;
        }
        prev_man |= leading_one();
        // Prepare for right shift to adjust the mantissa
        man_bits_delta += new_exp - 1;
        new_exp = 0;
    }

    // Initial value for mantissa
    man_t new_man;

    if (man_bits_delta < 0) { // Quantization of mantissa needed
        const man_t bits_to_discard = -man_bits_delta;
        new_man = prev_man >> bits_to_discard;
        // Calculate quantization bit
        man_t G, // Guard (bit after LSB)
            T,   // Sticky bit, logical OR of all the bits after the guard bit
            B;   // Quantization bit to add to LSB

        G = (prev_man >> (bits_to_discard - 1)) & 1;
        T = (prev_man & ((1ULL << (bits_to_discard - 1)) - 1)) != 0;

        // Using 'new_man' directly here is fine since G can only be '0' or '1',
        // thus calculating the LSB of 'new_man' is not needed.
        B = G & (new_man | T);

        new_man += B;
        if (static_cast<std::uint64_t>(new_man) > res.man_mask()) {
            ++new_exp;
            new_man = 0;
        }
    } else {
        new_man = (prev_man << man_bits_delta);
    }

    if (new_exp >= res.max_exponent()) {
        new_exp = res.max_exponent() - 1;
        new_man = res.man_mask();
    }

    res.man = new_man;
    res.exp = new_exp;
    return res;
}

//! Cast to a larger format
APyFloat APyFloat::cast_no_quant(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, std::optional<exp_t> new_bias
) const
{
    return cast_no_quant(
        new_exp_bits, new_man_bits, new_bias.value_or(APyFloat::ieee_bias(new_exp_bits))
    );
}

APyFloat APyFloat::cast_no_quant(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, exp_t new_bias
) const
{
    APyFloatSpec src_spec = { exp_bits, man_bits, bias };
    APyFloatSpec dst_spec = { new_exp_bits, new_man_bits, new_bias };
    APyFloatData res_data = ::cast_no_quant(get_data(), src_spec, dst_spec);
    return APyFloat(res_data, new_exp_bits, new_man_bits, new_bias);
}

double APyFloat::to_double() const
{
    const auto apytypes_d = _cast_to_double();
    double d {};
    set_sign_of_double(d, apytypes_d.sign);
    set_exp_of_double(d, apytypes_d.exp);
    set_man_of_double(d, apytypes_d.man);
    return d;
}

APyFloat::operator double() const { return to_double(); }

APyFloat APyFloat::from_bits(
    nb::int_ python_long_int_bit_pattern,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    APyFloat f(exp_bits, man_bits, bias);
    return f.update_from_bits(python_long_int_bit_pattern);
}

//! Create a floating-point object with the value one.
APyFloat
APyFloat::one(std::uint8_t exp_bits, std::uint8_t man_bits, std::optional<exp_t> bias)
{
    const exp_t res_bias = bias.value_or(APyFloat::ieee_bias(exp_bits));
    return APyFloat(0, res_bias, 0, exp_bits, man_bits, res_bias);
}

APyFloat& APyFloat::update_from_bits(nb::int_ python_long_int_bit_pattern)
{
    auto data_vec = python_long_to_limb_vec(python_long_int_bit_pattern);
    uint64_t low = uint64_t_from_limb_vector(data_vec, 0);

    man = low & man_mask();

    low >>= man_bits;
    exp = low & exp_mask();
    low >>= exp_bits;
    sign = low & 1;

    const int exp_man_bits = man_bits + exp_bits;
    if (data_vec.size() > 1) {
        auto high = data_vec[1];
        const int bits_left = exp_man_bits - 64;
        exp |= (high & (exp_mask() >> (exp_bits - bits_left)))
            << (exp_bits - bits_left);
        high >>= bits_left;
        sign |= high & 1;
    }
    return *this;
}

APyFloat& APyFloat::update_from_bits(std::uint64_t bits)
{
    assert((1 + exp_bits + man_bits) <= 64);

    man = bits & man_mask();
    bits >>= man_bits;

    exp = bits & exp_mask();
    bits >>= exp_bits;

    sign = bits & 1;

    return *this;
}

nb::int_ APyFloat::to_bits() const
{
    return apyfloat_to_bits({ sign, exp, man }, exp_bits, man_bits);
}

std::string APyFloat::str() const { return fmt::format("{:g}", to_double()); }

std::string APyFloat::repr() const
{
    std::string str = "APyFloat(sign=" + std::to_string(static_cast<int>(sign))
        + ", exp=" + std::to_string(exp) + ", man=" + std::to_string(man)
        + ", exp_bits=" + std::to_string(exp_bits)
        + ", man_bits=" + std::to_string(man_bits)
        + (bias == ieee_bias() ? std::string() : ", bias=" + std::to_string(bias))
        + ")";
    return str;
}

std::string APyFloat::latex() const
{
    if (is_nan()) {
        return "$\\textrm{NaN}$";
    } else if (is_inf()) {
        return "$\\infty$";
    } else if (is_zero()) {
        return "$0$";
    }

    std::string str = (sign ? "$-" : "$");
    if (is_normal()) {
        str += "\\left(1 + ";
    }
    str += "\\frac{" + std::to_string(man) + "}{2^{" + std::to_string(man_bits) + "}}";
    if (is_normal()) {
        str += "\\right)";
    }
    str += "2^{";
    if (is_normal()) {
        str += std::to_string(exp);
    } else {
        str += "1";
    }
    str += "-" + std::to_string(bias) + "} = ";
    if (sign) {
        str += "-";
    }
    str += std::to_string(true_man()) + "\\times 2^{"
        + std::to_string(true_exp() - man_bits) + "} = " + to_fixed().to_string_dec()
        + "$";

    return str;
}

APyFixed APyFloat::to_fixed() const
{
    if (is_nan()) {
        throw nb::value_error("Cannot convert nan to fixed-point");
    } else if (is_inf()) {
        throw nb::value_error("Cannot convert inf to fixed-point");
    } else if (is_zero()) {
        return APyFixed(1, 1, std::vector<apy_limb_t>({ 0 }));
    }

    APyFixed res(man_bits + 2, 2, { UINT64_TO_LIMB(true_man()) });
    if (sign) {
        res = -res;
    }
    const exp_t exponent = true_exp();
    if (exponent <= 0) {
        res >>= -exponent;
    } else {
        res <<= exponent;
    }
    return res;
}

/* ******************************************************************************
 * * Arithmetic operators                                                       *
 * ******************************************************************************
 */

APyFloat APyFloat::operator+(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    APyFloatData res_data;
    floating_point_sum(
        get_data(), rhs.get_data(), res_data, spec(), rhs.spec(), res.spec(), qntz
    );
    res.set_data(res_data);

    return res;
}

APyFloat APyFloat::operator-(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    APyFloatData res_data;
    floating_point_sum<true>(
        get_data(), rhs.get_data(), res_data, spec(), rhs.spec(), res.spec(), qntz
    );
    res.set_data(res_data);

    return res;
}

APyFloat APyFloat::operator-() const
{
    auto res = *this;
    res.sign = !sign;
    return res;
}

APyFloat& APyFloat::operator+=(const APyFloat& rhs)
{
    QuantizationMode qntz = get_float_quantization_mode();
    APyFloatData res_data;
    floating_point_sum(
        get_data(), rhs.get_data(), res_data, spec(), rhs.spec(), spec(), qntz
    );
    set_data(res_data);
    return *this;
}

APyFloat APyFloat::operator*(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    // Perform the product
    APyFloatData res_data;
    floating_point_product(
        get_data(), rhs.get_data(), res_data, spec(), rhs.spec(), res.spec(), qntz
    );
    res.set_data(res_data);

    return res;
}

APyFloat APyFloat::operator/(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    // Perform the division
    APyFloatData res_data;
    floating_point_quotient(
        get_data(), rhs.get_data(), res_data, spec(), rhs.spec(), res.spec(), qntz
    );
    res.set_data(res_data);

    return res;
}

/* ******************************************************************************
 * * Mathematical functions *
 * ******************************************************************************
 */

APyFloat APyFloat::abs() const
{
    auto res = *this;
    res.sign = false;
    return res;
}

APyFloat APyFloat::pow(const APyFloat& x, const APyFloat& y)
{
    throw NotImplementedException(
        "Not implemented: APyFloat to the power of another APyFloat."
    );
}

APyFloat APyFloat::pown(const APyFloat& x, int n)
{
    if (n < 0) {
        throw NotImplementedException("Not implemented: power with negative integers.");
    }

    // Handling of special cases based on the 754-2019 standard
    if (x.is_nan() || (n == 1)) {
        return x;
    }

    if (n == 0) {
        return APyFloat(0, x.bias, 0, x.exp_bits, x.man_bits, x.bias); // Return '1'
    }

    // Early exit for one of the most common cases
    if (n == 2) {
        return x * x;
    }

    bool new_sign = x.sign & (n % 2);

    if (x.is_zero()) {
        if (n < 0) {
            return x.construct_inf(new_sign);
        } else {
            return x.construct_zero(new_sign);
        }
    }

    if (x.is_inf()) {
        new_sign &= x.sign;

        if (n > 0) {
            return x.construct_inf(new_sign);
        } else {
            return x.construct_zero(new_sign);
        }
    }

    const auto quantization = get_float_quantization_mode();

    const int abs_n = std::abs(n);

    int tmp_man_bits = x.man_bits;

    std::int64_t new_exp = (static_cast<std::int64_t>(x.exp) - x.bias) * n + x.bias;

    std::uint64_t new_man = 0, mx = x.true_man();

    const int x_is_normal = x.is_normal();
    const unsigned int max_man_bits = abs_n * (x.man_bits + x_is_normal);

    if (max_man_bits <= _MAN_T_SIZE_BITS) {
        new_man = ipow(mx, abs_n);

        // Calculate carries from mantissa to perform normalization
        const int man_width = bit_width(new_man);
        const int num_bits_no_carry
            = max_man_bits - abs_n + 1; // width if no carry was generated
        const int carries = man_width - num_bits_no_carry;

        // carries < 0 can occur with subnormal numbers
        if (carries >= 0) {
            new_exp += carries;
            tmp_man_bits = man_width - x_is_normal;
        }

        // Handle subnormal case
        if (new_exp <= 0) {
            tmp_man_bits += -new_exp + 1;
            new_exp = 0;
        }

        // If a leading one was added, mask it away
        if (x_is_normal) {
            new_man &= (1ULL << tmp_man_bits) - 1;
        }

        APyFloat res(new_sign, new_exp, new_man, x.exp_bits, tmp_man_bits, x.bias);
        res.cast_mantissa_shorter(x.man_bits, quantization);
        return res;
    }

    // Slow path
    const APyFixed apy_mx(2 + x.man_bits, 2, { UINT64_TO_LIMB(mx) });
    APyFixed apy_res = apy_mx.ipow(abs_n);

    // Normalize mantissa
    while (apy_res.positive_greater_than_equal_pow2(1)) {
        apy_res >>= 1;
        new_exp++;
    }

    // Handle subnormal case
    if (new_exp <= 0) {
        apy_res >>= std::abs(new_exp) + 1;
        new_exp = 0;
    }

    // Quantize mantissa
    quantize_apymantissa(apy_res, new_sign, x.man_bits, quantization);

    // Carry from quantization
    if (apy_res.positive_greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    }

    if (new_exp >= x.max_exponent()) {
        return x.construct_inf();
    }

    if (apy_res.positive_greater_than_equal_pow2(0)) { // Remove leading one
        apy_res = apy_res - APyFixed(2, 2, { 1 });
    }
    apy_res <<= x.man_bits;
    new_man = (man_t)(apy_res).to_double();
    return APyFloat(new_sign, new_exp, new_man, x.exp_bits, x.man_bits, x.bias);
}

/* ******************************************************************************
 * * Binary logic operators *
 * ******************************************************************************
 */

APyFloat APyFloat::operator&(const APyFloat& rhs) const
{
    if (same_type_as(rhs)) {
        return APyFloat(
            sign & rhs.sign, exp & rhs.exp, man & rhs.man, exp_bits, man_bits
        );
    }

    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

    return APyFloat(
        lhs_big.sign & rhs_big.sign,
        lhs_big.exp & rhs_big.exp,
        lhs_big.man & rhs_big.man,
        max_exp_bits,
        max_man_bits
    );
}

APyFloat APyFloat::operator|(const APyFloat& rhs) const
{
    if (same_type_as(rhs)) {
        return APyFloat(
            sign | rhs.sign, exp | rhs.exp, man | rhs.man, exp_bits, man_bits
        );
    }
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

    return APyFloat(
        lhs_big.sign | rhs_big.sign,
        lhs_big.exp | rhs_big.exp,
        lhs_big.man | rhs_big.man,
        max_exp_bits,
        max_man_bits
    );
}

APyFloat APyFloat::operator^(const APyFloat& rhs) const
{
    if (same_type_as(rhs)) {
        return APyFloat(
            sign ^ rhs.sign, exp ^ rhs.exp, man ^ rhs.man, exp_bits, man_bits
        );
    }
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

    return APyFloat(
        lhs_big.sign ^ rhs_big.sign,
        lhs_big.exp ^ rhs_big.exp,
        lhs_big.man ^ rhs_big.man,
        max_exp_bits,
        max_man_bits
    );
}

APyFloat APyFloat::operator~() const
{
    return APyFloat(
        !sign,
        ~exp & ((1ULL << exp_bits) - 1),
        ~man & ((1ULL << man_bits) - 1),
        exp_bits,
        man_bits
    );
}

/* ******************************************************************************
 * * Binary comparison operators *
 * ******************************************************************************
 */

bool APyFloat::operator==(const APyFloat& rhs) const
{
    if (sign != rhs.sign && !(is_zero() && rhs.is_zero())) {
        return false;
    }

    if (is_nan() || rhs.is_nan()) {
        return false;
    }

    if (same_type_as(rhs)) {
        return (exp == rhs.exp) && (man == rhs.man);
    } else {

        // Cast operands to a larger format that can represent both numbers
        const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
        const auto max_man_bits = std::max(man_bits, rhs.man_bits);
        const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
        const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
        const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

        return (lhs_big.exp == rhs_big.exp) && (lhs_big.man == rhs_big.man);
    }
}

bool APyFloat::is_identical(const APyFloat& other) const
{
    return (sign == other.sign) && (exp == other.exp) && (bias == other.bias)
        && (man == other.man) && (exp_bits == other.exp_bits)
        && (man_bits == other.man_bits);
}

bool APyFloat::operator!=(const APyFloat& rhs) const
{
    if (is_nan() || rhs.is_nan()) {
        return false;
    } else {
        return !(*this == rhs);
    }
}

bool APyFloat::operator<=(const APyFloat& rhs) const
{
    return (*this < rhs) || (*this == rhs);
}

bool APyFloat::operator<(const APyFloat& rhs) const
{
    return floating_point_less_than(get_data(), spec(), rhs.get_data(), rhs.spec());
}

bool APyFloat::operator>=(const APyFloat& rhs) const
{
    return (*this > rhs) || (*this == rhs);
}

bool APyFloat::operator>(const APyFloat& rhs) const
{
    if (is_nan() || rhs.is_nan() || (*this == rhs)) {
        return false;
    } else {
        return !(*this < rhs);
    }
}

bool APyFloat::operator==(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this == rhs_fp);
}

bool APyFloat::operator!=(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this != rhs_fp);
}

bool APyFloat::operator<=(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this <= rhs_fp);
}

bool APyFloat::operator<(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this < rhs_fp);
}

bool APyFloat::operator>=(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this >= rhs_fp);
}

bool APyFloat::operator>(const double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this > rhs_fp);
}

bool APyFloat::operator==(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        return false;
    }
    return ((to_fixed()) == rhs);
}

bool APyFloat::operator!=(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        return true;
    }
    return ((to_fixed()) != rhs);
}

bool APyFloat::operator<=(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        if (man == 0) { // inf
            return sign;
        }
        return false;
    }

    return ((to_fixed()) <= rhs);
}

bool APyFloat::operator<(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        if (man == 0) { // inf
            return sign;
        }
        return false;
    }

    return ((to_fixed()) < rhs);
}

bool APyFloat::operator>=(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        if (man == 0) { // inf
            return !sign;
        }
        return false;
    }

    return ((to_fixed()) >= rhs);
}

bool APyFloat::operator>(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        if (man == 0) { // inf
            return !sign;
        }
        return false;
    }

    return ((to_fixed()) > rhs);
}

/* ******************************************************************************
 * * Helper functions *
 * ******************************************************************************
 */

APyFloat APyFloat::construct_zero(std::optional<bool> new_sign) const
{
    return APyFloat(new_sign.value_or(sign), 0, 0, exp_bits, man_bits, bias);
}

APyFloat APyFloat::construct_inf(std::optional<bool> new_sign) const
{
    return construct_nan(new_sign, 0);
}

APyFloat
APyFloat::construct_nan(std::optional<bool> new_sign, man_t payload /*= 1*/) const
{
    return APyFloat(
        new_sign.value_or(sign), max_exponent(), payload, exp_bits, man_bits, bias
    );
}

//! Add mantissa bits so that a number is no longer subnormal
APyFloat APyFloat::normalized() const
{
    if (!is_subnormal()) {
        return *this;
    }

    man_t new_man = man;
    std::int64_t tmp_exp = true_exp();

    while (!(new_man & leading_one())) {
        new_man <<= 1;
        tmp_exp--;
    }

    // Possibly use more exponent bits
    int new_exp_bits = exp_bits;
    exp_t extended_bias = bias;
    auto new_exp = tmp_exp + extended_bias;
    while (new_exp <= 0) {
        new_exp_bits++;
        extended_bias = APyFloat::ieee_bias(new_exp_bits);
        new_exp = tmp_exp + extended_bias;
    }

    return APyFloat(sign, new_exp, new_man, new_exp_bits, man_bits, extended_bias);
}

/*
 * Convenience methods
 */
APyFloat APyFloat::cast_to_double(std::optional<QuantizationMode> quantization) const
{
    return _cast(11, 52, 1023, quantization);
}

APyFloat APyFloat::cast_to_single(std::optional<QuantizationMode> quantization) const
{
    return _cast(8, 23, 127, quantization);
}

APyFloat APyFloat::cast_to_half(std::optional<QuantizationMode> quantization) const
{
    return _cast(5, 10, 15, quantization);
}

APyFloat APyFloat::cast_to_bfloat16(std::optional<QuantizationMode> quantization) const
{
    return _cast(8, 7, 127, quantization);
}

APyFloat APyFloat::next_up() const
{
    if (is_zero()) {
        return APyFloat(0, 0, 1, exp_bits, man_bits, bias);
    } else if (is_nan()) {
        return *this;
    }

    if (sign) {
        const man_t new_man = man - 1; // Underflow is defined for unsigned numbers

        if (new_man >= leading_one()) {
            return APyFloat(1, exp - 1, man_mask(), exp_bits, man_bits, bias);
        }
        return APyFloat(1, exp, new_man, exp_bits, man_bits, bias);
    }

    if (is_max_exponent()) {
        return *this;
    }

    const man_t new_man = man + 1;

    if (new_man >= leading_one()) {
        return APyFloat(0, exp + 1, 0, exp_bits, man_bits, bias);
    }
    return APyFloat(0, exp, new_man, exp_bits, man_bits, bias);
}

APyFloat APyFloat::next_down() const
{
    // next_down(x) should behave as -next_up(-x)

    if (is_zero()) {
        return APyFloat(1, 0, 1, exp_bits, man_bits, bias);
    } else if (is_nan()) {
        return *this;
    }

    if (sign) {
        if (is_max_exponent()) {
            return *this;
        }

        const man_t new_man = man + 1;

        if (new_man >= leading_one()) {
            return APyFloat(1, exp + 1, 0, exp_bits, man_bits, bias);
        }
        return APyFloat(1, exp, new_man, exp_bits, man_bits, bias);
    }

    const man_t new_man = man - 1; // Underflow is defined for unsigned numbers

    if (new_man >= leading_one()) {
        return APyFloat(0, exp - 1, man_mask(), exp_bits, man_bits, bias);
    }
    return APyFloat(0, exp, new_man, exp_bits, man_bits, bias);
}
