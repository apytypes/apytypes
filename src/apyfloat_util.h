#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfloat.h"
#include "apytypes_common.h"
#include "python_util.h"
#include <optional>
#include <vector>

/*!
 * Sizes of APyFloat datatypes
 */
static constexpr std::size_t _MAN_T_SIZE_BYTES = sizeof(man_t);
static constexpr std::size_t _MAN_T_SIZE_BITS = 8 * _MAN_T_SIZE_BYTES;
static constexpr std::size_t _EXP_T_SIZE_BYTES = sizeof(exp_t);
static constexpr std::size_t _EXP_T_SIZE_BITS = 8 * _EXP_T_SIZE_BYTES;

/*!
 * APyFloat word length limits.
 */
static constexpr std::size_t _MAN_LIMIT_BITS = _MAN_T_SIZE_BITS - 3;
static constexpr std::size_t _EXP_LIMIT_BITS = _EXP_T_SIZE_BITS - 2;

//! Check if one should saturate to infinity or maximum normal number
APY_INLINE bool do_infinity(QuantizationMode mode, bool sign)
{
    switch (mode) {
    case QuantizationMode::TRN_ZERO:     // TO_ZERO
    case QuantizationMode::JAM:          // JAM
    case QuantizationMode::JAM_UNBIASED: // JAM_UNBIASED
        return false;
    case QuantizationMode::TRN: // TO_NEG
        return sign;
    case QuantizationMode::TRN_INF: // TO_POS
        return !sign;
    default:
        return true;
    }
}

//! Return the bit pattern of a floating-point data field. No checks on bit width is
//! done.
APY_INLINE std::uint64_t
to_bits_uint64(const APyFloatData& d, std::uint8_t exp_bits, std::uint8_t man_bits)
{
    return (std::uint64_t(d.sign) << (exp_bits + man_bits))
        | (std::uint64_t(d.exp) << man_bits) | std::uint64_t(d.man);
}

APY_INLINE nb::int_
apyfloat_to_bits(const APyFloatData& d, std::uint8_t exp_bits, std::uint8_t man_bits)
{
    std::uint64_t lower = d.man;
    const int exp_man_bits = exp_bits + man_bits;
    lower |= (std::uint64_t)d.exp << man_bits;
    lower |= (std::uint64_t)d.sign << exp_man_bits;

    std::uint64_t higher = (std::uint64_t)d.exp >> (64 - man_bits);
    const int high_sign_delta = 64 - exp_man_bits;
    if (high_sign_delta < 0) {
        higher |= d.sign << -high_sign_delta;
    }

    auto limb_vec = limb_vector_from_uint64_t({ lower, higher });
    return python_limb_vec_to_long(limb_vec.begin(), limb_vec.end(), false);
}

//! Calculate new bias. Assumes new_exp_bits is larger than exp_bits1 and exp_bits2.
APY_INLINE exp_t
calc_bias(int new_exp_bits, int exp_bits1, exp_t bias1, int exp_bits2, exp_t bias2)
{
    return ((((bias1 + 1) << (new_exp_bits - exp_bits1))
             + ((bias2 + 1) << (new_exp_bits - exp_bits2)))
            >> 1)
        - 1;
}

//! General calculation of new bias. Should only be used if new_exp_bits can be strictly
//! less than exp_bits1 or exp_bits2.
exp_t calc_bias_general(
    int new_exp_bits, int exp_bits1, exp_t bias1, int exp_bits2, exp_t bias2
);

//! Create a floating-point object with the value one.
static APY_INLINE APyFloat
one(std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias = std::nullopt)
{
    const exp_t res_bias = bias.value_or(APyFloat::ieee_bias(exp_bits));
    return APyFloat(0, res_bias, 0, exp_bits, man_bits, res_bias);
}

