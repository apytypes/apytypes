#include <nanobind/nanobind.h>
namespace nb = nanobind;

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>

#include <fmt/format.h>

#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apytypes_util.h"
#include "python_util.h"

#include "ieee754.h"

/*!
 * APyFloat word length limits.
 */
static constexpr std::size_t _MAN_LIMIT_BITS = _MAN_T_SIZE_BITS - 3;
static constexpr std::size_t _EXP_LIMIT_BITS = _EXP_T_SIZE_BITS - 2;

constexpr bool PRINT_WARNINGS = false;

void print_warning(const std::string msg)
{
    if constexpr (PRINT_WARNINGS) {
        std::cerr << "Warning: " << msg;
    }
}

/* **********************************************************************************
 * * Constructors                                                                   *
 * **********************************************************************************
 */

void APyFloat::create_in_place(
    APyFloat* apyfloat,
    int sign,
    exp_t exp,
    man_t man,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    const exp_t ieee_bias = APyFloat::ieee_bias(exp_bits);
    if (bias.has_value() && bias.value() != ieee_bias) {
        print_warning("non 'ieee-like' biases are not sure to work yet.\n");
    }

    if (exp_bits > _EXP_LIMIT_BITS) {
        throw nb::value_error("Too many bits for the exponent field.");
    }

    if (man_bits > _MAN_LIMIT_BITS) {
        throw nb::value_error("Too many bits for the mantissa field.");
    }

    new (apyfloat)
        APyFloat(sign, exp, man, exp_bits, man_bits, bias.value_or(ieee_bias));
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

APyFloat APyFloat::from_double(
    double value,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    APyFloat apytypes_double(
        sign_of_double(value), exp_of_double(value), man_of_double(value), 11, 52, 1023
    );
    return apytypes_double.cast_from_double(
        exp_bits, man_bits, bias.value_or(APyFloat::ieee_bias(exp_bits))
    );
}

APyFloat APyFloat::cast(
    std::optional<uint8_t> new_exp_bits,
    std::optional<uint8_t> new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    const auto actual_exp_bits = new_exp_bits.value_or(exp_bits);
    return _cast(
        actual_exp_bits,
        new_man_bits.value_or(man_bits),
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
    APyFloat res(new_exp_bits, new_man_bits, new_bias);
    res.sign = sign;

    // Handle special values first
    if (is_max_exponent()) {
        if (man) {
            return res.construct_nan(sign);
        }
        return res.construct_inf(sign);
    }
    if (is_zero()) {
        return res.construct_zero(sign);
    }

    // Initial value for exponent
    std::int64_t new_exp = true_exp() + (std::int64_t)res.bias;

    // Normalize the exponent and mantissa if converting from a subnormal
    man_t prev_man = man;
    if (is_subnormal()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = new_exp - man_bits + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    }

    if (new_exp < -static_cast<std::int64_t>(res.man_bits
        )) { // Exponent too small after rounding
        return res.construct_zero();
    }

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        prev_man |= leading_one();
        res.exp = 0;
        // Cast mantissa
        // The mantissa is temporarily set to a larger format in order to use
        // 'cast_mantissa_subnormal'
        res.man = prev_man;
        res.man_bits = man_bits + -new_exp + 1;

        res.cast_mantissa_subnormal(new_man_bits, quantization);
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

    quantize_mantissa(
        man, exp, max_exp, man_bits_delta, sign, leading_one(), quantization
    );
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
    quantize_mantissa(
        man, exp, max_exp, man_bits_delta, sign, leading_one(), quantization
    );
}

// Simplified version of cast_mantissa with exp = 0
void APyFloat::cast_mantissa_subnormal(
    std::uint8_t new_man_bits, QuantizationMode quantization
)
{
    auto man_bits_delta = man_bits - new_man_bits;
    man_bits = new_man_bits;

    // Check if only zeros should be added
    if (man_bits_delta <= 0) {
        man <<= -man_bits_delta;
        return;
    }

    // The overflow check with max_exponent() is not needed here, but send it anyway
    quantize_mantissa(
        man, exp, max_exponent(), man_bits_delta, sign, leading_one(), quantization
    );
}

APyFloat APyFloat::_cast_to_double() const
{
    APyFloat res(11, 52, 1023);
    res.sign = sign;

    // Handle special values first
    if (is_max_exponent()) {
        if (man) {
            return res.construct_nan(sign);
        }
        return res.construct_inf(sign);
    }

    if (is_zero()) {
        return res.construct_zero(sign);
    }

    // Initial value for exponent
    std::int64_t new_exp = true_exp() + 1023;

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man = man;
    if (is_subnormal()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = new_exp - man_bits + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    }

    auto man_bits_delta = 52 - man_bits;

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        if (new_exp <= -52) { // Exponent too small
            return res.construct_zero();
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
        return res.construct_inf();
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
            return res.construct_nan(sign);
        }
        return res.construct_inf(sign);
    }
    if (is_zero()) {
        return res.construct_zero(sign);
    }

    // Initial value for exponent
    std::int64_t new_exp;

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man;
    if (is_subnormal()) {
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
            return res.construct_zero();
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
        if (do_infinity(get_float_quantization_mode(), res.sign)) {
            new_exp = res.max_exponent();
            new_man = 0;
        } else {
            new_exp = res.max_exponent() - 1;
            new_man = res.man_mask();
        }
    }

    res.man = new_man;
    res.exp = new_exp;
    return res;
}

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
    APyFloat res(new_exp_bits, new_man_bits, new_bias);
    res.sign = sign;

    // Handle special values first
    if (is_max_exponent()) {
        if (man) {
            return res.construct_nan(sign);
        }
        return res.construct_inf(sign);
    }
    if (is_zero()) {
        return res.construct_zero(sign);
    }

    // Initial value for exponent
    std::int64_t new_exp = true_exp() + (std::int64_t)res.bias;

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man = man;
    if (is_subnormal()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = new_exp - man_bits + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    }

    // Initial value for mantissa
    man_t new_man = prev_man << (res.man_bits - man_bits);

    // This can only happen here if we play around with bias values, which we currently
    // do not, so commenting out for now
    /* if (new_exp <= 0) { // The number will be converted to a subnormal in the new
    format new_man |= res.leading_one();             // Add leading one new_man <<=
    (res.man_bits + new_exp - 1); // Shift the difference between E_min
                                                  // and exp
        new_man /= 1ULL << res.man_bits; // Divide by the minimum subnorm (i.e. E_min)
        new_man &= res.man_mask();       // Mask away the leading ones
        new_exp = 0;
    } */

    res.man = new_man;
    res.exp = new_exp;
    return res;
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
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    APyFloat f(exp_bits, man_bits, bias);
    return f.update_from_bits(python_long_int_bit_pattern);
}

