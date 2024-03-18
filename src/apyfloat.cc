#include <nanobind/nanobind.h>
namespace nb = nanobind;

#include <climits>
#include <cmath>
#include <iostream>

#include <fmt/format.h>

#include "apyfixed.h"
#include "apyfloat.h"
#include "apytypes_util.h"

#include "ieee754.h"

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
    if (bias.has_value() && bias.value() != ieee_bias()) {
        print_warning("non 'ieee-like' biases is not sure to work yet.\n");
    }
}
APyFloat::APyFloat(
    int sign,
    exp_t exp,
    man_t man,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : APyFloat(bool(sign), exp, man, exp_bits, man_bits, bias)
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

/* **********************************************************************************
 * * Methods for conversions                                                        *
 * **********************************************************************************
 */

APyFloat APyFloat::from_double(
    double value,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias,
    std::optional<QuantizationMode> quantization
)
{
    APyFloat f(exp_bits, man_bits);
    return f.update_from_double(value, quantization);
}

APyFloat&
APyFloat::update_from_double(double value, std::optional<QuantizationMode> quantization)
{

    // Initialize an APyFloat from the double
    APyFloat apytypes_double(
        sign_of_double(value), exp_of_double(value), man_of_double(value), 11, 52
    );

    // Cast it to the correct format
    *this = apytypes_double.cast(exp_bits, man_bits, bias, quantization);

    return *this;
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
    std::int64_t new_exp = (std::int64_t)exp - (std::int64_t)bias + is_subnormal()
        + (std::int64_t)res.bias;

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

    const auto man_bits_delta = res.man_bits - man_bits;

    // Initial value for mantissa
    man_t new_man = (man_bits_delta > 0) ? (prev_man << man_bits_delta)
                                         : (prev_man >> -man_bits_delta);

    if (new_exp <= 0) { // The number will be converted to a subnormal in the new format
        new_man |= res.leading_one();             // Add leading one
        new_man <<= (res.man_bits + new_exp - 1); // Shift the difference between E_min
                                                  // and exp
        new_man /= 1ULL << res.man_bits; // Divide by the minimum subnorm (i.e. E_min)
        new_man &= res.man_mask();       // Mask away the leading ones
        new_exp = 0;
    } else if (man_bits_delta < 0) { // Normal case, exponent is positive
        // Calculate quantization bit
        man_t G, // Guard (bit after LSB)
            T,   // Sticky bit, logical OR of all the bits after the guard bit
            B;   // Quantization bit to add to LSB

        const man_t bits_to_discard = std::abs(man_bits_delta);
        G = (prev_man >> (bits_to_discard - 1)) & 1;
        T = (prev_man & ((1ULL << (bits_to_discard - 1)) - 1)) != 0;

        switch (quantization.value_or(get_quantization_mode())) {
        case QuantizationMode::TRN_INF: // TO_POSITIVE
            B = sign ? 0 : (G | T);
            break;
        case QuantizationMode::TRN: // TO_NEGATIVE
            B = sign ? (G | T) : 0;
            break;
        case QuantizationMode::TRN_ZERO: // TO_ZERO
            B = 0;
            break;
        case QuantizationMode::RND_CONV: // TIES_TO_EVEN
            // Using 'new_man' directly here is fine since G can only be '0' or '1',
            // thus calculating the LSB of 'new_man' is not needed.
            B = G & (new_man | T);
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

APyFloat APyFloat::cast_no_quant(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, std::optional<exp_t> new_bias
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
    std::int64_t new_exp = (std::int64_t)exp - (std::int64_t)bias + is_subnormal()
        + (std::int64_t)res.bias;

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

    if (new_exp <= 0) { // The number will be converted to a subnormal in the new format
        new_man |= res.leading_one();             // Add leading one
        new_man <<= (res.man_bits + new_exp - 1); // Shift the difference between E_min
                                                  // and exp
        new_man /= 1ULL << res.man_bits; // Divide by the minimum subnorm (i.e. E_min)
        new_man &= res.man_mask();       // Mask away the leading ones
        new_exp = 0;
    }

    res.man = new_man;
    res.exp = new_exp;
    return res;
}

double APyFloat::to_double() const
{
    const auto apytypes_d = cast(11, 52);
    double d {};
    set_sign_of_double(d, apytypes_d.sign);
    set_exp_of_double(d, apytypes_d.exp);
    set_man_of_double(d, apytypes_d.man);
    return d;
}

APyFloat::operator double() const { return to_double(); }

APyFloat APyFloat::from_bits(
    unsigned long long bits,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    APyFloat f(exp_bits, man_bits, bias);
    return f.update_from_bits(bits);
}

APyFloat& APyFloat::update_from_bits(unsigned long long bits)
{
    man = bits & man_mask();
    bits >>= man_bits;

    exp = bits & exp_mask();
    bits >>= exp_bits;

    sign = bits != 0;

    return *this;
}

unsigned long long APyFloat::to_bits() const
{
    return (sign << (exp_bits + man_bits)) | (exp << man_bits) | man;
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
    APyFloat x = *this;
    APyFloat res(std::max(x.exp_bits, y.exp_bits), std::max(x.man_bits, y.man_bits));

    // Handle the NaN cases first, other special cases are further down
    if (x.is_nan() || y.is_nan()
        || ((x.is_inf() && y.is_inf()) && (x.sign != y.sign))) {
        return res.construct_nan();
    }

    if ((res.man_bits + 5UL)
        > (sizeof(man_t) * CHAR_BIT
        )) { // +1 (leading one) +3 (guard bits) +1 (addition)
        throw nb::value_error(
            "The intermediate mantissa can potentially exceed "
            "its underlaying data type."
        );
    }

    // Compute sign and swap operands if need to make sure |x| >= |y|
    const APyFloat xabs = APyFloat::abs(x);
    const APyFloat yabs = APyFloat::abs(y);

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

    if (x.is_zero()) {
        return y.cast_no_quant(res.exp_bits, res.man_bits, res.bias);
    }

    if (y.is_zero()) {
        return x.cast_no_quant(res.exp_bits, res.man_bits, res.bias);
    }

    std::int64_t new_exp = x.exp - x.bias + res.bias;

    // Conditionally add leading one's
    man_t mx = x.leading_bit() | x.man;
    man_t my = y.leading_bit() | y.man;

    // Align mantissas based on mixed formats
    const auto man_bits_delta = x.man_bits - y.man_bits;
    if (man_bits_delta < 0) {
        mx <<= -man_bits_delta;
    } else {
        my <<= man_bits_delta;
    }

    // Two integer bits, sign bit and leading one
    APyFixed apy_mx(2 + res.man_bits, 2, std::vector<mp_limb_t>({ mx }));
    APyFixed apy_my(2 + res.man_bits, 2, std::vector<mp_limb_t>({ my }));

    // Align mantissas based on exponent difference
    const int delta
        = (x.exp - x.bias - x.is_normal()) - (y.exp - y.bias - y.is_normal());

    apy_my >>= delta;

    // Perform addition/subtraction
    auto apy_res = (x.sign == y.sign) ? apy_mx + apy_my : apy_mx - apy_my;

    int c = 0;
    if (apy_res.to_double() >= 2.0) {
        new_exp++;
        c = 1;
    } else if (apy_res.to_double() >= 1.0 && new_exp == 0) {
        new_exp++;
    }

    if (new_exp >= res.max_exponent()) {
        return res.construct_inf();
    }

    int leading_zeros = leading_zeros_apyfixed(apy_res);
    if (leading_zeros) {
        if ((new_exp - leading_zeros) > 0) {
            new_exp -= leading_zeros;
            apy_res <<= leading_zeros;
        } else {
            apy_res = (new_exp > 0) ? apy_res << int(new_exp - 1) : apy_res;
            new_exp = 0;
        }
    }

    int tmp_man_bits = res.man_bits + 1 + delta + c;
    apy_res <<= (tmp_man_bits - c);
    man_t new_man = static_cast<man_t>(apy_res.to_double());
    new_man &= (1ULL << (tmp_man_bits)) - 1;

    return APyFloat(res.sign, new_exp, new_man, res.exp_bits, tmp_man_bits)
        .cast(res.exp_bits, res.man_bits, res.bias);
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
    APyFloat res(std::max(exp_bits, y.exp_bits), std::max(man_bits, y.man_bits));

    if ((2ULL * res.man_bits + 1ULL) > (sizeof(man_t) * CHAR_BIT)) {
        throw nb::value_error(
            "The intermediate mantissa can potentially exceed "
            "its underlaying data type."
        );
    }

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

    // Normalize both inputs
    APyFloat norm_x = is_subnormal() ? normalized() : *this;
    APyFloat norm_y = y.is_subnormal() ? y.normalized() : y;

    // Add leading one's
    man_t mx = norm_x.leading_bit() | norm_x.man;
    man_t my = norm_y.leading_bit() | norm_y.man;

    std::int64_t new_exp = ((std::int64_t)norm_x.exp - (std::int64_t)norm_x.bias)
        + ((std::int64_t)norm_y.exp - (std::int64_t)norm_y.bias) + res.bias;

    man_t new_man;
    int tmp_man_bits;

    // One of the operands should be scaled but since (a*scale)*b == (a*b)*scale
    // we can just scale the result.
    const exp_t man_bits_delta = std::abs(norm_x.man_bits - norm_y.man_bits);
    if (unsigned(norm_x.man_bits + norm_y.man_bits + 4) <= _LIMB_SIZE_BITS) {
        new_man = mx * my;
        man_t one = 1ULL << (norm_x.man_bits + norm_y.man_bits);
        man_t two = one << 1;
        if (new_exp < 0) {
            while (new_man < one) {
                new_man <<= 1;
                new_exp++;
            }
        }
        int c = 0;
        if (new_man >= two) { // Carry
            new_exp++;
            c = 1;
        }
        tmp_man_bits = norm_x.man_bits + norm_y.man_bits + c;
    } else {

        // Two integer bits, sign bit and leading one
        APyFixed apy_mx(2 + norm_x.man_bits, 2, std::vector<mp_limb_t>({ mx }));
        APyFixed apy_my(2 + norm_y.man_bits, 2, std::vector<mp_limb_t>({ my }));

        auto apy_res = (apy_mx * apy_my);

        // Normalize result if needed
        if (new_exp < 0) {
            while (apy_res.to_double() < 1.0) {
                apy_res <<= 1;
                new_exp++;
            }
        }

        // Perform quantization
        const double apy_res_d = apy_res.to_double();

        int c = 0;
        if (apy_res_d >= 2.0) { // Carry
            new_exp++;
            c = 1;
        }

        tmp_man_bits = 2 * (res.man_bits);

        apy_res <<= tmp_man_bits;
        new_man = static_cast<man_t>(apy_res.to_double());
        tmp_man_bits += c;
    }
    new_man &= (1ULL << (tmp_man_bits)) - 1;

    int tmp_exp_bits = std::max(norm_x.exp_bits, norm_y.exp_bits) + 1;
    exp_t extended_bias = APyFloat::ieee_bias(tmp_exp_bits);
    new_exp = new_exp - res.bias + extended_bias;

    return APyFloat(
               res.sign, new_exp, new_man, tmp_exp_bits, tmp_man_bits, extended_bias
    )
        .cast(res.exp_bits, res.man_bits, res.bias);
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

    // Normalize both inputs
    APyFloat norm_x = is_subnormal() ? normalized() : *this;
    APyFloat norm_y = y.is_subnormal() ? y.normalized() : y;

    std::int64_t new_exp
        = (norm_x.exp - norm_x.bias) - (norm_y.exp - norm_y.bias) + res.bias;

    // Add leading one's
    man_t mx = norm_x.leading_bit() | norm_x.man;
    man_t my = norm_y.leading_bit() | norm_y.man;

    // Align mantissas based on mixed formats
    /*const auto man_bits_delta = man_bits - y.man_bits;
    if (man_bits_delta < 0) {
        mx <<= -man_bits_delta;
    } else {
        my <<= man_bits_delta;
    }*/

    // Two integer bits, sign bit and leading one, and two extra guard bits
    const auto guard_bits = res.man_bits * 2;

    // Two integer bits, sign bit and leading one
    APyFixed apy_mx(
        2 + guard_bits + norm_x.man_bits,
        2,
        std::vector<mp_limb_t>({ mx << guard_bits })
    );
    APyFixed apy_my(
        2 + guard_bits + norm_y.man_bits,
        2,
        std::vector<mp_limb_t>({ my << guard_bits })
    );

    // Determines if the exponent needs to be decremented
    int dec_exp = (apy_mx < apy_my) ? 1 : 0;

    auto apy_man_res = apy_mx / apy_my;

    // The result from the division will be in [1/2, 2) so normalization may be required
    if (apy_man_res.to_double() < 1.0) {
        apy_man_res <<= 1;
    }

    int tmp_man_bits = res.man_bits + guard_bits;
    apy_man_res = apy_man_res << tmp_man_bits;
    man_t new_man = static_cast<man_t>(apy_man_res.to_double());

    const auto lower_man_mask = (1 << guard_bits) - 1;
    new_man &= (res.man_mask() << guard_bits) | lower_man_mask;

    int tmp_exp_bits = std::max(norm_x.exp_bits, norm_y.exp_bits) + 1;

    if (tmp_man_bits > (sizeof(man_t) * CHAR_BIT)) {
        throw nb::value_error(
            "The intermediate mantissa can potentially exceed "
            "its underlaying data type."
        );
    }

    exp_t extended_bias = APyFloat::ieee_bias(tmp_exp_bits);
    new_exp = new_exp - res.bias + extended_bias;

    return APyFloat(
               res.sign,
               new_exp - dec_exp,
               new_man,
               tmp_exp_bits,
               tmp_man_bits,
               extended_bias
    )
        .cast(res.exp_bits, res.man_bits, res.bias);
}

/* ******************************************************************************
 * * Mathematical functions                                                     *
 * ******************************************************************************
 */

APyFloat APyFloat::abs(const APyFloat& x) { return x.sign ? -x : x; }

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

    const exp_t max_exp_bits = x.exp_bits + (count_trailing_bits(n) + 1);
    const exp_t extended_bias = (1 << (max_exp_bits - 1)) - 1;

    std::int64_t new_exp
        = (static_cast<std::int64_t>(x.exp) - x.bias) * n + extended_bias;
    std::uint64_t new_man = std::pow(x.leading_bit() | x.man, n);

    // Perform quantization
    const man_t trailing_bits = count_trailing_bits(new_man);

    // If a leading one was added, mask it away
    if (x.is_normal()) {
        new_man &= (1ULL << trailing_bits) - 1;
    }

    return APyFloat(
               new_sign, new_exp, new_man, max_exp_bits, trailing_bits, extended_bias
    )
        .cast(x.exp_bits, x.man_bits, x.bias);
}

/* ******************************************************************************
 * * Binary logic operators                                                     *
 * ******************************************************************************
 */

APyFloat APyFloat::operator&(APyFloat& rhs)
{
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits);

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
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits);

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
    const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits);
    const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits);

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

    if ((exp_bits == rhs.exp_bits) && (man_bits == rhs.man_bits)) {
        return (exp == rhs.exp) && (man == rhs.man);

    } else {

        // Cast operands to a larger format that can represent both numbers
        const auto max_exp_bits = std::max(exp_bits, rhs.exp_bits);
        const auto max_man_bits = std::max(man_bits, rhs.man_bits);
        const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits);
        const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits);

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

    if ((exp_bits == rhs.exp_bits) && (man_bits == rhs.man_bits)) {
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
        const auto lhs_big = cast_no_quant(max_exp_bits, max_man_bits);
        const auto rhs_big = rhs.cast_no_quant(max_exp_bits, max_man_bits);

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

// True if and only if x is normal (not zero, subnormal, infinite, or NaN).
bool APyFloat::is_normal() const { return is_finite() && !is_subnormal(); }

// True if and only if x is zero, subnormal, or normal.
bool APyFloat::is_finite() const { return is_subnormal() || exp != max_exponent(); }

// True if and only if x is subnormal. Zero is also considered a subnormal
// number.
bool APyFloat::is_subnormal() const { return exp == 0; }

// True if and only if x is zero.
bool APyFloat::is_zero() const { return exp == 0 && man == 0; }

// True if and only if x is NaN.
bool APyFloat::is_nan() const { return exp == max_exponent() && man != 0; }

// True if and only if x is infinite.
bool APyFloat::is_inf() const { return exp == max_exponent() && man == 0; }

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

APyFloat APyFloat::normalized() const
{
    if (!is_subnormal() || is_zero()) {
        return *this;
    }

    man_t new_man = man;
    std::int64_t true_exp = exp - bias;

    while (!(new_man & leading_one())) {
        new_man <<= 1;
        true_exp--;
    }

    APyFloat res;
    res.man_bits = man_bits;
    res.exp_bits = exp_bits + 1;
    res.sign = sign;
    res.bias = res.ieee_bias();
    res.exp = true_exp + res.bias + 1;
    res.man = new_man;

    return res;
}

int APyFloat::leading_zeros_apyfixed(APyFixed fx) const
{
    int zeros = 0;
    while (fx.to_double() < 1.0) {
        fx = fx << 1;
        zeros++;
    }
    return zeros;
}

/*
 * Convenience methods
 */
APyFloat APyFloat::cast_to_double(std::optional<QuantizationMode> quantization) const
{
    return cast(11, 52, std::nullopt, quantization);
}

APyFloat APyFloat::cast_to_single(std::optional<QuantizationMode> quantization) const
{
    return cast(8, 23, std::nullopt, quantization);
}

APyFloat APyFloat::cast_to_half(std::optional<QuantizationMode> quantization) const
{
    return cast(5, 10, std::nullopt, quantization);
}

APyFloat APyFloat::cast_to_bfloat16(std::optional<QuantizationMode> quantization) const
{
    return cast(8, 7, std::nullopt, quantization);
}