//! Quantize mantissa
APY_INLINE void quantize_mantissa(
    man_t& man,
    exp_t& exp,
    exp_t max_exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant,
    QuantizationMode quantization
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man_t res_man = man >> bits_to_quantize;

    switch (quantization) {
    case QuantizationMode::RND_CONV: // TIES_EVEN
        // Using 'res_man' directly here is fine since G can only be '0' or '1',
        // thus calculating the LSB of 'res_man' is not needed.
        B = G & (res_man | T);
        break;
    case QuantizationMode::RND_CONV_ODD: // TIES_ODD
        B = G & ((res_man ^ 1) | T);
        break;
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        B = sign ? 0 : (G | T);
        break;
    case QuantizationMode::TRN: // TO_NEGATIVE
        B = sign ? (G | T) : 0;
        break;
    case QuantizationMode::TRN_AWAY: // TO_AWAY
        B = G | T;
        break;
    case QuantizationMode::TRN_ZERO: // TO_ZERO
        B = 0;
        break;
    case QuantizationMode::TRN_MAG: // Does not really make sense for
                                    // floating-point
        B = sign;
        break;
    case QuantizationMode::RND_INF: // TIES_AWAY
        B = G;
        break;
    case QuantizationMode::RND_ZERO: // TIES_ZERO
        B = G & T;
        break;
    case QuantizationMode::RND: // TIES_POS
        B = G & (T | !sign);
        break;
    case QuantizationMode::RND_MIN_INF: // TIES_NEG
        B = G & (T | sign);
        break;
    case QuantizationMode::JAM:
        B = 0;
        res_man |= 1;
        break;
    case QuantizationMode::JAM_UNBIASED:
        B = 0;
        if (T || G) {
            res_man |= 1;
        }
        break;
    case QuantizationMode::STOCH_WEIGHTED: {
        const man_t trailing_bits = man & ((1ULL << bits_to_quantize) - 1);
        const man_t weight = random_number_float() & ((1ULL << bits_to_quantize) - 1);
        // Since the weight won't be greater than the discarded bits,
        // this will never round an already exact number.
        B = (trailing_bits + weight) >> bits_to_quantize;
    } break;
    case QuantizationMode::STOCH_EQUAL:
        // Only perform the quantization if the result is not exact.
        B = (G || T) ? random_number_float() & 1 : 0;
        break;
    default:
        throw NotImplementedException(
            "Not implemented: quantize_mantissa() with "
            "unknown (did you pass `int` as `QuantizationMode`?)"
        );
    }

    man = res_man;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }

    // Check for overflow. This must always be checked since other methods depend on it.
    if (exp >= max_exp) {
        if (do_infinity(quantization, sign)) {
            exp = max_exp;
            man = 0;
        } else {
            exp = max_exp - 1;
            man = man_msb_constant - 1;
        }
    }
}

//! Quantize mantissa
APY_INLINE void quantize_mantissa(
    man_t& man,
    exp_t& exp,
    exp_t max_exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    QuantizationMode quantization
)
{
    quantize_mantissa(
        man,
        exp,
        max_exp,
        bits_to_quantize,
        sign,
        man_msb_constant,
        bits_to_quantize - 1,
        (1ULL << (bits_to_quantize - 1)) - 1,
        quantization
    );
}

//! Quantize mantissa when the result is guaranteed to be either be 0 or 1.
//! Quantization mode `STOCH_WEIGHTED` should be used with this function.
APY_INLINE man_t
quantize_close_to_zero(bool sign, man_t man, QuantizationMode quantization)
{
    switch (quantization) {
    case QuantizationMode::TRN_AWAY:
    case QuantizationMode::JAM:
    case QuantizationMode::JAM_UNBIASED:
        return 1;
    case QuantizationMode::TRN:
        return sign;
    case QuantizationMode::TRN_INF:
        return !sign;
    case QuantizationMode::STOCH_EQUAL:
        // STOCH_WEIGHTED should not use this function
        return random_number_float() & 1;
    default:
        return 0;
    }
}

//! Fast integer power by squaring.
man_t ipow(man_t base, unsigned int n);

//! Get the number of left shifts needed to make fx>=1.0
APY_INLINE unsigned int leading_zeros_apyfixed(const APyFixed& fx)
{
    const int zeros = fx.leading_zeros() - fx.int_bits();
    return std::max(0, zeros + 1);
}

//! Quantize mantissa stored as an `APyFixed`
void quantize_apymantissa(
    APyFixed& apyman, bool sign, int bits, QuantizationMode quantization
);
//! Translate the quantization mode for floating-point to the fixed-point equivalent.
//! This is used for the mantissa so the sign must be taken into account.
QuantizationMode translate_quantization_mode(QuantizationMode quantization, bool sign);

//! Check that the number of exponent bits is allowed, throw otherwise
void check_exponent_format(int exp_bits);

//! Check that the number of mantissa bits is allowed, throw otherwise
void check_mantissa_format(int man_bits);

/* ********************************************************************************** *
 * *                     Floating-point helper utilities                            * *
 * ********************************************************************************** */

[[maybe_unused]] static APY_INLINE bool is_zero(const APyFloatData& src)
{
    return src.exp == 0 && src.man == 0;
}

[[maybe_unused]] static APY_INLINE bool is_zero_exponent(const APyFloatData& src)
{
    return src.exp == 0;
}

template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE bool
is_max_exponent(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    return src.exp == ((1ULL << ref.get_exp_bits()) - 1);
}