APyFloat& APyFloat::update_from_bits(nb::int_ python_long_int_bit_pattern)
{
    auto data_vec = python_long_to_limb_vec(python_long_int_bit_pattern);
    auto low = data_vec[0];

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

nb::int_ APyFloat::to_bits() const
{
    mp_limb_t lower = man;
    const int exp_man_bits = exp_bits + man_bits;
    lower |= (std::uint64_t)exp << man_bits;
    lower |= (std::uint64_t)sign << exp_man_bits;

    const int bits = _LIMB_SIZE_BITS;
    mp_limb_t higher = (std::uint64_t)exp >> (bits - man_bits);

    const int high_sign_delta = bits - exp_man_bits;
    if (high_sign_delta < 0) {
        higher |= sign << -high_sign_delta;
    }

    auto limb_vec = std::vector<mp_limb_t> { lower, higher };
    return python_limb_vec_to_long(
        limb_vec.begin(), limb_vec.end(), false, bits % (1 + exp_man_bits)
    );
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
        throw nb::value_error("Cannot convert NaN to APyFixed.");
    } else if (is_inf()) {
        throw nb::value_error("Cannot convert inf to APyFixed.");
    } else if (is_zero()) {
        return APyFixed(1, 1, std::vector<mp_limb_t>({ 0 }));
    }

    APyFixed res(man_bits + 2, 2, std::vector<mp_limb_t>({ true_man() }));
    if (sign) {
        res = -res;
    }
    exp_t exponent = true_exp();
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
    APyFloat res, x, y;
    // Handle the zero cases, other special cases are further down
    if (same_type_as(rhs)) {
        if (is_zero()) {
            return rhs;
        }
        if (rhs.is_zero()) {
            return *this;
        }
        res = APyFloat(exp_bits, man_bits, bias);
        // Handle the NaN and inf cases
        if (is_max_exponent() || rhs.is_max_exponent()) {
            if (is_nan() || rhs.is_nan()
                || (sign != rhs.sign && is_inf() && rhs.is_inf())) {
                res.set_to_nan();
                return res;
            }
            res.sign = is_max_exponent() ? sign : rhs.sign;
            res.set_to_inf();
            return res;
        }
        x = *this;
        y = rhs;
        // Compute sign and swap operands if need to make sure |x| >= |y|
        if (exp < rhs.exp || (exp == rhs.exp && man < rhs.man)) {
            std::swap(x, y);
        } else if (sign != rhs.sign && exp == rhs.exp && man == rhs.man) {
            res.set_to_zero(true);
            return res;
        }
    } else {
        std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
        exp_t res_bias = APyFloat::ieee_bias(res_exp_bits);
        // Cast once to resulting word length to get faster comparisons later
        x = cast_no_quant(res_exp_bits, res_man_bits, res_bias);
        y = rhs.cast_no_quant(res_exp_bits, res_man_bits, res_bias);
        if (is_zero()) {
            return y;
        }
        if (rhs.is_zero()) {
            return x;
        }
        res = APyFloat(res_exp_bits, res_man_bits, res_bias);
        // Handle the NaN and inf cases
        if (is_max_exponent() || rhs.is_max_exponent()) {
            if (is_nan() || rhs.is_nan()
                || (sign != rhs.sign && is_inf() && rhs.is_inf())) {
                res.set_to_nan();
                return res;
            }
            res.sign = is_max_exponent() ? sign : rhs.sign;
            res.set_to_inf();
            return res;
        }
        if (x.abs() < y.abs()) {
            std::swap(x, y);
        } else if (sign != rhs.sign && x.exp == y.exp && x.man == y.man) {
            res.set_to_zero(true);
            return res;
        }
    }
    res.sign = x.sign;

    const auto quantization = get_float_quantization_mode();

    const auto x_true_exp = x.true_exp();
    // Tentative exponent
    std::int64_t new_exp = x_true_exp + res.bias;

    // Conditionally add leading one's
    man_t mx = x.true_man();
    man_t my = y.true_man();

    // Align mantissas based on exponent difference
    const unsigned exp_delta = x_true_exp - y.true_exp();

    // +5 to give room for leading one, carry, and 3 guard bits
    // A tighter bound would sometimes be sufficient, but checking that is
    // probably not worth it
    const unsigned int max_man_bits = res.man_bits + 5;
    if (max_man_bits <= _MAN_T_SIZE_BITS
        && quantization != QuantizationMode::STOCH_WEIGHTED) {
        // Add room for guard bits
        mx <<= 3;
        my <<= 3;

        // Align mantissa based on difference in exponent
        man_t highY;
        if (exp_delta <= 3) {
            highY = my >> exp_delta;
        } else if (exp_delta >= max_man_bits) {
            highY = (my >> max_man_bits) | 1;
        } else {
            highY = (my >> exp_delta) | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
        }

        // Perform addition / subtraction
        man_t new_man = (sign == rhs.sign) ? mx + highY : mx - highY;

        // Check for carry and cancellation
        int c = 0;
        const man_t res_leading_one = res.leading_one() << 3;
        if (new_man & (res_leading_one << 1)) {
            // Carry
            c = 1;
            new_exp++;
        } else if (new_man & res_leading_one) {
            // Do nothing
        } else {
            // Cancellation or addition with subnormals
            // Mantissa should be shifted until 1.xx is obtained or new_exp
            // equals 0
            const unsigned int man_leading_zeros = leading_zeros(new_man);
            const unsigned int normalizing_shift = man_leading_zeros
                - (_MAN_T_SIZE_BITS - res.man_bits - 4
                ); // -4 for leading 1 and 3 guard bits

            if (new_exp > normalizing_shift) {
                new_man <<= normalizing_shift;
                new_exp -= normalizing_shift;
            } else {
                // The result will be a subnormal
                // -1 is for compensating that 1.xx is not desired here
                new_man <<= new_exp - 1;
                new_exp = 0;
            }
        }

        new_man &= (res_leading_one << c) - 1;

        // Use longer format for intermediate result and quantize mantissa
        res.exp = new_exp;
        res.man = new_man;
        std::uint8_t res_man_bits = res.man_bits;
        res.man_bits = res.man_bits + 3 + c; // +3 is from the added guard bits
        res.cast_mantissa_shorter(
            res_man_bits, quantization
        ); // The exponent is also updated here if needed
        return res;
    }
    // Slower path

    // Two integer bits, sign bit and leading one
    APyFixed apy_mx(2 + x.man_bits, 2, std::vector<mp_limb_t>({ mx }));
    APyFixed apy_my(2 + y.man_bits, 2 - exp_delta, std::vector<mp_limb_t>({ my }));

    // Perform addition/subtraction
    auto apy_res = (x.sign == y.sign) ? apy_mx + apy_my : apy_mx - apy_my;

    if (apy_res.positive_greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    } else {
        // Check for cancellation by counting the number of left shifts needed
        // to make fx>=1.0
        const int leading_zeros = leading_zeros_apyfixed(apy_res);
        if (leading_zeros) {
            if (new_exp > leading_zeros) {
                new_exp -= leading_zeros;
                apy_res <<= leading_zeros;
            } else {
                // The result will be a subnormal
                // -1 is for compensating that 1.xx is not desired here
                apy_res <<= int(new_exp - 1);
                new_exp = 0;
            }
        }
    }

    // Quantize mantissa
    quantize_apymantissa(apy_res, res.sign, res.man_bits, quantization);

    // Carry from quantization. In practice, the exponent will never be
    // incremented twice
    if (apy_res.positive_greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    }

    // Check for overflow
    if (new_exp >= res.max_exponent()) {
        if (do_infinity(quantization, res.sign)) {
            res.exp = res.max_exponent();
            res.man = 0;
        } else {
            res.exp = res.max_exponent() - 1;
            res.man = res.man_mask();
        }
        return res;
    }

    // Remove leading one
    if (apy_res.positive_greater_than_equal_pow2(0)) {
        apy_res = apy_res - fx_one;
    }

    apy_res <<= res.man_bits;
    // TODO: Get bit-pattern directly
    res.man = (man_t)(apy_res).to_double();
    res.exp = new_exp;
    return res;
}

