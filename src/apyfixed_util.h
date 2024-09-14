#ifndef _APYFIXED_UTIL_H
#define _APYFIXED_UTIL_H

#include "apyfixed.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"

#include <functional> // std::bind, std::function, std::placeholders

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

//! Constant fixed-point one for convenience
const APyFixed fx_one(2, 2, std::vector<mp_limb_t>({ 1 }));

//! Specialized method when only one limb is used
mp_limb_t get_data_from_double(double value, int bits, int frac_bits, int shift_amnt);

//! Fast integer power by squaring.
APyFixed ipow(APyFixed base, unsigned int n);

//! Get bit pattern for the value one
static APY_INLINE APyFixed one(int bits, int int_bits)
{
    std::size_t bit_index = bits - int_bits;
    std::size_t limb_bits = sizeof(mp_limb_t) * 8;
    std::size_t limb_index = bit_index / limb_bits;
    std::size_t bit_offset = bit_index % limb_bits;

    std::size_t num_limbs = limb_index + 1;
    std::vector<mp_limb_t> data(num_limbs, static_cast<mp_limb_t>(0));

    // Set the specified bit to 1
    data[limb_index] |= static_cast<mp_limb_t>(1) << bit_offset;

    return APyFixed(bits, int_bits, data);
}
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
        unsigned start_idx = -left_shift_amnt;
        if (limb_vector_is_negative(it_begin, it_end)) {
            if (start_idx < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                    limb_vector_add_pow2(it_begin, it_end, start_idx);
                }
                limb_vector_asr(it_begin, it_end, start_idx);
            } else {
                std::fill(it_begin, it_end, 0);
            }
        } else { /* !limb_vector_is_negative(it_begin, it_end) */
            if (start_idx < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                    limb_vector_add_pow2(it_begin, it_end, start_idx);
                }
                limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_asr(it_begin, it_end, start_idx);
        } else { /* limb_vector_is_negative(it_begin, it_end) */
            if (start_idx < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                    limb_vector_add_pow2(it_begin, it_end, start_idx);
                }
                limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (!limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_asr(it_begin, it_end, start_idx);
        } else { /* limb_vector_is_negative(it_begin, it_end) */
            if (start_idx < unsigned(bits)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx);
                limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (limb_vector_is_negative(it_begin, it_end)) {
            limb_vector_asr(it_begin, it_end, start_idx);
        } else {
            if (start_idx < unsigned(bits)) {
                if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                    limb_vector_add_pow2(it_begin, it_end, start_idx);
                }
                limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx <= unsigned(bits)) {
            limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx <= unsigned(bits)) {
            if (limb_vector_is_negative(it_begin, it_end)
                || limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
            limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx <= unsigned(bits)) {
            if (!limb_vector_is_negative(it_begin, it_end)
                || limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
            limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx <= unsigned(bits)) {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
            limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx < unsigned(bits)) {
            if (limb_vector_test_bit(it_begin, it_end, start_idx)
                || limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
            limb_vector_asr(it_begin, it_end, start_idx);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx < unsigned(bits)) {
            if (!limb_vector_test_bit(it_begin, it_end, start_idx)
                || limb_vector_or_reduce(it_begin, it_end, start_idx - 1)) {
                limb_vector_add_pow2(it_begin, it_end, start_idx - 1);
            }
            limb_vector_asr(it_begin, it_end, start_idx);
        } else if (start_idx == unsigned(bits)) {
            bool fill_neg = limb_vector_is_negative(it_begin, it_end)
                && !limb_vector_or_reduce(it_begin, it_end, start_idx - 1);
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
        unsigned start_idx = -left_shift_amnt;
        if (start_idx < unsigned(bits)) {
            if (limb_vector_or_reduce(it_begin, it_end, start_idx)) {
                limb_vector_set_bit(it_begin, it_end, start_idx, true);
            }
            limb_vector_asr(it_begin, it_end, start_idx);
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
 * *     Fixed-point iterator based arithmetic functions with multi-limb support    * *
 * ********************************************************************************** */

//! Iterator-based multi-limb fixed-point product used for multiple calls
template <
    typename RANDOM_ACCESS_ITERATOR_IN,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename OP_ABS_VECTOR_T,
    typename PROD_ABS_VECTOR_T>
static APY_INLINE void fixed_point_product(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs, // Number of limbs `src1`
    std::size_t src2_limbs, // Number of limbs `src2`
    std::size_t dst_limbs,  // `dst_limbs` <= `src1_limbs` + `src2_limbs`
    OP_ABS_VECTOR_T op1_abs,
    OP_ABS_VECTOR_T op2_abs,
    PROD_ABS_VECTOR_T prod_abs
)
{
    // Resulting sign
    bool sign1 = mp_limb_signed_t(*(src1 + src1_limbs - 1)) < 0;
    bool sign2 = mp_limb_signed_t(*(src2 + src2_limbs - 1)) < 0;
    bool result_sign = sign1 ^ sign2;

    // Retrieve the absolute value of both operands
    limb_vector_abs(src1, src1 + src1_limbs, std::begin(op1_abs));
    limb_vector_abs(src2, src2 + src2_limbs, std::begin(op2_abs));

    // Perform the multiplication and possibly negate
    if (src1_limbs < src2_limbs) {
        mpn_mul(&prod_abs[0], &op2_abs[0], src2_limbs, &op1_abs[0], src1_limbs);
    } else {
        mpn_mul(&prod_abs[0], &op1_abs[0], src1_limbs, &op2_abs[0], src2_limbs);
    }

    // Negate of copy the result back
    if (result_sign) {
        limb_vector_negate(prod_abs.cbegin(), prod_abs.cbegin() + dst_limbs, dst);
    } else {
        std::copy(prod_abs.cbegin(), prod_abs.cbegin() + dst_limbs, dst);
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
            op1_abs,
            op2_abs,
            prod_abs
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
                op1_abs,               // op1_abs scratch vector
                op2_abs,               // op2_abs scratch vector
                product                // product_abs scratch_vector
            );
            mpn_add(&dst[0], &dst[0], dst_limbs, &product[0], dst_limbs);
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
                op1_abs,               // op1_abs scratch vector
                op2_abs,               // op2_abs scratch vector
                product                // product_abs scratch_vector
            );
            if (mp_limb_signed_t(product[product_limbs - 1]) < 0) {
                // Sign-extend
                std::fill(
                    std::begin(product) + product_limbs,
                    std::end(product),
                    mp_limb_t(-1)
                );
            } else {
                // Zero-extend
                std::fill(
                    std::begin(product) + product_limbs, std::end(product), mp_limb_t(0)
                );
            }
            mpn_add(&dst[0], &dst[0], dst_limbs, &product[0], dst_limbs);
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
                op1_abs,
                op2_abs,
                product
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
            mpn_add(&dst[0], &dst[0], dst_limbs, &product[0], dst_limbs);
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
                op1_abs,
                op2_abs,
                product
            );

            if (mp_limb_signed_t(product[product_limbs - 1]) < 0) {
                // Sign-extend
                std::fill(
                    std::begin(product) + product_limbs,
                    std::end(product),
                    mp_limb_t(-1)
                );
            } else {
                // Zero-extend
                std::fill(
                    std::begin(product) + product_limbs, std::end(product), mp_limb_t(0)
                );
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
            mpn_add(&dst[0], &dst[0], dst_limbs, &product[0], dst_limbs);
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

#endif // _APYFIXED_UTIL_H
