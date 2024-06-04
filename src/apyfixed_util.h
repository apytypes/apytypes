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

//! Iterator-based hadamard product
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
void hadamard_product(
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
        simd::vector_mul(src1, src2, dst, n_items);
        return; // early exit
    }

    // General case. This always works, but is the slowest variant.
    std::vector<mp_limb_t> op1_abs(src1_limbs);
    std::vector<mp_limb_t> op2_abs(src2_limbs);
    std::vector<mp_limb_t> product_abs(src1_limbs + src2_limbs);
    auto op1_begin = src1;
    auto op2_begin = src2;
    for (std::size_t i = 0; i < n_items; i++) {
        // Current working operands
        auto op1_end = op1_begin + src1_limbs;
        auto op2_end = op2_begin + src2_limbs;

        // Resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;

        // Retrieve the absolute value of both operands
        limb_vector_abs(op1_begin, op1_end, std::begin(op1_abs));
        limb_vector_abs(op2_begin, op2_end, std::begin(op2_abs));

        // Perform the multiplication and possibly negate
        mpn_mul(&product_abs[0], &op1_abs[0], src1_limbs, &op2_abs[0], src2_limbs);
        if (result_sign) {
            limb_vector_negate(
                product_abs.cbegin(),
                product_abs.cbegin() + dst_limbs,
                dst + i * dst_limbs
            );
        } else {
            std::copy(
                product_abs.cbegin(),
                product_abs.cbegin() + dst_limbs,
                dst + i * dst_limbs
            );
        }

        // Adjust iterators for next iteration
        op1_begin = op1_end;
        op2_begin = op2_end;
    }
}

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
    std::vector<mp_limb_t> hadamard(dst_limbs * n_items);
    hadamard_product(
        src1, src2, hadamard.begin(), src1_limbs, src2_limbs, dst_limbs, n_items
    );
    for (std::size_t i = 0; i < n_items; i++) {
        mpn_add(&dst[0], &dst[0], dst_limbs, &hadamard[i * dst_limbs], dst_limbs);
    }
}

#endif // _APYFIXED_UTIL_H
