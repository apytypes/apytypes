/*
 * Arithmetic and utility functions for APyTypes floating-point types
 */

#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apytypes_common.h"
#include "apytypes_fwd.h"
#include "apytypes_mp.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// Standard header includes
#include <algorithm>
#include <cassert>     // assert
#include <functional>  // std::invoke, std::cref, std::ref
#include <optional>    // std::optional
#include <string_view> // std::string_view

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

/* ********************************************************************************** *
 * *                        Floating-point helper utilities                         * *
 * ********************************************************************************** */

[[maybe_unused]] static APY_INLINE bool is_zero(const APyFloatData& src)
{
    return src.exp == 0 && src.man == 0;
}

[[maybe_unused]] static APY_INLINE bool
is_max_exponent(const APyFloatData& src, uint8_t exp_bits)
{
    return src.exp == ((1ULL << exp_bits) - 1);
}

[[maybe_unused]] static APY_INLINE bool
is_max_exponent(const APyFloatData& src, const APyFloatSpec& spec)
{
    return is_max_exponent(src, spec.exp_bits);
}

[[maybe_unused]] static APY_INLINE bool
is_normal(const APyFloatData& src, uint8_t exp_bits)
{
    return src.exp != 0 && !is_max_exponent(src, exp_bits);
}

[[maybe_unused]] static APY_INLINE bool
is_nan(const APyFloatData& src, uint8_t exp_bits)
{
    return is_max_exponent(src, exp_bits) && src.man != 0;
}

[[maybe_unused]] static APY_INLINE bool
is_nan(const APyFloatData& src, const APyFloatSpec& spec)
{
    return is_nan(src, spec.exp_bits);
}

[[maybe_unused]] static APY_INLINE bool
is_inf(const APyFloatData& src, uint8_t exp_bits)
{
    return is_max_exponent(src, exp_bits) && src.man == 0;
}

[[maybe_unused]] static APY_INLINE bool
is_inf(const APyFloatData& src, const APyFloatSpec& spec)
{
    return is_inf(src, spec.exp_bits);
}

[[maybe_unused]] static APY_INLINE bool
is_finite(const APyFloatData& src, const APyFloatSpec& spec)
{
    return !is_inf(src, spec) && !is_nan(src, spec);
}

[[maybe_unused]] static APY_INLINE int64_t true_exp(const APyFloatData& src, exp_t bias)
{
    return std::int64_t(src.exp) - std::int64_t(bias) + (src.exp == 0);
}

[[maybe_unused]] static APY_INLINE int64_t
true_exp(const APyFloatData& src, const APyFloatSpec& spec)
{
    return true_exp(src, spec.bias);
}

[[maybe_unused]] static APY_INLINE man_t
true_man(const APyFloatData& src, uint8_t exp_bits, uint8_t man_bits)
{
    return ((static_cast<man_t>(is_normal(src, exp_bits)) << man_bits) | src.man);
}

[[maybe_unused]] static APY_INLINE man_t
true_man(const APyFloatData& src, const APyFloatSpec& spec)
{
    return true_man(src, spec.exp_bits, spec.man_bits);
}

//! The pure exponent (`pure_exp`) is the bit index of the first non-zero bit in a
//! floating-point number, if it exists. It can be thought of as the exponent, in base
//! 2, for the number written using scientific notation. The pure exponent is equal to
//! the true biased exponent for all normal numbers.
[[maybe_unused]] static APY_INLINE std::tuple<int64_t, std::size_t>
pure_exp(const APyFloatData& src, const APyFloatSpec& spec)
{
    if (src.exp == 0) {
        std::size_t bw = bit_width(src.man);
        return { std::int64_t(bw) - std::int64_t(spec.bias) - spec.man_bits, bw };
    } else {
        return { std::int64_t(src.exp) - std::int64_t(spec.bias), spec.man_bits + 1 };
    }
}

//! Return a normalized (non subnormal) floating-point copy of `src`. Returns a
//! three-tuple, [ `APyFloatData`, `exp_bits`, `bias` ]
[[maybe_unused]] static APY_INLINE std::tuple<APyFloatData, uint8_t, exp_t>
normalize(const APyFloatData& src, uint8_t exp_bits, uint8_t man_bits, exp_t bias)
{
    if (!(src.exp == 0 && src.man != 0)) { // if not subnormal
        return { src, exp_bits, bias };
    }

    man_t new_man = src.man;
    std::int64_t tmp_exp = true_exp(src, bias);
    while (!(new_man & (1ULL << man_bits))) {
        new_man <<= 1;
        tmp_exp--;
    }

    // Possibly use more exponent bits
    int new_exp_bits = exp_bits;
    exp_t extended_bias = bias;
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

[[maybe_unused]] static APY_INLINE std::tuple<APyFloatData, uint8_t, exp_t>
normalize(const APyFloatData& src, const APyFloatSpec& spec)
{
    return normalize(src, spec.exp_bits, spec.man_bits, spec.bias);
}

//! Retrieve a generalized IEEE-754 bias for `exp_bits`
[[maybe_unused]] static APY_INLINE exp_t ieee_bias(std::uint8_t exp_bits)
{
    return (1ULL << (exp_bits - 1)) - 1;
}

//! Calculate new bias. Assumes new_exp_bits is larger than exp_bits1 and exp_bits2.
[[maybe_unused]] static APY_INLINE exp_t
calc_bias(int new_exp_bits, int exp_bits1, exp_t bias1, int exp_bits2, exp_t bias2)
{
    const auto s1 = (bias1 + 1) << (new_exp_bits - exp_bits1);
    const auto s2 = (bias2 + 1) << (new_exp_bits - exp_bits2);
    return ((s1 + s2) >> 1) - 1;
}

//! Calculate new bias. Assumes new_exp_bits is larger than exp_bits1 and exp_bits2.
[[maybe_unused]] static APY_INLINE exp_t
calc_bias(int new_exp_bits, const APyFloatSpec& spec1, const APyFloatSpec& spec2)
{
    return calc_bias(
        new_exp_bits, spec1.exp_bits, spec1.bias, spec2.exp_bits, spec2.bias
    );
}

/* ********************************************************************************** *
 * *                     Floating-point quantization functions                      * *
 * ********************************************************************************** */

//! Test if a quantization `mode` shall saturate to infinity or greatest normal number
[[maybe_unused]] static APY_INLINE bool do_infinity(QuantizationMode mode, bool sign)
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

//! Specialized floating-point mantissa quantization functions.
template <QuantizationMode QNTZ, bool SUPPORT_NEGATIVE_BITS_TO_QUANTIZE = false>
[[maybe_unused]] static APY_INLINE auto _qntz_func(
    man_t& man,
    exp_t& exp,
    exp_t max_exp,
    int bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    man_t sticky_constant
)
{
    if constexpr (SUPPORT_NEGATIVE_BITS_TO_QUANTIZE) {
        if (bits_to_quantize < 0) {
            man <<= -bits_to_quantize;
            return; // early exit
        }
    }

    // Calculate quantization bit
    man_t G, T, B;
    man_t res_man = man >> bits_to_quantize;
    G = (man >> (bits_to_quantize - 1)) & 1;
    T = (man & sticky_constant) != 0;

    if constexpr (QNTZ == QuantizationMode::RND_CONV) {
        B = G & (res_man | T);
    } else if constexpr (QNTZ == QuantizationMode::RND_CONV_ODD) {
        B = G & ((res_man ^ 1) | T);
    } else if constexpr (QNTZ == QuantizationMode::TRN_INF) {
        B = sign ? 0 : (G | T);
    } else if constexpr (QNTZ == QuantizationMode::TRN) {
        B = sign ? (G | T) : 0;
    } else if constexpr (QNTZ == QuantizationMode::TRN_AWAY) {
        B = G | T;
    } else if constexpr (QNTZ == QuantizationMode::TRN_ZERO) {
        B = 0;
    } else if constexpr (QNTZ == QuantizationMode::TRN_MAG) {
        B = sign;
    } else if constexpr (QNTZ == QuantizationMode::RND_INF) {
        B = G;
    } else if constexpr (QNTZ == QuantizationMode::RND_ZERO) {
        B = G & T;
    } else if constexpr (QNTZ == QuantizationMode::RND) {
        B = G & (T | !sign);
    } else if constexpr (QNTZ == QuantizationMode::RND_MIN_INF) {
        B = G & (T | sign);
    } else if constexpr (QNTZ == QuantizationMode::JAM) {
        B = 0;
        res_man |= 1;
    } else if constexpr (QNTZ == QuantizationMode::JAM_UNBIASED) {
        B = 0;
        if (T || G) {
            res_man |= 1;
        }
    } else if constexpr (QNTZ == QuantizationMode::STOCH_WEIGHTED) {
        const man_t trailing_bits = man & ((1ULL << bits_to_quantize) - 1);
        const man_t weight = random_number_float() & ((1ULL << bits_to_quantize) - 1);
        B = (trailing_bits + weight) >> bits_to_quantize;
    } else if constexpr (QNTZ == QuantizationMode::STOCH_EQUAL) {
        B = (G || T) ? random_number_float() & 1 : 0;
    } else {
        apytypes_unreachable();
    }

    man = res_man;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }

    // Check for overflow. This must always be checked since other methods depend on
    // it.
    if (exp >= max_exp) {
        if (do_infinity(QNTZ, sign)) {
            exp = max_exp;
            man = 0;
        } else {
            exp = max_exp - 1;
            man = man_msb_constant - 1;
        }
    }
}

