#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include "apyfixed.h"
#include "apytypes_common.h"

/*!
 * Sizes of APyFloat datatypes
 */
static constexpr std::size_t _MAN_T_SIZE_BYTES = sizeof(man_t);
static constexpr std::size_t _MAN_T_SIZE_BITS = 8 * _MAN_T_SIZE_BYTES;
static constexpr std::size_t _EXP_T_SIZE_BYTES = sizeof(exp_t);
static constexpr std::size_t _EXP_T_SIZE_BITS = 8 * _EXP_T_SIZE_BYTES;

//! Quantize mantissa
void APY_INLINE quantize_mantissa(
    man_t& man,
    exp_t& exp,
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

    switch (quantization) {
    case QuantizationMode::RND_CONV: // TIES_EVEN
        // Using 'res_man' directly here is fine since G can only be '0' or '1',
        // thus calculating the LSB of 'res_man' is not needed.
        man >>= bits_to_quantize;
        B = (G & (man | T));
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::RND_CONV_ODD: // TIES_ODD
        man >>= bits_to_quantize;
        B = (G & ((man ^ 1) | T));
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        man >>= bits_to_quantize;
        B = (!sign) & (G | T);
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::TRN: // TO_NEGATIVE
        man >>= bits_to_quantize;
        B = sign & (G | T);
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::TRN_AWAY: // TO_AWAY
        man >>= bits_to_quantize;
        B = G | T;
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::TRN_ZERO: // TO_ZERO
        man >>= bits_to_quantize;
        return;
    case QuantizationMode::TRN_MAG: // Does not really make sense for
                                    // floating-point
        man >>= bits_to_quantize;
        if (!sign) {
            return;
        }
        break;
    case QuantizationMode::RND_INF: // TIES_AWAY
        man >>= bits_to_quantize;
        if (!G) {
            return;
        };
        break;
    case QuantizationMode::RND_ZERO: // TIES_ZERO
        man >>= bits_to_quantize;
        B = G & T;
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::RND: // TIES_POS
        man >>= bits_to_quantize;
        B = G & (T | !sign);
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::RND_MIN_INF: // TIES_NEG
        man >>= bits_to_quantize;
        B = G & (T | sign);
        if (!B) {
            return;
        };
        break;
    case QuantizationMode::JAM:
        man >>= bits_to_quantize;
        man |= 1;
        return;
    case QuantizationMode::JAM_UNBIASED:
        man >>= bits_to_quantize;
        if (T || G) {
            man |= 1;
        }
        return;
    case QuantizationMode::STOCH_WEIGHTED: {
        const man_t trailing_bits = man & ((1ULL << bits_to_quantize) - 1);
        const man_t weight = random_number_float() & ((1ULL << bits_to_quantize) - 1);
        // Since the weight won't be greater than the discarded bits,
        // this will never round an already exact number.
        B = (trailing_bits + weight) >> bits_to_quantize;
        man >>= bits_to_quantize;
        if (!B) {
            return;
        };
    } break;
    case QuantizationMode::STOCH_EQUAL:
        // Only perform the quantization if the result is not exact.
        man >>= bits_to_quantize;
        B = (G || T) ? random_number_float() & 1 : 0;
        if (!B) {
            return;
        };
        break;
    default:
        throw NotImplementedException(
            "Not implemented: quantize_mantissa() with "
            "unknown (did you pass `int` as `QuantizationMode`?)"
        );
    }
    man++;
    // Carry from adding one LSB
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa
void APY_INLINE quantize_mantissa(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    QuantizationMode quantization
)
{
    quantize_mantissa(
        man,
        exp,
        bits_to_quantize,
        sign,
        man_msb_constant,
        bits_to_quantize - 1,
        (1ULL << (bits_to_quantize - 1)) - 1,
        quantization
    );
}

//! Fast integer power by squaring.
man_t ipow(man_t base, unsigned int n);

APY_INLINE int leading_zeros_apyfixed(const APyFixed& fx)
{
    // Calculate the number of left shifts needed to make fx>=1.0
    const int zeros = fx.leading_zeros() - fx.int_bits();
    return std::max(0, zeros + 1);
}

#endif // _APYFLOAT_UTIL_H
