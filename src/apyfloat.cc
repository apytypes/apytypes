#include <nanobind/nanobind.h>
namespace nb = nanobind;

#include <climits>
#include <cmath>
#include <iostream>

#include <fmt/format.h>

#include "apyfixed.h"
#include "apyfloat.h"
#include "apytypes_util.h"
#include "python_util.h"

#include "ieee754.h"

constexpr bool PRINT_WARNINGS = false;

void print_warning(const std::string msg)
{
    if constexpr (PRINT_WARNINGS) {
        std::cerr << "Warning: " << msg;
    }
}

static const auto fx_one = APyFixed::from_double(1, 2, 2);

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
    const APyFloatData& data,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias.value_or(ieee_bias()))
    , sign(data.sign)
    , exp(data.exp)
    , man(data.man)
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

APyFloat APyFloat::resize(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    PyErr_WarnEx(
        PyExc_DeprecationWarning, "resize() is deprecated, use cast() instead.", 1
    );
    return cast(new_exp_bits, new_man_bits, new_bias, quantization);
}

APyFloat APyFloat::cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    return _cast(
        new_exp_bits,
        new_man_bits,
        new_bias.value_or(APyFloat::ieee_bias(new_exp_bits)),
        quantization.value_or(get_quantization_mode())
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
        quantization.value_or(get_quantization_mode())
    );
}

APyFloat APyFloat::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    QuantizationMode quantization
) const
{
    if ((new_exp_bits == exp_bits) && (new_man_bits == man_bits)
        && (new_bias == bias)) {
        return *this;
    }
    APyFloat res(new_exp_bits, new_man_bits, new_bias);
    res.sign = sign;

    // Handle special values first
    if (is_nan()) {
        return res.construct_nan(sign);
    } else if (is_inf()) {
        return res.construct_inf(sign);
    } else if (is_zero()) {
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

    if (new_exp <= -static_cast<std::int64_t>(res.man_bits)) { // Exponent too small
        return res.construct_zero();
    }

    auto man_bits_delta = res.man_bits - man_bits;

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        prev_man |= leading_one();
        // Prepare for right shift to adjust the mantissa
        man_bits_delta += new_exp - 1;
        new_exp = 0;
    }

    // Initial value for mantissa
    man_t new_man = (man_bits_delta > 0) ? (prev_man << man_bits_delta)
                                         : (prev_man >> -man_bits_delta);

    if (man_bits_delta < 0) { // Quantization of mantissa needed
        // Calculate quantization bit
        man_t G, // Guard (bit after LSB)
            T,   // Sticky bit, logical OR of all the bits after the guard bit
            B;   // Quantization bit to add to LSB

        const man_t bits_to_discard = std::abs(man_bits_delta);
        G = (prev_man >> (bits_to_discard - 1)) & 1;
        T = (prev_man & ((1ULL << (bits_to_discard - 1)) - 1)) != 0;

        switch (quantization) {
        case QuantizationMode::RND_CONV: // TIES_TO_EVEN
            // Using 'new_man' directly here is fine since G can only be '0' or '1',
            // thus calculating the LSB of 'new_man' is not needed.
            B = G & (new_man | T);
            break;
        case QuantizationMode::TRN_INF: // TO_POSITIVE
            B = sign ? 0 : (G | T);
            break;
        case QuantizationMode::TRN: // TO_NEGATIVE
            B = sign ? (G | T) : 0;
            break;
        case QuantizationMode::TRN_ZERO: // TO_ZERO
            B = 0;
            break;
        case QuantizationMode::TRN_MAG: // Does not really make sense for floating-point
            B = sign;
            break;
        case QuantizationMode::RND_INF: // TIES_TO_AWAY
            B = G;
            break;
        case QuantizationMode::RND_ZERO: // TIES_TO_ZERO
            B = G & T;
            break;
        case QuantizationMode::JAM:
            B = 0;
            new_man |= 1;
            break;
        case QuantizationMode::JAM_UNBIASED:
            B = 0;
            if (T || G) {
                new_man |= 1;
            }
            break;
        case QuantizationMode::STOCH_WEIGHTED: {
            const man_t trailing_bits = prev_man & ((1ULL << bits_to_discard) - 1);
            const man_t weight = random_number() & ((1ULL << bits_to_discard) - 1);
            // Since the weight won't be greater than the discarded bits,
            // this will never round an already exact number.
            B = (trailing_bits + weight) >> bits_to_discard;
        } break;
        case QuantizationMode::STOCH_EQUAL:
            // Only perform the quantization if the result is not exact.
            B = (G || T) ? random_number() & 1 : 0;
            break;
        default:
            throw NotImplementedException("APyFloat: Unknown quantization mode.");
        }

        new_man += B;
        if (static_cast<std::uint64_t>(new_man) > res.man_mask()) {
            ++new_exp;
            new_man = 0;
        }
    }

    if (new_exp >= res.max_exponent()) {
        return res.construct_inf();
    }

    res.man = new_man;
    res.exp = new_exp;
    return res;
}