//! Retrieve a specialized quantization function
[[maybe_unused]] static APY_INLINE auto
get_qntz_func(QuantizationMode qntz, bool support_negative_bits_to_quantize = false)
{
#define GET_QNTZ_FUNC_ALTERNATIVE(Q, SUPPORT)                                          \
    case Q:                                                                            \
        return _qntz_func<Q, SUPPORT>

    if (support_negative_bits_to_quantize) {
        switch (qntz) {
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_CONV, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_CONV_ODD, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_INF, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_AWAY, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_ZERO, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_MAG, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_INF, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_ZERO, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_MIN_INF, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::JAM, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::JAM_UNBIASED, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::STOCH_WEIGHTED, true);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::STOCH_EQUAL, true);
        }
    } else { /* !support_negatibe_bits_to_quantize */
        switch (qntz) {
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_CONV, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_CONV_ODD, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_INF, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_AWAY, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_ZERO, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::TRN_MAG, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_INF, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_ZERO, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::RND_MIN_INF, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::JAM, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::JAM_UNBIASED, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::STOCH_WEIGHTED, false);
            GET_QNTZ_FUNC_ALTERNATIVE(QuantizationMode::STOCH_EQUAL, false);
        }
    }

    // No quantization function found...
    throw NotImplementedException(
        "get_qntz_func(): unknown quantization, did you pass `int` as "
        "`QuantizationMode`?"
    );

#undef GET_QNTZ_FUNC_ALTERNATIVE
}

//! Quantize mantissa when the result is guaranteed to be either be 0 or 1.
//! Quantization mode `STOCH_WEIGHTED` should be used with this function.
[[maybe_unused]] static APY_INLINE man_t
quantize_close_to_zero(bool sign, QuantizationMode quantization)
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

/* ********************************************************************************** *
 * *                        Floating-point cast functions                           * *
 * ********************************************************************************** */

//! Cast a floating-point value from one format to another
template <typename QNTZ_FUNC_SIGNATURE>
[[maybe_unused]] static APY_INLINE APyFloatData floating_point_cast(
    const APyFloatData& src,
    const APyFloatSpec& src_spec,
    const APyFloatSpec& dst_spec,
    QuantizationMode qntz,
    QNTZ_FUNC_SIGNATURE qntz_func
)
{
    // Handle special values first
    if (is_max_exponent(src, src_spec)) {
        if (is_nan(src, src_spec)) {
            return { src.sign, exp_t((1ULL << dst_spec.exp_bits) - 1), 1 }; // NaN
        } else if (is_inf(src, src_spec)) {
            return { src.sign, exp_t((1ULL << dst_spec.exp_bits) - 1), 0 }; // inf
        }
    } else if (is_zero(src)) {
        return { src.sign, 0, 0 };
    }

    // Initial values for cast data
    man_t man = src.man;
    std::int64_t exp = true_exp(src, src_spec) + std::int64_t(dst_spec.bias);

    // Normalize the exponent and mantissa if converting from subnormal
    if (src.exp == 0) {
        exp_t subn_adjustment = count_trailing_bits(src.man);
        man_t remainder = src.man % (1ULL << subn_adjustment);
        exp = exp - src_spec.man_bits + subn_adjustment;
        man = remainder << (src_spec.man_bits - subn_adjustment);
    }

    // Check if the result will be subnormal after cast
    if (exp <= 0) {
        if (exp < -std::int64_t(dst_spec.man_bits)) {
            // Exponent to small after rounding
            return { src.sign, 0, quantize_close_to_zero(src.sign, qntz) };
        }

        const int MAN_BITS_DELTA = 1 - exp + (src_spec.man_bits - dst_spec.man_bits);
        const man_t LEADING_ONE = (1ULL << src_spec.man_bits);
        man |= (1ULL << src_spec.man_bits); // Add the hidden one
        if (MAN_BITS_DELTA <= 0) {
            return { src.sign, 0, (man << -MAN_BITS_DELTA) };
        } else { /* man_bits_delta > 0 */
            const exp_t MAX_EXP = (1ULL << dst_spec.exp_bits) - 1;
            const man_t STICKY = (1ULL << (MAN_BITS_DELTA - 1)) - 1;
            APyFloatData res = { src.sign, 0, man };
            qntz_func(
                res.man, res.exp, MAX_EXP, MAN_BITS_DELTA, res.sign, LEADING_ONE, STICKY
            );
            return res;
        }
    }

    // Quantize the mantissa and return
    const int MAN_BITS_DELTA = src_spec.man_bits - dst_spec.man_bits;
    const exp_t MAX_EXP = (1ULL << dst_spec.exp_bits) - 1;
    const man_t LEADING_ONE = (1ULL << dst_spec.man_bits);
    if (MAN_BITS_DELTA <= 0) {
        if (exp >= MAX_EXP) {
            if (do_infinity(qntz, src.sign)) {
                return { src.sign, MAX_EXP, 0 }; // inf
            } else {
                return { src.sign, MAX_EXP - 1, LEADING_ONE - 1 }; // largest normal
            }
        } else { /* exp < MAX_EXP */
            return { src.sign, exp_t(exp), (man << -MAN_BITS_DELTA) };
        }
    }

    APyFloatData res { src.sign, exp_t(exp), man };
    const man_t STICKY = (1ULL << (MAN_BITS_DELTA - 1)) - 1;
    qntz_func(res.man, res.exp, MAX_EXP, MAN_BITS_DELTA, res.sign, LEADING_ONE, STICKY);
    return res;
}

//! Cast a floating-point number when it is known that no quantization happens
[[maybe_unused]] static APY_INLINE APyFloatData floating_point_cast_no_quant(
    const APyFloatData& src, const APyFloatSpec& src_spec, const APyFloatSpec& dst_spec
)
{
    // Handle special values first
    if (is_max_exponent(src, src_spec.exp_bits)) {
        if (src.man) {
            return { src.sign, exp_t((1ULL << dst_spec.exp_bits) - 1), 1 }; // NaN
        } else {
            return { src.sign, exp_t((1ULL << dst_spec.exp_bits) - 1), 0 }; // +-inf
        }
    } else if (is_zero(src)) {
        return { src.sign, 0, 0 };
    }

    // Initial value for exponent
    std::int64_t new_exp = true_exp(src, src_spec) + std::int64_t(dst_spec.bias);

    // Adjust the exponent and mantissa if convertering from a subnormal
    man_t new_man;
    if (src.exp == 0) {
        const exp_t subn_adjustment = count_trailing_bits(src.man);
        if (new_exp + subn_adjustment < src_spec.man_bits) {
            // The result remains subnormal
            new_man = src.man << new_exp;
            new_exp = 0;
        } else {
            // The result becomes normal
            new_man = src.man << (src_spec.man_bits - subn_adjustment);
            new_man &= (1ULL << src_spec.man_bits) - 1;
            new_exp = new_exp + subn_adjustment - src_spec.man_bits;
        }
    } else {
        new_man = src.man;
    }

    new_man <<= dst_spec.man_bits - src_spec.man_bits;
    return { src.sign, exp_t(new_exp), man_t(new_man) };
}

//! Return the bit pattern of a floating-point data field. No checks on bit width is
//! done.
[[maybe_unused]] static APY_INLINE std::uint64_t
to_bits_uint64(const APyFloatData& d, std::uint8_t exp_bits, std::uint8_t man_bits)
{
    return (std::uint64_t(d.sign) << (exp_bits + man_bits))
        | (std::uint64_t(d.exp) << man_bits) | std::uint64_t(d.man);
}

[[maybe_unused]] static APY_INLINE nb::int_
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

    auto limb_list = { UINT64_TO_LIMB(lower), UINT64_TO_LIMB(higher) };
    return python_limb_vec_to_long(limb_list.begin(), limb_list.end(), false);
}

//! Fast integer power by squaring.
[[maybe_unused]] static APY_INLINE man_t ipow(man_t base, unsigned int n)
{
    man_t result = 1;
    for (;;) {
        if (n & 1) {
            result *= base;
        }
        n >>= 1;
        if (!n) {
            break;
        }
        base *= base;
    }
    return result;
}

//! Get the number of left shifts needed to make fx>=1.0
[[maybe_unused]] static APY_INLINE unsigned int
leading_zeros_apyfixed(const APyFixed& fx)
{
    const int zeros = fx.leading_zeros() - fx.int_bits();
    return std::max(0, zeros + 1);
}

//! Translate the quantization mode for floating-point to the fixed-point equivalent.
//! This is used for the mantissa so the sign must be taken into account.
[[maybe_unused]] static APY_INLINE QuantizationMode
translate_quantization_mode(QuantizationMode quantization, bool sign)
{
    switch (quantization) {
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        return sign ? QuantizationMode::TRN : QuantizationMode::TRN_INF;
    case QuantizationMode::TRN: // TO_NEGATIVE
        return sign ? QuantizationMode::TRN_INF : QuantizationMode::TRN;
    case QuantizationMode::RND: // TIES_POS
        return sign ? QuantizationMode::RND_ZERO : QuantizationMode::RND;
    case QuantizationMode::RND_MIN_INF: // TIES_NEG
        return sign ? QuantizationMode::RND : QuantizationMode::RND_ZERO;
    default:
        return quantization;
    }
}

//! Quantize mantissa stored as an `APyFixed`
[[maybe_unused]] static APY_INLINE void quantize_apymantissa(
    APyFixed& apyman, bool sign, int bits, QuantizationMode quantization
)
{
    if (quantization == QuantizationMode::STOCH_WEIGHTED) {
        auto rnd_data = { UINT64_TO_LIMB(random_number_float()),
                          UINT64_TO_LIMB(random_number_float()),
                          UINT64_TO_LIMB(0) };
        APyFixed rnd_num(64 * 3, 64 - bits, rnd_data);
        apyman = apyman + rnd_num;
        apyman = apyman.cast_no_overflow(2 + bits, 2, QuantizationMode::TRN);
    } else if (quantization == QuantizationMode::STOCH_EQUAL) {
        const apy_limb_t rnd = -(random_number_float() % 2);
        auto rnd_data = { UINT64_TO_LIMB(rnd), UINT64_TO_LIMB(rnd), UINT64_TO_LIMB(0) };
        APyFixed rnd_num(64 * 3, 64 - bits, rnd_data);
        apyman = apyman + rnd_num;
        apyman = apyman.cast_no_overflow(2 + bits, 2, QuantizationMode::TRN);
    } else {
        quantization = translate_quantization_mode(quantization, sign);
        apyman = apyman.cast_no_overflow(3 + bits, 3, quantization);
    }
}

