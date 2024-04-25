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
man_t APY_INLINE quantize_mantissa(
    man_t man, std::uint8_t bits_to_quantize, bool sign, QuantizationMode quantization
)
{
    // Initial value for mantissa
    man_t res_man = man >> bits_to_quantize;

    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> (bits_to_quantize - 1)) & 1;
    T = (man & ((1ULL << (bits_to_quantize - 1)) - 1)) != 0;

    switch (quantization) {
    case QuantizationMode::RND_CONV: // TIES_EVEN
        // Using 'res_man' directly here is fine since G can only be '0' or '1',
        // thus calculating the LSB of 'res_man' is not needed.
        if (!(G & (res_man | T))) {
            return res_man;
        }
        break;
    case QuantizationMode::RND_CONV_ODD: // TIES_ODD
        if (!(G & ((res_man ^ 1) | T))) {
            return res_man;
        }
        break;
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        if (!(sign ? 0 : (G | T))) {
            return res_man;
        }
        break;
    case QuantizationMode::TRN: // TO_NEGATIVE
        if (!(sign ? (G | T) : 0)) {
            return res_man;
        }
        break;
    case QuantizationMode::TRN_AWAY: // TO_AWAY
        if (!(G | T)) {
            return res_man;
        }
        break;
    case QuantizationMode::TRN_ZERO: // TO_ZERO
        return res_man;
    case QuantizationMode::TRN_MAG: // Does not really make sense for
                                    // floating-point
        if (!sign) {
            return res_man;
        }
        break;
    case QuantizationMode::RND_INF: // TIES_AWAY
        if (!G) {
            return res_man;
        }
        break;
    case QuantizationMode::RND_ZERO: // TIES_ZERO
        if (!(G & T)) {
            return res_man;
        }
        break;
    case QuantizationMode::RND: // TIES_POS
        if (!(G & (T | !sign))) {
            return res_man;
        }
        break;
    case QuantizationMode::RND_MIN_INF: // TIES_NEG
        if (!(G & (T | sign))) {
            return res_man;
        }
        break;
    case QuantizationMode::JAM:
        return res_man | 1;
    case QuantizationMode::JAM_UNBIASED:
        return res_man | (T | G);
    case QuantizationMode::STOCH_WEIGHTED: {
        const man_t trailing_bits = man & ((1ULL << bits_to_quantize) - 1);
        const man_t weight = random_number() & ((1ULL << bits_to_quantize) - 1);
        // Since the weight won't be greater than the discarded bits,
        // this will never round an already exact number.
        if (!((trailing_bits + weight) >> bits_to_quantize)) {
            return res_man;
        }
    } break;
    case QuantizationMode::STOCH_EQUAL:
        // Only perform the quantization if the result is not exact.
        if (!(G || T) || (random_number() & 1)) {
            return res_man;
        }
        break;
    default:
        throw NotImplementedException(fmt::format(
            "Not implemented: quantization mode '{}'",
            quantization_mode_to_string(quantization)
        ));
    }

    return ++res_man;
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
