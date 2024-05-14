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

void quantize_apymantissa(
    APyFixed& apyman, bool sign, int bits, QuantizationMode quantization
)
{
    if (quantization == QuantizationMode::STOCH_WEIGHTED) {
        std::vector<mp_limb_t> rnd_data = limb_vector_from_uint64_t(
            { random_number_float(), random_number_float(), 0 }
        );
        APyFixed rnd_num(64 * 3, 64 - bits, rnd_data);
        apyman = apyman + rnd_num;
    } else if (quantization == QuantizationMode::STOCH_EQUAL) {
        const mp_limb_t rnd = -(random_number_float() % 2);
        std::vector<mp_limb_t> rnd_data = limb_vector_from_uint64_t({ rnd, rnd, 0 });
        APyFixed rnd_num(64 * 3, 64 - bits, rnd_data);
        apyman = apyman + rnd_num;
    } else {
        quantization = translate_quantization_mode(quantization, sign);
        apyman = apyman.cast_no_overflow(3 + bits, 3, quantization);
    }
}

QuantizationMode translate_quantization_mode(QuantizationMode quantization, bool sign)
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