//! Check that the number of exponent bits is allowed, throw otherwise
[[maybe_unused]] static APY_INLINE std::uint8_t
check_exponent_format(int exp_bits, std::string_view exception_msg_prefix = "apytypes")
{
    if ((unsigned(exp_bits) > _EXP_LIMIT_BITS) || (exp_bits < 0)) {
        std::string msg = fmt::format(
            "{}: "
            "exponent bits must be a non-negative integer less or equal to {} but {} "
            "was given",
            exception_msg_prefix,
            _EXP_LIMIT_BITS,
            exp_bits
        );
        throw nb::value_error(msg.c_str());
    }

    return std::uint8_t(exp_bits);
}

//! Check that the number of mantissa bits is allowed, throw otherwise
[[maybe_unused]] static APY_INLINE std::uint8_t
check_mantissa_format(int man_bits, std::string_view exception_msg_prefix = "apytypes")
{
    if ((unsigned(man_bits) > _MAN_LIMIT_BITS) || (man_bits < 0)) {
        std::string msg = fmt::format(
            "{}: "
            "mantissa bits must be a non-negative integer less or equal to {} but {} "
            "was given",
            exception_msg_prefix,
            _MAN_LIMIT_BITS,
            man_bits

        );
        throw nb::value_error(msg.c_str());
    }

    return std::uint8_t(man_bits);
}

/* ********************************************************************************** *
 * *                           Floating-point conversion                            * *
 * ********************************************************************************** */

template <typename RANDOM_ACCESS_ITERATOR_IN>
APyFloatData floating_point_from_fixed_point(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    int bits,
    int int_bits,
    int exp_bits,
    int man_bits,
    exp_t bias,
    QuantizationMode q_mode = QuantizationMode::RND_CONV
)
{
    assert(std::distance(cbegin_it, cend_it) == ptrdiff_t(bits_to_limbs(bits)));

    if (limb_vector_is_zero(cbegin_it, cend_it)) {
        return { 0, 0, 0 };
    }

    // Create the resulting floating-point data and extract its sign by taking absolute
    // value of fixed-point number.
    ScratchVector<apy_limb_t, 8> fx_abs(std::distance(cbegin_it, cend_it));
    bool sign = limb_vector_abs(cbegin_it, cend_it, std::begin(fx_abs));

    // Extract leading zeros in the fixed-point number
    std::size_t fx_lz = limb_vector_leading_zeros(std::begin(fx_abs), std::end(fx_abs));
    assert(fx_lz > 0);
    std::size_t bits_last_limb = ((bits - 1) % APY_LIMB_SIZE_BITS) + 1;
    fx_lz -= (APY_LIMB_SIZE_BITS - bits_last_limb);

    // Make the fixed-point number become [1, 2)
    const std::int64_t target_exp = int_bits - fx_lz - 1;
    int_bits -= target_exp;

    std::int64_t tmp_exp = target_exp + bias;
    if (tmp_exp <= 0) {
        int_bits += tmp_exp - 1;
        tmp_exp = 0;

        // Forcing a one (1.xx) here makes the rest of the code the same
        const unsigned binary_point = std::max(0, bits - int_bits);
        limb_vector_set_bit(std::begin(fx_abs), std::end(fx_abs), binary_point, 1);
    }

    // Cast the absolute fixed-point mantissa to a `fx<int_bits=3, frac_bits=man_bits>`
    // `fixed_point_cast_unsafe` is safe to use because `fx_man` is guaranteed have as
    // many limbs as `fx_abs`.
    ScratchVector<apy_limb_t, 8> fx_man(bits_to_limbs(std::max(3 + man_bits, bits)));
    assert(fx_man.size() >= fx_abs.size());
    fixed_point_cast_unsafe(
        std::begin(fx_abs), // src_begin
        std::end(fx_abs),   // src_end
        std::begin(fx_man), // dst_begin
        std::end(fx_man),   // dst_end
        bits,               // src_bits
        int_bits,           // src_int_bits
        3 + man_bits,       // dst_bits
        3,                  // dst_int_bits
        q_mode,             // q_mode
        OverflowMode::WRAP  // v_mode (NOTE: this can never overflow, use `WRAP`)
    );

    // Check for carry
    bool c = limb_vector_gte_pow2(std::begin(fx_man), std::end(fx_man), man_bits + 1);
    tmp_exp += c;

    // Check for overflow
    const exp_t MAX_EXP = (1ULL << exp_bits) - 1;
    if (tmp_exp >= MAX_EXP) {
        return { sign, MAX_EXP, 0 }; // inf
    } else {
        // We know the mantissa is in [1, 2), so remove the leading one
        limb_vector_set_bit(std::begin(fx_man), std::end(fx_man), man_bits + c, 0);
        return { sign, exp_t(tmp_exp), uint64_t_from_limb_vector(fx_man, 0) };
    }
}

[[maybe_unused]] static APY_INLINE double
floating_point_to_double(const APyFloatData& data_in, const APyFloatSpec& spec)
{
    constexpr APyFloatSpec double_spec = { 11, 52, 1023 };
    constexpr QuantizationMode qntz = QuantizationMode::RND_CONV;
    const APyFloatData data_double
        = floating_point_cast(data_in, spec, double_spec, qntz, get_qntz_func(qntz));

    double res {};
    set_sign_of_double(res, data_double.sign);
    set_exp_of_double(res, data_double.exp);
    set_man_of_double(res, data_double.man);
    return res;
}

/* ********************************************************************************** *
 * *              Floating-point iterator-based arithmetic functions                * *
 * ********************************************************************************** */

//! Multiply a floating-point value (`data` of `spec`) with 2^(`exp`) efficiently.
//! Correct rounding, as specified by `qntz` and `qntz_func` are guaranteed on
//! floating-point underflow. See cppreference for `scalbn`:
//! https://en.cppreference.com/w/c/numeric/math/scalbn
template <typename QNTZ_FUNC_SIGNATURE>
[[maybe_unused]] static APY_INLINE APyFloatData floating_point_scalbn(
    const APyFloatData& src,
    const APyFloatSpec& src_spec,
    int exp,
    QuantizationMode qntz,
    QNTZ_FUNC_SIGNATURE qntz_func

)
{
    if (is_zero(src) || is_max_exponent(src, src_spec)) {
        // If `src` is +-0, `src` is returned unmodified.
        // If `src` is +-inf, `src` is returned unmodified.
        // If `src` is NaN, NaN is returned.
        return src; // early exit
    }

    APyFloatData res;
    if (exp < 0) {
        /*
         * `exp` < 0:
         *   1) The mantissa may be right shifted if product is subnormal ==>
         *      quantization may happen.
         *   2) Product becomes smaller the floating-point source ==> product can never
         *      overflow to infinity.
         */
        int new_exp = int(src.exp) + exp;
        if (new_exp <= 0 /* result is subnormal */) {
            res = { src.sign, /* exp = */ 0, src.man };
            int shift = std::min(-new_exp + (src.exp > 0), int(_MAN_LIMIT_BITS));

            // Quantize the result
            if (shift > 0) {
                const exp_t MAX_EXP = (1ULL << src_spec.exp_bits) - 1;
                const man_t STICKY = (1ULL << (shift - 1)) - 1;
                const man_t MSB_CNST = (1ULL << (src_spec.man_bits));
                if (src.exp > 0) {
                    res.man |= MSB_CNST;
                }
                qntz_func(res.man, res.exp, MAX_EXP, shift, res.sign, MSB_CNST, STICKY);
            }
        } else {
            // No quantization, floating-point source is normal and so is the product.
            res = { src.sign, src.exp + exp, src.man };
        }
    } else { /* exp >= 0 */
        /*
         * `exp` >= 0:
         *   1) The mantissa may be left shifted if the source is subnormal ==>
         *      quantization will *never* happen.
         *   2) Product becomes larger than floating-point source ==> floating-point
         *      product may overflow to infinity.
         */
        if (src.exp == 0) {
            int bw = src.exp == 0 ? bit_width(src.man) : src_spec.man_bits + 1;
            int man_shift = std::min(exp, src_spec.man_bits + 1 - bw);
            bool res_is_normal = bw + exp > src_spec.man_bits;
            man_t res_man = (src.man << man_shift) & ((1ULL << src_spec.man_bits) - 1);
            exp_t res_exp = src.exp + exp - int(man_shift) + res_is_normal;
            res = { src.sign, res_exp, res_man };
        } else { /* src.exp > 0 */
            // Floating-point source is normal, and so is the product.
            res = { src.sign, src.exp + exp, src.man };
        }

        // Handle potential overflowing
        const exp_t MAX_EXP = (1ULL << src_spec.exp_bits) - 1;
        if (res.exp >= MAX_EXP) {
            if (do_infinity(qntz, src.sign)) {
                res = { src.sign, MAX_EXP, 0 }; // inf
            } else {
                res = { src.sign, MAX_EXP - 1, (1ULL << src_spec.man_bits) - 1 };
            }
        }
    }

    return res;
}

