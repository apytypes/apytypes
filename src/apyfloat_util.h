#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apytypes_common.h"
#include "apytypes_fwd.h"
#include "python_util.h"

// Standard header includes
#include <iostream>
#include <optional>
#include <type_traits>
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

//! Quantize mantissa
[[maybe_unused]] static APY_INLINE void quantize_mantissa(
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
        // Using 'res_man' directly here is fine since G can only be '0' or '1', thus
        // calculating the LSB of 'res_man' is not needed.
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
    case QuantizationMode::TRN_MAG: // Does not really make sense for floating-point
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
[[maybe_unused]] static APY_INLINE void quantize_mantissa(
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
man_t ipow(man_t base, unsigned int n);

//! Get the number of left shifts needed to make fx>=1.0
[[maybe_unused]] static APY_INLINE unsigned int
leading_zeros_apyfixed(const APyFixed& fx)
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

/* ********************************************************************************** *
 * *              Floating-point iterator-based arithmetic functions                * *
 * ********************************************************************************** */

static APY_INLINE APyFloatData cast_no_quant(
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

    // Normalize the exponent and mantissa if convertering from a subnormal
    man_t prev_man = src.man;
    if (src.exp == 0) {
        const exp_t subn_adjustment = count_trailing_bits(src.man);
        const man_t remainder = src.man % (1ULL << subn_adjustment);
        new_exp = new_exp - src_spec.man_bits + subn_adjustment;
        prev_man = remainder << (src_spec.man_bits - subn_adjustment);
    }

    // Initial value for mantissa
    man_t new_man = prev_man << (dst_spec.man_bits - src_spec.man_bits);
    return { src.sign, exp_t(new_exp), man_t(new_man) };
}

//! Iterator-based less-than function, comparing `src1 < src2`
static APY_INLINE bool floating_point_less_than(
    const APyFloatData& src1,
    const APyFloatSpec& src1_spec,
    const APyFloatData& src2,
    const APyFloatSpec& src2_spec
)
{
    if (is_nan(src1, src1_spec.exp_bits) || is_nan(src2, src2_spec.exp_bits)) {
        return false;
    } else if (is_zero(src1) && is_zero(src2)) {
        return false;
    } else if (src1.sign != src2.sign) {
        return src1.sign;
    }

    if (src1_spec == src2_spec) {
        if (src1.exp < src2.exp) {
            return !src1.sign;
        } else if (src1.exp == src2.exp) {
            return src1.sign ^ (src1.man < src2.man);
        } else {
            return src1.sign;
        }
    } else {
        // Cast operands to a larger format that can represent both numbers
        uint8_t exp_bits = std::max(src1_spec.exp_bits, src2_spec.exp_bits);
        uint8_t man_bits = std::max(src1_spec.man_bits, src2_spec.man_bits);
        exp_t bias = ieee_bias(exp_bits);
        auto lhs_big = cast_no_quant(src1, src1_spec, { exp_bits, man_bits, bias });
        auto rhs_big = cast_no_quant(src2, src2_spec, { exp_bits, man_bits, bias });

        if (lhs_big.exp < rhs_big.exp) {
            return !src1.sign;
        } else if (lhs_big.exp == rhs_big.exp) {
            return src1.sign ^ (lhs_big.man < rhs_big.man);
        } else {
            return src1.sign;
        }
    }
}

//! Iterator-based less-than function, comparing `abs(src1) < abs(src2)`
static APY_INLINE bool floating_point_less_than_abs(
    const APyFloatData& src1,
    const APyFloatSpec& src1_spec,
    const APyFloatData& src2,
    const APyFloatSpec& src2_spec
)
{
    if (is_nan(src1, src1_spec.exp_bits) || is_nan(src2, src2_spec.exp_bits)) {
        return false;
    } else if (is_zero(src1) && is_zero(src2)) {
        return false;
    }

    if (src1_spec == src2_spec) {
        if (src1.exp < src2.exp) {
            return true;
        } else if (src1.exp == src2.exp) {
            return src1.man < src2.man;
        } else {
            return false;
        }
    } else {
        // Cast operands to a larger format that can represent both numbers
        uint8_t exp_bits = std::max(src1_spec.exp_bits, src2_spec.exp_bits);
        uint8_t man_bits = std::max(src1_spec.man_bits, src2_spec.man_bits);
        exp_t bias = ieee_bias(exp_bits);
        auto lhs_big = cast_no_quant(src1, src1_spec, { exp_bits, man_bits, bias });
        auto rhs_big = cast_no_quant(src2, src2_spec, { exp_bits, man_bits, bias });

        if (lhs_big.exp < rhs_big.exp) {
            return true;
        } else if (lhs_big.exp == rhs_big.exp) {
            return lhs_big.man < rhs_big.man;
        } else {
            return false;
        }
    }
}

//! Iterator-based floating-point sum
template <
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void floating_point_sums(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const std::size_t n_elements,
    const QuantizationMode& qntz
)
{
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = src1_spec.man_bits + 5;
    if (src1_spec == src2_spec && (max_man_bits <= _MAN_T_SIZE_BITS)
        && (qntz != QuantizationMode::STOCH_WEIGHTED)) {
        /*
         * Specialization #1: LHS and RHS have same format, the mantissa sum can fit
         * in a single `std::uint64_t`, and quantization mode is not `STOCK_WEIGHTED`.
         */
        const exp_t res_max_exponent = ((1ULL << src1_spec.exp_bits) - 1);
        const man_t final_res_leading_one = (1ULL << src1_spec.man_bits);
        const man_t res_leading_one = final_res_leading_one << 3;
        const man_t carry_res_leading_one = res_leading_one << 1;
        const std::uint8_t tmp_man_bits = src1_spec.man_bits + 4;
        const auto shift_normalization_const = _MAN_T_SIZE_BITS - tmp_man_bits;
        const auto man_mask = carry_res_leading_one - 1;
        const bool is_to_neg = qntz == QuantizationMode::TRN;
        for (std::size_t i = 0; i < n_elements; i++) {
            auto x = src1[SRC1_INC * i];
            auto y = src2[SRC2_INC * i];
            auto& z = dst[DST_INC * i];

            // Handle zero cases
            bool x_is_zero_exponent = (x.exp == 0);
            bool y_is_zero_exponent = (y.exp == 0);
            if (x_is_zero_exponent && x.man == 0) {
                if (y_is_zero_exponent && y.man == 0) {
                    y.sign = (x.sign == y.sign) ? x.sign : is_to_neg;
                }
                z = y;
                continue;
            }
            if (y_is_zero_exponent && y.man == 0) {
                z = x;
                continue;
            }
            bool x_is_max_exponent = x.exp == res_max_exponent;
            bool y_is_max_exponent = y.exp == res_max_exponent;

            // Handle the NaN and inf cases
            if (x_is_max_exponent || y_is_max_exponent) {
                if ((x_is_max_exponent && x.man != 0)
                    || (y_is_max_exponent && y.man != 0)
                    || (x_is_max_exponent && y_is_max_exponent && x.sign != y.sign)) {
                    // Set to NaN
                    z = { x.sign, exp_t(res_max_exponent), man_t(1) };
                    continue;
                }

                // Handle inf cases
                if (x_is_max_exponent && x.man == 0) {
                    // Set to inf
                    z = { x.sign, exp_t(res_max_exponent), man_t(0) };
                    continue;
                }

                if (y_is_max_exponent && y.man == 0) {
                    // Set to inf
                    z = { y.sign, exp_t(res_max_exponent), man_t(0) };
                    continue;
                }
            }

            // Compute sign and swap operands if need to make sure |x| >= |y|
            if (x.exp < y.exp || (x.exp == y.exp && x.man < y.man)) {
                std::swap(x, y);
                std::swap(x_is_zero_exponent, y_is_zero_exponent);
            } else if (x.sign != y.sign && x.exp == y.exp && x.man == y.man) {
                // Set to zero
                z = { is_to_neg, exp_t(0), man_t(0) };
                continue;
            }

            // Tentative exponent
            exp_t new_exp = x.exp + x_is_zero_exponent;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);
            man_t my = (y_is_zero_exponent ? 0 : res_leading_one) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = new_exp - y.exp - y_is_zero_exponent;

            // Align mantissa based on difference in exponent
            man_t m_aligned;
            if (exp_delta <= 3) {
                m_aligned = my >> exp_delta;
            } else if (exp_delta >= max_man_bits) {
                m_aligned = (my >> max_man_bits) | 1;
            } else {
                m_aligned
                    = (my >> exp_delta) | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
            }

            // Perform addition / subtraction
            man_t new_man = (x.sign == y.sign) ? mx + m_aligned : mx - m_aligned;

            // Check for carry and cancellation
            if (new_man & carry_res_leading_one) {
                // Carry
                new_exp++;
            } else if (new_man & res_leading_one) {
                // Align mantissa to carry case
                new_man <<= 1;
            } else {
                // Cancellation or addition with subnormals
                // Mantissa should be shifted until 1.xx is obtained or new_exp equals 0
                const unsigned int man_leading_zeros = leading_zeros(new_man);
                const unsigned int normalizing_shift
                    = man_leading_zeros - shift_normalization_const;

                if (new_exp > normalizing_shift) {
                    new_man <<= normalizing_shift + 1;
                    new_exp -= normalizing_shift;
                } else {
                    // The result will be a subnormal
                    new_man <<= new_exp;
                    new_exp = 0;
                }
            }

            new_man &= man_mask;

            quantize_mantissa(
                new_man,
                new_exp,
                res_max_exponent,
                4,
                x.sign,
                final_res_leading_one,
                3,
                7,
                qntz
            );
            z = { x.sign, exp_t(new_exp), man_t(new_man) };
        }

        return; /* Return from Specialization #1 */
    }

    std::uint8_t res_exp_bits = std::max(src1_spec.exp_bits, src2_spec.exp_bits);
    std::uint8_t res_man_bits = std::max(src1_spec.man_bits, src2_spec.man_bits);
    exp_t res_bias = calc_bias(
        res_exp_bits,
        src1_spec.exp_bits,
        src1_spec.bias,
        src2_spec.exp_bits,
        src2_spec.bias
    );
    APyFloatSpec res_spec = { res_exp_bits, res_man_bits, res_bias };

    if (max_man_bits <= _MAN_T_SIZE_BITS && qntz != QuantizationMode::STOCH_WEIGHTED) {
        /*
         * Specialization #2: Not the same format, but the mantissa sum still fits into
         * a single `std::uint64_t`, and the quantization mode is still not
         * `STOCH_WEIGHTED`.
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            bool swap = floating_point_less_than_abs(
                src1[SRC1_INC * i], src1_spec, src2[SRC2_INC * i], src2_spec
            );
            const APyFloatSpec& x_spec = swap ? src2_spec : src1_spec;
            const APyFloatSpec& y_spec = swap ? src1_spec : src2_spec;
            const APyFloatData& x = swap ? src2[SRC2_INC * i] : src1[SRC1_INC * i];
            const APyFloatData& y = swap ? src1[SRC1_INC * i] : src2[SRC2_INC * i];
            APyFloatData& z = dst[DST_INC * i];

            std::cout << "Woop!" << std::endl;

            // Handle zero cases
            if (is_zero(x) && is_zero(y)) {
                bool same_sign = x.sign == y.sign;
                bool new_sign = same_sign ? x.sign : qntz == QuantizationMode::TRN;
                z = { new_sign, 0, 0 };
                continue;
            } else if (is_zero(x)) {
                z = cast_no_quant(y, y_spec, res_spec);
                if (y_spec.man_bits - res_spec.man_bits >= 0) {
                    quantize_mantissa(
                        z.man,
                        z.exp,
                        (1ULL << res_spec.exp_bits) - 1,
                        y_spec.man_bits - res_spec.man_bits,
                        y.sign,
                        (1ULL << res_spec.man_bits),
                        qntz
                    );
                }
                continue;
            } else if (is_zero(y)) {
                z = cast_no_quant(x, x_spec, res_spec);
                if (x_spec.man_bits - res_spec.man_bits >= 0) {
                    quantize_mantissa(
                        z.man,
                        z.exp,
                        (1ULL << res_spec.exp_bits) - 1,
                        x_spec.man_bits - res_spec.man_bits,
                        x.sign,
                        (1ULL << res_spec.man_bits),
                        qntz
                    );
                }
                continue;
            }

            std::cout << "YAY!" << std::endl;

            // Handle the NaN and inf cases
            if (is_max_exponent(x, x_spec) || is_max_exponent(y, y_spec)) {
                exp_t res_max_exponent = exp_t((1ULL << res_spec.exp_bits) - 1);
                if (is_nan(x, x_spec) || is_nan(y, y_spec)
                    || (x.sign != y.sign && is_inf(x, x_spec) && is_inf(y, y_spec))) {
                    z = { 0, res_max_exponent, 1 }; // NaN
                    continue;
                } else {
                    bool sign = is_max_exponent(x, x_spec) ? x.sign : y.sign;
                    z = { sign, res_max_exponent, 0 }; // Inf
                    continue;
                }
            }

            std::cout << fmt::format("x_before: ( {}, {}, {} )", x.sign, x.exp, x.man)
                      << std::endl;
            std::cout << fmt::format("y_before: ( {}, {}, {} )", y.sign, y.exp, y.man)
                      << std::endl;

            APyFloatData x_wide = cast_no_quant(
                x, x_spec, { res_spec.exp_bits, res_spec.man_bits, x_spec.bias }
            );
            APyFloatData y_wide = cast_no_quant(
                y, y_spec, { res_spec.exp_bits, res_spec.man_bits, y_spec.bias }
            );
            if (x.sign != y.sign) {
                if (x_wide.man == y_wide.man) {
                    if (true_exp(x_wide, x_spec.bias)
                        == true_exp(y_wide, y_spec.bias)) {
                        bool sign = qntz == QuantizationMode::TRN;
                        z = { sign, 0, 0 };
                        continue;
                    }
                }
            }

            std::cout << fmt::format(
                "x_wide: ( {}, {}, {} )", x_wide.sign, x_wide.exp, x_wide.man
            ) << std::endl;
            std::cout << fmt::format(
                "y_wide: ( {}, {}, {} )", y_wide.sign, y_wide.exp, y_wide.man
            ) << std::endl;

            const std::int64_t x_true_exp = true_exp(x_wide, x_spec.bias);
            const unsigned exp_delta = x_true_exp - true_exp(y_wide, y_spec.bias);
            std::int64_t new_exp = x_true_exp + res_spec.bias;
            std::cout << "New exp: " << new_exp << std::endl;

            man_t mx = true_man(x_wide, res_spec) << 3;
            man_t my = true_man(y_wide, res_spec) << 3;

            std::cout << "mx: " << mx << std::endl;
            std::cout << "my: " << my << std::endl;

            // Align mantissa based on difference in exponent
            man_t highY;
            if (exp_delta <= 3) {
                highY = my >> exp_delta;
            } else if (exp_delta >= max_man_bits) {
                highY = (my >> max_man_bits) | 1;
            } else {
                highY
                    = (my >> exp_delta) | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
            }

            // Perform addition / subtraction
            man_t new_man = (x_wide.sign == y_wide.sign) ? mx + highY : mx - highY;

            std::cout << "NEW_MAN: " << new_man << std::endl;

            // Check for carry and cancellation
            int c = 0;
            const man_t res_leading_one = (1ULL << res_spec.man_bits) << 3;
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
                    - (_MAN_T_SIZE_BITS - res_spec.man_bits - 4
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
            man_t res_man = new_man;
            exp_t res_exp = new_exp;
            quantize_mantissa(
                res_man,
                res_exp,
                ((1ULL << res_spec.exp_bits) - 1), // max_exp
                3 + c,                             // bits_to_quantize
                x.sign,                            // sign
                1ULL << (res_spec.man_bits),       // man_msb_constant
                qntz                               // quantization
            );

            z = { x.sign, res_exp, res_man };
            std::cout << "MAMMA MIA!" << std::endl;
            continue;
        }

        return; /* Return from specialization #2 */
    } else {
        /*
         * General path. Always works, but is the slowest.
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            bool swap = floating_point_less_than_abs(
                src1[SRC1_INC * i], src1_spec, src2[SRC2_INC * i], src2_spec
            );
            const APyFloatSpec& x_spec = swap ? src2_spec : src1_spec;
            const APyFloatSpec& y_spec = swap ? src1_spec : src2_spec;
            const APyFloatData& x = swap ? src2[SRC2_INC * i] : src1[SRC1_INC * i];
            const APyFloatData& y = swap ? src1[SRC1_INC * i] : src2[SRC2_INC * i];
            APyFloatData& z = dst[DST_INC * i];

            // Handle zero cases
            if (is_zero(x) && is_zero(y)) {
                bool same_sign = x.sign == y.sign;
                bool new_sign = same_sign ? x.sign : qntz == QuantizationMode::TRN;
                z = { new_sign, 0, 0 };
                continue;
            } else if (is_zero(x)) {
                z = cast_no_quant(y, y_spec, res_spec);
                if (y_spec.man_bits - res_spec.man_bits >= 0) {
                    quantize_mantissa(
                        z.man,
                        z.exp,
                        (1ULL << res_spec.exp_bits) - 1,
                        y_spec.man_bits - res_spec.man_bits,
                        y.sign,
                        (1ULL << res_spec.man_bits),
                        qntz
                    );
                }
                continue;
            } else if (is_zero(y)) {
                z = cast_no_quant(x, x_spec, res_spec);
                if (x_spec.man_bits - res_spec.man_bits >= 0) {
                    quantize_mantissa(
                        z.man,
                        z.exp,
                        (1ULL << res_spec.exp_bits) - 1,
                        x_spec.man_bits - res_spec.man_bits,
                        x.sign,
                        (1ULL << res_spec.man_bits),
                        qntz
                    );
                }
                continue;
            }

            // Handle the NaN and inf cases
            if (is_max_exponent(x, x_spec) || is_max_exponent(y, y_spec)) {
                exp_t res_max_exponent = exp_t((1ULL << res_spec.exp_bits) - 1);
                if (is_nan(x, x_spec) || is_nan(y, y_spec)
                    || (x.sign != y.sign && is_inf(x, x_spec) && is_inf(y, y_spec))) {
                    z = { 0, res_max_exponent, 1 }; // NaN
                    continue;
                } else {
                    bool sign = is_max_exponent(x, x_spec) ? x.sign : y.sign;
                    z = { sign, res_max_exponent, 0 }; // Inf
                    continue;
                }
            }

            APyFloatData x_wide = cast_no_quant(x, x_spec, res_spec);
            APyFloatData y_wide = cast_no_quant(y, y_spec, res_spec);
            if (x_wide.sign != y_wide.sign) {
                if (x_wide.man == y_wide.man) {
                    if (true_exp(x_wide, res_spec) == true_exp(y_wide, res_spec)) {
                        bool sign = qntz == QuantizationMode::TRN;
                        z = { sign, 0, 0 };
                        continue;
                    }
                }
            }

            const std::int64_t x_true_exp = true_exp(x_wide, res_spec);
            const unsigned exp_delta = x_true_exp - true_exp(y_wide, res_spec);
            std::int64_t new_exp = x_true_exp + res_spec.bias;

            man_t mx = true_man(x_wide, res_spec);
            man_t my = true_man(y_wide, res_spec);

            const APyFixed apy_mx(2 + res_spec.man_bits, 2, { UINT64_TO_LIMB(mx) });
            const APyFixed apy_my(
                2 + res_spec.man_bits, 2 - exp_delta, { UINT64_TO_LIMB(my) }
            );

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
            quantize_apymantissa(apy_res, x.sign, res_spec.man_bits, qntz);

            // Carry from quantization. In practice, the exponent will never be
            // incremented twice
            if (apy_res.positive_greater_than_equal_pow2(1)) {
                new_exp++;
                apy_res >>= 1;
            }

            // Check for overflow
            if (new_exp >= ((1LL << res_spec.exp_bits) - 1)) {
                exp_t res_max_exponent = ((1LL << res_spec.exp_bits) - 1);
                if (do_infinity(qntz, x.sign)) {
                    z = { x.sign, res_max_exponent, 0 };
                    continue;
                } else {
                    z = { x.sign, res_max_exponent, (1ULL << res_spec.man_bits) - 1 };
                    continue;
                }
            }

            // Remove leading one
            if (apy_res.positive_greater_than_equal_pow2(0)) {
                apy_res = apy_res - APyFixed(2, 2, { 1 });
            }

            z = { x.sign, exp_t(new_exp), apy_res.get_lsbs() };
            continue;
        }

        return; // Return from the most general path
    }
}

//! Iterator-based floating-point hadamard product
template <
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void floating_point_products(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const std::size_t n_elements,
    const QuantizationMode& qntz
)
{
    // Compute result bit specification
    const std::uint8_t res_exp_bits = std::max(src1_spec.exp_bits, src2_spec.exp_bits);
    const std::uint8_t res_man_bits = std::max(src1_spec.man_bits, src2_spec.man_bits);
    const exp_t res_bias = calc_bias(
        res_exp_bits,
        src1_spec.exp_bits,
        src1_spec.bias,
        src2_spec.exp_bits,
        src2_spec.bias
    );

    // Pre-compute some useful values
    const unsigned sum_man_bits = src1_spec.man_bits + src2_spec.man_bits;
    const exp_t src1_max_exponent = ((1ULL << src1_spec.exp_bits) - 1);
    const exp_t src2_max_exponent = ((1ULL << src2_spec.exp_bits) - 1);
    const exp_t res_max_exponent = ((1ULL << res_exp_bits) - 1);

    if (sum_man_bits <= _MAN_LIMIT_BITS) {
        /*
         * Mantissa product fits in a single `std::uint64_t`
         */

        // Pre-compute some useful values
        const unsigned new_man_bits = sum_man_bits + 2;
        const man_t two = 1ULL << (new_man_bits);
        const man_t two_before = 1ULL << (new_man_bits - 1);
        const man_t one_before = 1ULL << (new_man_bits - 2);
        const man_t two_res = (1ULL << res_man_bits);
        const int man_bits_delta = new_man_bits - res_man_bits;
        const man_t sticky_constant = (1ULL << (man_bits_delta - 1)) - 1;

        for (std::size_t i = 0; i < n_elements; i++) {
            const auto& x = src1[SRC1_INC * i];
            const auto& y = src2[SRC2_INC * i];
            auto& z = dst[DST_INC * i];

            // Compute result sign
            bool res_sign = x.sign ^ y.sign;
            const bool x_is_subnormal = (x.exp == 0);
            const bool x_is_maxexp = (x.exp == src1_max_exponent);
            const bool y_is_subnormal = (y.exp == 0);
            const bool y_is_maxexp = (y.exp == src2_max_exponent);
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
                    z = { res_sign, res_max_exponent, 1 };
                    continue; // early exit
                } else if (src1_inf || src2_inf) {
                    // Set to inf
                    z = { res_sign, res_max_exponent, 0 };
                    continue; // early exit
                } else if (src1_zero || src2_zero) {
                    // Set to zero
                    z = { res_sign, 0, 0 };
                    continue; // early exit
                }
            }

            // Tentative mantissa and exponent
            man_t mx = (man_t(!x_is_subnormal) << src1_spec.man_bits) | x.man;
            man_t my = (man_t(!y_is_subnormal) << src2_spec.man_bits) | y.man;
            man_t new_man = mx * my;
            auto tmp_exp = true_exp(x, src1_spec) + true_exp(y, src2_spec) + res_bias;

            // Check result from multiplication larger than/equal two
            if (new_man & two_before) {
                tmp_exp++;
                new_man <<= 1;
            } else if (new_man & one_before) {
                // Align with longer result
                new_man <<= 2;
            } else {
                // One or two of the operands were subnormal. If the exponent is
                // positive, the result is normalized by left-shifting until the
                // exponent is zero or the mantissa is 1.xx
                std::int64_t leading_zeros = 1 + sum_man_bits - bit_width(new_man);
                int shift = std::max(std::min(tmp_exp, leading_zeros), std::int64_t(0));
                tmp_exp -= shift;
                new_man <<= shift + 2; // + 2 to align with longer result
            }

            if (tmp_exp <= 0) {
                if (tmp_exp < -std::int64_t(res_man_bits)) {
                    // Exponent too small after rounding
                    z = { res_sign, 0, quantize_close_to_zero(res_sign, qntz) };
                    continue; // early exit
                }
                new_man = (new_man >> (-tmp_exp + 1))
                    | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
                tmp_exp = 0;
            }

            exp_t res_exp = exp_t(tmp_exp);
            new_man &= two - 1;
            quantize_mantissa(
                new_man,
                res_exp,
                res_max_exponent,
                man_bits_delta,
                res_sign,
                two_res,
                man_bits_delta - 1,
                sticky_constant,
                qntz
            );

            z = { res_sign, res_exp, new_man };
        }
    } else { /* sum_man_bits > _MAN_LIMIT_BITS */
        /*
         * Mantissa product does not fit in a single `std::uint64_t`
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            const auto& x = src1[SRC1_INC * i];
            const auto& y = src2[SRC2_INC * i];
            auto& z = dst[DST_INC * i];

            // Compute result sign
            bool res_sign = x.sign ^ y.sign;

            const bool x_is_subnormal = (x.exp == 0);
            const bool x_is_maxexp = (x.exp == src1_max_exponent);
            const bool y_is_subnormal = (y.exp == 0);
            const bool y_is_maxexp = (y.exp == src2_max_exponent);
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
                    z = { res_sign, res_max_exponent, 1 };
                    continue; // early exit
                } else if (src1_inf || src2_inf) {
                    // Set to inf
                    z = { res_sign, res_max_exponent, 0 };
                    continue; // early exit
                } else if (src1_zero || src2_zero) {
                    // Set to zero
                    z = { res_sign, 0, 0 };
                    continue; // early exit
                }
            }

            // Normalize both inputs
            auto [norm_x, norm_x_exp_bits, norm_x_bias] = normalize(x, src1_spec);
            auto [norm_y, norm_y_exp_bits, norm_y_bias] = normalize(y, src2_spec);

            // Add leading one's
            const man_t mx = true_man(norm_x, norm_x_exp_bits, src1_spec.man_bits);
            const man_t my = true_man(norm_y, norm_y_exp_bits, src2_spec.man_bits);

            // Tentative exponent
            std::int64_t new_exp
                = ((std::int64_t)norm_x.exp - (std::int64_t)norm_x_bias)
                + ((std::int64_t)norm_y.exp - (std::int64_t)norm_y_bias) + res_bias;

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
            quantize_apymantissa(apy_res, res_sign, res_man_bits, qntz);

            // Carry from quantization
            if (apy_res.positive_greater_than_equal_pow2(1)) {
                new_exp++;
                apy_res >>= 1;
            }

            if (new_exp >= exp_t((1ULL << res_exp_bits) - 1)) {
                if (do_infinity(qntz, res_sign)) {
                    z = { res_sign, res_max_exponent, 0 };
                } else {
                    z = { res_sign, res_max_exponent - 1, (1ULL << res_man_bits) - 1 };
                }
                continue;
            }

            if (apy_res.positive_greater_than_equal_pow2(0)) { // Remove leading one
                apy_res = apy_res - APyFixed(2, 2, { 1 });

                // If a leading one is present while the exponent is zero,
                // then it 'acts like a carry' and creates a normal number
                if (new_exp == 0) {
                    new_exp = 1;
                }
            }
            apy_res <<= res_man_bits;
            z = { res_sign, exp_t(new_exp), man_t(apy_res.to_double()) };
        }
    }
}

//! Perform a single float product
static APY_INLINE void float_product(
    const APyFloatData& src1,
    const APyFloatData& src2,
    APyFloatData& dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const QuantizationMode& quantization
)
{
    floating_point_products(
        &src1, &src2, &dst, src1_spec, src2_spec, 1 /* n */, quantization
    );
}

//! Perform a single float product
static APY_INLINE void floating_point_sum(
    const APyFloatData& src1,
    const APyFloatData& src2,
    APyFloatData& dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const QuantizationMode& quantization
)
{
    floating_point_sums(
        &src1, &src2, &dst, src1_spec, src2_spec, 1 /* n */, quantization
    );
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
    scalar_variant_t<APYFLOAT_TYPE> accumulator(
        0, 0, 0, res_exp_bits, res_man_bits, res_bias
    );
    for (std::size_t i = 0; i < n_items; i++) {
        // Multiply
        floating_point_products(
            src1 + i,
            src2 + i,
            &product,
            { x.get_exp_bits(), x.get_man_bits(), x.get_bias() },
            { y.get_exp_bits(), y.get_man_bits(), y.get_bias() },
            1 /* n_elements= */,
            get_float_quantization_mode()
        );

        // Accumulate
        accumulator += APyFloat(product, res_exp_bits, res_man_bits, res_bias);
    }

    *dst = accumulator.get_data();
}

#endif // _APYFLOAT_UTIL_H
