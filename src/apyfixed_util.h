#ifndef _APYFIXED_UTIL_H
#define _APYFIXED_UTIL_H

#include "apyfixed.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

//! Constant fixed-point one for convenience
const APyFixed fx_one(2, 2, std::vector<mp_limb_t>({ 1 }));

//! Specialized method when only one limb is used
mp_limb_t get_data_from_double(double value, int bits, int frac_bits, int shift_amnt);

//! Fast integer power by squaring.
APyFixed ipow(APyFixed base, unsigned int n);

//! Iterator-based inner product
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
void inner_product(
    RANDOM_ACCESS_ITERATOR_IN src1,
    RANDOM_ACCESS_ITERATOR_IN src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs, // Number of limbs in one fixed-point of `src1`
    std::size_t src2_limbs, // Number of limbs in one fixed-point of `src2`
    std::size_t dst_limbs,  // Number of limbs in the result
    std::size_t n_items     // Number of elements to use in inner product
)
{
    // Specialization #1: the resulting number of limbs is exactly one
    if (dst_limbs == 1) {
        dst[0] = simd::vector_multiply_accumulate(src1, src2, n_items);
        return; // early exit
    }

    // General case. This always works, but is the slowest variant.
    std::vector<mp_limb_t> hadamard((src1_limbs + src2_limbs) * n_items);
    for (std::size_t i = 0; i < n_items; i++) {
        throw NotImplementedException();
    }
}

#endif // _APYFIXED_UTIL_H