//! Iterator-based less-than function, comparing `src1 < src2`
[[maybe_unused]] static APY_INLINE bool floating_point_less_than(
    const APyFloatData& src1,
    const APyFloatSpec& src1_spec,
    const APyFloatData& src2,
    const APyFloatSpec& src2_spec
)
{
    if (is_max_exponent(src1, src1_spec) || is_max_exponent(src2, src2_spec)) {
        if (is_nan(src1, src1_spec.exp_bits) || is_nan(src2, src2_spec.exp_bits)) {
            return false;
        } else {
            const bool src1_is_inf = is_inf(src1, src1_spec);
            const bool src2_is_inf = is_inf(src2, src2_spec);
            if (src1_is_inf && src2_is_inf) {
                return src1.sign && !src2.sign;
            } else {
                return src1_is_inf ? src1.sign : !src2.sign;
            }
        }
    } else if (is_zero(src1)) {
        return is_zero(src2) ? false : !src2.sign;
    } else if (is_zero(src2)) {
        return src1.sign;
    } else if (src1.sign != src2.sign) {
        return src1.sign;
    }

    auto [src1_pure_exp, src1_bw] = pure_exp(src1, src1_spec);
    auto [src2_pure_exp, src2_bw] = pure_exp(src2, src2_spec);
    if (src1_pure_exp == src2_pure_exp) {
        man_t src1_man = true_man(src1, src1_spec) << (_MAN_T_SIZE_BITS - src1_bw);
        man_t src2_man = true_man(src2, src2_spec) << (_MAN_T_SIZE_BITS - src2_bw);
        return src1.sign ? (src1_man > src2_man) : (src1_man < src2_man);
    } else {
        return src1_pure_exp < src2_pure_exp ? !src1.sign : src1.sign;
    }
}

//! Iterator-based less-than function, comparing `abs(src1) < abs(src2)`
[[maybe_unused]] static APY_INLINE bool floating_point_less_than_abs(
    const APyFloatData& src1,
    const APyFloatSpec& src1_spec,
    const APyFloatData& src2,
    const APyFloatSpec& src2_spec
)
{
    if (is_nan(src1, src1_spec.exp_bits) || is_nan(src2, src2_spec.exp_bits)) {
        return false;
    } else if (is_zero(src2)) {
        return false;
    } else if (is_zero(src1)) {
        return true;
    }

    auto [src1_pure_exp, src1_bw] = pure_exp(src1, src1_spec);
    auto [src2_pure_exp, src2_bw] = pure_exp(src2, src2_spec);
    if (src1_pure_exp == src2_pure_exp) {
        std::uint64_t n1_man = src1.man << (_MAN_T_SIZE_BITS - src1_bw);
        std::uint64_t n2_man = src2.man << (_MAN_T_SIZE_BITS - src2_bw);
        return n1_man < n2_man;
    } else {
        return src1_pure_exp < src2_pure_exp;
    }
}

//! Memory-based absolute value less-than function, comparing `abs(src1) < abs(src2)`
//! when `src1` and `src2` have the same bit specifiers. This method assumes that
//! neither `src1` nor `src2` are NaN or inf.
[[maybe_unused]] static APY_INLINE bool
floating_point_less_than_abs_same_wl(const APyFloatData& src1, const APyFloatData& src2)
{
    return ((src1.exp == src2.exp) && (src1.man < src2.man)) || (src1.exp < src2.exp);
}

//! Floating-point addition of `src1` and `src2` when they share the exact word length
//! (`exp_bits`, `man_bits`, and `bias`). To use this function, all of the following
//! must be satisfied:
//! * `spec.man_bits + 5 <= _MAN_T_SIZE_BITS`
//! * `qntz != QuantizationMode::STOCH_WEIGHTED`
template <
    const bool SUB = false,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT,
    typename QNTZ_FUNC_SIGNATURE>
[[maybe_unused]] static APY_INLINE void _floating_point_add_same_wl(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& spec,
    const QuantizationMode& qntz,
    QNTZ_FUNC_SIGNATURE qntz_func,
    const exp_t MAX_EXP,
    const man_t FINAL_RES_LO,
    const man_t RES_LO,
    const man_t CARRY_RES_LO,
    const man_t MAN_MASK,
    const unsigned NORMALIZATION_CONST
)
{
    // Constexpr sign-flipping logic for conditionally performing subtraction
    constexpr auto GET_SIGN = [](bool sign, bool swap) { return sign ^ (SUB && swap); };

    // Handle the NaN and inf cases
    APyFloatData& z = *dst;
    if (src1->exp == MAX_EXP || src2->exp == MAX_EXP) {
        const bool swap = floating_point_less_than_abs(*src1, spec, *src2, spec);
        const APyFloatData& x = swap ? *src2 : *src1;
        const APyFloatData& y = swap ? *src1 : *src2;
        const bool x_sign = GET_SIGN(x.sign, swap);
        const bool y_sign = GET_SIGN(y.sign, !swap);
        bool x_is_nan = x.exp == MAX_EXP && x.man != 0;
        bool y_is_nan = y.exp == MAX_EXP && y.man != 0;
        bool both_inf = x.exp == MAX_EXP && y.exp == MAX_EXP;
        if (x_is_nan || y_is_nan || (both_inf && x_sign != y_sign)) {
            z = { x_sign, exp_t(MAX_EXP), man_t(1) }; // Set to NaN
        } else {
            bool sign = x.man == 0 ? x_sign : y_sign;
            z = { sign, exp_t(MAX_EXP), man_t(0) }; // Set to inf
        }
        return;
    }

    // Make sure `x` has a bigger absolute value than `y`
    const bool swap = floating_point_less_than_abs_same_wl(*src1, *src2);
    const APyFloatData& x = swap ? *src2 : *src1;
    const APyFloatData& y = swap ? *src1 : *src2;
    const bool x_sign = GET_SIGN(x.sign, swap);
    const bool y_sign = GET_SIGN(y.sign, !swap);

    // Tentative exponent
    exp_t new_exp = x.exp + (x.exp == 0);

    // Conditionally add leading one. Also make room for guard bits. Note that
    // `exp` can never be `res_max_exponent` here.
    const man_t mx = (x.exp == 0 ? 0 : RES_LO) | x.man << 3;
    const man_t my = (y.exp == 0 ? 0 : RES_LO) | y.man << 3;

    // Align mantissa based on difference in exponent
    man_t my_aligned;
    const unsigned exp_delta = new_exp - y.exp - (y.exp == 0);
    if (exp_delta <= 3) {
        my_aligned = my >> exp_delta;
    } else if (exp_delta >= _MAN_T_SIZE_BITS) {
        my_aligned = (my != 0);
    } else {
        bool round_bit = ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
        my_aligned = (my >> exp_delta) | round_bit;
    }

    // Perform addition / subtraction
    man_t new_man = (x_sign == y_sign) ? mx + my_aligned : mx - my_aligned;

    // Check for carry and cancellation
    if (new_man & CARRY_RES_LO) {
        // Carry
        new_exp++;
    } else if (new_man & RES_LO) {
        // Align mantissa to carry case
        new_man <<= 1;
    } else {
        if (new_man == 0 && x_sign != y_sign && x.exp == y.exp) {
            man_t res_man = qntz == QuantizationMode::JAM ? 1 : 0;
            bool res_sign = qntz == QuantizationMode::TRN;
            z = { res_sign, exp_t(0), res_man };
            return;
        }

        // Cancellation or addition with subnormals
        // Mantissa should be shifted until 1.xx is obtained or new_exp equals 0
        const unsigned man_leading_zeros = leading_zeros(new_man);
        const unsigned shift = man_leading_zeros - NORMALIZATION_CONST;
        if (new_exp > shift) {
            new_man <<= shift + 1;
            new_exp -= shift;
        } else {
            new_man <<= new_exp;
            new_exp = 0;
        }
    }
    new_man &= MAN_MASK;

    qntz_func(new_man, new_exp, MAX_EXP, 4, x_sign, FINAL_RES_LO, 7);
    z = { x_sign, new_exp, new_man };
    return;
}

//! Floating-point addition of `src1` and `src2` when they don't share word lengths
//! (any of: `exp_bits`, `man_bits`, and `bias`). To use this function, all of the
//! following must be satisfied:
//! * `dst_spec.man_bits + 5 <= _MAN_T_SIZE_BITS`
//! * `qntz != QuantizationMode::STOCH_WEIGHTED`
template <
    const bool SUB = false,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT,
    typename QNTZ_FUNC_SIGNATURE>