[[maybe_unused]] static APY_INLINE bool
is_max_exponent(const APyFloatData& src, uint8_t exp_bits)
{
    return src.exp == ((1ULL << exp_bits) - 1);
}

template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE bool
is_normal(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    return !is_zero_exponent(src) && !is_max_exponent(src, ref);
}

[[maybe_unused]] static APY_INLINE bool
is_normal(const APyFloatData& src, uint8_t exp_bits)
{
    return !is_zero_exponent(src) && !is_max_exponent(src, exp_bits);
}

template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE bool
is_nan(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    return is_max_exponent(src, ref.get_exp_bits()) && src.man != 0;
}

template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE bool
is_inf(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    return is_max_exponent(src, ref.get_exp_bits()) && src.man == 0;
}

template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE std::int64_t
true_exp(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    return std::int64_t(src.exp) - std::int64_t(ref.get_bias()) + is_zero_exponent(src);
}

template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE man_t
true_man(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    return ((static_cast<man_t>(is_normal(src, ref)) << ref.get_man_bits()) | src.man);
}

[[maybe_unused]] static APY_INLINE man_t
true_man(const APyFloatData& src, uint8_t exp_bits, uint8_t man_bits)
{
    return ((static_cast<man_t>(is_normal(src, exp_bits)) << man_bits) | src.man);
}

//! Return a normalized (non subnormal) floating-point copy of `src`
template <typename APYFLOAT_TYPE>
[[maybe_unused]] static APY_INLINE std::tuple<APyFloatData, uint8_t, exp_t>
normalize(const APyFloatData& src, const APYFLOAT_TYPE& ref)
{
    if (!(is_zero_exponent(src) && src.man != 0)) { // if not subnormal
        return { src, ref.get_exp_bits(), ref.get_bias() };
    }

    man_t new_man = src.man;
    std::int64_t tmp_exp = true_exp(src, ref);
    while (!(new_man & (1ULL << ref.get_man_bits()))) {
        new_man <<= 1;
        tmp_exp--;
    }

    // Possibly use more exponent bits
    int new_exp_bits = ref.get_exp_bits();
    exp_t extended_bias = ref.get_bias();
    auto new_exp = tmp_exp + extended_bias;
    while (new_exp <= 0) {
        new_exp_bits++;
        extended_bias = exp_t((1ULL << (new_exp_bits - 1)) - 1);
        new_exp = tmp_exp + extended_bias;
    }

    return { APyFloatData { src.sign, exp_t(new_exp), new_man },
             new_exp_bits,
             extended_bias };
}

/* ********************************************************************************** *
 * *             Floating-point iterator-based arithmetic functions                 * *
 * ********************************************************************************** */

//! Iterator-based floating-point product
template <
    typename RANDOM_ACCESS_ITERATOR_IN,
    typename RANDOM_ACCESS_ITERATOR_INOUT,
    typename APYFLOAT_TYPE>