APyFloat APyFloat::_cast_to_double() const
{
    if ((11 == exp_bits) && (52 == man_bits) && (1023 == bias)) {
        return *this;
    }
    APyFloat res(11, 52, 1023);
    res.sign = sign;

    // Handle special values first
    if (is_nan()) {
        return res.construct_nan(sign);
    } else if (is_inf()) {
        return res.construct_inf(sign);
    } else if (is_zero()) {
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

    if (new_exp <= -52) { // Exponent too small
        return res.construct_zero();
    }

    auto man_bits_delta = 52 - man_bits;

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        prev_man |= leading_one();
        // Prepare for right shift to adjust the mantissa
        man_bits_delta += new_exp - 1;
        new_exp = 0;
    }

    // Initial value for mantissa
    man_t new_man = (man_bits_delta > 0) ? (prev_man << man_bits_delta)
                                         : (prev_man >> -man_bits_delta);

    if (man_bits_delta < 0) { // Quantization of mantissa needed
        // Calculate quantization bit
        man_t G, // Guard (bit after LSB)
            T,   // Sticky bit, logical OR of all the bits after the guard bit
            B;   // Quantization bit to add to LSB

        const man_t bits_to_discard = std::abs(man_bits_delta);
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
    if ((new_exp_bits == 11) && (new_man_bits == 52) && (new_bias == 1023)) {
        return *this;
    }
    APyFloat res(new_exp_bits, new_man_bits, new_bias);
    res.sign = sign;

    // Handle special values first
    if (is_nan()) {
        return res.construct_nan(sign);
    } else if (is_inf()) {
        return res.construct_inf(sign);
    } else if (is_zero()) {
        return res.construct_zero(sign);
    }

    // Initial value for exponent
    std::int64_t new_exp
        = (std::int64_t)exp - 1023 + is_subnormal() + (std::int64_t)res.bias;

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man = man;
    if (is_subnormal()) {
        const exp_t subn_adjustment = count_trailing_bits(man);
        new_exp = new_exp - 52 + subn_adjustment;
        const man_t remainder = man % (1ULL << subn_adjustment);
        prev_man = remainder << (man_bits - subn_adjustment);
    }

    if (new_exp <= -static_cast<std::int64_t>(res.man_bits)) { // Exponent too small
        return res.construct_zero();
    }

    auto man_bits_delta = res.man_bits - 52;

    // Check if the number will be converted to a subnormal
    if (new_exp <= 0) {
        prev_man |= leading_one();
        // Prepare for right shift to adjust the mantissa
        man_bits_delta += new_exp - 1;
        new_exp = 0;
    }

    // Initial value for mantissa
    man_t new_man = (man_bits_delta > 0) ? (prev_man << man_bits_delta)
                                         : (prev_man >> -man_bits_delta);

    if (man_bits_delta < 0) { // Quantization of mantissa needed
        // Calculate quantization bit
        man_t G, // Guard (bit after LSB)
            T,   // Sticky bit, logical OR of all the bits after the guard bit
            B;   // Quantization bit to add to LSB

        const man_t bits_to_discard = std::abs(man_bits_delta);
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
    }

    if (new_exp >= res.max_exponent()) {
        return res.construct_inf();
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
    if ((new_exp_bits == exp_bits) && (new_man_bits == man_bits)
        && (new_bias == bias)) {
        return *this;
    }
    APyFloat res(new_exp_bits, new_man_bits, new_bias);
    res.sign = sign;

    // Handle special values first
    if (is_nan()) {
        return res.construct_nan(sign);
    } else if (is_inf()) {
        return res.construct_inf(sign);
    } else if (is_zero()) {
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

void APyFloat::quantize_apymantissa(
    APyFixed& apyman, bool sign, int bits, QuantizationMode quantization
)
{
    quantization = APyFloat::translate_quantization_mode(quantization, sign);
    if (quantization == QuantizationMode::STOCH_WEIGHTED) {
        std::vector<mp_limb_t> rnd_data = { random_number(), random_number(), 0 };
        APyFixed rnd_num(_LIMB_SIZE_BITS * 3, _LIMB_SIZE_BITS - bits, rnd_data);
        apyman = apyman + rnd_num;
    } else if (quantization == QuantizationMode::STOCH_EQUAL) {
        const mp_limb_t rnd = random_number() % 2 ? -1 : 0;
        std::vector<mp_limb_t> rnd_data = { rnd, rnd, 0 };
        APyFixed rnd_num(_LIMB_SIZE_BITS * 3, _LIMB_SIZE_BITS - bits, rnd_data);
        apyman = apyman + rnd_num;
    } else {
        apyman = apyman.cast_no_overflow(3 + bits, 3, quantization);
    }
}

QuantizationMode
APyFloat::translate_quantization_mode(QuantizationMode quantization, bool sign)
{
    switch (quantization) {
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        return sign ? QuantizationMode::TRN : QuantizationMode::TRN_INF;
    case QuantizationMode::TRN: // TO_NEGATIVE
        return sign ? QuantizationMode::TRN_INF : QuantizationMode::TRN;
    default:
        return quantization;
    }
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

std::string APyFloat::pretty_string() const
{
    std::string str = "fp<" + std::to_string(exp_bits) + "," + std::to_string(man_bits)
        + (sign ? ">(-" : ">(");

    if (is_nan()) {
        return str + "NaN)";
    } else if (is_inf()) {
        return str + "inf)";
    }

    str += "2**"
        + std::to_string(
               static_cast<std::int64_t>(exp) - bias - man_bits + 1 - is_normal()
        )
        + "*" + std::to_string((is_normal() << man_bits) | man) + ")";

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
    str += std::to_string((is_normal() ? ((1ULL << man_bits) + man) : man))
        + "\\times 2^{"
        + std::to_string(
               static_cast<std::int64_t>(exp) - bias - man_bits + 1 - is_normal()
        )
        + "} = " + fmt::format("{:g}", float(*this)) + "$";

    return str;
}

/* ******************************************************************************
 * * Arithmetic operators                                                       *
 * ******************************************************************************
 */

APyFloat APyFloat::operator+(APyFloat y) const
{
    const auto res_exp_bits = std::max(exp_bits, y.exp_bits);
    const auto res_man_bits = std::max(man_bits, y.man_bits);

    const auto quantization = get_quantization_mode();

    const auto res_bias = APyFloat::ieee_bias(res_exp_bits);

    // Handle the zero cases, other special cases are further down
    if (is_zero()) {
        return y.cast_no_quant(res_exp_bits, res_man_bits, res_bias);
    }

    if (y.is_zero()) {
        return cast_no_quant(res_exp_bits, res_man_bits, res_bias);
    }

    APyFloat x = *this;
    APyFloat res(res_exp_bits, res_man_bits, res_bias);

    // Handle the NaN cases, other special cases are further down
    if (x.is_nan() || y.is_nan()
        || ((x.is_inf() && y.is_inf()) && (x.sign != y.sign))) {
        return res.construct_nan();
    }

    // Compute sign and swap operands if need to make sure |x| >= |y|
    const APyFloat xabs = x.abs();
    const APyFloat yabs = y.abs();

    if (xabs > yabs) {
        res.sign = x.sign;
    } else if (xabs < yabs) {
        res.sign = y.sign;
        std::swap(x, y);
    } else {
        if (x.sign != y.sign) {
            return res.construct_zero(true);
        }
        res.sign = x.sign | y.sign;
    }

    // Handle other special cases
    if ((x.is_inf() || y.is_inf())) {
        return res.construct_inf();
    }

    std::int64_t new_exp = x.exp - x.bias + res.bias;

    // Conditionally add leading one's
    man_t mx = x.true_man();
    man_t my = y.true_man();

    // Align mantissas based on exponent difference
    const int delta = x.true_exp() - y.true_exp();

    // Two integer bits, sign bit and leading one
    APyFixed apy_mx(2 + x.man_bits, 2, std::vector<mp_limb_t>({ mx }));
    APyFixed apy_my(2 + y.man_bits, 2 - delta, std::vector<mp_limb_t>({ my }));

    // Perform addition/subtraction
    auto apy_res = (x.sign == y.sign) ? apy_mx + apy_my : apy_mx - apy_my;

    if (apy_res.greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    } else if (new_exp == 0 && apy_res.greater_than_equal_pow2(0)) {
        new_exp++;
    }

    if (new_exp >= res.max_exponent()) {
        return res.construct_inf();
    }

    int leading_zeros = leading_zeros_apyfixed(apy_res);
    if (leading_zeros) {
        if (new_exp > leading_zeros) {
            new_exp -= leading_zeros;
            apy_res <<= leading_zeros;
        } else {
            if (new_exp > 0) {
                apy_res <<= int(new_exp - 1);
            }
            new_exp = 0;
        }
    }

    // Quantize mantissa
    APyFloat::quantize_apymantissa(apy_res, res.sign, res.man_bits, quantization);

    // Carry from quantization
    if (apy_res.greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    }

    if (new_exp >= res.max_exponent()) {
        return res.construct_inf();
    }

    // Remove leading one
    if (apy_res.greater_than_equal_pow2(0)) {
        apy_res = apy_res - fx_one;
    }

    apy_res <<= res.man_bits;
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

APyFloat APyFloat::operator*(const APyFloat& y) const
{
    auto res_exp_bits = std::max(exp_bits, y.exp_bits);
    APyFloat res(res_exp_bits, std::max(man_bits, y.man_bits));

    // Calculate sign
    res.sign = sign ^ y.sign;

    // Handle special operands
    if ((is_nan() || y.is_nan()) || (is_inf() && y.is_zero())
        || (is_zero() && y.is_inf())) {
        return res.construct_nan();
    }

    if ((is_inf() || y.is_inf())) {
        return res.construct_inf();
    }

    if (is_zero() || y.is_zero()) {
        return res.construct_zero();
    }
    const auto quantization = get_quantization_mode();
    auto sum_man_bits = man_bits + y.man_bits;

    if (unsigned(sum_man_bits) + 2 <= _MAN_T_SIZE_BITS) {
        // Tentative exponent
        std::int64_t tmp_exp = true_exp() + y.true_exp();
        man_t mx = true_man();
        man_t my = y.true_man();
        man_t one = 1ULL << sum_man_bits;
        man_t two = one << 1;

        man_t new_man = mx * my;

        const auto new_man_bits = sum_man_bits + 1;
        // In case of denormalized data
        if (new_man < one) {
            int cnt = new_man_bits - bit_width(new_man);
            new_man <<= cnt + 1;
            tmp_exp -= cnt;
        } else {
            // Result may be larger than two
            if (new_man >= two) {
                tmp_exp++;
            } else {
                // Align with longer result
                new_man <<= 1;
            }
        }
        // Remove leading one
        new_man &= two - 1;

        // Possibly use more exponent bits
        int new_exp_bits = res_exp_bits;
        exp_t extended_bias = res.bias;
        auto new_exp = tmp_exp + extended_bias;
        while (new_exp <= 0) {
            new_exp_bits++;
            extended_bias = APyFloat::ieee_bias(new_exp_bits);
            new_exp = tmp_exp + extended_bias;
        }
        // Use longer format for intermediate result
        APyFloat larger_float(
            res.sign, new_exp, new_man, new_exp_bits, new_man_bits, extended_bias
        );
        return larger_float._cast(res.exp_bits, res.man_bits, res.bias, quantization);
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

        // Carry from multiplication
        if (apy_res.greater_than_equal_pow2(1)) {
            apy_res >>= 1;
            new_exp++;
        }

        // Handle subnormal case
        if (new_exp <= 0) {
            apy_res >>= std::abs(new_exp) + 1;
            new_exp = 0;
        }

        // Quantize mantissa
        APyFloat::quantize_apymantissa(apy_res, res.sign, res.man_bits, quantization);

        // Carry from quantization
        if (apy_res.greater_than_equal_pow2(1)) {
            new_exp++;
            apy_res >>= 1;
        }

        if (new_exp >= res.max_exponent()) {
            return res.construct_inf();
        }

        if (apy_res.greater_than_equal_pow2(0)) { // Remove leading one
            apy_res = apy_res - fx_one;
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
    if ((is_nan() || y.is_nan()) || (is_zero() && y.is_zero())
        || (is_inf() && y.is_inf())) {
        return res.construct_nan();
    }

    if (is_zero() || y.is_inf()) {
        return res.construct_zero();
    }

    if (is_inf() || y.is_zero()) {
        return res.construct_inf();
    }

    const auto quantization = get_quantization_mode();

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

    // The result from the division will be in (1/2, 2) so normalization may be required
    if (!apy_man_res.greater_than_equal_pow2(0)) {
        apy_man_res <<= 1;
        new_exp--;
    }

    // Check limits
    if (new_exp >= res.max_exponent()) {
        return res.construct_inf();
    }

    // Handle subnormal case
    if (new_exp <= 0) {
        apy_man_res >>= std::abs(new_exp) + 1;
        new_exp = 0;
    }

    // Quantize mantissa. This will never create carry.
    APyFloat::quantize_apymantissa(apy_man_res, res.sign, res.man_bits, quantization);

    // Remove leading one
    if (apy_man_res.greater_than_equal_pow2(0)) {
        apy_man_res = apy_man_res - fx_one;
    }
    apy_man_res <<= res.man_bits;
    res.man = (man_t)(apy_man_res).to_double();
    res.exp = new_exp;
    return res;
}

/* ******************************************************************************
 * * Mathematical functions                                                     *
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
        "APyFloat: Power to another APyFloat has not yet been implemented."
    );
}

APyFloat APyFloat::pown(const APyFloat& x, int n)
{
    // Handling of special cases based on the 754-2019 standard
    if (x.is_nan()) {
        return x;
    }

    if (n == 0) {
        return APyFloat(0, x.bias, 0, x.exp_bits, x.man_bits, x.bias); // Return '1'
    }

    bool new_sign = ((n % 2) == 0) ? false : x.sign;

    if (x.is_zero()) {
        if (n < 0) {
            return x.construct_inf(new_sign);
        } else {
            return x.construct_zero(new_sign);
        }
    }

    if (x.is_inf()) {
        new_sign = x.sign ? new_sign : false;

        if (n > 0) {
            return x.construct_inf(new_sign);
        } else {
            return x.construct_zero(new_sign);
        }
    }

    const auto quantization = get_quantization_mode();

    const int abs_n = std::abs(n);

    const int max_exp_bits = x.exp_bits + (count_trailing_bits(abs_n) + 1);
    int extended_man_bits {};
    const exp_t extended_bias = (1 << (max_exp_bits - 1)) - 1;

    std::int64_t new_exp
        = (static_cast<std::int64_t>(x.exp) - x.bias) * n + extended_bias;

    std::uint64_t new_man = 0, mx = x.true_man();

    // Check is done for 52 since std::pow uses double
    if ((abs_n * (x.man_bits + x.is_normal()) + 1) <= 52) {
        new_man = std::pow(mx, abs_n);

        // Perform quantization
        extended_man_bits = count_trailing_bits(new_man);

        // If a leading one was added, mask it away
        if (x.is_normal()) {
            new_man &= (1ULL << extended_man_bits) - 1;
        }
    } else {
        const APyFixed apy_mx(2 + x.man_bits, 2, std::vector<mp_limb_t>({ mx }));
        auto apy_res = apy_mx;
        for (auto i = 1; i < abs_n; i++) {
            apy_res = apy_res * apy_mx;
        }

        // Normalize mantissa
        while (apy_res.greater_than_equal_pow2(1)) {
            apy_res >>= 1;
            new_exp++;
        }

        // Quantize mantissa
        APyFloat::quantize_apymantissa(apy_res, new_sign, x.man_bits, quantization);
        // Carry from quantization
        if (apy_res.greater_than_equal_pow2(1)) {
            new_exp++;
            apy_res >>= 1;
        }

        if (apy_res.greater_than_equal_pow2(0)) { // Remove leading one
            apy_res = apy_res - fx_one;
        }
        apy_res <<= x.man_bits;
        new_man = (man_t)(apy_res).to_double();
        extended_man_bits = x.man_bits;
    }

    return APyFloat(
               new_sign,
               new_exp,
               new_man,
               max_exp_bits,
               extended_man_bits,
               extended_bias
    )
        ._cast(x.exp_bits, x.man_bits, x.bias, quantization);
}

/* ******************************************************************************
 * * Binary logic operators                                                     *
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
 * * Binary comparison operators                                                *
 * ******************************************************************************
 */

bool APyFloat::operator==(const APyFloat& rhs) const
{
    if (sign != rhs.sign) {
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

    if (sign != rhs.sign) {
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

/* ******************************************************************************
 * * Non-computational functions                                                *
 * ******************************************************************************
 */

//! True if and only if value is normal (not zero, subnormal, infinite, or NaN).
bool APyFloat::is_normal() const { return exp != 0 && exp != max_exponent(); }

//! True if and only if value is zero, subnormal, or normal.
bool APyFloat::is_finite() const { return exp == 0 || exp != max_exponent(); }

//! True if and only if value is subnormal. Zero is also considered a subnormal
//! number.
bool APyFloat::is_subnormal() const { return exp == 0; }

//! True if and only if value is zero.
bool APyFloat::is_zero() const { return exp == 0 && man == 0; }

//! True if and only if value is NaN.
bool APyFloat::is_nan() const { return man != 0 && exp == max_exponent(); }

//! True if and only if value is infinite.
bool APyFloat::is_inf() const { return man == 0 && exp == max_exponent(); }

/* ******************************************************************************
 * * Helper functions                                                           *
 * ******************************************************************************
 */

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

APY_INLINE int APyFloat::leading_zeros_apyfixed(APyFixed fx) const
{
    // Calculate the number of left shifts needed to make fx>=1.0
    const int zeros = fx.leading_zeros() - fx.int_bits();
    return std::max(0, zeros + 1);
}

APY_INLINE bool APyFloat::same_type_as(APyFloat other) const
{
    return man_bits == other.man_bits && exp_bits == other.exp_bits
        && bias == other.bias;
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