[[maybe_unused]] static APY_INLINE void _floating_point_add_diff_wl(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz,
    QNTZ_FUNC_SIGNATURE qntz_func,
    const exp_t RES_MAX_EXP,
    const man_t FINAL_RES_LO,
    const man_t RES_LO,
    const man_t CARRY_RES_LO,
    const man_t MAN_MASK,
    const unsigned NORMALIZATION_CONST
)
{
    // Constexpr sign-flipping logic for conditionally performing subtraction
    constexpr auto GET_SIGN = [](bool sign, bool swap) { return sign ^ (SUB && swap); };

    // Make sure `x` has a bigger absolute value than `y`
    const bool swap = floating_point_less_than_abs(*src1, src1_spec, *src2, src2_spec);
    const APyFloatSpec& x_spec = swap ? src2_spec : src1_spec;
    const APyFloatSpec& y_spec = swap ? src1_spec : src2_spec;
    const APyFloatData& x = swap ? *src2 : *src1;
    const APyFloatData& y = swap ? *src1 : *src2;
    const bool x_sign = GET_SIGN(x.sign, swap);
    const bool y_sign = GET_SIGN(y.sign, !swap);
    APyFloatData& z = *dst;

    // Handle the NaN and inf cases
    if (is_max_exponent(x, x_spec) || is_max_exponent(y, y_spec)) {
        if (is_nan(x, x_spec) || is_nan(y, y_spec)
            || (x_sign != y_sign && is_inf(x, x_spec) && is_inf(y, y_spec))) {
            z = { x_sign, RES_MAX_EXP, 1 }; // NaN
            return;
        } else {
            bool sign = is_max_exponent(x, x_spec) ? x_sign : y_sign;
            z = { sign, RES_MAX_EXP, 0 }; // Inf
            return;
        }
    } else if (is_zero(x)) {
        // If `x` is zero, than so is `y`
        bool res_sign = x_sign == y_sign ? x_sign : qntz == QuantizationMode::TRN;
        man_t res_man = qntz == QuantizationMode::JAM ? 1 : 0;
        z = { res_sign, 0, res_man };
        return;
    }

    APyFloatData x_wide = x_spec == dst_spec
        ? x
        : floating_point_cast_no_quant(
              x, x_spec, { dst_spec.exp_bits, dst_spec.man_bits, x_spec.bias }
          );
    APyFloatData y_wide = y_spec == dst_spec
        ? y
        : floating_point_cast_no_quant(
              y, y_spec, { dst_spec.exp_bits, dst_spec.man_bits, y_spec.bias }
          );

    // Tentative exponent
    exp_t new_exp = true_exp(x_wide, x_spec.bias) + dst_spec.bias;
    if (new_exp > RES_MAX_EXP) {
        exp_t exp = int(new_exp) < 0 ? 0 : RES_MAX_EXP;
        man_t res_man = exp ? (qntz == QuantizationMode::JAM ? 1 : 0) : 0;
        z = { x_sign, exp, res_man };
        return;
    }

    // Conditionally add leading one. Also make room for guard bits. Note that
    // `exp` can never be `res_max_exponent` here.
    man_t mx = (x_wide.exp == 0 ? 0 : RES_LO) | x_wide.man << 3;
    man_t my = (y_wide.exp == 0 ? 0 : RES_LO) | y_wide.man << 3;

    // Align mantissa based on difference in exponent
    man_t my_aligned;
    const unsigned exp_delta = true_exp(x_wide, x_spec) - true_exp(y_wide, y_spec);
    if (exp_delta <= 3) {
        my_aligned = my >> exp_delta;
    } else if (exp_delta >= _MAN_T_SIZE_BITS) {
        my_aligned = (my != 0);
    } else {
        bool round_bit = ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
        my_aligned = (my >> exp_delta) | round_bit;
    }

    // Perform addition / subtraction
    man_t new_man = (x_sign == y_sign) ? mx + my_aligned : mx - my_aligned;

    // Check for carry and cancellation
    if (new_man & CARRY_RES_LO) {
        new_exp++;
    } else if (new_man & RES_LO) {
        new_man <<= 1;
    } else {
        if (x_sign != y_sign && x_wide.man == y_wide.man) {
            if (true_exp(x_wide, x_spec) == true_exp(y_wide, y_spec)) {
                man_t res_man = qntz == QuantizationMode::JAM ? 1 : 0;
                bool res_sign = qntz == QuantizationMode::TRN;
                z = { res_sign, 0, res_man };
                return;
            }
        }

        // Cancellation or addition with subnormals. Mantissa should be shifted
        // until 1.xx is obtained or new_exp equals 0.
        const unsigned man_leading_zeros = leading_zeros(new_man);
        const unsigned shift = man_leading_zeros - NORMALIZATION_CONST;
        if (new_exp > shift) {
            new_man <<= shift + 1;
            new_exp -= shift;
        } else {
            new_man <<= new_exp;
            new_exp = 0;
        }
    }
    new_man &= MAN_MASK;

    qntz_func(new_man, new_exp, RES_MAX_EXP, 4, x_sign, FINAL_RES_LO, 7);
    z = { x_sign, new_exp, new_man };
    return;
}

//! Floating-point addition of `src1` and `src2` for when`_floating_point_add_same_wl`
//! or `_floating_point_sum_diff_wl` can't be used. Works in all cases, but is the
//! slowest.
template <
    const bool SUB = false,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void _floating_point_add_general(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz,
    const exp_t RES_MAX_EXP
)
{

    // Constexpr sign-flipping logic for conditionally performing subtraction
    constexpr auto GET_SIGN = [](bool sign, bool swap) { return sign ^ (SUB && swap); };

    // Make sure `x` has a bigger absolute value than `y`
    const bool swap = floating_point_less_than_abs(*src1, src1_spec, *src2, src2_spec);
    const APyFloatSpec& x_spec = swap ? src2_spec : src1_spec;
    const APyFloatSpec& y_spec = swap ? src1_spec : src2_spec;
    const APyFloatData& x = swap ? *src2 : *src1;
    const APyFloatData& y = swap ? *src1 : *src2;
    const bool x_sign = GET_SIGN(x.sign, swap);
    const bool y_sign = GET_SIGN(y.sign, !swap);
    APyFloatData& z = *dst;

    // Handle the NaN and inf cases
    if (is_max_exponent(x, x_spec) || is_max_exponent(y, y_spec)) {
        exp_t res_max_exponent = exp_t((1ULL << dst_spec.exp_bits) - 1);
        if (is_nan(x, x_spec) || is_nan(y, y_spec)
            || (x_sign != y_sign && is_inf(x, x_spec) && is_inf(y, y_spec))) {
            z = { x_sign, res_max_exponent, 1 }; // NaN
            return;
        } else {
            bool sign = is_max_exponent(x, x_spec) ? x_sign : y_sign;
            z = { sign, res_max_exponent, 0 }; // Inf
            return;
        }
    } else if (is_zero(x)) {
        // If `x` is zero, than so is `y`
        bool res_sign = x_sign == y_sign ? x_sign : qntz == QuantizationMode::TRN;
        man_t res_man = qntz == QuantizationMode::JAM ? 1 : 0;
        z = { res_sign, 0, res_man };
        return;
    }

    APyFloatData x_wide = x_spec == dst_spec
        ? x
        : floating_point_cast_no_quant(
              x, x_spec, { dst_spec.exp_bits, dst_spec.man_bits, x_spec.bias }
          );
    APyFloatData y_wide = y_spec == dst_spec
        ? y
        : floating_point_cast_no_quant(
              y, y_spec, { dst_spec.exp_bits, dst_spec.man_bits, y_spec.bias }
          );

    if (x_sign != y_sign) {
        if (x_wide.man == y_wide.man) {
            if (true_exp(x_wide, x_spec.bias) == true_exp(y_wide, y_spec.bias)) {
                man_t res_man = qntz == QuantizationMode::JAM ? 1 : 0;
                bool res_sign = qntz == QuantizationMode::TRN;
                z = { res_sign, 0, res_man };
                return;
            }
        }
    }

    const std::int64_t x_true_exp = true_exp(x_wide, x_spec.bias);
    const unsigned exp_delta = x_true_exp - true_exp(y_wide, y_spec.bias);
    exp_t new_exp = x_true_exp + dst_spec.bias;
    if (new_exp > RES_MAX_EXP) {
        exp_t exp = int(new_exp) < 0 ? 0 : RES_MAX_EXP;
        man_t res_man = exp ? (qntz == QuantizationMode::JAM ? 1 : 0) : 0;
        z = { x_sign, exp, res_man };
        return;
    }

    man_t mx = true_man(x_wide, dst_spec);
    man_t my = true_man(y_wide, dst_spec);

    const APyFixed apy_mx(2 + dst_spec.man_bits, 2, { UINT64_TO_LIMB(mx) });
    const APyFixed apy_my(2 + dst_spec.man_bits, 2 - exp_delta, { UINT64_TO_LIMB(my) });

    // Perform addition/subtraction
    auto apy_res = (x_sign == y_sign) ? apy_mx + apy_my : apy_mx - apy_my;

    if (apy_res.positive_greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    } else {
        // Check for cancellation by counting the number of left shifts needed
        // to make fx>=1.0
        const unsigned leading_zeros = leading_zeros_apyfixed(apy_res);
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
    quantize_apymantissa(apy_res, x_sign, dst_spec.man_bits, qntz);

    // Carry from quantization. In practice, the exponent will never be
    // incremented twice
    if (apy_res.positive_greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    }

    // Check for overflow
    if (new_exp >= ((1LL << dst_spec.exp_bits) - 1)) {
        exp_t res_max_exponent = ((1LL << dst_spec.exp_bits) - 1);
        if (do_infinity(qntz, x_sign)) {
            z = { x_sign, res_max_exponent, 0 };
            return;
        } else {
            z = { x_sign, res_max_exponent - 1, (1ULL << dst_spec.man_bits) - 1 };
            return;
        }
    }

    // Remove leading one
    if (apy_res.positive_greater_than_equal_pow2(0)) {
        apy_res = apy_res - APyFixed(2, 2, { 1 });
    }

    z = { x_sign, exp_t(new_exp), apy_res.get_lsbs() };
    return;
}

//! Floating-point multiplication of `src1` and `src2` for when the mantissa product fit
//! a single limb , i.e., `src1_spec.man_bits + src2_spec.man_bits <= _MAN_LIMIT_BITS`.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT,
    typename QNTZ_FUNC_SIGNATURE>
[[maybe_unused]] static APY_INLINE void _floating_point_mul_short(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz,
    QNTZ_FUNC_SIGNATURE qntz_func,
    const unsigned SUM_MAN_BITS,
    const exp_t SRC1_MAX_EXP,
    const exp_t SRC2_MAX_EXP,
    const exp_t RES_MAX_EXP,
    const man_t TWO,
    const man_t TWO_BEFORE,
    const man_t ONE_BEFORE,
    const man_t TWO_RES,
    const int MAN_DELTA,
    const man_t STICKY
)
{
    const APyFloatData& x = *src1;
    const APyFloatData& y = *src2;
    APyFloatData& z = *dst;

    // Compute result sign
    bool res_sign = x.sign ^ y.sign;
    const bool x_is_subnormal = (x.exp == 0);
    const bool x_is_maxexp = (x.exp == SRC1_MAX_EXP);
    const bool y_is_subnormal = (y.exp == 0);
    const bool y_is_maxexp = (y.exp == SRC2_MAX_EXP);
    if (x_is_maxexp || y_is_maxexp || x_is_subnormal || y_is_subnormal) {
        bool src1_nan = is_nan(x, src1_spec.exp_bits);
        bool src2_nan = is_nan(y, src2_spec.exp_bits);
        bool src1_inf = is_inf(x, src1_spec.exp_bits);
        bool src2_inf = is_inf(y, src2_spec.exp_bits);
        bool src1_zero = is_zero(x);
        bool src2_zero = is_zero(y);
        if (src1_nan || src2_nan || (src1_inf && src2_zero)
            || (src2_inf && src1_zero)) {
            // Set to NaN
            z = { res_sign, RES_MAX_EXP, 1 };
            return; // early exit
        } else if (src1_inf || src2_inf) {
            // Set to inf
            z = { res_sign, RES_MAX_EXP, 0 };
            return; // early exit
        } else if (src1_zero || src2_zero) {
            // Set to zero
            z = { res_sign, 0, 0 };
            return; // early exit
        }
    }

    // Tentative mantissa and exponent
    man_t mx = (man_t(!x_is_subnormal) << src1_spec.man_bits) | x.man;
    man_t my = (man_t(!y_is_subnormal) << src2_spec.man_bits) | y.man;
    man_t new_man = mx * my;
    std::int64_t tmp_exp
        = true_exp(x, src1_spec) + true_exp(y, src2_spec) + dst_spec.bias;

    // Check result from multiplication larger than/equal two
    if (new_man & TWO_BEFORE) {
        tmp_exp++;
        new_man <<= 1;
    } else if (new_man & ONE_BEFORE) {
        // Align with longer result
        new_man <<= 2;
    } else {
        // One or two of the operands were subnormal. If the exponent is
        // positive, the result is normalized by left-shifting until the
        // exponent is zero or the mantissa is 1.xx
        std::int64_t leading_zeros = 1 + SUM_MAN_BITS - bit_width(new_man);
        int shift = std::max(std::min(tmp_exp, leading_zeros), std::int64_t(0));
        tmp_exp -= shift;
        new_man <<= shift + 2; // + 2 to align with longer result
    }

    if (tmp_exp <= 0) {
        if (tmp_exp < -std::int64_t(dst_spec.man_bits)) {
            // Exponent too small after rounding
            z = { res_sign, 0, quantize_close_to_zero(res_sign, qntz) };
            return; // early exit
        }
        new_man = (new_man >> (-tmp_exp + 1))
            | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
        tmp_exp = 0;
    }

    exp_t res_exp = exp_t(tmp_exp);
    new_man &= TWO - 1;

    qntz_func(new_man, res_exp, RES_MAX_EXP, MAN_DELTA, res_sign, TWO_RES, STICKY);
    z = { res_sign, res_exp, new_man };
    return;
}

//! Floating-point multiplication of `src1` and `src2`. This is the most general
//! low-level multiplication function to use only when others won't work.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void _floating_point_mul_general(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz,
    const exp_t SRC1_MAX_EXP,
    const exp_t SRC2_MAX_EXP,
    const exp_t RES_MAX_EXP
)
{
    const APyFloatData& x = *src1;
    const APyFloatData& y = *src2;
    APyFloatData& z = *dst;

    // Compute result sign
    bool res_sign = x.sign ^ y.sign;

    const bool x_is_subnormal = (x.exp == 0);
    const bool x_is_maxexp = (x.exp == SRC1_MAX_EXP);
    const bool y_is_subnormal = (y.exp == 0);
    const bool y_is_maxexp = (y.exp == SRC2_MAX_EXP);
    if (x_is_maxexp || y_is_maxexp || x_is_subnormal || y_is_subnormal) {
        bool src1_nan = is_nan(x, src1_spec.exp_bits);
        bool src2_nan = is_nan(y, src2_spec.exp_bits);
        bool src1_inf = is_inf(x, src1_spec.exp_bits);
        bool src2_inf = is_inf(y, src2_spec.exp_bits);
        bool src1_zero = is_zero(x);
        bool src2_zero = is_zero(y);
        if (src1_nan || src2_nan || (src1_inf && src2_zero)
            || (src2_inf && src1_zero)) {
            // Set to NaN
            z = { res_sign, RES_MAX_EXP, 1 };
            return; // early exit
        } else if (src1_inf || src2_inf) {
            // Set to inf
            z = { res_sign, RES_MAX_EXP, 0 };
            return; // early exit
        } else if (src1_zero || src2_zero) {
            // Set to zero
            z = { res_sign, 0, 0 };
            return; // early exit
        }
    }

    // Normalize both inputs
    auto [norm_x, norm_x_exp_bits, norm_x_bias] = normalize(x, src1_spec);
    auto [norm_y, norm_y_exp_bits, norm_y_bias] = normalize(y, src2_spec);

    // Add leading one's
    const man_t mx = true_man(norm_x, norm_x_exp_bits, src1_spec.man_bits);
    const man_t my = true_man(norm_y, norm_y_exp_bits, src2_spec.man_bits);

    // Tentative exponent
    std::int64_t new_exp = ((std::int64_t)norm_x.exp - (std::int64_t)norm_x_bias)
        + ((std::int64_t)norm_y.exp - (std::int64_t)norm_y_bias) + dst_spec.bias;

    // Two integer bits, sign bit and leading one
    const APyFixed apy_mx(2 + src1_spec.man_bits, 2, { UINT64_TO_LIMB(mx) });
    const APyFixed apy_my(2 + src2_spec.man_bits, 2, { UINT64_TO_LIMB(my) });
    APyFixed apy_res = (apy_mx * apy_my);

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
    quantize_apymantissa(apy_res, res_sign, dst_spec.man_bits, qntz);

    // Carry from quantization
    if (apy_res.positive_greater_than_equal_pow2(1)) {
        new_exp++;
        apy_res >>= 1;
    }

    if (new_exp >= exp_t((1ULL << dst_spec.exp_bits) - 1)) {
        if (do_infinity(qntz, res_sign)) {
            z = { res_sign, RES_MAX_EXP, 0 };
        } else {
            z = { res_sign, RES_MAX_EXP - 1, (1ULL << dst_spec.man_bits) - 1 };
        }
        return;
    }

    if (apy_res.positive_greater_than_equal_pow2(0)) { // Remove leading one
        apy_res = apy_res - APyFixed(2, 2, { 1 });

        // If a leading one is present while the exponent is zero,
        // then it 'acts like a carry' and creates a normal number
        if (new_exp == 0) {
            new_exp = 1;
        }
    }
    apy_res <<= dst_spec.man_bits;
    z = { res_sign, exp_t(new_exp), man_t(apy_res.to_double()) };
    return;
}

