/*
 * APyFixed utility functions
 */

#ifndef _APYFIXED_UTIL_H
#define _APYFIXED_UTIL_H

#include "apytypes_common.h"
#include "apytypes_fwd.h"
#include "apytypes_intrinsics.h"
#include "apytypes_mp.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

#include <algorithm>
#include <cassert>
#include <cstdint>    // std::int64_t
#include <functional> // std::bind, std::function, std::placeholders
#include <iterator>   // std::begin
#include <numeric>
#include <optional> // std::optional

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
                apy_limb_t add_one = limb_vector_or_reduce(it_begin, it_end, bits);
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
                apy_limb_t add_one = limb_vector_or_reduce(it_begin, it_end, bits);
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

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_stoch_equal(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits,
    std::function<std::uint64_t()> get_random_uint64
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    auto left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        /*
         * Proper 50/50 Bernoulli distribution if `GET_RANDOM_UINT64()` is uniformally
         * distributed with high entropy.
         */
        limb_vector_asr(it_begin, it_end, -left_shift_amnt);
        if (get_random_uint64() % 2) {
            limb_vector_add_pow2(it_begin, it_end, 0);
        }
    }
}

template <class RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void _quantize_stoch_weighted(
    RANDOM_ACCESS_ITERATOR_INOUT it_begin,
    RANDOM_ACCESS_ITERATOR_INOUT it_end,
    int bits,
    int int_bits,
    int new_bits,
    int new_int_bits,
    std::function<std::uint64_t()> get_random_uint64
)
{
    int frac_bits = bits - int_bits;
    int new_frac_bits = new_bits - new_int_bits;
    int left_shift_amnt = new_frac_bits - frac_bits;
    if (left_shift_amnt >= 0) {
        limb_vector_lsl(it_begin, it_end, left_shift_amnt);
    } else {
        std::size_t src_nlimbs = std::distance(it_begin, it_end);
        unsigned bits_to_qntz = unsigned(-left_shift_amnt);
        unsigned limbs_to_qntz = (bits_to_qntz - 1) / APY_LIMB_SIZE_BITS + 1;
        unsigned qntz_bit_idx = bits_to_qntz % APY_LIMB_SIZE_BITS;
        if (limbs_to_qntz == 1) {
            // Fast path, only only limb to quantize
            apy_limb_t rnd_word = get_random_uint64();
            if (qntz_bit_idx) {
                rnd_word &= (apy_limb_t(1) << qntz_bit_idx) - 1;
            }
            apy_inplace_addition_single_limb(&*it_begin, src_nlimbs, rnd_word);
            limb_vector_asr(it_begin, it_end, bits_to_qntz);
        } else {
            // General path, can quantize infinitely many limbs
            assert(limbs_to_qntz > 1);
            ScratchVector<apy_limb_t> rnd_words(limbs_to_qntz);
            for (auto&& rnd_word : rnd_words) {
                rnd_word = get_random_uint64();
            }
            if (qntz_bit_idx) {
                rnd_words.back() &= (apy_limb_t(1) << qntz_bit_idx) - 1;
            }
            apy_inplace_addition(&*it_begin, src_nlimbs, &rnd_words[0], limbs_to_qntz);
            limb_vector_asr(it_begin, it_end, bits_to_qntz);
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
    QuantizationMode quantization,
    std::optional<std::function<std::uint64_t()>> get_random_uint64 = std::nullopt
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
    case QuantizationMode::STOCH_EQUAL:
        _quantize_stoch_equal(
            it_begin, it_end, bits, int_bits, new_bits, new_int_bits, *get_random_uint64
        );
        break;
    case QuantizationMode::STOCH_WEIGHTED:
        _quantize_stoch_weighted(
            it_begin, it_end, bits, int_bits, new_bits, new_int_bits, *get_random_uint64
        );
        break;
    default:
        APYTYPES_UNREACHABLE();
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
    (void)it_begin;
    if (bits % APY_LIMB_SIZE_BITS) {
        RANDOM_ACCESS_ITERATOR ms_limb_it = std::prev(it_end);
        unsigned shift_amount = APY_LIMB_SIZE_BITS - (bits % APY_LIMB_SIZE_BITS);
        *ms_limb_it = apy_limb_signed_t(*ms_limb_it << shift_amount) >> shift_amount;
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
    std::size_t utilized_bits_last_limb = (bits - 1) % APY_LIMB_SIZE_BITS + 1;

    bool sign = apy_limb_signed_t(*std::prev(it_end)) < 0;
    if (sign) {
        if (!limb_vector_all_ones(ms_limb_it, it_end, utilized_bits_last_limb - 1)) {
            std::fill(it_begin, ms_limb_it, 0);
            *ms_limb_it = ~((apy_limb_t(1) << (utilized_bits_last_limb - 1)) - 1);
        }
    } else { /* !sign */
        if (!limb_vector_all_zeros(ms_limb_it, it_end, utilized_bits_last_limb - 1)) {
            std::fill(it_begin, ms_limb_it, apy_limb_t(-1));
            *ms_limb_it = (apy_limb_t(1) << (utilized_bits_last_limb - 1)) - 1;
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
    std::size_t utilized_bits_last_limb = (bits - 1) % APY_LIMB_SIZE_BITS + 1;

    bool sign = apy_limb_signed_t(*std::prev(it_end)) < 0;
    if (sign) {
        // Force a `1` into the sign position (and above)
        *ms_limb_it |= ~((apy_limb_t(1) << (utilized_bits_last_limb - 1)) - 1);
    } else { /* !sign */
        // Force a `0` into the sign position (and above)
        *ms_limb_it &= ((apy_limb_t(1) << (utilized_bits_last_limb - 1)) - 1);
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
        throw NotImplementedException(
            fmt::format(
                "Not implemented: APyFixed::_overflow(): with mode: {}",
                "unknown (did you pass `int` as `OverflowMode`?)"
            )
        );
    }
}

/* ********************************************************************************** *
 * *    Fixed-point iterator based casting with quantization and overflowing        * *
 * ********************************************************************************** */

/*!
 * General casting method for fixed-point numbers. General casting can perform both
 * quantization and overflowing. The size of the output region
 * (`std::distance(dst_begin, dst_end)`) must be greater than or equal to the size of
 * the input region (`std::distance(src_begin, src_end)`), even when the output
 * bit-specifiers are smaller then the input bit-specifiers (hence the name unsafe).
 */
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static APY_INLINE void fixed_point_cast_unsafe(
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
    assert(std::distance(src_begin, src_end) <= std::distance(dst_begin, dst_end));

    // Copy data into the result region and sign extend
    limb_vector_copy_sign_extend(src_begin, src_end, dst_begin, dst_end);

    // First perform quantization
    quantize(
        dst_begin,
        dst_end,
        src_bits,
        src_int_bits,
        dst_bits,
        dst_int_bits,
        q_mode,
        rnd64_fx
    );

    // Then perform overflowing
    overflow(dst_begin, dst_end, dst_bits, dst_int_bits, v_mode);
}

template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
static APY_INLINE void fixed_point_cast(
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
    const auto FX_CAST_UNSAFE = [&](auto begin, auto end, auto d_begin, auto d_end) {
        fixed_point_cast_unsafe(
            begin,
            end,
            d_begin,
            d_end,
            src_bits,
            src_int_bits,
            dst_bits,
            dst_int_bits,
            q_mode,
            v_mode
        );
    };

    if (std::distance(src_begin, src_end) <= std::distance(dst_begin, dst_end)) {
        FX_CAST_UNSAFE(src_begin, src_end, dst_begin, dst_end);
    } else {
        ScratchVector<apy_limb_t, 8> res(std::distance(src_begin, src_end));
        FX_CAST_UNSAFE(src_begin, src_end, std::begin(res), std::end(res));
        std::copy_n(std::begin(res), bits_to_limbs(dst_bits), dst_begin);
    }
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
                unsigned limb_skip = left_shift_amount / APY_LIMB_SIZE_BITS;
                unsigned limb_shift = left_shift_amount % APY_LIMB_SIZE_BITS;
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
//! vector `prod_abs` must have space for at least `src1_limbs + src2_limbs` limbs. The
//! scratch vectors `op1_abs` and `op2_abs` must have space for at least `src1_limbs`
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
    // Retrieve the absolute value of both operands
    bool sign1 = limb_vector_abs(src1, src1 + src1_limbs, op1_abs);
    bool sign2 = limb_vector_abs(src2, src2 + src2_limbs, op2_abs);

    // Resulting sign
    bool result_sign = sign1 ^ sign2;

    // Perform the multiplication of absolute values
    if (src1_limbs < src2_limbs) {
        apy_unsigned_multiplication(
            &prod_abs[0], &op2_abs[0], src2_limbs, &op1_abs[0], src1_limbs
        );
    } else {
        apy_unsigned_multiplication(
            &prod_abs[0], &op1_abs[0], src1_limbs, &op2_abs[0], src2_limbs
        );
    }

    // Possibly negate and copy the result back
    std::size_t prod_limbs = src1_limbs + src2_limbs;
    if (result_sign) {
        if (dst_limbs <= prod_limbs) {
            limb_vector_negate(prod_abs, prod_abs + dst_limbs, dst);
        } else {
            bool is_zero = limb_vector_negate(prod_abs, prod_abs + prod_limbs, dst);
            apy_limb_t fill_val = is_zero ? apy_limb_t(0) : apy_limb_t(-1);
            std::fill_n(prod_abs + prod_limbs, dst_limbs - prod_limbs, fill_val);
        }
    } else {
        if (dst_limbs <= prod_limbs) {
            std::copy(prod_abs, prod_abs + dst_limbs, dst);
        } else {
            std::copy(prod_abs, prod_abs + prod_limbs, dst);
            std::fill_n(prod_abs + prod_limbs, dst_limbs - prod_limbs, apy_limb_t(0));
        }
    }
}

//! Iterator-based multi-limb two's complement fixed-point squaring. The scratch
//! vector `prod_abs` must have space for at least `2 * src_limbs` limbs. The scratch
//! vector `op_abs` must have space for at least `src_limbs` limbs. No overlap
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
    apy_unsigned_square(&*prod_abs, &*op_abs, src_limbs);
    if (dst_limbs <= 2 * src_limbs) {
        std::copy_n(prod_abs, dst_limbs, dst);
    } else {
        std::copy_n(prod_abs, 2 * src_limbs, dst);
        std::fill_n(prod_abs + 2 * src_limbs, dst_limbs - 2 * src_limbs, apy_limb_t(0));
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
    ScratchVector<apy_limb_t, 8> op1_abs(src1_limbs);
    ScratchVector<apy_limb_t, 8> op2_abs(src2_limbs);
    ScratchVector<apy_limb_t, 16> prod_abs(src1_limbs + src2_limbs);
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

struct FixedPointInnerProduct {
    explicit FixedPointInnerProduct(
        const APyFixedSpec& src1_spec,
        const APyFixedSpec& src2_spec,
        const APyFixedSpec& dst_spec,
        const std::optional<APyFixedAccumulatorOption> acc_mode
    )
        : src1_limbs { bits_to_limbs(src1_spec.bits) }
        , src2_limbs { bits_to_limbs(src2_spec.bits) }
        , dst_limbs { bits_to_limbs(dst_spec.bits) }
        , acc_mode { acc_mode }
    {
        if (!acc_mode.has_value()) {
            if (dst_limbs == 1) {
                // Specialization #1: the resulting number of limbs is exactly one and
                // no accumulator mode has been set. Use the SIMD inner product.
                assert(src1_limbs == 1);
                assert(src2_limbs == 1);
                f = &FixedPointInnerProduct::inner_product_simd;
                return;
            } else if (dst_limbs == 2 && src1_limbs == 1 && src2_limbs == 1) {
                // Special case #2: resulting elements fit into two limbs, and
                // each argument element fits into a single limb.
                f = &FixedPointInnerProduct::inner_product_one_limb_src_two_limb_dst;
                return;
            }
        }

        // Initialize scratch vectors
        product_limbs = src1_limbs + src2_limbs;
        product = ScratchVector<apy_limb_t, 16>(std::max(product_limbs, dst_limbs));
        op1_abs = ScratchVector<apy_limb_t, 8>(src1_limbs);
        op2_abs = ScratchVector<apy_limb_t, 8>(src2_limbs);

        if (acc_mode.has_value()) {
            // Accumulator mode set, use the accumulator inner product
            assert(acc_mode->bits == dst_spec.bits);
            assert(acc_mode->int_bits == dst_spec.int_bits);
            product_bits = src1_spec.bits + src2_spec.bits;
            product_int_bits = src1_spec.int_bits + src2_spec.int_bits;
            if (dst_limbs <= src1_limbs + src2_limbs) {
                f = &FixedPointInnerProduct::inner_product<true, false>;
            } else {
                f = &FixedPointInnerProduct::inner_product<true, true>;
            }
        } else {
            if (dst_limbs <= product_limbs) {
                f = &FixedPointInnerProduct::inner_product<false, false>;
            } else {
                f = &FixedPointInnerProduct::inner_product<false, true>;
            }
        }
    }

    using It = APyBuffer<apy_limb_t>::vector_type::iterator;
    using CIt = APyBuffer<apy_limb_t>::vector_type::const_iterator;

    void operator()(
        CIt src1,
        CIt src2,
        It dst,
        std::size_t N,
        std::size_t M = 1,
        std::size_t DST_STEP = 1
    ) const
    {
        // Matrix-vector multiplication $`A \times b`$, where
        // * A (src1): [ `M` x `N` ]
        // * b (src2): [ `N` x `1` ]
        std::invoke(f, this, src1, src2, dst, N, M, DST_STEP);
    }

private:
    void inner_product_simd(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const
    {
        assert(src1_limbs == 1);
        assert(src2_limbs == 1);
        assert(dst_limbs == 1);
        for (std::size_t m = 0; m < M; m++) {
            dst[m * DST_STEP] = simd::vector_multiply_accumulate(src1 + m * N, src2, N);
        }
    }

    void inner_product_one_limb_src_two_limb_dst(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const
    {
        assert(src1_limbs == 1);
        assert(src2_limbs == 1);
        assert(dst_limbs == 2);
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + src1_limbs * N * m;
#if (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
            /*
             * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++
             * compiler (ICC).
             */
            __int128 acc = 0;
            for (std::size_t n = 0; n < N; n++) {
                __int128 a = apy_limb_signed_t(A_it[n]);
                __int128 b = apy_limb_signed_t(src2[n]);
                acc += a * b;
            }
            dst[2 * m * DST_STEP + 0] = apy_limb_t(acc >> 0);
            dst[2 * m * DST_STEP + 1] = apy_limb_t(acc >> 64);
#else
            /*
             * Microsoft Visual C/C++ compiler (or other unknown compiler)
             */
            auto acc = dst + 2 * m * DST_STEP;
            std::fill_n(acc, 2, 0);
            for (std::size_t n = 0; n < N; n++) {
                auto&& [prod_hi, prod_lo] = long_signed_mult(A_it[n], src2[n]);
                acc[0] += prod_lo;
                acc[1] += prod_hi + (acc[0] < prod_lo);
            }
#endif
#elif (COMPILER_LIMB_SIZE == 32)
            std::int64_t acc = 0;
            for (std::size_t n = 0; n < N; n++) {
                std::int64_t a = apy_limb_signed_t(A_it[n]);
                std::int64_t b = apy_limb_signed_t(src2[n]);
                acc += a * b;
            }
            dst[2 * m * DST_STEP + 0] = apy_limb_t(acc >> 0);
            dst[2 * m * DST_STEP + 1] = apy_limb_t(acc >> 32);
#endif
        }
    }

    template <bool is_acc_context = false, bool support_sign_ext = false>
    void inner_product(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const
    {
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + src1_limbs * N * m;
            auto acc = dst + m * dst_limbs * DST_STEP;
            std::fill_n(acc, dst_limbs, 0);
            for (std::size_t n = 0; n < N; n++) {
                fixed_point_product(
                    A_it + n * src1_limbs, // src1
                    src2 + n * src2_limbs, // src2
                    std::begin(product),   // dst
                    src1_limbs,            // src1_limbs
                    src2_limbs,            // src2_limbs
                    product_limbs,         // dst_limbs
                    std::begin(op1_abs),   // op1_abs scratch vector
                    std::begin(op2_abs),   // op2_abs scratch vector
                    std::begin(product)    // product_abs scratch_vector
                );

                // *Constexpr*: conditionally sign extend product. This is needed when
                // the resulting accumulation adds one or more additional limbs compared
                // to the product, as adding fixed-point numbers increases integer bits.
                if constexpr (support_sign_ext) {
                    apy_limb_t msb_limb = product[product_limbs - 1];
                    apy_limb_t limb_ext = apy_limb_signed_t(msb_limb) < 0 ? -1 : 0;
                    auto&& start_it = std::begin(product) + product_limbs;
                    std::fill(start_it, std::end(product), limb_ext);
                }

                // *Constexpr*: conditionally quantize and overflow the product if this
                // inner product takes place within an accumulator context.
                if constexpr (is_acc_context) {
                    quantize(
                        std::begin(product),
                        std::end(product),
                        product_bits,
                        product_int_bits,
                        acc_mode->bits,
                        acc_mode->int_bits,
                        acc_mode->quantization
                    );
                    overflow(
                        std::begin(product),
                        std::end(product),
                        acc_mode->bits,
                        acc_mode->int_bits,
                        acc_mode->overflow
                    );
                }

                // Accumulate the data
                apy_inplace_addition_same_length(&*acc, &product[0], dst_limbs);
            }
        }
    }

    // Pointer `f` to the correct function based on the limb lengths
    void (FixedPointInnerProduct::*f)(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const;

    std::size_t src1_limbs, src2_limbs, dst_limbs, product_limbs;
    std::optional<APyFixedAccumulatorOption> acc_mode;
    int product_bits, product_int_bits;
    mutable ScratchVector<apy_limb_t, 8> op1_abs;
    mutable ScratchVector<apy_limb_t, 8> op2_abs;
    mutable ScratchVector<apy_limb_t, 16> product;
};

/* ********************************************************************************** *
 * *                       Fixed-point to and from other types                      * *
 * ********************************************************************************** */

[[maybe_unused]] static APY_INLINE apy_limb_t
fixed_point_from_double_single_limb(double value, int frac_bits, unsigned shift_amount)
{
    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }

    int exp = exp_of_double(value);
    std::uint64_t man = man_of_double(value);

    // Left-shift amount needed to align floating binary point with fixed binary point
    int left_shift_amnt = exp + frac_bits - 52 - 1023 + 1;
    if (exp) {
        // Append mantissa hidden one
        man |= std::uint64_t(1) << 52;
        left_shift_amnt--;
    }

    if (left_shift_amnt >= 0) {
        if (left_shift_amnt < 64) {
            man <<= left_shift_amnt;
        } else {
            man = 0;
        }
    } else {
        unsigned right_shift_amount = -left_shift_amnt;
        if (right_shift_amount < 64) {
            // Round the mantissa value
            man += std::uint64_t(1) << (right_shift_amount - 1);
            man >>= right_shift_amount;
        } else {
            man = 0;
        }
    }

    if (sign_of_double(value)) {
        man = -man;
    }

    // Two's complement overflow
    man = apy_limb_t(std::int64_t(man << shift_amount) >> shift_amount);
    return man;
}

template <typename RANDOM_ACCESS_IT>
void fixed_point_from_double(
    double value,
    RANDOM_ACCESS_IT begin_it,
    RANDOM_ACCESS_IT end_it,
    int bits,
    int int_bits
)
{
    // RANDOM_ACCESS_ITERATOR is `apy_limb_t` iterator (32-bit or 64-bit)
    using IT_VALUE_TYPE = typename std::remove_const_t<RANDOM_ACCESS_IT>::value_type;
    static_assert(std::is_same_v<apy_limb_t, IT_VALUE_TYPE>);
    static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);

    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }

    // Zero limb vector data
    std::fill(begin_it, end_it, 0);

    int frac_bits = bits - int_bits;
    int exp = exp_of_double(value);
    std::uint64_t man = man_of_double(value);

    // Left-shift amount needed to align floating binary point with fixed binary point
    int left_shift_amnt = exp + frac_bits - 52 - 1023 + 1;
    if (exp) {
        // Append mantissa hidden one
        man |= std::uint64_t(1) << 52;
        left_shift_amnt--;
    }

    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        /*
         * Limb vector size is wide enough to accommodate the full mantissa of a
         * double-precision floating point number
         */
        if (left_shift_amnt >= 0) {
            *begin_it = man;
            limb_vector_lsl(begin_it, end_it, left_shift_amnt);
        } else {
            auto right_shift_amount = -left_shift_amnt;
            if (right_shift_amount < 64) {
                // Round the mantissa value
                man += std::uint64_t(1) << (right_shift_amount - 1);
            }
            *begin_it = man;
            limb_vector_lsr(begin_it, end_it, right_shift_amount);
        }

    } else { /* APY_LIMB_SIZE_BITS == 32 */
        /*
         * Limb vector size is *NOT* wide enough to accommodate the full mantissa of a
         * double-precision floating point number.
         */
        assert(std::distance(begin_it, end_it) >= 2);
        if (left_shift_amnt >= 0) {
            *(begin_it + 0) = (man & 0xFFFFFFFF);
            *(begin_it + 1) = (man >> 32);
            limb_vector_lsl(begin_it, end_it, left_shift_amnt);
        } else {
            auto right_shift_amount = -left_shift_amnt;
            if (right_shift_amount < 64) {
                // Round the mantissa value
                man += std::uint64_t(1) << (right_shift_amount - 1);
            }
            *(begin_it + 0) = (man & 0xFFFFFFFF);
            *(begin_it + 1) = (man >> 32);
            limb_vector_lsr(begin_it, end_it, right_shift_amount);
        }
    }

    // Adjust result from sign
    if (sign_of_double(value)) {
        limb_vector_negate_inplace(begin_it, end_it);
    }
    _overflow_twos_complement(begin_it, end_it, bits, int_bits);
}

template <typename RANDOM_ACCESS_IT>
double
fixed_point_to_double(RANDOM_ACCESS_IT begin_it, RANDOM_ACCESS_IT end_it, int frac_bits)
{
    // RANDOM_ACCESS_ITERATOR is `apy_limb_t` iterator (32-bit or 64-bit)
    static_assert(std::is_same_v<
                  apy_limb_t,
                  std::remove_const_t<typename RANDOM_ACCESS_IT::value_type>>);
    static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);

    if (limb_vector_is_zero(begin_it, end_it)) {
        return 0.0;
    }

    std::uint64_t man {};
    int exp {};
    bool sign = limb_vector_is_negative(begin_it, end_it);

    if constexpr (APY_LIMB_SIZE_BITS == 64) {

        ScratchVector<apy_limb_t, 8> man_vec(std::distance(begin_it, end_it));
        limb_vector_abs(begin_it, end_it, std::begin(man_vec));
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Compute the shift amount and exponent value
        int left_shift_n = 53 - APY_LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
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

            int pow2 = right_shift_n + 53;
            if (limb_vector_gte_pow2(std::begin(man_vec), std::end(man_vec), pow2)) {
                exp++;
                limb_vector_lsr(std::begin(man_vec), std::end(man_vec), rnd_pow2);
            } else {
                limb_vector_lsr(std::begin(man_vec), std::end(man_vec), right_shift_n);
            }
        }
        man = man_vec[0];

    } else { /* APY_LIMB_SIZE_BITS == 32 */

        std::size_t n_limbs = std::distance(begin_it, end_it);
        ScratchVector<apy_limb_t, 8> man_vec(std::max(n_limbs, std::size_t(2)));
        if (n_limbs == 1) {
            man_vec[0] = *begin_it;
            man_vec[1] = apy_limb_signed_t(*begin_it) < 0 ? -1 : 0;
        } else {
            std::copy(begin_it, end_it, man_vec.begin());
        }
        limb_vector_abs(man_vec.cbegin(), man_vec.cend(), man_vec.begin());
        unsigned man_leading_zeros
            = limb_vector_leading_zeros(man_vec.begin(), man_vec.end());

        // Compute the shift amount and exponent value
        int left_shift_n = 53 - APY_LIMB_SIZE_BITS * man_vec.size() + man_leading_zeros;
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

            int pow2 = right_shift_n + 53;
            if (limb_vector_gte_pow2(std::begin(man_vec), std::end(man_vec), pow2)) {
                exp++;
                limb_vector_lsr(std::begin(man_vec), std::end(man_vec), rnd_pow2);
            } else {
                limb_vector_lsr(std::begin(man_vec), std::end(man_vec), right_shift_n);
            }
        }

        man = std::uint64_t(man_vec[0]);
        man |= std::uint64_t(man_vec[1]) << 32;
    }

    // Check for overflow to infinity
    if (exp >= 2047) {
        exp = 2047;
        man = 0;
    }

    // Return the result
    double result {};
    set_sign_of_double(result, sign);
    set_exp_of_double(result, exp);
    set_man_of_double(result, man);
    return result;
}

template <typename RANDOM_ACCESS_IT>
void fixed_point_from_py_integer(
    const nb::int_& value,
    RANDOM_ACCESS_IT begin_it,
    RANDOM_ACCESS_IT end_it,
    int bits,
    int int_bits
)
{
    // Zero limb vector data
    std::fill(begin_it, end_it, 0);

    const auto int_limb_vec
        = python_long_to_limb_vec(value, std::distance(begin_it, end_it));
    std::copy(int_limb_vec.begin(), int_limb_vec.end(), begin_it);

    // Adjust the number
    const int frac_bits = bits - int_bits;
    if (frac_bits > 0) { // frac_bits > 0
        limb_vector_lsl(begin_it, end_it, frac_bits);
    } else {
        limb_vector_asr(begin_it, end_it, -frac_bits);
    }

    _overflow_twos_complement(begin_it, end_it, bits, int_bits);
}

template <typename RANDOM_ACCESS_IT>
void fixed_point_from_integer(
    const std::uint64_t value,
    RANDOM_ACCESS_IT begin_it,
    RANDOM_ACCESS_IT end_it,
    int bits,
    int int_bits
)
{
    // Zero limb vector data
    std::fill(begin_it, end_it, 0);

    std::vector<apy_limb_t> int_limb_vec = { UINT64_TO_LIMB(value) };
    const auto n = std::min(
        std::distance(std::begin(int_limb_vec), std::end(int_limb_vec)),
        std::distance(begin_it, end_it)
    );
    std::copy_n(std::begin(int_limb_vec), n, begin_it);

    // Adjust the number
    const int frac_bits = bits - int_bits;
    if (frac_bits > 0) {
        limb_vector_lsl(begin_it, end_it, frac_bits);
    } else {
        limb_vector_asr(begin_it, end_it, -frac_bits);
    }

    _overflow_twos_complement(begin_it, end_it, bits, int_bits);
}

template <typename RANDOM_ACCESS_IT>
std::string fixed_point_to_string_dec(
    RANDOM_ACCESS_IT begin_it, RANDOM_ACCESS_IT end_it, int bits, int int_bits
)
{
    // Construct a string from the absolute value of number, and conditionally append a
    // minus sign to the string if negative
    std::vector<apy_limb_t> abs_val(std::distance(begin_it, end_it));
    bool is_negative = limb_vector_abs(begin_it, end_it, std::begin(abs_val));

    // Convert this number to BCD with the double-dabble algorithm
    std::vector<apy_limb_t> bcd_limb_list = double_dabble(abs_val);
    std::size_t bcd_limb_list_start_size = bcd_limb_list.size();

    // Divide BCD limb list by two, one time per fractional bit (if any)
    long decimal_point = 0;
    const int frac_bits = bits - int_bits;
    for (int i = 0; i < frac_bits; i++) {
        bcd_limb_vec_div2(bcd_limb_list);
        decimal_point += bcd_limb_list.size() > bcd_limb_list_start_size ? 1 : 0;
    }
    long rjust = ((APY_LIMB_SIZE_BITS / 4) - decimal_point) % (APY_LIMB_SIZE_BITS / 4);

    // Multiply BCD list by two, one time per for each missing fractional bit (if any)
    for (int i = 0; i < -frac_bits; i++) {
        bcd_limb_vec_mul2(bcd_limb_list);
    }

    // Convert BCD limb list to regular BCD list (`std::vector<std::uint8_t>`)
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
    assert(src_limbs >= 1 && acc_limbs >= 1 && acc_limbs >= src_limbs);
    if (acc_limbs == 1) {
        /* single limb specialization */
        return [](auto acc_it, auto src_it) { *acc_it += *src_it; };
    } else if (src_limbs == acc_limbs) {
        return [src_limbs](auto acc_it, auto src_it) {
            apy_inplace_addition_same_length(&acc_it[0], &src_it[0], src_limbs);
        };
    } else { /* acc_limbs > src_limbs */
        return [src_limbs, acc_limbs](auto acc_it, auto src_it) {
            apy_inplace_addition(&acc_it[0], acc_limbs, &src_it[0], src_limbs);
            if (limb_vector_is_negative(src_it, src_it + src_limbs)) {
                apy_limb_t carry = 0;
                for (std::size_t i = 0; i < acc_limbs - src_limbs; i++) {
                    apy_limb_t tmp_res = *(acc_it + src_limbs + i) + carry;
                    carry = (tmp_res < carry);
                    tmp_res += apy_limb_t(-1);
                    carry += (tmp_res < apy_limb_t(-1));
                    *(acc_it + src_limbs + i) = tmp_res;
                }
            }
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
    assert(src_limbs >= 1 && acc_limbs >= 1 && acc_limbs >= src_limbs);
    if (acc_limbs == 1) {
        /* single limb (one real, one imag) specialization */
        return [](auto acc_it, auto src_it) {
            *(acc_it + 0) += *(src_it + 0); // real part
            *(acc_it + 1) += *(src_it + 1); // imag part
        };
    } else if (src_limbs == acc_limbs) {
        return [acc_limbs](auto acc_it, auto src_it) {
            std::size_t limbs = acc_limbs;
            apy_inplace_addition_same_length(&acc_it[0], &src_it[0], limbs);
            apy_inplace_addition_same_length(&acc_it[limbs], &src_it[limbs], limbs);
        };
    } else { /* acc_limbs > src_limbs */
        return [src_limbs, acc_limbs](auto acc_it, auto src_it) {
            std::size_t src_j = src_limbs;
            std::size_t acc_j = acc_limbs;
            apy_inplace_addition(&acc_it[0], acc_limbs, &src_it[0], src_limbs);
            apy_inplace_addition(&acc_it[acc_j], acc_j, &src_it[src_j], src_j);

            // Real part
            if (limb_vector_is_negative(src_it, src_it + src_limbs)) {
                apy_limb_t carry = 0;
                for (std::size_t i = 0; i < acc_limbs - src_limbs; i++) {
                    apy_limb_t tmp_res = *(acc_it + src_limbs + i) + carry;
                    carry = (tmp_res < carry);
                    tmp_res += apy_limb_t(-1);
                    carry += (tmp_res < apy_limb_t(-1));
                    *(acc_it + src_limbs + i) = tmp_res;
                }
            }

            // Imag part
            if (limb_vector_is_negative(src_it + src_limbs, src_it + 2 * src_limbs)) {
                apy_limb_t carry = 0;
                for (std::size_t i = 0; i < acc_limbs - src_limbs; i++) {
                    apy_limb_t tmp_res = *(acc_it + acc_limbs + src_limbs + i) + carry;
                    carry = (tmp_res < carry);
                    tmp_res += apy_limb_t(-1);
                    carry += (tmp_res < apy_limb_t(-1));
                    *(acc_it + acc_limbs + src_limbs + i) = tmp_res;
                }
            }
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
            apy_limb_signed_t acc_real = acc_it[0];
            apy_limb_signed_t acc_imag = acc_it[1];
            apy_limb_signed_t src_real = src_it[0];
            apy_limb_signed_t src_imag = src_it[1];
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