void float_product(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APYFLOAT_TYPE& x, // Floating point src1
    const APYFLOAT_TYPE& y  // Floating point src2
)
{
    // Compute result bit specification
    auto res_exp_bits = std::max(x.get_exp_bits(), y.get_exp_bits());
    auto res_man_bits = std::max(x.get_man_bits(), y.get_man_bits());
    auto res_bias = calc_bias(
        res_exp_bits, x.get_exp_bits(), x.get_bias(), y.get_exp_bits(), y.get_bias()
    );

    // Compute result sign
    bool res_sign = src1->sign ^ src2->sign;
    dst->sign = res_sign;

    // Handle special operands
    if (is_max_exponent(*src1, x) || is_max_exponent(*src2, y) || is_zero(*src1)
        || is_zero(*src2)) {

        if (is_nan(*src1, x) || is_nan(*src2, y) || (is_inf(*src1, x) && is_zero(*src2))
            || (is_zero(*src1) && is_inf(*src2, y))) {
            // Set to NaN
            dst->exp = ((1ULL << res_exp_bits) - 1);
            dst->man = 1;
            return; // early exit
        }

        if (is_inf(*src1, x) || is_inf(*src2, y)) {
            // Set to inf
            dst->exp = ((1ULL << res_exp_bits) - 1);
            dst->man = 0;
            return; // early exit
        }

        if (is_zero(*src1) || is_zero(*src2)) {
            // Set to zero
            dst->exp = 0;
            dst->man = 0;
            return; // early exit
        }
    }

    const auto quantization = get_float_quantization_mode();
    const unsigned int sum_man_bits = x.get_man_bits() + y.get_man_bits();

    if (sum_man_bits + 3 <= _MAN_T_SIZE_BITS) {
        const auto new_man_bits = sum_man_bits + 2;
        const auto man_bits_delta = new_man_bits - res_man_bits;

        // Tentative exponent
        std::int64_t tmp_exp = true_exp(*src1, x) + true_exp(*src2, y) + res_bias;
        const man_t two = 1ULL << (new_man_bits);
        const man_t two_before = 1ULL << (new_man_bits - 1);
        const man_t one_before = 1ULL << (new_man_bits - 2);
        const man_t two_res = (1ULL << res_man_bits);

        man_t new_man = true_man(*src1, x) * true_man(*src2, y);

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
            if (tmp_exp < -static_cast<std::int64_t>(res_man_bits)) {
                // Exponent too small after rounding
                dst->man = quantize_close_to_zero(res_sign, new_man, quantization);
                dst->exp = 0;
                return; // early exit
            }
            new_man = (new_man >> (-tmp_exp + 1))
                | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
            tmp_exp = 0;
        }

        exp_t res_exp = static_cast<exp_t>(tmp_exp);
        new_man &= (two - 1);
        quantize_mantissa(
            new_man,
            res_exp,
            ((1ULL << res_exp_bits) - 1), // max_exponent
            man_bits_delta,
            res_sign,
            two_res,
            quantization
        );

        dst->man = new_man;
        dst->exp = res_exp;
        return;
    } else {
        // Normalize both inputs
        auto [norm_x, norm_x_exp_bits, norm_x_bias] = normalize(*src1, x);
        auto [norm_y, norm_y_exp_bits, norm_y_bias] = normalize(*src2, y);

        // Add leading one's
        const man_t mx = true_man(norm_x, norm_x_exp_bits, x.get_man_bits());
        const man_t my = true_man(norm_y, norm_y_exp_bits, y.get_man_bits());

        // Tentative exponent
        std::int64_t new_exp = ((std::int64_t)norm_x.exp - (std::int64_t)norm_x_bias)
            + ((std::int64_t)norm_y.exp - (std::int64_t)norm_y_bias) + res_bias;

        // Two integer bits, sign bit and leading one
        const APyFixed apy_mx(
            2 + x.get_man_bits(), 2, limb_vector_from_uint64_t({ mx })
        );
        const APyFixed apy_my(
            2 + y.get_man_bits(), 2, limb_vector_from_uint64_t({ my })
        );

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
        quantize_apymantissa(apy_res, res_sign, res_man_bits, quantization);

        // Carry from quantization
        if (apy_res.positive_greater_than_equal_pow2(1)) {
            new_exp++;
            apy_res >>= 1;
        }

        if (new_exp >= exp_t((1ULL << res_exp_bits) - 1)) {
            if (do_infinity(quantization, res_sign)) {
                dst->exp = ((1ULL << res_exp_bits) - 1);
                dst->man = 0;
            } else {
                dst->exp = ((1ULL << res_exp_bits) - 1) - 1;
                dst->man = (1ULL << res_man_bits) - 1;
            }
            return;
        }

        if (apy_res.positive_greater_than_equal_pow2(0)) { // Remove leading one
            apy_res = apy_res - fx_one;

            // If a leading one is present while the exponent is zero,
            // then it 'acts like a carry' and creates a normal number
            if (new_exp == 0) {
                new_exp = 1;
            }
        }
        apy_res <<= res_man_bits;
        dst->man = (man_t)(apy_res).to_double();
        dst->exp = new_exp;
        return;
    }
}

//! Iterator-based multiply-accumulate
template <
    typename RANDOM_ACCESS_ITERATOR_IN,
    typename RANDOM_ACCESS_ITERATOR_INOUT,
    typename APYFLOAT_TYPE>
void float_inner_product(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APYFLOAT_TYPE& x, // Floating point src1
    const APYFLOAT_TYPE& y, // Floating point src2
    std::size_t n_items     // Number of elements to use in inner product
)
{
    // Compute result bit specification
    auto res_exp_bits = std::max(x.get_exp_bits(), y.get_exp_bits());
    auto res_man_bits = std::max(x.get_man_bits(), y.get_man_bits());
    auto res_bias = calc_bias(
        res_exp_bits, x.get_exp_bits(), x.get_bias(), y.get_exp_bits(), y.get_bias()
    );

    APyFloatData product { 0, 0, 0 };
    APyFloat accumulator(0, 0, 0, res_exp_bits, res_man_bits, res_bias);
    for (std::size_t i = 0; i < n_items; i++) {
        // Multiply
        float_product(src1 + i, src2 + i, &product, x, y);

        // Accumulate
        accumulator += APyFloat(product, res_exp_bits, res_man_bits, res_bias);
    }

    *dst = accumulator.get_data();
}

#endif // _APYFLOAT_UTIL_H