//! Iterator-based floating-point quotients
template <
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void floating_point_quotients(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const std::size_t n_elements,
    const QuantizationMode& qntz
)
{
    const exp_t RES_MAX_EXP = ((1ULL << dst_spec.exp_bits) - 1);

    const APyFloatSpec& x_spec = src1_spec;
    const APyFloatSpec& y_spec = src2_spec;
    const APyFloatSpec& z_spec = dst_spec;
    for (std::size_t i = 0; i < n_elements; i++) {
        const APyFloatData& x = src1[SRC1_INC * i];
        const APyFloatData& y = src2[SRC2_INC * i];
        APyFloatData& z = dst[DST_INC * i];
        bool sign = x.sign ^ y.sign;

        // Handle special operands
        if (is_nan(x, x_spec) || is_nan(y, y_spec) || (is_zero(x) && is_zero(y))
            || (is_inf(x, x_spec) && is_inf(y, y_spec))) {
            z = { sign, RES_MAX_EXP, 1 }; // NaN
            continue;
        }
        if (is_zero(x) || is_inf(y, y_spec)) {
            z = { sign, 0, 0 };
            continue;
        }
        if (is_inf(x, x_spec) || is_zero(y)) {
            z = { sign, RES_MAX_EXP, 0 }; // inf
            continue;
        }

        const auto [nx_data, nx_exp_bits, nx_bias] = normalize(x, x_spec);
        const auto [ny_data, ny_exp_bits, ny_bias] = normalize(y, y_spec);

        std::int64_t new_exp = std::int64_t(nx_data.exp) - std::int64_t(nx_bias)
            - std::int64_t(ny_data.exp) + ny_bias + z_spec.bias;

        const man_t mx = true_man(nx_data, { nx_exp_bits, x_spec.man_bits, nx_bias });
        const man_t my = true_man(ny_data, { ny_exp_bits, y_spec.man_bits, ny_bias });
        const APyFixed apy_mx(2 + 64 + x_spec.man_bits, 2, { UINT64_TO_LIMB(mx) });
        const APyFixed apy_my(2 + 64 + y_spec.man_bits, 2, { UINT64_TO_LIMB(my) });
        APyFixed apy_man_res = apy_mx / apy_my;

        // The result from the division will be in (1/2, 2) so normalization may be
        // required.
        if (!apy_man_res.positive_greater_than_equal_pow2(0)) {
            apy_man_res <<= 1;
            new_exp--;
        }

        // Check limits
        if (new_exp >= RES_MAX_EXP) {
            if (do_infinity(qntz, sign)) {
                z = { sign, RES_MAX_EXP, 0 }; // Inf
                continue;
            } else {
                z = { sign, RES_MAX_EXP - 1, (1ULL << z_spec.man_bits) - 1 };
                continue;
            }
        }

        // Handle subnormal case
        if (new_exp <= 0) {
            apy_man_res >>= unsigned(1 - new_exp);
            new_exp = 0;
        }

        // Quantize mantissa. This will never create carry.
        quantize_apymantissa(apy_man_res, sign, z_spec.man_bits, qntz);

        // Remove leading one
        if (apy_man_res.positive_greater_than_equal_pow2(0)) {
            apy_man_res = apy_man_res - APyFixed(2, 2, { 1 });

            // If a leading one is present while the exponent is zero,
            // then it 'acts like a carry' and creates a normal number
            if (new_exp == 0) {
                new_exp = 1;
            }
        }
        apy_man_res <<= z_spec.man_bits;
        z = { sign, exp_t(new_exp), man_t(apy_man_res.to_double()) };
        continue;
    }
}

