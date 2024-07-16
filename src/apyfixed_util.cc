#include <cmath>
#include <fmt/format.h>

#include "apyfixed_util.h"
#include "apytypes_util.h"
#include "ieee754.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

// Single limb specialization for extracting fixed-point value from `double`
mp_limb_t get_data_from_double(double value, int bits, int frac_bits, int shift_amnt)
{
    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }

    int exp = exp_of_double(value);
    uint64_t man = man_of_double(value);

    // Append mantissa hidden one
    if (exp) {
        man |= uint64_t(1) << 52;
    }

    // Adjust the actual exponent with bias (-1023) and shift the data into its
    // correct position
    auto left_shift_amnt = exp + frac_bits - 52 - 1023;
    if (left_shift_amnt >= 0) {
        if (left_shift_amnt < 64) {
            man <<= left_shift_amnt;
        } else {
            man = 0;
        }
    } else {
        auto right_shift_amount = -left_shift_amnt;
        if (right_shift_amount < 55) {
            // Round the value and shift
            man += mp_limb_t(1) << (right_shift_amount - 1);
            man >>= right_shift_amount;
        } else {
            man = 0;
        }
    }
    // Adjust result from sign
    if (sign_of_double(value)) {
        man = -man;
    }

    // Two's complement overflow
    man = mp_limb_t(int64_t(man << shift_amnt) >> shift_amnt);
    return man;
}

APyFixed ipow(APyFixed base, unsigned int n)
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
