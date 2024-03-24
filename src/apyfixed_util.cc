#include <nanobind/nanobind.h>
namespace nb = nanobind;

#include <cmath>

#include "apyfixed_util.h"
#include "apytypes_util.h"
#include "ieee754.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

mp_limb_t twos_complement_overflow(mp_limb_t value, int bits)
{
    unsigned limb_shift_val = bits & (_LIMB_SIZE_BITS - 1);

    if (limb_shift_val) {
        auto shft_amnt = _LIMB_SIZE_BITS - limb_shift_val;
        auto signed_limb = mp_limb_signed_t(value << shft_amnt) >> shft_amnt;
        return mp_limb_t(signed_limb);
    }
    return value;
}

//! Specialized method when only one limb is used
mp_limb_t get_data_from_double(double value, int bits, int frac_bits)
{
    if (std::isnan(value)) {
        throw nb::value_error("Cannot convert NaN to fixed-point");
    }
    if (std::isinf(value)) {
        throw nb::value_error("Cannot convert Infinity to fixed-point");
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
        return twos_complement_overflow(man, bits);
    } else {
        throw NotImplementedException(
            "Not implemented: get_data_from_double() for 32-bit systems"
        );
    }
}
