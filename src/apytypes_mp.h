/* ********************************************************************************** *
 * *               APyTypes limb types and multi-precision operations               * *
 * ********************************************************************************** */

#ifndef __APYTYPES_MP_H__
#define __APYTYPES_MP_H__

#include <cassert> // assert
#include <cstddef> // std::size_t
#include <cstdint> // std::int64_t, std::uint64_t, std::int32_t, std::uint32_t

#include "apytypes_fwd.h"
#include "apytypes_intrinsics.h"

// TODO: document the functions

// Addition

//! Add two limb vectors in place: dest += src, where len(dest) >= len(src)
apy_limb_t apy_inplace_addition(
    apy_limb_t*, const std::size_t, const apy_limb_t*, const std::size_t
);

//! Add two limb vectors: dest = src0 + src1, where len(dest) == len(src0) ==
//! len(src1)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_addition_same_length(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const apy_limb_t* src1,
    const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    dest[0] = src0[0] + src1[0];
    apy_limb_t carry = (dest[0] < src1[0]);

    for (std::size_t i = 1; i < limbs; i++) {
        add_single_limbs_with_carry(src0[i], src1[i], &dest[i], carry, &carry);
    }
    return carry;
}

//! Add a single limb to a limb vector in place: dest += src
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_addition_single_limb(
    apy_limb_t* dest, const std::size_t limbs, const apy_limb_t src
)
{
    assert(limbs > 0);

    /* src is initial "carry" */
    apy_limb_t carry = src;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] += carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Add a single one to a limb vector in place: dest++
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_add_one_lsb(apy_limb_t* dest, const std::size_t limbs)
{
    assert(limbs > 0);

    apy_limb_t carry = 1;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] += carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Add two limb vectors of the same length in place: dest += src, where len(dest)
//! == len(src)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_addition_same_length(
    apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    dest[0] += src[0];
    apy_limb_t carry = (dest[0] < src[0]);

    for (std::size_t i = 1; i < limbs; i++) {
        add_single_limbs_with_carry(src[i], dest[i], &dest[i], carry, &carry);
    }
    return carry;
}

// Subtraction

//! Negate a limb vector in place: dest = -dest
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_negate(apy_limb_t* dest, const std::size_t limbs)
{
    assert(limbs > 0);

    apy_limb_t carry = 1;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] = ~dest[i] + carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Negate a limb vector: dest = -src
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_negate(apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs)
{
    assert(limbs > 0);

    apy_limb_t carry = 1;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] = ~src[i] + carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Subtract a single limb from a limb vectors in place: dest -= src
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_subtraction_single_limb(
    apy_limb_t* dest, const std::size_t limbs, const apy_limb_t src
)
{
    assert(limbs > 0);

    /* src is initial "carry" */
    apy_limb_t carry = src;

    for (std::size_t i = 0; i < limbs; i++) {
        /* Determine carry to next limb */
        apy_limb_t carry_tmp = dest[i] < carry;
        dest[i] -= carry;
        carry = carry_tmp;
    }

    return carry;
}

//! Subtract two limb vectors in place: dest -= src, where len(dest) == len(src)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_subtraction_same_length(
    apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    apy_limb_t carry = (dest[0] < src[0]);
    dest[0] -= src[0];

    for (std::size_t i = 1; i < limbs; i++) {
        sub_single_limbs_with_carry(dest[i], src[i], &dest[i], carry, &carry);
    }
    return carry;
}

//! Subtract two limb vectors in place: dest = src - dest, where len(dest) ==
//! len(src)
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_reversed_subtraction_same_length(
    apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    apy_limb_t carry = (src[0] < dest[0]);
    dest[0] = src[0] - dest[0];

    for (std::size_t i = 1; i < limbs; i++) {
        sub_single_limbs_with_carry(src[i], dest[i], &dest[i], carry, &carry);
    }
    return carry;
}

//! Subtract two limb vectors: dest = src0 - src1, where len(dest) == len(src0) ==
//! len(src1)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_subtraction_same_length(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const apy_limb_t* src1,
    const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    apy_limb_t carry = (src0[0] < src1[0]);
    dest[0] = src0[0] - src1[0];

    for (std::size_t i = 1; i < limbs; i++) {
        sub_single_limbs_with_carry(src0[i], src1[i], &dest[i], carry, &carry);
    }
    return carry;
}

// Shift

//! Left-shift limb vector in place
apy_limb_t apy_inplace_left_shift(apy_limb_t*, const std::size_t, unsigned int);
//! Left-shift limb vector: dest == src << shift_amount
apy_limb_t apy_left_shift(
    apy_limb_t* dest,
    const apy_limb_t* src,
    const std::size_t limbs,
    const unsigned shift_amount
);

//! Right-shift limb vector in place
apy_limb_t apy_inplace_right_shift(apy_limb_t*, const std::size_t, unsigned int);
/* apy_limb_t
apy_rshift(apy_limb_t*, const apy_limb_t*, const std::size_t, const unsigned int); */

// Multiplication

//! Multiply two unsigned limb vectors
apy_limb_t apy_unsigned_multiplication(
    apy_limb_t*,
    const apy_limb_t*,
    const std::size_t,
    const apy_limb_t*,
    const std::size_t
);

//! Square an unsigned limb vector
apy_limb_t apy_unsigned_square(apy_limb_t*, const apy_limb_t*, const std::size_t);

// Division

//! Class representing the inverse/reciprocal of the denominator
class APyDivInverse {
public:
    //! Normalization shift count.
    unsigned norm_shift;
    //! Normalized denominator, most significant limb
    apy_limb_t norm_denominator_1;
    //! Normalized denominator, least significant limb (unused for single limb
    //! denominators)
    apy_limb_t norm_denominator_0;
    //! Inverse, for 2/1 or 3/2.
    apy_limb_t inverse;
    //! Constructor from limb vector.
    explicit APyDivInverse(const apy_limb_t*, std::size_t);

private:
    //! Compute the 3/2 (or 2/1) inverse.
    void compute_3by2_inverse();
};

//! Divide two unsigned limb vectors
void apy_unsigned_division(
    apy_limb_t*, apy_limb_t*, const std::size_t, const apy_limb_t*, const std::size_t
);

//! Divide two unsigned limb vectors where the APyDivInverse is pre-computed
void apy_unsigned_division_preinverted(
    apy_limb_t* quotient,
    apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const apy_limb_t* denominator,
    const std::size_t denominator_limbs,
    const APyDivInverse* inv
);

#endif