APyFloat APyFloat::operator-(const APyFloat& y) const { return *this + (-y); }

APyFloat APyFloat::operator-() const
{
    auto res = *this;
    res.sign = !sign;
    return res;
}

APyFloat& APyFloat::operator+=(const APyFloat& rhs)
{
    assert(same_type_as(rhs));
    // Check that only the slow case is using this method
    // If this fails, add a fast path
    assert(man_bits + 5 > _MAN_T_SIZE_BITS);

    // Handle the zero cases, other special cases are further down
    if (is_zero()) {
        sign = rhs.sign;
        man = rhs.man;
        exp = rhs.exp;
        return *this;
    }
    if (rhs.is_zero()) {
        return *this;
    }
    // Handle the NaN and inf cases
    if (is_max_exponent() || rhs.is_max_exponent()) {
        if (is_nan() || rhs.is_nan()
            || (sign != rhs.sign && is_inf() && rhs.is_inf())) {
            set_to_nan();
            return *this;
        }
        if (rhs.is_max_exponent()) {
            sign = rhs.sign;
            set_to_inf();
        }
        // Otherwise *this is already inf so nothing should be done, just return
        return *this;
    }

    // Compute if the signs are the same to be able to overwrite this.sign
    bool same_sign = sign == rhs.sign;
    const APyFloat* x = &*this;
    const APyFloat* y = &rhs;
    // Compute sign and swap operands if need to make sure |x| >= |y|
    if (exp < rhs.exp || (exp == rhs.exp && man < rhs.man)) {
        sign = rhs.sign;
        std::swap(x, y);
    } else if (!same_sign && exp == rhs.exp && man == rhs.man) {
        set_to_zero(true);
        return *this;
    }

    const auto quantization = get_float_quantization_mode();

    // Compute smaller exponent so that one can overwrite exp later
    exp_t smaller_exp = y->exp + y->is_subnormal();

    // Conditionally add leading one's
    man_t mx = x->true_man();
    man_t my = y->true_man();

    // Tentative exponent, write directly
    exp = x->exp + x->is_subnormal();

    // Align mantissas based on exponent difference
    const unsigned exp_delta = exp - smaller_exp;

    // Slower path only as fast path is not currently used

    // Two integer bits, sign bit and leading one
    APyFixed apy_mx(2 + man_bits, 2, std::vector<mp_limb_t>({ mx }));
    APyFixed apy_my(2 + man_bits, 2 - exp_delta, std::vector<mp_limb_t>({ my }));

    // Perform addition/subtraction
    auto apy_res = (same_sign) ? apy_mx + apy_my : apy_mx - apy_my;

    if (apy_res.positive_greater_than_equal_pow2(1)) {
        exp++;
        apy_res >>= 1;
    } else {
        // Check for cancellation by counting the number of left shifts needed
        // to make fx>=1.0
        const unsigned int leading_zeros = leading_zeros_apyfixed(apy_res);
        if (leading_zeros) {
            if (exp > leading_zeros) {
                exp -= leading_zeros;
                apy_res <<= leading_zeros;
            } else {
                // The result will be a subnormal
                // -1 is for compensating that 1.xx is not desired here
                apy_res <<= int(exp - 1);
                exp = 0;
            }
        }
    }

    // Quantize mantissa
    quantize_apymantissa(apy_res, sign, man_bits, quantization);

    // Carry from quantization. In practice, the exponent will never be
    // incremented twice
    if (apy_res.positive_greater_than_equal_pow2(1)) {
        exp++;
        apy_res >>= 1;
    }

    // Check for overflow
    const exp_t max_exp = max_exponent();
    if (exp >= max_exp) {
        if (do_infinity(quantization, sign)) {
            exp = max_exp;
            man = 0;
        } else {
            exp = max_exp - 1;
            man = leading_one() - 1;
        }
        return *this;
    }

    // Remove leading one
    if (apy_res.positive_greater_than_equal_pow2(0)) {
        apy_res = apy_res - fx_one;
    }

    apy_res <<= man_bits;
    // TODO: Get bit-pattern directly
    man = (man_t)(apy_res).to_double();
    return *this;
}

