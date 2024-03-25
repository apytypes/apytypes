#include <cmath>
#include <fmt/format.h>

#include "apyfixed_util.h"
#include "apytypes_util.h"
#include "ieee754.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

//! Specialized method when only one limb is used
mp_limb_t get_data_from_double(double value, int bits, int frac_bits, int shift_amnt)
{
    if (std::isnan(value) || std::isinf(value)) {
        throw std::domain_error(fmt::format("Cannot convert {} to fixed-point", value));
    }
    if constexpr (_LIMB_SIZE_BITS == 64) {
        mp_limb_signed_t exp = exp_of_double(value);
        mp_limb_t man = man_of_double(value);

        // Append mantissa hidden one
        if (exp) {
            man |= mp_limb_t(1) << 52;
        }

        // Adjust the actual exponent with bias (-1023) and
        // shift the data into its correct position
        auto left_shift_amnt = exp + frac_bits - 52 - 1023;
        if (left_shift_amnt >= 0) {
            man <<= left_shift_amnt;
        } else {
            auto right_shift_amount = -left_shift_amnt;
            if (right_shift_amount - 1 < 64) {
                // Round the value
                man += mp_limb_t(1) << (right_shift_amount - 1);
            }
            man >>= right_shift_amount;
        }
        // Adjust result from sign
        if (sign_of_double(value)) {
            man = -man;
        }

        // Two's complement overflow
        man = mp_limb_t(mp_limb_signed_t(man << shift_amnt) >> shift_amnt);
        return man;
    } else {
        throw NotImplementedException(
            "Not implemented: get_data_from_double() for 32-bit systems"
        );
    }
}
