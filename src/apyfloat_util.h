#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include <functional>

#include "apyfixed.h"
#include "apytypes_common.h"

/*!
 * Sizes of APyFloat datatypes
 */
static constexpr std::size_t _MAN_T_SIZE_BYTES = sizeof(man_t);
static constexpr std::size_t _MAN_T_SIZE_BITS = 8 * _MAN_T_SIZE_BYTES;
static constexpr std::size_t _EXP_T_SIZE_BYTES = sizeof(exp_t);
static constexpr std::size_t _EXP_T_SIZE_BITS = 8 * _EXP_T_SIZE_BYTES;

typedef std::function<
    void(man_t&, exp_t&, std::uint8_t, bool, man_t, std::uint8_t, man_t)>
    quantization_function;
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

//! Quantize mantissa with round to nearest, ties to even
void quantize_mantissa_rnd_conv(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round to nearest, ties to odd
void quantize_mantissa_rnd_conv_odd(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round to nearest, ties away from zero
void quantize_mantissa_rnd_inf(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round to nearest, ties towards zero
void quantize_mantissa_rnd_zero(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round to nearest, ties to positive infinity
void quantize_mantissa_rnd(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round to nearest, ties to negative infinity
void quantize_mantissa_rnd_min_inf(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round towards positive infinity
void quantize_mantissa_trn_inf(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round towards negative infinity
void quantize_mantissa_trn(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round away from zero
void quantize_mantissa_trn_away(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with round towards zero (truncation)
void quantize_mantissa_trn_zero(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with magnitude truncation
//! Does not really make sense for floating-point
void quantize_mantissa_trn_mag(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with von Neumann rounding (jamming)
void quantize_mantissa_jam(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa with unbiased von Neumann rounding (unbiased jamming)
void quantize_mantissa_jam_unbiased(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa weighted stochastic rounding
void quantize_mantissa_stoch_weighted(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Quantize mantissa unweighted stochastic rounding
void quantize_mantissa_stoch_equal(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
);

//! Get quantization method
quantization_function get_quantization_function(QuantizationMode quantization);

APY_INLINE int leading_zeros_apyfixed(const APyFixed& fx)
{
    // Calculate the number of left shifts needed to make fx>=1.0
    const int zeros = fx.leading_zeros() - fx.int_bits();
    return std::max(0, zeros + 1);
}

//! Fast integer power by squaring.
man_t ipow(man_t base, unsigned int n);

#endif // _APYFLOAT_UTIL_H