APyFloat APyFloat::operator*(const APyFloat& y) const
{
    auto res_exp_bits = std::max(exp_bits, y.exp_bits);
    auto res_man_bits = std::max(man_bits, y.man_bits);
    APyFloat res(res_exp_bits, res_man_bits);

    // Calculate sign
    res.sign = sign ^ y.sign;

    // Handle special operands
    if (is_max_exponent() || y.is_max_exponent() || is_zero() || y.is_zero()) {
        if (is_nan() || y.is_nan() || (is_inf() && y.is_zero())
            || (is_zero() && y.is_inf())) {
            return res.construct_nan();
        }

        if (is_inf() || y.is_inf()) {
            return res.construct_inf();
        }

        if (is_zero() || y.is_zero()) {
            return res.construct_zero();
        }
    }
    const auto quantization = get_float_quantization_mode();
    const unsigned int sum_man_bits = man_bits + y.man_bits;

    if (sum_man_bits + 3 <= _MAN_T_SIZE_BITS) {
        const auto new_man_bits = sum_man_bits + 2;
        const auto man_bits_delta = new_man_bits - res.man_bits;

        // Tentative exponent
        std::int64_t tmp_exp = true_exp() + y.true_exp() + res.bias;
        const man_t mx = true_man();
        const man_t my = y.true_man();
        const man_t two = 1ULL << (new_man_bits);
        const man_t two_before = 1ULL << (new_man_bits - 1);
        const man_t one_before = 1ULL << (new_man_bits - 2);
        const man_t two_res = res.leading_one();

        man_t new_man = mx * my;

        // Check result from multiplication larger than/equal two
        if (new_man & two_before) {
            tmp_exp++;
            new_man <<= 1;
        } else if (new_man & one_before) {
            // Align with longer result
            new_man <<= 2;
        } else {
            // One or two of the operands were subnormal.
            // If the exponent is positive, the result is normalized by
            // left-shifting until the exponent is zero or the mantissa is 1.xx
            const int leading_zeros = 1 + sum_man_bits - bit_width(new_man);
            const int shift = std::max(
                std::min(tmp_exp, (std::int64_t)leading_zeros), (std::int64_t)0
            );
            tmp_exp -= shift;
            // + 2 to align with longer result
            new_man <<= shift + 2;
        }

        if (tmp_exp <= 0) {
            new_man = (new_man >> (-tmp_exp + 1))
                | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
            tmp_exp = 0;
        }

        exp_t res_exp = static_cast<exp_t>(tmp_exp);
        new_man &= (two - 1);
        quantize_mantissa(
            new_man,
            res_exp,
            res.max_exponent(),
            man_bits_delta,
            sign,
            two_res,
            quantization
        );

        res.man = new_man;
        res.exp = res_exp;
        return res;
    } else {
        // Normalize both inputs
        APyFloat norm_x = normalized();
        APyFloat norm_y = y.normalized();

        // Add leading one's
        man_t mx = norm_x.true_man();
        man_t my = norm_y.true_man();

        // Tentative exponent
        std::int64_t new_exp = ((std::int64_t)norm_x.exp - (std::int64_t)norm_x.bias)
            + ((std::int64_t)norm_y.exp - (std::int64_t)norm_y.bias) + res.bias;

        // Two integer bits, sign bit and leading one
        APyFixed apy_mx(2 + norm_x.man_bits, 2, std::vector<mp_limb_t>({ mx }));
        APyFixed apy_my(2 + norm_y.man_bits, 2, std::vector<mp_limb_t>({ my }));

        auto apy_res = (apy_mx * apy_my);

        // Check result from multiplication larger than/equal two
        if (apy_res.positive_greater_than_equal_pow2(1)) {
            apy_res >>= 1;
            new_exp++;
        }

        // Handle subnormal case
        if (new_exp <= 0) {
            apy_res >>= std::abs(new_exp) + 1;
            new_exp = 0;
        }

        // Quantize mantissa
        quantize_apymantissa(apy_res, res.sign, res.man_bits, quantization);

        // Carry from quantization
        if (apy_res.positive_greater_than_equal_pow2(1)) {
            new_exp++;
            apy_res >>= 1;
        }

        if (new_exp >= res.max_exponent()) {
            if (do_infinity(quantization, res.sign)) {
                res.exp = res.max_exponent();
                res.man = 0;
            } else {
                res.exp = res.max_exponent() - 1;
                res.man = res.man_mask();
            }
            return res;
        }

        if (apy_res.positive_greater_than_equal_pow2(0)) { // Remove leading one
            apy_res = apy_res - fx_one;

            // If a leading one is present while the exponent is zero,
            // then it 'acts like a carry' and creates a normal number
            if (new_exp == 0) {
                new_exp = 1;
            }
        }
        apy_res <<= res.man_bits;
        res.man = (man_t)(apy_res).to_double();
        res.exp = new_exp;
        return res;
    }
}

