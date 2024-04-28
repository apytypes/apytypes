#include "apyfloat_util.h"

/* Helper functions */
man_t ipow(man_t base, unsigned int n)
{
    // Solution from
    // https://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
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

//! Quantize mantissa with round to nearest, ties to even
void quantize_mantissa_rnd_conv(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    // Using 'man' directly here is fine since G can only be '0' or '1',
    // thus calculating the LSB of 'res_man' is not needed.
    B = G & (man | T);
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round to nearest, ties to odd
void quantize_mantissa_rnd_conv_odd(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    // Using 'man' directly here is fine since G can only be '0' or '1',
    // thus calculating the LSB of 'res_man' is not needed.
    B = G & ((man ^ 1) | T);
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round to nearest, ties away from zero
void quantize_mantissa_rnd_inf(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = G;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round to nearest, ties towards zero
void quantize_mantissa_rnd_zero(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    // Using 'man' directly here is fine since G can only be '0' or '1',
    // thus calculating the LSB of 'res_man' is not needed.
    B = G & T;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round to nearest, ties to positive infinity
void quantize_mantissa_rnd(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = G & (T | !sign);
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round to nearest, ties to negative infinity
void quantize_mantissa_rnd_min_inf(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = G & (T | sign);
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round towards positive infinity
void quantize_mantissa_trn_inf(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = sign ? 0 : (G | T);
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round towards negative infinity
void quantize_mantissa_trn(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = sign ? (G | T) : 0;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round away from zero
void quantize_mantissa_trn_away(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = G | T;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with round towards zero (truncation)
void quantize_mantissa_trn_zero(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    man >>= bits_to_quantize;
}

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
)
{
    // Calculate quantization bit
    man_t B; // Quantization bit to add to LSB

    // Initial value for mantissa
    man >>= bits_to_quantize;

    B = sign;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa with von Neumann rounding (jamming)
void quantize_mantissa_jam(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    man >>= bits_to_quantize;
    man |= 1;
}

//! Quantize mantissa with unbiased von Neumann rounding (unbiased jamming)
void quantize_mantissa_jam_unbiased(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T;   // Sticky bit, logical OR of all the bits after the guard bit

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    man >>= bits_to_quantize;
    man |= (T || G);
}

//! Quantize mantissa weighted stochastic rounding
void quantize_mantissa_stoch_weighted(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    const man_t trailing_bits = man & ((1ULL << bits_to_quantize) - 1);
    const man_t weight = random_number_float() & ((1ULL << bits_to_quantize) - 1);
    // Since the weight won't be greater than the discarded bits,
    // this will never round an already exact number.

    // Calculate quantization bit
    man_t B; // Quantization bit to add to LSB

    // Initial value for mantissa
    man >>= bits_to_quantize;

    // Using 'man' directly here is fine since G can only be '0' or '1',
    // thus calculating the LSB of 'res_man' is not needed.
    B = (trailing_bits + weight) >> bits_to_quantize;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Quantize mantissa unweighted stochastic rounding
void quantize_mantissa_stoch_equal(
    man_t& man,
    exp_t& exp,
    std::uint8_t bits_to_quantize,
    bool sign,
    man_t man_msb_constant,
    std::uint8_t bits_to_quantize_dec,
    man_t sticky_constant
)
{
    // Calculate quantization bit
    man_t G, // Guard (bit after LSB)
        T,   // Sticky bit, logical OR of all the bits after the guard bit
        B;   // Quantization bit to add to LSB

    G = (man >> bits_to_quantize_dec) & 1;
    T = (man & sticky_constant) != 0;

    // Initial value for mantissa
    man >>= bits_to_quantize;

    // Only perform the quantization if the result is not exact.
    B = (G || T) ? random_number_float() & 1 : 0;
    man += B;
    if (man & man_msb_constant) {
        ++exp;
        man = 0;
    }
}

//! Get quantization method
quantization_function get_quantization_function(QuantizationMode quantization)
{
    switch (quantization) {
    case QuantizationMode::RND_CONV: // TIES_EVEN
        return quantize_mantissa_rnd_conv;
    case QuantizationMode::RND_CONV_ODD: // TIES_ODD
        return quantize_mantissa_rnd_conv_odd;
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        return quantize_mantissa_trn_inf;
    case QuantizationMode::TRN: // TO_NEGATIVE
        return quantize_mantissa_trn;
    case QuantizationMode::TRN_AWAY: // TO_AWAY
        return quantize_mantissa_trn_away;
    case QuantizationMode::TRN_ZERO: // TO_ZERO
        return quantize_mantissa_trn_zero;
    case QuantizationMode::TRN_MAG: // Does not really make sense for
                                    // floating-point
        return quantize_mantissa_trn_mag;
    case QuantizationMode::RND_INF: // TIES_AWAY
        return quantize_mantissa_rnd_inf;
    case QuantizationMode::RND_ZERO: // TIES_ZERO
        return quantize_mantissa_rnd_zero;
    case QuantizationMode::RND: // TIES_POS
        return quantize_mantissa_rnd;
    case QuantizationMode::RND_MIN_INF: // TIES_NEG
        return quantize_mantissa_rnd_min_inf;
    case QuantizationMode::JAM:
        return quantize_mantissa_jam;
    case QuantizationMode::JAM_UNBIASED:
        return quantize_mantissa_jam_unbiased;
    case QuantizationMode::STOCH_WEIGHTED:
        return quantize_mantissa_stoch_weighted;
    case QuantizationMode::STOCH_EQUAL:
        return quantize_mantissa_stoch_equal;
    default:
        throw NotImplementedException(
            "Not implemented: get_quantization_function() with "
            "unknown (did you pass `int` as `QuantizationMode`?)"
        );
    }
}
