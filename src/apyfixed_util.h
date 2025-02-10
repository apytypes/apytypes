/*
 * APyFixed utility functions
 */

#ifndef _APYFIXED_UTIL_H
#define _APYFIXED_UTIL_H

#include "apytypes_common.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"
#include "ieee754.h"

#include <functional> // std::bind, std::function, std::placeholders

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/* ********************************************************************************** *
 * *    Fixed-point iterator based in-place quantization with multi-limb support    * *
 * ********************************************************************************** */

template <typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_trn(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        limb_vector_asr(it_begin, it_end, -left_shift_amnt);
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_trn_inf(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (limb_vector_is_negative(it_begin, it_end)) {
            if (right_shift_amnt < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, right_shift_amnt)) {
                    limb_vector_add_pow2(it_begin, it_end, right_shift_amnt);
                }
                limb_vector_asr(it_begin, it_end, right_shift_amnt);
            } else {
                std::fill(it_begin, it_end, 0);
            }
        } else { /* !limb_vector_is_negative(it_begin, it_end) */
            if (right_shift_amnt < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, right_shift_amnt)) {
                    limb_vector_add_pow2(it_begin, it_end, right_shift_amnt);
                }
                limb_vector_asr(it_begin, it_end, right_shift_amnt);
            } else {
                mp_limb_t add_one = limb_vector_or_reduce(it_begin, it_end, bits);
                std::fill(it_begin, it_end, 0);
                *it_begin = add_one;
            }
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_trn_zero(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else { /* limb_vector_is_negative(it_begin, it_end) */
            if (right_shift_amnt < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, right_shift_amnt)) {
                    limb_vector_add_pow2(it_begin, it_end, right_shift_amnt);
                }
                limb_vector_asr(it_begin, it_end, right_shift_amnt);
            } else {
                std::fill(it_begin, it_end, 0);
            }
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_trn_mag(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else { /* limb_vector_is_negative(it_begin, it_end) */
            if (right_shift_amnt < unsigned(bits)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt);
                limb_vector_asr(it_begin, it_end, right_shift_amnt);
            } else {
                std::fill(it_begin, it_end, 0);
            }
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_trn_away(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            if (right_shift_amnt < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, right_shift_amnt)) {
                    limb_vector_add_pow2(it_begin, it_end, right_shift_amnt);
                }
                limb_vector_asr(it_begin, it_end, right_shift_amnt);
            } else {
                mp_limb_t add_one = limb_vector_or_reduce(it_begin, it_end, bits);
                std::fill(it_begin, it_end, 0);
                *it_begin = add_one;
            }
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_rnd(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            std::fill(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_rnd_zero(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            if (limb_vector_is_negative(it_begin, it_end)
                || limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            std::fill(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_rnd_inf(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            if (!limb_vector_is_negative(it_begin, it_end)
                || limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else if (right_shift_amnt == unsigned(bits)) {
            if (limb_vector_is_negative(it_begin, it_end)
                && limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            std::fill(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_rnd_min_inf(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            if (limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else if (right_shift_amnt == unsigned(bits)) {
            if (limb_vector_is_negative(it_begin, it_end)
                && limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            std::fill(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_rnd_conv(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            if (limb_vector_test_bit(it_begin, it_end, right_shift_amnt)
                || limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            std::fill(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_rnd_conv_odd(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            if (!limb_vector_test_bit(it_begin, it_end, right_shift_amnt)
                || limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1)) {
                limb_vector_add_pow2(it_begin, it_end, right_shift_amnt - 1);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else if (right_shift_amnt == unsigned(bits)) {
            bool fill_neg = limb_vector_is_negative(it_begin, it_end)
                && !limb_vector_or_reduce(it_begin, it_end, right_shift_amnt - 1);
            std::fill(it_begin, it_end, fill_neg ? -1 : 0);
        } else {
            std::fill(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_jam(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        limb_vector_asr(it_begin, it_end, -left_shift_amnt);
    }
    limb_vector_set_bit(it_begin, it_end, 0, true);
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_jam_unbiased(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        unsigned right_shift_amnt = -left_shift_amnt;
        if (right_shift_amnt < unsigned(bits)) {
            if (limb_vector_or_reduce(it_begin, it_end, right_shift_amnt)) {
                limb_vector_set_bit(it_begin, it_end, right_shift_amnt, true);
            }
            limb_vector_asr(it_begin, it_end, right_shift_amnt);
        } else {
            bool jam = limb_vector_or_reduce(it_begin, it_end, bits);
            if (limb_vector_is_negative(it_begin, it_end)) {
                std::fill(it_begin, it_end, -1);
            } else {
                std::fill(it_begin, it_end, 0);
            }
            limb_vector_set_bit(it_begin, it_end, 0, jam);
        }
    }
}

template <typename RANDOM_ACCESS_ITERATOR_INOUT>
static void quantize(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits,
    QuantizationMode quantization
)
{
    /*
     * Note to authors trying to implement quantization modes: All the quantization
     * methods (e.g., `_quantize_trn_zero()`) assumes that that the data being quantized
     * has already been copied into the iterator region pointed to by `it_begin` and
     * `it_end`. These set of functions simply shift the data in the iterator region
     * into the correct place and performs any necessary quantization bit-fiddling.
     */
    switch (quantization) {
    case QuantizationMode::TRN:
        _quantize_trn(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::TRN_INF:
        _quantize_trn_inf(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::TRN_ZERO:
        _quantize_trn_zero(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::TRN_MAG:
        _quantize_trn_mag(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::TRN_AWAY:
        _quantize_trn_away(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND:
        _quantize_rnd(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_ZERO:
        _quantize_rnd_zero(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_INF:
        _quantize_rnd_inf(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_MIN_INF:
        _quantize_rnd_min_inf(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_CONV:
        _quantize_rnd_conv(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::RND_CONV_ODD:
        _quantize_rnd_conv_odd(
            it_begin, it_end, bits, int_bits, new_bits, new_int_bits
        );
        break;
    case QuantizationMode::JAM:
        _quantize_jam(it_begin, it_end, bits, int_bits, new_bits, new_int_bits);
        break;
    case QuantizationMode::JAM_UNBIASED:
        _quantize_jam_unbiased(
            it_begin, it_end, bits, int_bits, new_bits, new_int_bits
        );
        break;
    default:
        throw NotImplementedException(fmt::format(
            "Not implemented: fixed-point quantize() with mode {}",
            quantization == QuantizationMode::STOCH_WEIGHTED ? "`STOCH_WEIGHTED`"
                : quantization == QuantizationMode::STOCH_EQUAL
                ? "`STOCH_EQUAL`"
                : "unknown (did you pass `int` as `QuantizationMode`?)"
        ));
    }
}

/* ********************************************************************************** *
 * *     Fixed-point iterator based in-place overflowing with multi-limb support    * *
 * ********************************************************************************** */

template <class RANDOM_ACCESS_ITERATOR>
static APY_INLINE void _overflow_twos_complement(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int bits,
    int int_bits
)
{
    (void)int_bits;
    (void)it_end;
    if (bits % _LIMB_SIZE_BITS) {
        RANDOM_ACCESS_ITERATOR ms_limb_it = it_begin + bits_to_limbs(bits) - 1;
        unsigned shift_amount = _LIMB_SIZE_BITS - (bits % _LIMB_SIZE_BITS);
        *ms_limb_it = mp_limb_signed_t(*ms_limb_it << shift_amount) >> shift_amount;
    }
}

template <class RANDOM_ACCESS_ITERATOR>
static APY_INLINE void _overflow_saturate(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int bits,
    int int_bits
)
{
    (void)int_bits;
    RANDOM_ACCESS_ITERATOR ms_limb_it = it_begin + bits_to_limbs(bits) - 1;
    std::size_t utilized_bits_last_limb = (bits - 1) % _LIMB_SIZE_BITS + 1;

    bool sign = mp_limb_signed_t(*std::prev(it_end)) < 0;
    if (sign) {
        if (!limb_vector_all_ones(ms_limb_it, it_end, utilized_bits_last_limb - 1)) {
            std::fill(it_begin, ms_limb_it, 0);
            *ms_limb_it = ~((mp_limb_t(1) << (utilized_bits_last_limb - 1)) - 1);
        }
    } else { /* !sign */
        if (!limb_vector_all_zeros(ms_limb_it, it_end, utilized_bits_last_limb - 1)) {
            std::fill(it_begin, ms_limb_it, mp_limb_t(-1));
            *ms_limb_it = (mp_limb_t(1) << (utilized_bits_last_limb - 1)) - 1;
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR>
static APY_INLINE void _overflow_numeric_std(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int bits,
    int int_bits
)
{
    (void)int_bits;
    RANDOM_ACCESS_ITERATOR ms_limb_it = it_begin + bits_to_limbs(bits) - 1;
    std::size_t utilized_bits_last_limb = (bits - 1) % _LIMB_SIZE_BITS + 1;

    bool sign = mp_limb_signed_t(*std::prev(it_end)) < 0;
    if (sign) {
        // Force a `1` into the sign position (and above)
        *ms_limb_it |= ~((mp_limb_t(1) << (utilized_bits_last_limb - 1)) - 1);
    } else { /* !sign */
        // Force a `0` into the sign position (and above)
        *ms_limb_it &= ((mp_limb_t(1) << (utilized_bits_last_limb - 1)) - 1);
    }
}

template <class RANDOM_ACCESS_ITERATOR>
static void overflow(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    int new_bits,
    int new_int_bits,
    OverflowMode overflow
)
{
    /*
     * All overflow methods (e.g., `_overflow_saturate()`) assume that data being
     * overflown has been copied to the iterator region [ `it_begin`, `it_end` ) and is
     * shifted so that the binary point location is correct. The overflowing methods
     * only manipulates bits in: [ `it_begin`, `it_begin + bits_to_limbs(new_bits)` ).
     */
    switch (overflow) {
    case OverflowMode::WRAP:
        _overflow_twos_complement(it_begin, it_end, new_bits, new_int_bits);
        break;
    case OverflowMode::SAT:
        _overflow_saturate(it_begin, it_end, new_bits, new_int_bits);
        break;
    case OverflowMode::NUMERIC_STD:
        _overflow_numeric_std(it_begin, it_end, new_bits, new_int_bits);
        break;
    default:
        throw NotImplementedException(fmt::format(
            "Not implemented: APyFixed::_overflow(): with mode: {}",
            "unknown (did you pass `int` as `OverflowMode`?)"
        ));
    }
}

/* ********************************************************************************** *
 * *    Fixed-point iterator based casting with quantization and overflowing        * *
 * ********************************************************************************** */

/*!
 * General casting method for fixed-point numbers. It is named `_cast` to differentiate
 * it from the Python exposed `cast` methods (that usually just call this method).
 * General casting can perform both quantization and overflowing. The size of the
 * output region (`std::distance(dst_begin, dst_end)`) must be greater than or equal to
 * the size of the input region (`std::distance(src_begin, src_end)`), even when the
 * output bit-specifiers are smaller then the input bit-specifiers.
 */
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static APY_INLINE void _cast(
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    RANDOM_ACCESS_ITERATOR_IN src_end,
    RANDOM_ACCESS_ITERATOR_OUT dst_begin,
    RANDOM_ACCESS_ITERATOR_OUT dst_end,
    int src_bits,
    int src_int_bits,
    int dst_bits,
    int dst_int_bits,
    QuantizationMode q_mode,
    OverflowMode v_mode
)
{
    // Copy data into the result region and sign extend
    limb_vector_copy_sign_extend(src_begin, src_end, dst_begin, dst_end);

    // First perform quantization
    quantize(
        dst_begin, dst_end, src_bits, src_int_bits, dst_bits, dst_int_bits, q_mode
    );

    // Then perform overflowing
    overflow(dst_begin, dst_end, dst_bits, dst_int_bits, v_mode);
}

/*!
 * Casting when there is known before hand that no quantization or overflowing will
 * occur. Takes `left_shift_amount` which is the destination fractional bits minus the
 * source fractional bits.
 */
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static APY_INLINE void _cast_no_quantize_no_overflow(
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    RANDOM_ACCESS_ITERATOR_IN src_end,
    RANDOM_ACCESS_ITERATOR_OUT dst_begin,
    RANDOM_ACCESS_ITERATOR_OUT dst_end,
    unsigned int left_shift_amount
)
{
    // Copy data into the result region and sign extend
    limb_vector_copy_sign_extend(src_begin, src_end, dst_begin, dst_end);

    // Shift data into position
    limb_vector_lsl(dst_begin, dst_end, left_shift_amount);
}

/*!
 * Casting when there is known before hand that no quantization or overflowing will
 * occur. Takes `left_shift_amount` which is the destination fractional bits minus the
 * source fractional bits.
 */
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static APY_INLINE void _cast_no_quantize_no_overflow(
    RANDOM_ACCESS_ITERATOR_IN src,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src_limbs,
    std::size_t dst_limbs,
    std::size_t n_items,
    unsigned int left_shift_amount
)
{

    /*
     * Specialization #1: `src` and `dst` have equally many limbs
     */
    if (src_limbs == dst_limbs) {

        // Copy data into the result
        std::copy_n(src, src_limbs * n_items, dst);

        if (left_shift_amount > 0) {
            if (src_limbs == 1) { /* src_limbs == dst_limbs == 1 */
                for (std::size_t i = 0; i < n_items; i++) {
                    dst[i] = src[i] << left_shift_amount;
                }
            } else { /* src_limbs == dst_limbs > 1 */
                unsigned limb_skip = left_shift_amount / _LIMB_SIZE_BITS;
                unsigned limb_shift = left_shift_amount % _LIMB_SIZE_BITS;
                for (std::size_t i = 0; i < n_items; i++) {
                    limb_vector_lsl_inner(
                        dst + (i + 0) * dst_limbs,
                        dst + (i + 1) * dst_limbs,
                        limb_skip,
                        limb_shift,
                        dst_limbs
                    );
                }
            }
        }

        return; // early return specialization #1
    }

    /*
     * General case: `dst_limbs > src_limbs`
     */
    if (left_shift_amount > 0) {
        for (std::size_t i = 0; i < n_items; i++) {
            _cast_no_quantize_no_overflow(
                src + (i + 0) * src_limbs, // src_begin
                src + (i + 1) * src_limbs, // src_end
                dst + (i + 0) * dst_limbs, // dst_begin,
                dst + (i + 1) * dst_limbs, // dst_end
                left_shift_amount          // left_shift_amount
            );
        }
    } else {
        for (std::size_t i = 0; i < n_items; i++) {
            limb_vector_copy_sign_extend(
                src + (i + 0) * src_limbs, // src_begin
                src + (i + 1) * src_limbs, // src_end
                dst + (i + 0) * dst_limbs, // dst_begin,
                dst + (i + 1) * dst_limbs  // dst_end
            );
        }
    }
}

/* ********************************************************************************** *
 * *     Fixed-point iterator based arithmetic functions with multi-limb support    * *
 * ********************************************************************************** */

//! Iterator-based multi-limb two's complement fixed-point product. The scratch
//! vector `prod_abs` must have space for atleast `src1_limbs + src2_limbs` limbs. The
//! scratch vectors `op1_abs` and `op2_abs` must have space for atleast `src1_limbs`
//! and `src2_limbs` limbs, respectively. No overlap between `prod_abs` and `op[12]_abs`
//! allowed. Overlap between `prod_abs` and `dst` is allowed
//! if `dst_limbs >= src1_limbs + src2_limbs`.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void fixed_point_product(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs,
    std::size_t src2_limbs,
    std::size_t dst_limbs,
    RANDOM_ACCESS_ITERATOR_INOUT op1_abs,
    RANDOM_ACCESS_ITERATOR_INOUT op2_abs,
    RANDOM_ACCESS_ITERATOR_INOUT prod_abs
)
{
    // Resulting sign
    bool sign1 = mp_limb_signed_t(*(src1 + src1_limbs - 1)) < 0;
    bool sign2 = mp_limb_signed_t(*(src2 + src2_limbs - 1)) < 0;
    bool result_sign = sign1 ^ sign2;

    // Retrieve the absolute value of both operands
    limb_vector_abs(src1, src1 + src1_limbs, op1_abs);
    limb_vector_abs(src2, src2 + src2_limbs, op2_abs);

    // Perform the multiplication and possibly negate
    if (src1_limbs < src2_limbs) {
        mpn_mul(&prod_abs[0], &op2_abs[0], src2_limbs, &op1_abs[0], src1_limbs);
    } else {
        mpn_mul(&prod_abs[0], &op1_abs[0], src1_limbs, &op2_abs[0], src2_limbs);
    }

    // Negate and copy the result back
    std::size_t prod_limbs = src1_limbs + src2_limbs;
    if (result_sign) {
        if (dst_limbs <= prod_limbs) {
            limb_vector_negate(prod_abs, prod_abs + dst_limbs, dst);
        } else {
            bool is_zero = limb_vector_negate(prod_abs, prod_abs + prod_limbs, dst);
            mp_limb_t fill_val = is_zero ? mp_limb_t(0) : mp_limb_t(-1);
            std::fill_n(prod_abs + prod_limbs, dst_limbs - prod_limbs, fill_val);
        }
    } else {
        if (dst_limbs <= prod_limbs) {
            std::copy(prod_abs, prod_abs + dst_limbs, dst);
        } else {
            std::copy(prod_abs, prod_abs + prod_limbs, dst);
            std::fill_n(prod_abs + prod_limbs, dst_limbs - prod_limbs, mp_limb_t(0));
        }
    }
}

//! Iterator-based multi-limb two's complement fixed-point squaring. The scratch
//! vector `prod_abs` must have space for atleast `2 * src_limbs` limbs. The scratch
//! vector `op_abs` must have space for atleast `src_limbs` limbs. No overlap
//! between `prod_abs` and `op_abs` allowed. Overlap between `prod_abs` and `dst` is
//! allowed if `dst_limbs >= 2 * src_limbs`.
template <
    typename RANDOM_ACCESS_ITERATOR_IN,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void fixed_point_square(
    RANDOM_ACCESS_ITERATOR_IN src,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src_limbs,
    std::size_t dst_limbs,
    RANDOM_ACCESS_ITERATOR_INOUT op_abs,
    RANDOM_ACCESS_ITERATOR_INOUT prod_abs
)
{
    limb_vector_abs(src, src + src_limbs, op_abs);
    mpn_sqr(&*prod_abs, &*op_abs, src_limbs);
    if (dst_limbs <= 2 * src_limbs) {
        std::copy_n(prod_abs, dst_limbs, dst);
    } else {
        std::copy_n(prod_abs, 2 * src_limbs, dst);
        std::fill_n(prod_abs + 2 * src_limbs, dst_limbs - 2 * src_limbs, mp_limb_t(0));
    }
}

//! Iterator-based multi-limb two's complement complex-valued fixed-point
//! multiplication. The scratch vector `prod_imm` must have space for
//! atleast `2 + 2 * src1_limbs + 2 * src2_limbs` limbs. The scratch vectors `op1_abs`
//! and `op2_abs` must have space for atleast `src1_limbs` and `src2_limbs`
//! limbs, respectively. No overlap between `prod_imm` and `op[12]_abs` allowed. No
//! overlap between `prod_imm` and `dst` allowed.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void complex_fixed_point_product(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs,
    std::size_t src2_limbs,
    std::size_t dst_limbs,
    RANDOM_ACCESS_ITERATOR_INOUT op1_abs,
    RANDOM_ACCESS_ITERATOR_INOUT op2_abs,
    RANDOM_ACCESS_ITERATOR_INOUT prod_imm
)
{
    /*
     * (a + bi)(c + di) = ac - bd + (bc + ad)i
     * Compute the imaginary part of the result first.
     */

    // b*c
    fixed_point_product(
        src1 + src1_limbs,           // src1 (b)
        src2,                        // src2 (c)
        prod_imm,                    // dst
        src1_limbs,                  // src1_limbs
        src2_limbs,                  // src2_limbs
        src1_limbs + src2_limbs + 1, // dst_limbs
        op1_abs,                     // op1_abs
        op2_abs,                     // op2_abs
        prod_imm                     // prod_abs
    );

    // a*d
    fixed_point_product(
        src1,                                   // src1 (a)
        src2 + src2_limbs,                      // src2 (d)
        prod_imm + src1_limbs + src2_limbs + 1, // dst
        src1_limbs,                             // src1_limbs
        src2_limbs,                             // src2_limbs
        src1_limbs + src2_limbs + 1,            // dst_limbs
        op1_abs,                                // op1_abs
        op2_abs,                                // op2_abs
        prod_imm + src1_limbs + src2_limbs + 1  // prod_abs
    );

    // Copy `src1 (imag)` to `op1_abs`, so we can use `src1` as both a source and
    // destination.
    std::copy_n(src1 + src1_limbs, src1_limbs, op1_abs);

    // bc + ad
    mpn_add_n(
        &*(dst + dst_limbs),                             // dst (imag part)
        &*(prod_imm),                                    // src1 (b*c)
        &*(prod_imm + src1_limbs + src2_limbs + 1),      // src2 (a*d)
        std::min(src1_limbs + src2_limbs + 1, dst_limbs) // limbs
    );

    // b*d
    fixed_point_product(
        op1_abs,                                // src1 (b)
        src2 + src2_limbs,                      // src2 (d)
        prod_imm + src1_limbs + src2_limbs + 1, // dst
        src1_limbs,                             // src1_limbs
        src2_limbs,                             // src2_limbs
        src1_limbs + src2_limbs + 1,            // dst_limbs
        op1_abs,                                // op1_abs
        op2_abs,                                // op2_abs
        prod_imm + src1_limbs + src2_limbs + 1  // prod_abs
    );

    // a*c
    fixed_point_product(
        src1,                        // src1 (a)
        src2,                        // src2 (c)
        prod_imm,                    // dst
        src1_limbs,                  // src1_limbs
        src2_limbs,                  // src2_limbs
        src1_limbs + src2_limbs + 1, // dst limbs
        op1_abs,                     // op1_abs
        op2_abs,                     // op2_abs
        prod_imm                     // prod_abs
    );

    // ac - bd
    mpn_sub_n(
        &*(dst),                                         // dst (real part)
        &*(prod_imm),                                    // src1 (a*c)
        &*(prod_imm + src1_limbs + src2_limbs + 1),      // src2 (b*d)
        std::min(src1_limbs + src2_limbs + 1, dst_limbs) // limbs
    );
}

//! Iterator-based multi-limb two's complement complex-valued fixed-point
//! division. The scratch vector `prod_imm` must have space for
//! atleast `2 + 2 * src1_limbs + 2 * src2_limbs` limbs. The scratch vectors `op1_abs`
//! and `op2_abs` must have space for atleast `src1_limbs` and `src2_limbs`
//! limbs, respectively. No overlap between `prod_imm` and `op[12]_abs` allowed. No
//! overlap between `prod_imm` and `dst` allowed.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void complex_fixed_point_division(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs,
    std::size_t src2_limbs,
    std::size_t dst_limbs,
    std::size_t src2_bits,
    std::size_t div_limbs,
    RANDOM_ACCESS_ITERATOR_INOUT op1_abs,
    RANDOM_ACCESS_ITERATOR_INOUT op2_abs,
    RANDOM_ACCESS_ITERATOR_INOUT prod_imm,
    RANDOM_ACCESS_ITERATOR_INOUT den_imm,
    RANDOM_ACCESS_ITERATOR_INOUT num_imm,
    RANDOM_ACCESS_ITERATOR_INOUT qte_imm
)
{
    //
    //  a + bi        ac + bd       bc - ad
    // --------  =  ----------- + ----------- i
    //  c + di       c^2 + d^2     c^2 + d^2
    //

    /*
     * Pre-compute the denominator: c^2 + d^2
     */
    fixed_point_square(
        src2,           // src (c)
        den_imm,        // dst
        src2_limbs,     // src_limbs
        2 * src2_limbs, // dst_limbs
        op2_abs,        // op_abs
        prod_imm        // prod_abs
    );
    fixed_point_square(
        src2 + src2_limbs, // src (c)
        prod_imm,          // dst
        src2_limbs,        // src_limbs
        2 * src2_limbs,    // dst_limbs
        op2_abs,           // op_abs
        prod_imm           // prod_abs
    );
    mpn_add_n(&den_imm[0], &den_imm[0], &prod_imm[0], 2 * src2_limbs);
    auto den_significant_limbs = significant_limbs(den_imm, den_imm + 2 * src2_limbs);

    /*                                          ac + bd
     * Compute the real part of the quotient: -----------
     *                                         c^2 + d^2
     */
    std::size_t prod_len = src1_limbs + src2_limbs + 1;
    fixed_point_product(
        src1,       // src1 (a)
        src2,       // src2 (c)
        prod_imm,   // dst
        src1_limbs, // src1_limbs
        src2_limbs, // src2_limbs
        prod_len,   // dst_limbs
        op1_abs,    // op1_abs
        op2_abs,    // op1_abs
        prod_imm    // prod_abs
    );
    fixed_point_product(
        src1 + src1_limbs,   // src1 (b)
        src2 + src2_limbs,   // src2 (d)
        prod_imm + prod_len, // dst
        src1_limbs,          // src1_limbs
        src2_limbs,          // src2_limbs
        prod_len,            // dst_limbs
        op1_abs,             // op1_abs
        op2_abs,             // op1_abs
        prod_imm + prod_len  // prod_abs
    );

    mpn_add_n(&prod_imm[0], &prod_imm[0], &prod_imm[0] + prod_len, prod_len);
    bool real_negative = limb_vector_is_negative(prod_imm, prod_imm + prod_len);
    if (real_negative) {
        limb_vector_negate(prod_imm, prod_imm + div_limbs, num_imm);
    } else {
        std::copy(prod_imm, prod_imm + div_limbs, num_imm);
    }
    limb_vector_lsl(num_imm, num_imm + div_limbs, src2_bits);

    mpn_div_qr(
        &qte_imm[0],          // Quotient
        &num_imm[0],          // Numerator
        div_limbs,            // Numerator limbs
        &den_imm[0],          // Denominator
        den_significant_limbs // Denominator significant limbs
    );

    if (real_negative) {
        limb_vector_negate(qte_imm, qte_imm + dst_limbs, dst);
    } else {
        std::copy_n(qte_imm, dst_limbs, dst);
    }

    /*                                               bc - ad
     * Compute the imaginary part of the quotient: -----------
     *                                              c^2 + d^2
     */
    fixed_point_product(
        src1 + src1_limbs, // src1 (b)
        src2,              // src2 (c)
        prod_imm,          // dst
        src1_limbs,        // src1_limbs
        src2_limbs,        // src2_limbs
        prod_len,          // dst_limbs
        op1_abs,           // op1_abs
        op2_abs,           // op1_abs
        prod_imm           // prod_abs
    );
    fixed_point_product(
        src1,                // src1 (a)
        src2 + src2_limbs,   // src2 (d)
        prod_imm + prod_len, // dst
        src1_limbs,          // src1_limbs
        src2_limbs,          // src2_limbs
        prod_len,            // dst_limbs
        op1_abs,             // op1_abs
        op2_abs,             // op1_abs
        prod_imm + prod_len  // prod_abs
    );

    mpn_sub_n(&prod_imm[0], &prod_imm[0], &prod_imm[0] + prod_len, prod_len);
    bool imag_negative = limb_vector_is_negative(prod_imm, prod_imm + prod_len);
    if (imag_negative) {
        limb_vector_negate(prod_imm, prod_imm + div_limbs, num_imm);
    } else {
        std::copy(prod_imm, prod_imm + div_limbs, num_imm);
    }
    limb_vector_lsl(num_imm, num_imm + div_limbs, src2_bits);

    mpn_div_qr(
        &qte_imm[0],          // Quotient
        &num_imm[0],          // Numerator
        div_limbs,            // Numerator limbs
        &den_imm[0],          // Denominator
        den_significant_limbs // Denominator significant limbs
    );

    if (imag_negative) {
        limb_vector_negate(qte_imm, qte_imm + dst_limbs, dst + dst_limbs);
    } else {
        std::copy_n(qte_imm, dst_limbs, dst + dst_limbs);
    }
}

//! Iterator-based multi-limb fixed-point hadamard product
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static void fixed_point_hadamard_product(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs, // Number of limbs in one fixed-point of `src1`
    std::size_t src2_limbs, // Number of limbs in one fixed-point of `src2`
    std::size_t dst_limbs,  // `dst_limbs` <= `src1_limbs` + `src2_limbs`
    std::size_t n_items     // Number of elements to use in inner product
)
{
    ScratchVector<mp_limb_t, 8> op1_abs(src1_limbs);
    ScratchVector<mp_limb_t, 8> op2_abs(src2_limbs);
    ScratchVector<mp_limb_t, 16> prod_abs(src1_limbs + src2_limbs);
    for (std::size_t i = 0; i < n_items; i++) {
        fixed_point_product(
            src1 + i * src1_limbs,
            src2 + i * src2_limbs,
            dst + i * dst_limbs,
            src1_limbs,
            src2_limbs,
            dst_limbs,
            std::begin(op1_abs),
            std::begin(op2_abs),
            std::begin(prod_abs)
        );
    }
}

//! Iterator-based multiply-accumulate
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static void fixed_point_inner_product(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs, // Number of limbs in one fixed-point of `src1`
    std::size_t src2_limbs, // Number of limbs in one fixed-point of `src2`
    std::size_t dst_limbs,  // Number of limbs in the result
    std::size_t n_items     // Number of elements to use in inner product
)
{
    //
    // Specialization #1: the resulting number of limbs is exactly one
    //
    if (dst_limbs == 1) {
        dst[0] = simd::vector_multiply_accumulate(src1, src2, n_items);
        return; // early exit
    }

    //
    // General case. This always works, but is the slowest variant.
    //
    ScratchVector<mp_limb_t, 8> op1_abs(src1_limbs);
    ScratchVector<mp_limb_t, 8> op2_abs(src2_limbs);

    // Absolute product must be long enough to contain a possibly sign extended result
    std::size_t product_limbs = src1_limbs + src2_limbs;
    ScratchVector<mp_limb_t, 16> product(std::max(product_limbs, dst_limbs));

    if (dst_limbs <= product_limbs) {
        /*
         * (1): Multiply-accumulate, no need to sign extend
         */
        for (std::size_t i = 0; i < n_items; i++) {
            fixed_point_product(
                src1 + i * src1_limbs, // src1
                src2 + i * src2_limbs, // src2
                std::begin(product),   // dst
                src1_limbs,            // src1_limbs
                src2_limbs,            // src2_limbs
                product_limbs,         // dst_limbs
                std::begin(op1_abs),   // op1_abs scratch vector
                std::begin(op2_abs),   // op2_abs scratch vector
                std::begin(product)    // product_abs scratch_vector
            );
            mpn_add_n(&dst[0], &dst[0], &product[0], dst_limbs);
        }
    } else { /* dst_limbs > product_limbs */
        /*
         * (2): Multiply-accumulate, but sign extend each product
         */
        for (std::size_t i = 0; i < n_items; i++) {
            fixed_point_product(
                src1 + i * src1_limbs, // src1
                src2 + i * src2_limbs, // src2
                std::begin(product),   // dst
                src1_limbs,            // src1_limbs
                src2_limbs,            // src2_limbs
                product_limbs,         // dst_limbs
                std::begin(op1_abs),   // op1_abs scratch vector
                std::begin(op2_abs),   // op2_abs scratch vector
                std::begin(product)    // product_abs scratch_vector
            );
            if (mp_limb_signed_t(product[product_limbs - 1]) < 0) {
                // Sign-extend
                std::fill(std::begin(product) + product_limbs, std::end(product), -1);
            } else {
                // Zero-extend
                std::fill(std::begin(product) + product_limbs, std::end(product), 0);
            }
            mpn_add_n(&dst[0], &dst[0], &product[0], dst_limbs);
        }
    }
}

//! Iterator-based multiply-accumulate using in accumulator context
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static void fixed_point_inner_product_accumulator(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs, // Number of limbs in one fixed-point of `src1`
    std::size_t src2_limbs, // Number of limbs in one fixed-point of `src2`
    std::size_t dst_limbs,  // Number of limbs in the result
    std::size_t n_items,    // Number of elements to use in inner product
    int product_bits,       // Number of bits in the raw product
    int product_int_bits,   // Number of int_bits in the raw product
    const APyFixedAccumulatorOption& acc
)
{
    ScratchVector<mp_limb_t, 8> op1_abs(src1_limbs);
    ScratchVector<mp_limb_t, 8> op2_abs(src2_limbs);

    // Absolute product must be long enough to contain a possibly sign extended result
    std::size_t product_limbs = src1_limbs + src2_limbs;
    ScratchVector<mp_limb_t, 16> product(std::max(product_limbs, dst_limbs));

    if (dst_limbs <= product_limbs) {
        /*
         * (1): Multiply-accumulate, no need to sign extend
         */
        for (std::size_t i = 0; i < n_items; i++) {
            // Multiply
            fixed_point_product(
                src1 + i * src1_limbs,
                src2 + i * src2_limbs,
                std::begin(product),
                src1_limbs,
                src2_limbs,
                product_limbs,
                std::begin(op1_abs),
                std::begin(op2_abs),
                std::begin(product)
            );

            // Quantize and overflow
            quantize(
                std::begin(product),
                std::end(product),
                product_bits,
                product_int_bits,
                acc.bits,
                acc.int_bits,
                acc.quantization
            );
            overflow(
                std::begin(product),
                std::end(product),
                acc.bits,
                acc.int_bits,
                acc.overflow
            );

            // Accumulate
            mpn_add_n(&dst[0], &dst[0], &product[0], dst_limbs);
        }
    } else { /* dst_limbs > product_limbs */
        /*
         * (2): Multiply-accumulate, but sign extend each product
         */
        for (std::size_t i = 0; i < n_items; i++) {
            // Multiply
            fixed_point_product(
                src1 + i * src1_limbs,
                src2 + i * src2_limbs,
                std::begin(product),
                src1_limbs,
                src2_limbs,
                product_limbs,
                std::begin(op1_abs),
                std::begin(op2_abs),
                std::begin(product)
            );

            if (mp_limb_signed_t(product[product_limbs - 1]) < 0) {
                // Sign-extend
                std::fill(std::begin(product) + product_limbs, std::end(product), -1);
            } else {
                // Zero-extend
                std::fill(std::begin(product) + product_limbs, std::end(product), 0);
            }

            // Quantize and overflow
            quantize(
                std::begin(product),
                std::end(product),
                product_bits,
                product_int_bits,
                acc.bits,
                acc.int_bits,
                acc.quantization
            );
            overflow(
                std::begin(product),
                std::end(product),
                acc.bits,
                acc.int_bits,
                acc.overflow
            );

            // Accumulate
            mpn_add_n(&dst[0], &dst[0], &product[0], dst_limbs);
        }
    }
}

/*!
 * Retrieve an appropriate fixed-point inner product function from `accumulator_mode`.
 * The returned inner product function object will have the accumulator mode bound, if
 * available. Otherwise, the return function object will be a plain inner product
 * function.
 */
template <typename VECTOR_TYPE>
[[maybe_unused]] static std::function<
    void(
        typename VECTOR_TYPE::const_iterator,
        typename VECTOR_TYPE::const_iterator,
        typename VECTOR_TYPE::iterator,
        std::size_t,
        std::size_t,
        std::size_t,
        std::size_t
    )>
inner_product_func_from_acc_mode(
    int product_bits,
    int product_int_bits,
    const std::optional<APyFixedAccumulatorOption>& accumulator_mode
)
{
    using namespace std::placeholders;
    if (accumulator_mode.has_value()) {
        return std::bind(
            fixed_point_inner_product_accumulator<
                typename VECTOR_TYPE::const_iterator,
                typename VECTOR_TYPE::iterator>,
            _1,
            _2,
            _3,
            _4,
            _5,
            _6,
            _7,
            product_bits,
            product_int_bits,
            *accumulator_mode
        );
    } else { /* !accumulator_mode.has_value() */
        return fixed_point_inner_product<
            typename VECTOR_TYPE::const_iterator,
            typename VECTOR_TYPE::iterator>;
    }
}

/* ********************************************************************************** *
 * *                       Fixed-point to and from other types                      * *
 * ********************************************************************************** */

template <typename RANDOM_ACCESS_IT>
void fixed_point_from_double(
    double value,
    RANDOM_ACCESS_IT begin_it,
    RANDOM_ACCESS_IT end_it,
    int bits,
    int int_bits
)
{
    // RANDOM_ACCESS_ITERATOR is `mp_limb_t` iterator (32-bit or 64-bit)
    static_assert(std::is_same_v<
                  mp_limb_t,
                  std::remove_const_t<typename RANDOM_ACCESS_IT::value_type>>);
    static_assert(_LIMB_SIZE_BITS == 64 || _LIMB_SIZE_BITS == 32);

    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }

    // Zero limb vector data
    std::fill(begin_it, end_it, 0);

    int frac_bits = bits - int_bits;
    int exp = exp_of_double(value);
    uint64_t man = man_of_double(value);

    // Left-shift amount needed to align floating binary point with fixed binary point
    int left_shift_amnt = exp + frac_bits - 52 - 1023 + 1;
    if (exp) {
        // Append mantissa hidden one
        man |= uint64_t(1) << 52;
        left_shift_amnt--;
    }

    if constexpr (_LIMB_SIZE_BITS == 64) {

        /*
         * Limb vector size is wide enough to accommodate the full mantissa of a
         * double-prection floating point number
         */

        if (left_shift_amnt >= 0) {
            *begin_it = man;
            limb_vector_lsl(begin_it, end_it, left_shift_amnt);
        } else {
            auto right_shift_amount = -left_shift_amnt;
            if (right_shift_amount < 64) {
                // Round the mantissa value
                man += uint64_t(1) << (right_shift_amount - 1);
            }
            *begin_it = man;
            limb_vector_lsr(begin_it, end_it, right_shift_amount);
        }

    } else { /* _LIMB_SIZE_BITS == 32 */

        /*
         * Limb vector size is *NOT* wide enough to accommodate the full mantissa of a
         * double-prection floating point number
         */

        if (std::distance(begin_it, end_it) == 1) {

            if (left_shift_amnt >= 0) {
                if (left_shift_amnt < 32) {
                    man <<= left_shift_amnt;
                    *(begin_it + 0) = (man & 0xFFFFFFFF);
                }
            } else {
                auto right_shift_amount = -left_shift_amnt;
                if (right_shift_amount < 64) {
                    // Round the mantissa value
                    man += uint64_t(1) << (right_shift_amount - 1);
                    man >>= right_shift_amount;
                    *(begin_it + 0) = (man & 0xFFFFFFFF);
                }
            }

        } else { /* std::distance(begin_it, end_it > 1 */

            if (left_shift_amnt >= 0) {
                *(begin_it + 0) = (man & 0xFFFFFFFF);
                *(begin_it + 1) = (man >> 32);
                limb_vector_lsl(begin_it, end_it, left_shift_amnt);
            } else {
                auto right_shift_amount = -left_shift_amnt;
                if (right_shift_amount < 64) {
                    // Round the mantissa value
                    man += uint64_t(1) << (right_shift_amount - 1);
                }
                *(begin_it + 0) = (man & 0xFFFFFFFF);
                *(begin_it + 1) = (man >> 32);
                limb_vector_lsr(begin_it, end_it, right_shift_amount);
            }
        }
    }

    // Adjust result from sign
    if (sign_of_double(value)) {
        limb_vector_negate(begin_it, end_it, begin_it);
    }
    _overflow_twos_complement(begin_it, end_it, bits, int_bits);
}

template <typename RANDOM_ACCESS_IT>
double fixed_point_to_double(
    RANDOM_ACCESS_IT begin_it, RANDOM_ACCESS_IT end_it, int bits, int int_bits
)
{
    // RANDOM_ACCESS_ITERATOR is `mp_limb_t` iterator (32-bit or 64-bit)
    static_assert(std::is_same_v<
                  mp_limb_t,
                  std::remove_const_t<typename RANDOM_ACCESS_IT::value_type>>);
    static_assert(_LIMB_SIZE_BITS == 64 || _LIMB_SIZE_BITS == 32);

    if (limb_vector_is_zero(begin_it, end_it)) {
        return 0.0;
    }

    uint64_t man {};
    int exp {};
    bool sign = limb_vector_is_negative(begin_it, end_it);
    int frac_bits = bits - int_bits;

    if constexpr (_LIMB_SIZE_BITS == 64) {

        ScratchVector<mp_limb_t, 8> man_vec(std::distance(begin_it, end_it));
        limb_vector_abs(begin_it, end_it, std::begin(man_vec));
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Compute the shift amount and exponent value
        int left_shift_n = 53 - _LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
        exp = 1023 + 52 - left_shift_n - frac_bits;
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

        std::size_t n_limbs = std::distance(begin_it, end_it);
        ScratchVector<mp_limb_t, 8> man_vec(std::max(n_limbs, std::size_t(2)));
        if (n_limbs == 1) {
            man_vec[0] = *begin_it;
            man_vec[1] = mp_limb_signed_t(*begin_it) < 0 ? -1 : 0;
        } else {
            std::copy(begin_it, end_it, man_vec.begin());
        }
        limb_vector_abs(man_vec.cbegin(), man_vec.cend(), man_vec.begin());
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Compute the shift amount and exponent value
        int left_shift_n = 53 - _LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
        exp = 1023 + 52 - left_shift_n - frac_bits;
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

template <typename RANDOM_ACCESS_IT>
std::string fixed_point_to_string_dec(
    RANDOM_ACCESS_IT begin_it, RANDOM_ACCESS_IT end_it, int bits, int int_bits
)
{
    // Construct a string from the absolute value of number, and conditionally append a
    // minus sign to the string if negative
    std::vector<mp_limb_t> abs_val(std::distance(begin_it, end_it));
    bool is_negative = limb_vector_abs(begin_it, end_it, std::begin(abs_val));

    // Convert this number to BCD with the double-dabble algorithm
    std::vector<mp_limb_t> bcd_limb_list = double_dabble(abs_val);
    std::size_t bcd_limb_list_start_size = bcd_limb_list.size();

    // Divide BCD limb list by two, one time per fractional bit (if any)
    long decimal_point = 0;
    const int frac_bits = bits - int_bits;
    for (int i = 0; i < frac_bits; i++) {
        bcd_limb_vec_div2(bcd_limb_list);
        decimal_point += bcd_limb_list.size() > bcd_limb_list_start_size ? 1 : 0;
    }
    long rjust = ((_LIMB_SIZE_BITS / 4) - decimal_point) % (_LIMB_SIZE_BITS / 4);

    // Multiply BCD list by two, one time per for each missing fractional bit (if any)
    for (int i = 0; i < -frac_bits; i++) {
        bcd_limb_vec_mul2(bcd_limb_list);
    }

    // Convert BCD limb list to regular BCD list (`std::vector<uint8_t>`)
    auto bcd_list = to_nibble_list(bcd_limb_list, decimal_point + rjust + 1);

    // Convert BCDs to ASCII
    std::string result = is_negative ? "-" : "";
    for (long i = bcd_list.size() - 1; i >= rjust; i--) {
        result.push_back(bcd_list[i] + 0x30);
        if (decimal_point && i == rjust + long(decimal_point)) {
            result.push_back('.');
        }
    }

    return result;
}

/* ********************************************************************************** *
 * *                        Fixed-point folding functions                           * *
 * ********************************************************************************** */

//! Retrieve an accumulative fixed-point fold function
template <typename VECTOR_TYPE>
static APY_INLINE std::function<
    void(typename VECTOR_TYPE::iterator, typename VECTOR_TYPE::const_iterator)>
fold_accumulate(std::size_t src_limbs, std::size_t acc_limbs)
{
    if (acc_limbs <= 1) {
        /* single limb specialization */
        return [](auto acc_it, auto src_it) { *acc_it += *src_it; };
    } else {
        return [src_limbs, acc_limbs](auto acc_it, auto src_it) {
            mpn_add(&acc_it[0], &acc_it[0], acc_limbs, &src_it[0], src_limbs);
        };
    }
}

//! Retrieve a multiplicative fold function
template <typename VECTOR_TYPE, typename SCRATCH_VEC>
static APY_INLINE std::function<
    void(typename VECTOR_TYPE::iterator, typename VECTOR_TYPE::const_iterator)>
fold_multiply(std::size_t src_limbs, std::size_t acc_limbs, SCRATCH_VEC& scratch)
{
    if (acc_limbs <= 1) {
        /* single limb specialization */
        return [](auto acc_it, auto src_it) { *acc_it *= *src_it; };
    } else {
        return [src_limbs, acc_limbs, &scratch](auto acc_it, auto src_it) {
            auto op1_abs = std::begin(scratch);
            auto op2_abs = op1_abs + acc_limbs;
            auto prod_abs = op2_abs + src_limbs;
            fixed_point_product(
                acc_it,    // src1
                src_it,    // src2
                acc_it,    // dst
                acc_limbs, // src1_limbs
                src_limbs, // src2_limbs
                acc_limbs, // dst_limbs
                op1_abs,   // op1_abs
                op2_abs,   // op2_abs
                prod_abs   // prod_abs
            );
        };
    }
}

//! Retrieve a complex-valued accumulative fixed-point fold function
template <typename VECTOR_TYPE>
static APY_INLINE std::function<
    void(typename VECTOR_TYPE::iterator, typename VECTOR_TYPE::const_iterator)>
fold_complex_accumulate(std::size_t src_limbs, std::size_t acc_limbs)
{
    if (acc_limbs <= 1) {
        /* single limb (one real, one imag) specialization */
        return [](auto acc_it, auto src_it) {
            *(acc_it + 0) += *(src_it + 0); // real part
            *(acc_it + 1) += *(src_it + 1); // imag part
        };
    } else {
        return [src_limbs, acc_limbs](auto acc_it, auto src_it) {
            std::size_t src_j = src_limbs;
            std::size_t acc_j = acc_limbs;
            mpn_add(&acc_it[0], &acc_it[0], acc_limbs, &src_it[0], src_limbs);
            mpn_add(&acc_it[acc_j], &acc_it[acc_j], acc_j, &src_it[src_j], src_j);
        };
    }
}

//! Retrieve a complex-valued multiplicative fold function
template <typename VECTOR_TYPE, typename SCRATCH_VEC>
static APY_INLINE std::function<
    void(typename VECTOR_TYPE::iterator, typename VECTOR_TYPE::const_iterator)>
fold_complex_multiply(
    std::size_t src_limbs, std::size_t acc_limbs, SCRATCH_VEC& scratch
)
{
    if (acc_limbs <= 1) {
        /* single limb specialization */
        return [](auto acc_it, auto src_it) {
            mp_limb_signed_t acc_real = acc_it[0];
            mp_limb_signed_t acc_imag = acc_it[1];
            mp_limb_signed_t src_real = src_it[0];
            mp_limb_signed_t src_imag = src_it[1];
            acc_it[0] = acc_real * src_real - acc_imag * src_imag;
            acc_it[1] = acc_imag * src_real + acc_real * src_imag;
        };
    } else {
        return [src_limbs, acc_limbs, &scratch](auto acc_it, auto src_it) {
            auto op1_abs = std::begin(scratch);
            auto op2_abs = op1_abs + acc_limbs;
            auto prod_imm = op2_abs + src_limbs;
            complex_fixed_point_product(
                acc_it,    // src1
                src_it,    // src2
                acc_it,    // dst
                acc_limbs, // src1_limbs
                src_limbs, // src2_limbs
                acc_limbs, // dst_limbs
                op1_abs,   // op1_abs
                op2_abs,   // op2_abs
                prod_imm   // prod_imm
            );
        };
    }
}

#endif // _APYFIXED_UTIL_H