APyFloat APyFloat::operator/(const APyFloat& y) const
{
    APyFloat res(std::max(exp_bits, y.exp_bits), std::max(man_bits, y.man_bits));

    // Calculate sign
    res.sign = sign ^ y.sign;

    // Handle special operands
    if (is_nan() || y.is_nan() || (is_zero() && y.is_zero())
        || (is_inf() && y.is_inf())) {
        return res.construct_nan();
    }

    if (is_zero() || y.is_inf()) {
        return res.construct_zero();
    }

    if (is_inf() || y.is_zero()) {
        return res.construct_inf();
    }

    const auto quantization = get_float_quantization_mode();

    // Normalize both inputs
    const APyFloat norm_x = normalized();
    const APyFloat norm_y = y.normalized();

    std::int64_t new_exp = ((std::int64_t)norm_x.exp - (std::int64_t)norm_x.bias)
        - ((std::int64_t)norm_y.exp - (std::int64_t)norm_y.bias)
        + (std::int64_t)res.bias;

    // Add leading one's
    man_t mx = norm_x.true_man();
    man_t my = norm_y.true_man();

    // At least 3 + max(x.man_bits, y.man_bits) bits are needed,
    // using a size of _LIMB_SIZE_BITS makes initialization quick and easy.
    const auto guard_bits = _LIMB_SIZE_BITS;

    // Two integer bits, sign bit and leading one
    APyFixed apy_mx(
        2 + guard_bits + norm_x.man_bits, 2, std::vector<mp_limb_t>({ 0, mx })
    );
    APyFixed apy_my(
        2 + guard_bits + norm_y.man_bits, 2, std::vector<mp_limb_t>({ 0, my })
    );

    auto apy_man_res = apy_mx / apy_my;

    // The result from the division will be in (1/2, 2) so normalization may be
    // required
    if (!apy_man_res.positive_greater_than_equal_pow2(0)) {
        apy_man_res <<= 1;
        new_exp--;
    }

    // Check limits
    if (new_exp >= res.max_exponent()) {
        if (do_infinity(quantization, res.sign)) {
            res.exp = res.max_exponent();
            res.man = 0;
        } else {
            res.exp = res.max_exponent() - 1;
            res.man = res.man_mask();
        }
        return res;
    }

    // Handle subnormal case
    if (new_exp <= 0) {
        apy_man_res >>= std::abs(new_exp) + 1;
        new_exp = 0;
    }

    // Quantize mantissa. This will never create carry.
    quantize_apymantissa(apy_man_res, res.sign, res.man_bits, quantization);

    // Remove leading one
    if (apy_man_res.positive_greater_than_equal_pow2(0)) {
        apy_man_res = apy_man_res - fx_one;

        // If a leading one is present while the exponent is zero,
        // then it 'acts like a carry' and creates a normal number
        if (new_exp == 0) {
            new_exp = 1;
        }
    }
    apy_man_res <<= res.man_bits;
    res.man = (man_t)(apy_man_res).to_double();
    res.exp = new_exp;
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
    // Handling of special cases based on the 754-2019 standard
    if (x.is_nan() || (n == 1)) {
        return x;
    }

    if (n == 0) {
        return APyFloat(0, x.bias, 0, x.exp_bits, x.man_bits, x.bias); // Return '1'
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
    const APyFixed apy_mx(2 + x.man_bits, 2, std::vector<mp_limb_t>({ mx }));
    APyFixed apy_res = ipow(apy_mx, abs_n);

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
        apy_res = apy_res - fx_one;
    }
    apy_res <<= x.man_bits;
    new_man = (man_t)(apy_res).to_double();
    return APyFloat(new_sign, new_exp, new_man, x.exp_bits, x.man_bits, x.bias);
}