/* ********************************************************************************** *
 * *              Floating-point iterator-based arithmetic functors                 * *
 * ********************************************************************************** */

//! Short floating-point multiplication functor. Available when:
//! `src1_spec.man_bits + src2_spec.man_bits <=_MAN_LIMIT_BITS`
class _FloatingPointMultiplierShort {
public:
    // Construct an uninitialized functor. Calling `this->operator()` on an
    // uninitialized functor is always undefined behaviour. To initialize the functor,
    // assign a new initialized functor in its place.
    explicit _FloatingPointMultiplierShort() { };

    // Initializing constructor. After calling this constructor, the functor is fully
    // initialized and ready to be used.
    explicit _FloatingPointMultiplierShort(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : src1_spec { src1_spec }
        , src2_spec { src2_spec }
        , dst_spec { dst_spec }
        , qntz { qntz }
    {
        SUM_MAN_BITS = unsigned(src1_spec.man_bits + src2_spec.man_bits);
        SRC1_MAX_EXP = exp_t((1ULL << src1_spec.exp_bits) - 1);
        SRC2_MAX_EXP = exp_t((1ULL << src2_spec.exp_bits) - 1);
        RES_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
        unsigned NEW_MAN_BITS = SUM_MAN_BITS + 2;
        MAN_DELTA = int(NEW_MAN_BITS) - int(dst_spec.man_bits);
        TWO = 1ULL << NEW_MAN_BITS;
        TWO_BEFORE = 1ULL << (NEW_MAN_BITS - 1);
        ONE_BEFORE = 1ULL << (NEW_MAN_BITS - 2);
        TWO_RES = 1ULL << dst_spec.man_bits;
        STICKY = (1ULL << (MAN_DELTA - 1)) - 1;
        qntz_func = get_qntz_func(qntz, MAN_DELTA < 0);
    }

    // Copy assign a new functor in the place of this one
    _FloatingPointMultiplierShort& operator=(const _FloatingPointMultiplierShort&)
        = default;

    void operator()(
        const APyFloatData* src1, const APyFloatData* src2, APyFloatData* dst
    ) const
    {
        _floating_point_mul_short(
            src1,
            src2,
            dst,
            src1_spec,
            src2_spec,
            dst_spec,
            qntz,
            qntz_func,
            SUM_MAN_BITS,
            SRC1_MAX_EXP,
            SRC2_MAX_EXP,
            RES_MAX_EXP,
            TWO,
            TWO_BEFORE,
            ONE_BEFORE,
            TWO_RES,
            MAN_DELTA,
            STICKY
        );
    }

private:
    // Set first during functor initialization
    APyFloatSpec src1_spec, src2_spec, dst_spec;
    QuantizationMode qntz;

    // The used quantization function
    decltype(get_qntz_func(qntz)) qntz_func;

    // Reusable constants used in the multiplication
    unsigned SUM_MAN_BITS;
    int MAN_DELTA;
    exp_t SRC1_MAX_EXP;
    exp_t SRC2_MAX_EXP;
    exp_t RES_MAX_EXP;
    man_t TWO;
    man_t TWO_BEFORE;
    man_t ONE_BEFORE;
    man_t TWO_RES;
    man_t STICKY;
};

//! General floating-point multiplication, always available but slower compared to
//! `_FloatingPointMultiplierShort`.
class _FloatingPointMultiplierGeneral {
public:
    // Construct an uninitialized functor. Calling `this->operator()` on an
    // uninitialized functor is always undefined behaviour. To initialize the functor,
    // assign a new initialized functor in its place.
    explicit _FloatingPointMultiplierGeneral() { };

