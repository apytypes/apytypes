#include "apyfloat_util.h"
#include <fmt/format.h>
#include <math.h>
#include <nanobind/nanobind.h>

namespace nb = nanobind;

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

exp_t calc_bias_general(
    int new_exp_bits, int exp_bits1, exp_t bias1, int exp_bits2, exp_t bias2
)
{
    return ((double)(bias1 + 1) / (1 << exp_bits1)
            + (double)(bias2 + 1) / (1 << exp_bits2))
        * (1 << (new_exp_bits - 1))
        - 1;
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
        apyman = apyman.cast_no_overflow(2 + bits, 2, QuantizationMode::TRN);
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

void check_exponent_format(int exp_bits)
{
    if ((unsigned(exp_bits) > _EXP_LIMIT_BITS) || (exp_bits < 0)) {
        throw nb::value_error(fmt::format(
                                  "Exponent bits must be a non-negative integer less "
                                  "or equal to {} but {} was given",
                                  _EXP_LIMIT_BITS,
                                  exp_bits
        )
                                  .c_str());
    }
}

void check_mantissa_format(int man_bits)
{
    if ((unsigned(man_bits) > _MAN_LIMIT_BITS) || (man_bits < 0)) {
        throw nb::value_error(fmt::format(
                                  "Mantissa bits must be a non-negative integer less "
                                  "or equal to {} but {} was given",
                                  _MAN_LIMIT_BITS,
                                  man_bits
        )
                                  .c_str());
    }
}