/* ******************************************************************************
 * * Binary logic operators *
 * ******************************************************************************
 */

APyFloat APyFloat::operator&(APyFloat& rhs)
{
    if (same_type_as(rhs)) {
        APyFloat f(sign & rhs.sign, exp & rhs.exp, man & rhs.man, exp_bits, man_bits);
        return f;
    }

    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

    APyFloat f(
        lhs_big.sign & rhs_big.sign,
        lhs_big.exp & rhs_big.exp,
        lhs_big.man & rhs_big.man,
        max_exp_bits,
        max_man_bits
    );
    return f;
}

APyFloat APyFloat::operator|(APyFloat& rhs)
{
    if (same_type_as(rhs)) {
        APyFloat f(sign | rhs.sign, exp | rhs.exp, man | rhs.man, exp_bits, man_bits);
        return f;
    }
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

    APyFloat f(
        lhs_big.sign | rhs_big.sign,
        lhs_big.exp | rhs_big.exp,
        lhs_big.man | rhs_big.man,
        max_exp_bits,
        max_man_bits
    );
    return f;
}

APyFloat APyFloat::operator^(APyFloat& rhs)
{
    if (same_type_as(rhs)) {
        APyFloat f(sign ^ rhs.sign, exp ^ rhs.exp, man ^ rhs.man, exp_bits, man_bits);
        return f;
    }
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

    APyFloat f(
        lhs_big.sign ^ rhs_big.sign,
        lhs_big.exp ^ rhs_big.exp,
        lhs_big.man ^ rhs_big.man,
        max_exp_bits,
        max_man_bits
    );
    return f;
}