    // Initializing constructor. After calling this constructor, the functor is fully
    // initialized and ready to be used.
    explicit _FloatingPointMultiplierGeneral(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : src1_spec { src1_spec }
        , src2_spec { src2_spec }
        , dst_spec { dst_spec }
        , qntz { qntz }
    {
        SRC1_MAX_EXP = exp_t((1ULL << src1_spec.exp_bits) - 1);
        SRC2_MAX_EXP = exp_t((1ULL << src2_spec.exp_bits) - 1);
        RES_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
        qntz_func = get_qntz_func(qntz, false);
    }

    void operator()(
        const APyFloatData* src1, const APyFloatData* src2, APyFloatData* dst
    ) const
    {
        _floating_point_mul_general(
            src1,
            src2,
            dst,
            src1_spec,
            src2_spec,
            dst_spec,
            qntz,
            SRC1_MAX_EXP,
            SRC2_MAX_EXP,
            RES_MAX_EXP
        );
    }

private:
    // Set first during functor initialization
    APyFloatSpec src1_spec, src2_spec, dst_spec;
    QuantizationMode qntz;

    // The used quantization function
    decltype(get_qntz_func(qntz)) qntz_func;

    // Reusable constants used in the multiplication
    exp_t SRC1_MAX_EXP;
    exp_t SRC2_MAX_EXP;
    exp_t RES_MAX_EXP;
};

//! Floating-point adder/subtractor that works when:
//! (1) `qntz != QuantizationMode::STOCK_WEIGHTED`,
//! (2) `dst_spec.man_bits + 5 <= _MAN_T_SIZE_BITS`,
//! (3) `src1_spec == src2_spec == dst_spec`.
template <bool IS_SUBTRACT> class _FloatingPointAddSubSameWl {
public:
    // Construct an uninitialized functor. Calling `this->operator()` on an
    // uninitialized functor is always undefined behaviour. To initialize the functor,
    // assign a new initialized functor in its place.
    explicit _FloatingPointAddSubSameWl() { };

    // Initializing constructor. After calling this constructor, the functor is fully
    // initialized and ready to be used.
    explicit _FloatingPointAddSubSameWl(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : dst_spec { dst_spec }
        , qntz { qntz }
    {
        (void)src1_spec;
        (void)src2_spec;

        RES_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
        FINAL_RES_LO = 1ULL << dst_spec.man_bits;
        RES_LO = FINAL_RES_LO << 3;
        CARRY_RES_LO = RES_LO << 1;
        MAN_MASK = CARRY_RES_LO - 1;
        NORM_CONST = unsigned(_MAN_T_SIZE_BITS - dst_spec.man_bits - 4);
        qntz_func = get_qntz_func(qntz);
    }

    void operator()(
        const APyFloatData* src1, const APyFloatData* src2, APyFloatData* dst
    ) const
    {
        _floating_point_add_same_wl<IS_SUBTRACT>(
            src1,
            src2,
            dst,
            dst_spec,
            qntz,
            qntz_func,
            RES_MAX_EXP,
            FINAL_RES_LO,
            RES_LO,
            CARRY_RES_LO,
            MAN_MASK,
            NORM_CONST
        );
    }

private:
    // Set during functor initialization
    APyFloatSpec dst_spec;
    QuantizationMode qntz;

    // Quantization function in use
    decltype(get_qntz_func(qntz)) qntz_func;

    // Reusable constants used in the addition
    exp_t RES_MAX_EXP;
    man_t FINAL_RES_LO;
    man_t RES_LO;
    man_t CARRY_RES_LO;
    man_t MAN_MASK;
    unsigned NORM_CONST;
};

//! Floating-point adder/subtractor that works when:
//! (1) `qntz != QuantizationMode::STOCK_WEIGHTED`,
//! (2) `dst_spec.man_bits + 5 <= _MAN_T_SIZE_BITS`.
template <bool IS_SUBTRACT> class _FloatingPointAddSubDiffWl {
public:
    // Construct an uninitialized functor. Calling `this->operator()` on an
    // uninitialized functor is always undefined behaviour. To initialize the functor,
    // assign a new initialized functor in its place.
    explicit _FloatingPointAddSubDiffWl() { };

    // Initializing constructor. After calling this constructor, the functor is fully
    // initialized and ready to be used.
    explicit _FloatingPointAddSubDiffWl(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : src1_spec { src1_spec }
        , src2_spec { src2_spec }
        , dst_spec { dst_spec }
        , qntz { qntz }
    {
        RES_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
        FINAL_RES_LO = 1ULL << dst_spec.man_bits;
        RES_LO = FINAL_RES_LO << 3;
        CARRY_RES_LO = RES_LO << 1;
        MAN_MASK = CARRY_RES_LO - 1;
        NORM_CONST = unsigned(_MAN_T_SIZE_BITS - dst_spec.man_bits - 4);
        qntz_func = get_qntz_func(qntz);
    }

    void operator()(
        const APyFloatData* src1, const APyFloatData* src2, APyFloatData* dst
    ) const
    {
        _floating_point_add_diff_wl<IS_SUBTRACT>(
            src1,
            src2,
            dst,
            src1_spec,
            src2_spec,
            dst_spec,
            qntz,
            qntz_func,
            RES_MAX_EXP,
            FINAL_RES_LO,
            RES_LO,
            CARRY_RES_LO,
            MAN_MASK,
            NORM_CONST
        );
    }

private:
    // Set during functor initialization
    APyFloatSpec src1_spec, src2_spec, dst_spec;
    QuantizationMode qntz;

    // Quantization function in use
    decltype(get_qntz_func(qntz)) qntz_func;

    // Reusable constants used in the addition
    exp_t RES_MAX_EXP;
    man_t FINAL_RES_LO;
    man_t RES_LO;
    man_t CARRY_RES_LO;
    man_t MAN_MASK;
    unsigned NORM_CONST;
};

//! Floating-point adder/subtractor that always works. It is slower compared to
//! `_FloatingPointAddSubSameWl` and `_FloatingPointAddSubDiffWl`.
template <bool IS_SUBTRACT> class _FloatingPointAddSubGeneral {
public:
    // Construct an uninitialized functor. Calling `this->operator()` on an
    // uninitialized functor is always undefined behaviour. To initialize the functor,
    // assign a new initialized functor in its place.
    explicit _FloatingPointAddSubGeneral() { };

    // Initializing constructor. After calling this constructor, the functor is fully
    // initialized and ready to be used.
    explicit _FloatingPointAddSubGeneral(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : src1_spec { src1_spec }
        , src2_spec { src2_spec }
        , dst_spec { dst_spec }
        , qntz { qntz }
    {
        RES_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
    }

    void operator()(
        const APyFloatData* src1, const APyFloatData* src2, APyFloatData* dst
    ) const
    {
        _floating_point_add_general<IS_SUBTRACT>(
            src1, src2, dst, src1_spec, src2_spec, dst_spec, qntz, RES_MAX_EXP
        );
    }

private:
    // Set during functor initialization
    APyFloatSpec src1_spec, src2_spec, dst_spec;
    QuantizationMode qntz;

    // Reusable constants used in the addition
    exp_t RES_MAX_EXP;
};

template <
    bool IS_SUBTRACT,
    std::size_t SRC1_INC,
    std::size_t SRC2_INC,
    std::size_t DST_INC>
class _FloatingPointAddSub {

public:
    explicit _FloatingPointAddSub(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
    {
        using F = _FloatingPointAddSub<IS_SUBTRACT, SRC1_INC, SRC2_INC, DST_INC>;
        if (qntz == QuantizationMode::STOCH_WEIGHTED) {
            _add_general = _FloatingPointAddSubGeneral<IS_SUBTRACT>(
                src1_spec, src2_spec, dst_spec, qntz
            );
            f = &F::add_general;
        } else { /* qntz != QuantizationMode::STOCK_WEIGHTED */
            const unsigned MAX_MAN_BITS = dst_spec.man_bits + 5;
            if (MAX_MAN_BITS <= _MAN_T_SIZE_BITS) {
                if (src1_spec == src2_spec) {
                    _add_same_wl = _FloatingPointAddSubSameWl<IS_SUBTRACT>(
                        src1_spec, src2_spec, dst_spec, qntz
                    );
                    f = &F::add_same_wl;
                } else {
                    _add_diff_wl = _FloatingPointAddSubDiffWl<IS_SUBTRACT>(
                        src1_spec, src2_spec, dst_spec, qntz
                    );
                    f = &F::add_diff_wl;
                }
            } else {
                _add_general = _FloatingPointAddSubGeneral<IS_SUBTRACT>(
                    src1_spec, src2_spec, dst_spec, qntz
                );
                f = &F::add_general;
            }
        }
    }

    // Perform a single floating-point addition/subtractions
    void operator()(
        const APyFloatData& src1, const APyFloatData& src2, APyFloatData& dst
    ) const
    {
        std::invoke(f, this, &src1, &src2, &dst, 1);
    }

    // Perform `nitems` floating-point additions/subtractions
    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems = 1
    ) const
    {
        std::invoke(f, this, src1, src2, dst, nitems);
    }

private:
    // Set during functor initialization
    _FloatingPointAddSubSameWl<IS_SUBTRACT> _add_same_wl;
    _FloatingPointAddSubDiffWl<IS_SUBTRACT> _add_diff_wl;
    _FloatingPointAddSubGeneral<IS_SUBTRACT> _add_general;

    // Pointer to the correct adder function based on the floating-point specs
    void (_FloatingPointAddSub::*f)(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const;

    void add_general(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {
        for (std::size_t i = 0; i < nitems; i++) {
            _add_general(src1 + SRC1_INC * i, src2 + SRC2_INC * i, dst + DST_INC * i);
        }
    }

    void add_same_wl(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {
        for (std::size_t i = 0; i < nitems; i++) {
            _add_same_wl(src1 + SRC1_INC * i, src2 + SRC2_INC * i, dst + DST_INC * i);
        }
    }

    void add_diff_wl(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {
        for (std::size_t i = 0; i < nitems; i++) {
            _add_diff_wl(src1 + SRC1_INC * i, src2 + SRC2_INC * i, dst + DST_INC * i);
        }
    }
};

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
class FloatingPointMultiplier {
public:
    explicit FloatingPointMultiplier(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
    {
        using F = FloatingPointMultiplier<SRC1_INC, SRC2_INC, DST_INC>;
        unsigned SUM_MAN_BITS = unsigned(src1_spec.man_bits + src2_spec.man_bits);
        if (SUM_MAN_BITS <= _MAN_LIMIT_BITS) {
            _mul_short
                = _FloatingPointMultiplierShort(src1_spec, src2_spec, dst_spec, qntz);
            f = &F::mul_short;
        } else {
            _mul_general
                = _FloatingPointMultiplierGeneral(src1_spec, src2_spec, dst_spec, qntz);
            f = &F::mul_general;
        }
    }

    // Perform a single floating-point multiplication
    void operator()(
        const APyFloatData& src1, const APyFloatData& src2, APyFloatData& dst
    ) const
    {
        std::invoke(f, this, &src1, &src2, &dst, 1);
    }

    // Perform `nitems` floating-point multiplications
    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems = 1
    ) const
    {
        std::invoke(f, this, src1, src2, dst, nitems);
    }

private:
    // Set first during functor initialization
    _FloatingPointMultiplierShort _mul_short;
    _FloatingPointMultiplierGeneral _mul_general;

    // Pointer `f` to the correct function based on the floating-point specs
    void (FloatingPointMultiplier::*f)(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const;

    void mul_short(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {
        for (std::size_t i = 0; i < nitems; i++) {
            _mul_short(src1 + SRC1_INC * i, src2 + SRC2_INC * i, dst + DST_INC * i);
        }
    }

    void mul_general(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {
        for (std::size_t i = 0; i < nitems; i++) {
            _mul_general(src1 + SRC1_INC * i, src2 + SRC2_INC * i, dst + DST_INC * i);
        }
    }
};

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
class FloatingPointDivider {

public:
    explicit FloatingPointDivider(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : src1_spec { src1_spec }
        , src2_spec { src2_spec }
        , dst_spec { dst_spec }
        , qntz { qntz }
    {
    }

    // Perform a single floating-point division
    void operator()(
        const APyFloatData& src1, const APyFloatData& src2, APyFloatData& dst
    ) const
    {
        this->operator()(&src1, &src2, &dst, /* nitems = */ 1);
    }

    // Perform `nitems` floating-point divisions
    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems = 1
    ) const
    {
        floating_point_quotients<SRC1_INC, SRC2_INC, DST_INC>(
            src1, src2, dst, src1_spec, src2_spec, dst_spec, nitems, qntz
        );
    }

private:
    // Set first during functor initialization
    const APyFloatSpec src1_spec, src2_spec, dst_spec;
    const QuantizationMode qntz;
};

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
using FloatingPointAdder = _FloatingPointAddSub<false, SRC1_INC, SRC2_INC, DST_INC>;

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
using FloatingPointSubtractor = _FloatingPointAddSub<true, SRC1_INC, SRC2_INC, DST_INC>;

class FloatingPointInnerProduct {
public:
    explicit FloatingPointInnerProduct(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
    {
        using F = FloatingPointInnerProduct;
        using MUL_F_SHORT = _FloatingPointMultiplierShort;
        using MUL_F_GENERAL = _FloatingPointMultiplierGeneral;
        using ADD_F_GENERAL = _FloatingPointAddSubGeneral</*IS_SUBTRACT=*/false>;
        using ADD_F_SHORT = _FloatingPointAddSubSameWl</*IS_SUBTRACT=*/false>;

        bool is_short_mul = src1_spec.man_bits + src2_spec.man_bits <= _MAN_LIMIT_BITS;
        bool is_short_add = qntz != QuantizationMode::STOCH_WEIGHTED
            && std::size_t(dst_spec.man_bits + 5) <= _MAN_T_SIZE_BITS;

        if (is_short_mul) {
            mul_short = MUL_F_SHORT(src1_spec, src2_spec, dst_spec, qntz);
            if (is_short_add) {
                add_same_wl = ADD_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/true, /*SHORT_ADD=*/true>;
            } else { /* !is_short_add */
                add_general = ADD_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/true, /*SHORT_ADD=*/false>;
            }
        } else { /* !is_short_mul */
            mul_general = MUL_F_GENERAL(src1_spec, src2_spec, dst_spec, qntz);
            if (is_short_add) {
                add_same_wl = ADD_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/false, /*SHORT_ADD=*/true>;
            } else { /* !is_short_add */
                add_general = ADD_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/false, /*SHORT_ADD=*/false>;
            }
        }
    }

    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t N,
        std::size_t M = 1,
        std::size_t DST_STEP = 1
    ) const
    {
        std::invoke(f, this, src1, src2, dst, N, M, DST_STEP);
    }

    template <bool SHORT_MUL, auto SHORT_ADD>
    void inner_product(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t N,
        std::size_t M,
        std::size_t DST_STEP
    ) const
    {
        // Matrix-vector multiplication $`A \times b`$, where
        // * A: [ `M` x `N` ]
        // * b: [ `N` x `1` ]
        APyFloatData product { 0, 0, 0 };
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + N * m;
            APyFloatData sum { 0, 0, 0 };
            for (std::size_t n = 0; n < N; n++) {
                if constexpr (SHORT_MUL) {
                    mul_short(A_it + n, src2 + n, &product);
                } else {
                    mul_general(A_it + n, src2 + n, &product);
                }
                if constexpr (SHORT_ADD) {
                    add_same_wl(&sum, &product, &sum);
                } else {
                    add_general(&sum, &product, &sum);
                }
            }
            *(dst + DST_STEP * m) = sum;
        }
    }

private:
    // Pointer `f` to the correct function based on the floating-point specs
    void (FloatingPointInnerProduct::*f)(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t N,
        std::size_t M,
        std::size_t DST_STEP
    ) const;

    _FloatingPointAddSubSameWl<false> add_same_wl;
    _FloatingPointAddSubGeneral<false> add_general;
    _FloatingPointMultiplierShort mul_short;
    _FloatingPointMultiplierGeneral mul_general;
};

#endif // _APYFLOAT_UTIL_H
