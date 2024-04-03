#include "apyfloat_util.h"

/* Helper function */
man_t quantize_mantissa(
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
    case QuantizationMode::RND_CONV: // TIES_TO_EVEN
        // Using 'res_man' directly here is fine since G can only be '0' or '1',
        // thus calculating the LSB of 'res_man' is not needed.
        B = G & (res_man | T);
        break;
    case QuantizationMode::TRN_INF: // TO_POSITIVE
        B = sign ? 0 : (G | T);
        break;
    case QuantizationMode::TRN: // TO_NEGATIVE
        B = sign ? (G | T) : 0;
        break;
    case QuantizationMode::TRN_ZERO: // TO_ZERO
        B = 0;
        break;
    case QuantizationMode::TRN_MAG: // Does not really make sense for
                                    // floating-point
        B = sign;
        break;
    case QuantizationMode::RND_INF: // TIES_TO_AWAY
        B = G;
        break;
    case QuantizationMode::RND_ZERO: // TIES_TO_ZERO
        B = G & T;
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
        const man_t weight = random_number() & ((1ULL << bits_to_quantize) - 1);
        // Since the weight won't be greater than the discarded bits,
        // this will never round an already exact number.
        B = (trailing_bits + weight) >> bits_to_quantize;
    } break;
    case QuantizationMode::STOCH_EQUAL:
        // Only perform the quantization if the result is not exact.
        B = (G || T) ? random_number() & 1 : 0;
        break;
    default:
        throw NotImplementedException("APyFloat: Unknown or invalid quantization mode."
        );
    }
    res_man += B;
    return res_man;
}

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

static const APyFixed fx_one(2, 2, std::vector<mp_limb_t>({ 1 }));

APyFixed ipow_apyfixed(APyFixed base, unsigned int n)
{
    // Early exit for one of the most common cases
    if (n == 2) {
        return base * base;
    }
    // Because how APyFloat::pown is written, we know n will be >= 2,
    // this fact can probably be used to optimize this code further.

    APyFixed result = fx_one;
    for (;;) {
        if (n & 1) {
            result = result * base; // Until *= is implemented
        }

        n >>= 1;

        if (!n) {
            break;
        }

        base = base * base; // Until *= is implemented
    }

    return result;
}