APyFloat APyFloat::operator~()
{
    APyFloat f(
        !sign,
        ~exp & ((1ULL << exp_bits) - 1),
        ~man & ((1ULL << man_bits) - 1),
        exp_bits,
        man_bits
    );
    return f;
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
    if (is_nan() || rhs.is_nan()) {
        return false;
    }

    if (is_zero() && rhs.is_zero()) {
        return false;
    } else if (sign != rhs.sign) {
        return sign;
    }

    if (same_type_as(rhs)) {
        if (exp < rhs.exp) {
            return !sign;
        }
        if (exp == rhs.exp) {
            return sign ^ (man < rhs.man);
        }
        return sign;
    } else {
        // Cast operands to a larger format that can represent both numbers
        const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
        const auto max_man_bits = std::max(man_bits, rhs.man_bits);
        const auto ieee_bias = APyFloat::ieee_bias(max_exp_bits);
        const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);
        const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits, ieee_bias);

        if (lhs_big.exp < rhs_big.exp) {
            return !sign;
        }
        if (lhs_big.exp == rhs_big.exp) {
            return sign ^ (lhs_big.man < rhs_big.man);
        }
        return sign;
    }
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

bool APyFloat::operator==(const float rhs) const { return *this == (double)rhs; }

bool APyFloat::operator!=(const float rhs) const { return *this != (double)rhs; }

