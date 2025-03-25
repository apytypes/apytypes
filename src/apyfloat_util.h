#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include "apyfixed.h"
#include "apytypes_common.h"
#include "apytypes_fwd.h"
#include "apytypes_util.h"
#include "python_util.h"

// Standard header includes
#include <functional> // std::invoke
#include <optional>

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

//! Calculate new bias. Assumes new_exp_bits is larger than exp_bits1 and exp_bits2.
[[maybe_unused]] static APY_INLINE exp_t
calc_bias(int new_exp_bits, int exp_bits1, exp_t bias1, int exp_bits2, exp_t bias2)
{
    const auto s1 = (bias1 + 1) << (new_exp_bits - exp_bits1);
    const auto s2 = (bias2 + 1) << (new_exp_bits - exp_bits2);
    return ((s1 + s2) >> 1) - 1;
}

[[maybe_unused]] static APY_INLINE exp_t
calc_bias(int new_exp_bits, const APyFloatSpec& spec1, const APyFloatSpec& spec2)
{
    return calc_bias(
        new_exp_bits, spec1.exp_bits, spec1.bias, spec2.exp_bits, spec2.bias
    );
}

//! General calculation of new bias. Should only be used if new_exp_bits can be strictly
//! less than exp_bits1 or exp_bits2.
[[maybe_unused]] static APY_INLINE exp_t calc_bias_general(
    int new_exp_bits, int exp_bits1, exp_t bias1, int exp_bits2, exp_t bias2
)
{
    double s1 = double(bias1 + 1) / double(1 << exp_bits1);
    double s2 = double(bias2 + 1) / double(1 << exp_bits2);
    double p = (1 << (new_exp_bits - 1));
    return (s1 + s2) * p - 1;
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
            return;
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

// Retrieve a specialized quantization function
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
        "get_qntz_func(): unknown quantization (did you pass `int` as "
        "`QuantizationMode`?)"
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
[[maybe_unused]] static APY_INLINE void check_exponent_format(int exp_bits)
{
    if ((unsigned(exp_bits) > _EXP_LIMIT_BITS) || (exp_bits < 0)) {
        std::string msg = fmt::format(
            "Exponent bits must be a non-negative integer less or equal to {} but {} "
            "was given",
            _EXP_LIMIT_BITS,
            exp_bits
        );
        throw nb::value_error(msg.c_str());
    }
}

//! Check that the number of mantissa bits is allowed, throw otherwise
[[maybe_unused]] static APY_INLINE void check_mantissa_format(int man_bits)
{
    if ((unsigned(man_bits) > _MAN_LIMIT_BITS) || (man_bits < 0)) {
        std::string msg = fmt::format(
            "Mantissa bits must be a non-negative integer less or equal to {} but {} "
            "was given",
            _MAN_LIMIT_BITS,
            man_bits

        );
        throw nb::value_error(msg.c_str());
    }
}

/* ********************************************************************************** *
 * *                        Floating-point helper utilities                         * *
 * ********************************************************************************** */

//! Retrieve a generalized IEEE-754 bias for `exp_bits`
[[maybe_unused]] static APY_INLINE exp_t ieee_bias(std::uint8_t exp_bits)
{
    return (1ULL << (exp_bits - 1)) - 1;
}

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

[[maybe_unused]] static APY_INLINE int64_t true_exp(const APyFloatData& src, exp_t bias)
{
    return std::int64_t(src.exp) - std::int64_t(bias) + (src.exp == 0);
}

[[maybe_unused]] static APY_INLINE int64_t
true_exp(const APyFloatData& src, const APyFloatSpec& spec)
{
    return true_exp(src, spec.bias);
}

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

//! Return a normalized (non subnormal) floating-point copy of `src`
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

[[maybe_unused]] static APY_INLINE APyFloatData cast_no_quant(
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

/* ********************************************************************************** *
 * *              Floating-point iterator-based arithmetic functions                * *
 * ********************************************************************************** */

//! Iterator-based less-than function, comparing `src1 < src2`
[[maybe_unused]] static APY_INLINE bool floating_point_less_than(
    const APyFloatData& src1,
    const APyFloatSpec& src1_spec,
    const APyFloatData& src2,
    const APyFloatSpec& src2_spec
)
{
    if (is_nan(src1, src1_spec.exp_bits) || is_nan(src2, src2_spec.exp_bits)) {
        return false;
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
        std::uint64_t n1_man = src1.man << (_MAN_T_SIZE_BITS - src1_bw);
        std::uint64_t n2_man = src2.man << (_MAN_T_SIZE_BITS - src2_bw);
        return src1.sign ^ (n1_man < n2_man);
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
            // Set to inf
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
            z = { qntz == QuantizationMode::TRN, exp_t(0), man_t(0) };
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
    bool swap = floating_point_less_than_abs(*src1, src1_spec, *src2, src2_spec);
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
    }

    APyFloatData x_wide = x_spec == dst_spec
        ? x
        : cast_no_quant(
              x, x_spec, { dst_spec.exp_bits, dst_spec.man_bits, x_spec.bias }
          );
    APyFloatData y_wide = y_spec == dst_spec
        ? y
        : cast_no_quant(
              y, y_spec, { dst_spec.exp_bits, dst_spec.man_bits, y_spec.bias }
          );

    // Tentative exponent
    exp_t new_exp = true_exp(x_wide, x_spec.bias) + dst_spec.bias;
    if (new_exp > RES_MAX_EXP) {
        exp_t exp = int(new_exp) < 0 ? 0 : RES_MAX_EXP;
        z = { x_sign, exp, 0 };
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
                z = { qntz == QuantizationMode::TRN, 0, 0 };
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

//! Floating-point addition of `src1` and `src2` for when`_floating_point_sum_same_wl`
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
    bool swap = floating_point_less_than_abs(*src1, src1_spec, *src2, src2_spec);
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
    }

    APyFloatData x_wide = x_spec == dst_spec
        ? x
        : cast_no_quant(
              x, x_spec, { dst_spec.exp_bits, dst_spec.man_bits, x_spec.bias }
          );
    APyFloatData y_wide = y_spec == dst_spec
        ? y
        : cast_no_quant(
              y, y_spec, { dst_spec.exp_bits, dst_spec.man_bits, y_spec.bias }
          );

    if (x_sign != y_sign) {
        if (x_wide.man == y_wide.man) {
            if (true_exp(x_wide, x_spec.bias) == true_exp(y_wide, y_spec.bias)) {
                bool sign = qntz == QuantizationMode::TRN;
                z = { sign, 0, 0 };
                return;
            }
        }
    }

    const std::int64_t x_true_exp = true_exp(x_wide, x_spec.bias);
    const unsigned exp_delta = x_true_exp - true_exp(y_wide, y_spec.bias);
    exp_t new_exp = x_true_exp + dst_spec.bias;
    if (new_exp > RES_MAX_EXP) {
        exp_t exp = int(new_exp) < 0 ? 0 : RES_MAX_EXP;
        z = { x_sign, exp, 0 };
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
//! into a single `std::uint64_t`.
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

//! Iterator-based floating-point addition
template <
    const bool SUB = false,
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void floating_point_sums(
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
    const unsigned MAX_MAN_BITS = dst_spec.man_bits + 5;

    if (MAX_MAN_BITS <= _MAN_T_SIZE_BITS && qntz != QuantizationMode::STOCH_WEIGHTED) {
        // Retrieve the specialized quantization method to use
        auto qntz_func = get_qntz_func(qntz);

        // Reusable constants
        const man_t FINAL_RES_LO = (1ULL << dst_spec.man_bits);
        const man_t RES_LO = FINAL_RES_LO << 3;
        const man_t CARRY_RES_LO = RES_LO << 1;
        const man_t MAN_MASK = CARRY_RES_LO - 1;
        const unsigned NORMALIZATION_CONST = _MAN_T_SIZE_BITS - dst_spec.man_bits - 4;

        if (src1_spec == src2_spec) {
            /*
             * Specialization #1: LHS and RHS have same format, the mantissa sum can fit
             * in a single `std::uint64_t`, and quantization mode is not
             * `STOCK_WEIGHTED`.
             */
            for (std::size_t i = 0; i < n_elements; i++) {
                _floating_point_add_same_wl<SUB>(
                    src1 + i * SRC1_INC,
                    src2 + i * SRC2_INC,
                    dst + i * DST_INC,
                    dst_spec,
                    qntz,
                    qntz_func,
                    RES_MAX_EXP,
                    FINAL_RES_LO,
                    RES_LO,
                    CARRY_RES_LO,
                    MAN_MASK,
                    NORMALIZATION_CONST
                );
            }
            return; /* Return from specialization #1 */
        } else {
            /*
             * Specialization #2: Not the same format, but the mantissa sum still fits
             * into a single `std::uint64_t`, and the quantization mode is still not
             * `STOCH_WEIGHTED`.
             */
            for (std::size_t i = 0; i < n_elements; i++) {
                _floating_point_add_diff_wl<SUB>(
                    src1 + i * SRC1_INC,
                    src2 + i * SRC2_INC,
                    dst + i * DST_INC,
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
                    NORMALIZATION_CONST
                );
            }
            return; /* Return from specialization #2 */
        }
    }

    /*
     * General path. Always works, but is the slowest.
     */
    for (std::size_t i = 0; i < n_elements; i++) {
        _floating_point_add_general<SUB>(
            src1 + i * SRC1_INC,
            src2 + i * SRC2_INC,
            dst + i * DST_INC,
            src1_spec,
            src2_spec,
            dst_spec,
            qntz,
            RES_MAX_EXP
        );
    }
    return; // Return from the most general path
}

//! Iterator-based floating-point products
template <
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void floating_point_products(
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
    // Retrieve the specialized quantization method to use
    auto qntz_func = get_qntz_func(qntz);

    // Pre-compute some useful values
    const unsigned SUM_MAN_BITS = src1_spec.man_bits + src2_spec.man_bits;
    const exp_t SRC1_MAX_EXP = ((1ULL << src1_spec.exp_bits) - 1);
    const exp_t SRC2_MAX_EXP = ((1ULL << src2_spec.exp_bits) - 1);
    const exp_t RES_MAX_EXP = ((1ULL << dst_spec.exp_bits) - 1);

    if (SUM_MAN_BITS <= _MAN_LIMIT_BITS) {
        /*
         * Mantissa product fits in a single `std::uint64_t`
         */
        const unsigned NEW_MAN_BITS = SUM_MAN_BITS + 2;
        const man_t TWO = 1ULL << (NEW_MAN_BITS);
        const man_t TWO_BEFORE = 1ULL << (NEW_MAN_BITS - 1);
        const man_t ONE_BEFORE = 1ULL << (NEW_MAN_BITS - 2);
        const man_t TWO_RES = (1ULL << dst_spec.man_bits);
        const int MAN_DELTA = NEW_MAN_BITS - dst_spec.man_bits;
        const man_t STICKY = (1ULL << (MAN_DELTA - 1)) - 1;

        for (std::size_t i = 0; i < n_elements; i++) {
            _floating_point_mul_short(
                src1 + i * SRC1_INC,
                src2 + i * SRC2_INC,
                dst + i * DST_INC,
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
    } else { /* sum_man_bits > _MAN_LIMIT_BITS */
        /*
         * Mantissa product does not fit in a single `std::uint64_t`
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            _floating_point_mul_general(
                src1 + i * SRC1_INC,
                src2 + i * SRC2_INC,
                dst + i * DST_INC,
                src1_spec,
                src2_spec,
                dst_spec,
                qntz,
                SRC1_MAX_EXP,
                SRC2_MAX_EXP,
                RES_MAX_EXP
            );
        }
    }
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

//! Perform a single float product
[[maybe_unused]] static APY_INLINE void floating_point_product(
    const APyFloatData& src1,
    const APyFloatData& src2,
    APyFloatData& dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz
)
{
    floating_point_products(
        &src1, &src2, &dst, src1_spec, src2_spec, dst_spec, 1 /* n */, qntz
    );
}

//! Perform a single float product
template <const bool SUB = false>
[[maybe_unused]] static APY_INLINE void floating_point_sum(
    const APyFloatData& src1,
    const APyFloatData& src2,
    APyFloatData& dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz
)
{
    floating_point_sums<SUB>(
        &src1, &src2, &dst, src1_spec, src2_spec, dst_spec, 1 /* n */, qntz
    );
}

//! Perform a single float quotient
template <const bool SUB = false>
[[maybe_unused]] static APY_INLINE void floating_point_quotient(
    const APyFloatData& src1,
    const APyFloatData& src2,
    APyFloatData& dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz
)
{
    floating_point_quotients(
        &src1, &src2, &dst, src1_spec, src2_spec, dst_spec, 1 /* n */, qntz
    );
}

template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
struct FloatInnerProdFunctor {
    FloatInnerProdFunctor(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : _src1_spec(src1_spec)
        , _src2_spec(src2_spec)
        , _dst_spec(dst_spec)
        , _qntz(qntz)
        , inner_f_ptr(get_inner_ptr(_src1_spec, _src2_spec, _dst_spec, _qntz))
    {
    }

    auto get_inner_ptr(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    ) const
    {
        using F = FloatInnerProdFunctor;
        const unsigned SUM_MAN_BITS = src1_spec.man_bits + src2_spec.man_bits;
        const unsigned MAX_MAN_BITS = dst_spec.man_bits + 5;

        if (qntz == QuantizationMode::STOCH_WEIGHTED) {
            if (SUM_MAN_BITS <= _MAN_LIMIT_BITS) {
                return &F::inner_product<&F::MUL_SHORT, &F::ADD_GENERAL>;
            } else {
                return &F::inner_product<&F::MUL_GENERAL, &F::ADD_GENERAL>;
            }
        } else { /* qntz != QuantizationMode::STOCH_WEIGHTED */
            if (MAX_MAN_BITS <= _MAN_T_SIZE_BITS) {
                if (SUM_MAN_BITS <= _MAN_T_SIZE_BITS) {
                    return &F::inner_product<&F::MUL_SHORT, &F::ADD_SAME_WL>;
                } else {
                    return &F::inner_product<&F::MUL_GENERAL, &F::ADD_SAME_WL>;
                }
            } else { /* MAX_MAN_BITS > _MAN_T_SIZE_BITS */
                if (SUM_MAN_BITS <= _MAN_LIMIT_BITS) {
                    return &F::inner_product<&F::MUL_SHORT, &F::ADD_GENERAL>;
                } else {
                    return &F::inner_product<&F::MUL_GENERAL, &F::ADD_GENERAL>;
                }
            }
        }
    }

    // Settable members
    const APyFloatSpec _src1_spec;
    const APyFloatSpec _src2_spec;
    const APyFloatSpec _dst_spec;
    const QuantizationMode _qntz;
    const std::optional<APyFloatAccumulatorOption> _accumulator_mode = std::nullopt;
    void (FloatInnerProdFunctor::* const inner_f_ptr)(
        RANDOM_ACCESS_ITERATOR_IN1 src1,
        RANDOM_ACCESS_ITERATOR_IN2 src2,
        RANDOM_ACCESS_ITERATOR_INOUT dst,
        std::size_t N,
        std::size_t M,
        std::size_t RES_STEP
    ) const;

    // Auto deduce constants for floating-point multiplication
    const unsigned SUM_MAN_BITS = _src1_spec.man_bits + _src2_spec.man_bits;
    const exp_t SRC1_MAX_EXP = ((1ULL << _src1_spec.exp_bits) - 1);
    const exp_t SRC2_MAX_EXP = ((1ULL << _src2_spec.exp_bits) - 1);
    const exp_t RES_MAX_EXP = ((1ULL << _dst_spec.exp_bits) - 1);
    const man_t TWO = 1ULL << (SUM_MAN_BITS + 2);
    const man_t TWO_BEFORE = 1ULL << (SUM_MAN_BITS + 1);
    const man_t ONE_BEFORE = 1ULL << (SUM_MAN_BITS + 0);
    const man_t TWO_RES = (1ULL << _dst_spec.man_bits);
    const int MAN_DELTA = int(SUM_MAN_BITS) + 2 - _dst_spec.man_bits;
    const man_t STICKY = (1ULL << (MAN_DELTA - 1)) - 1;

    // Auto deduce constants for floating-point addition
    const unsigned MAX_MAN_BITS = _dst_spec.man_bits + 5;
    const man_t FINAL_RES_LO = (1ULL << _dst_spec.man_bits);
    const man_t RES_LO = FINAL_RES_LO << 3;
    const man_t CARRY_RES_LO = RES_LO << 1;
    const man_t MAN_MASK = CARRY_RES_LO - 1;
    const unsigned NORM_CONST = _MAN_T_SIZE_BITS - _dst_spec.man_bits - 4;

    // The used quantization function. If `MAN_DELTA` is smaller than zero, we
    const decltype(get_qntz_func(_qntz, MAN_DELTA < 0)) _qntz_func
        = get_qntz_func(_qntz, MAN_DELTA < 0);

    // Macros for passing all the arguments...
#define _ARGS_SD _src1_spec, _src2_spec, _dst_spec
#define _ARGS_QZ _qntz, _qntz_func
#define _ARGS_ME SRC1_MAX_EXP, SRC2_MAX_EXP, RES_MAX_EXP
#define _ARGS_MS TWO, TWO_BEFORE, ONE_BEFORE, TWO_RES, MAN_DELTA, STICKY
#define _ARGS_AS RES_MAX_EXP, FINAL_RES_LO, RES_LO, CARRY_RES_LO, MAN_MASK, NORM_CONST

#define _ARGS_MUL_SHORT _ARGS_SD, _ARGS_QZ, SUM_MAN_BITS, _ARGS_ME, _ARGS_MS
#define _ARGS_MUL_GENERAL _ARGS_SD, _qntz, _ARGS_ME
#define _ARGS_ADD_SAME_WL _dst_spec, _ARGS_QZ, _ARGS_AS
#define _ARGS_ADD_GENERAL _dst_spec, _dst_spec, _dst_spec, _qntz, RES_MAX_EXP

#define DEFINE_NEW_MUL(_NAME_, _CALLBACK_)                                             \
    void _NAME_(                                                                       \
        RANDOM_ACCESS_ITERATOR_IN1 src1,                                               \
        RANDOM_ACCESS_ITERATOR_IN2 src2,                                               \
        APyFloatData* const dst                                                        \
    ) const                                                                            \
    {                                                                                  \
        _CALLBACK_(src1, src2, dst, _ARGS_##_NAME_);                                   \
    }

#define DEFINE_NEW_ADD(_NAME_, _CALLBACK_)                                             \
    void _NAME_(                                                                       \
        const APyFloatData* const src1,                                                \
        const APyFloatData* const src2,                                                \
        APyFloatData* const dst                                                        \
    ) const                                                                            \
    {                                                                                  \
        _CALLBACK_(src1, src2, dst, _ARGS_##_NAME_);                                   \
    }

    DEFINE_NEW_MUL(MUL_SHORT, _floating_point_mul_short);
    DEFINE_NEW_MUL(MUL_GENERAL, _floating_point_mul_general);
    DEFINE_NEW_ADD(ADD_SAME_WL, _floating_point_add_same_wl);
    DEFINE_NEW_ADD(ADD_GENERAL, _floating_point_add_general);

#undef DEFINE_NEW_ADD
#undef DEFINE_NEW_MUL

    template <auto MUL_FUNC, auto ADD_FUNC>
    void inner_product(
        RANDOM_ACCESS_ITERATOR_IN1 src1,
        RANDOM_ACCESS_ITERATOR_IN2 src2,
        RANDOM_ACCESS_ITERATOR_INOUT dst,
        std::size_t N,
        std::size_t M = 1,
        std::size_t DST_STEP = 1
    ) const
    {
        APyFloatData product { 0, 0, 0 };
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + N * m;
            APyFloatData sum { 0, 0, 0 };
            for (std::size_t n = 0; n < N; n++) {
                std::invoke(MUL_FUNC, this, A_it + n, src2 + n, &product);
                std::invoke(ADD_FUNC, this, &sum, &product, &sum);
            }
            *(dst + DST_STEP * m) = sum;
        }
    }

    // Perform the inner product
    void operator()(
        RANDOM_ACCESS_ITERATOR_IN1 src1,
        RANDOM_ACCESS_ITERATOR_IN2 src2,
        RANDOM_ACCESS_ITERATOR_INOUT dst,
        std::size_t N,
        std::size_t M = 1,
        std::size_t DST_STEP = 1
    ) const
    {
        std::invoke(inner_f_ptr, this, src1, src2, dst, N, M, DST_STEP);
    }
};

#endif // _APYFLOAT_UTIL_H