bool APyFloat::operator<=(const float rhs) const { return *this <= (double)rhs; }

bool APyFloat::operator<(const float rhs) const { return *this < (double)rhs; }

bool APyFloat::operator>=(const float rhs) const { return *this >= (double)rhs; }

bool APyFloat::operator>(const float rhs) const { return *this > (double)rhs; }

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
        return false;
    }
    return ((to_fixed()) <= rhs);
}

bool APyFloat::operator<(const APyFixed& rhs) const
{
    if (is_max_exponent()) {
        return false;
    }
    return ((to_fixed()) < rhs);
}

bool APyFloat::operator>=(const APyFixed& rhs) const
{
    if (is_nan()) {
        return false;
    }
    if (is_inf()) {
        return true;
    }
    return ((to_fixed()) >= rhs);
}

bool APyFloat::operator>(const APyFixed& rhs) const
{
    if (is_nan()) {
        return false;
    }
    if (is_inf()) {
        return true;
    }
    return ((to_fixed()) > rhs);
}

/* ******************************************************************************
 * * Helper functions *
 * ******************************************************************************
 */

void APyFloat::set_to_zero(std::optional<bool> new_sign)
{
    sign = new_sign.value_or(sign);
    exp = 0;
    man = 0;
}

void APyFloat::set_to_inf(std::optional<bool> new_sign) { set_to_nan(new_sign, 0); }

void APyFloat::set_to_nan(std::optional<bool> new_sign, man_t payload /*= 1*/)
{
    sign = new_sign.value_or(sign);
    exp = max_exponent();
    man = payload;
}

APyFloat APyFloat::construct_zero(std::optional<bool> new_sign) const
{
    return APyFloat(new_sign.value_or(sign), 0, 0, exp_bits, man_bits);
}

APyFloat APyFloat::construct_inf(std::optional<bool> new_sign) const
{
    return construct_nan(new_sign, 0);
}

APyFloat
APyFloat::construct_nan(std::optional<bool> new_sign, man_t payload /*= 1*/) const
{
    return APyFloat(
        new_sign.value_or(sign), max_exponent(), payload, exp_bits, man_bits
    );
}

//! Add mantissa bits so that a number is no longer subnormal
APyFloat APyFloat::normalized() const
{
    if (!is_subnormal() || is_zero()) {
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
