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

template <class RANDOM_ACCESS_ITERATOR_INOUT, class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_addition(
    RANDOM_ACCESS_ITERATOR_INOUT dest_begin,
    RANDOM_ACCESS_ITERATOR_INOUT dest_end,
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    RANDOM_ACCESS_ITERATOR_IN src_end
)
{
    assert(dest_begin != dest_end);
    assert(src_begin != src_end);
    assert(std::distance(dest_begin, dest_end) >= std::distance(src_begin, src_end));

    apy_limb_t carry = 0;

    auto dest_it = dest_begin;
    auto src_it = src_begin;

    for (; src_it != src_end; ++dest_it, ++src_it) {
        add_single_limbs_with_carry(*src_it, *dest_it, &*dest_it, carry, &carry);
    }
    if (carry) {
        for (; dest_it != dest_end; ++dest_it) {
            *dest_it += carry;
            carry = (*dest_it < carry);
        }
    }
    return carry;
}
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

//! Add two limb vectors of length two: dest = src0 + src1, where len(dest) == len(src0)
//! == len(src1) == 2
[[maybe_unused]] static APY_INLINE apy_limb_t apy_addition_length_two(
    apy_limb_t* dest, const apy_limb_t* src0, const apy_limb_t* src1
)
{
    dest[0] = src0[0] + src1[0];
    apy_limb_t carry = (dest[0] < src1[0]);

    add_single_limbs_with_carry(src0[1], src1[1], &dest[1], carry, &carry);
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

//! Add a single limb to a limb vector in place: dest += src
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_addition_single_limb(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    const apy_limb_t src
)
{
    assert(cbegin_it != cend_it);

    /* src is initial "carry" */
    apy_limb_t carry = src;
    for (auto it = cbegin_it; it != cend_it; ++it) {
        *it += carry;
        carry = (*it < carry);
    }

    return carry;
}

//! Add a single one to a limb vector in place: dest++
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_add_one_lsb(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it, RANDOM_ACCESS_ITERATOR_IN cend_it
)
{
    assert(cbegin_it != cend_it);

    apy_limb_t carry = 1;
    for (auto it = cbegin_it; it != cend_it; ++it) {
        *it += carry;
        carry = (*it < carry);
    }

    return carry;
}

//! Add two limb vectors of the same length in place: dest += src, where len(dest)
//! == len(src)
template <class RANDOM_ACCESS_ITERATOR_INOUT, class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_iterator_addition_same_length(
    RANDOM_ACCESS_ITERATOR_INOUT dest_begin,
    RANDOM_ACCESS_ITERATOR_INOUT dest_end,
    RANDOM_ACCESS_ITERATOR_IN src_begin
)
{
    assert(dest_begin != dest_end);

    apy_limb_t carry = 0;

    auto dest_it = dest_begin;
    auto src_it = src_begin;

    for (; dest_it != dest_end; ++dest_it, ++src_it) {
        add_single_limbs_with_carry(*src_it, *dest_it, &*dest_it, carry, &carry);
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

//! Add two limb vectors of length two in place: dest += src, where len(dest)
//! == len(src) == 2
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_addition_length_two(apy_limb_t* dest, const apy_limb_t* src)
{
    dest[0] += src[0];
    apy_limb_t carry = (dest[0] < src[0]);

    add_single_limbs_with_carry(src[1], dest[1], &dest[1], carry, &carry);
    return carry;
}

// Subtraction

//! Negate a limb vector in place: dest = -dest
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_negate(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it, RANDOM_ACCESS_ITERATOR_IN cend_it
)
{
    assert(cbegin_it != cend_it);

    apy_limb_t carry = 1;
    for (auto it = cbegin_it; it != cend_it; ++it) {
        *it = ~(*it) + carry;
        carry = (*it < carry);
    }

    return carry;
}
//! Negate a limb vector: dest = -src
template <class RANDOM_ACCESS_ITERATOR_IN, class RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_negate(
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    RANDOM_ACCESS_ITERATOR_IN src_end,
    RANDOM_ACCESS_ITERATOR_OUT dest_begin
)
{
    assert(src_begin != src_end);

    apy_limb_t carry = 1;
    auto dest_it = dest_begin;
    auto src_it = src_begin;

    for (; src_it != src_end; ++dest_it, ++src_it) {
        *dest_it = ~(*src_it) + carry;
        carry = (*dest_it < carry);
    }

    return carry;
}

//! Subtract a single limb from a limb vectors in place: dest -= src
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_subtraction_single_limb(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    const apy_limb_t src
)
{
    assert(cbegin_it != cend_it);

    /* src is initial "carry" */
    apy_limb_t carry = src;

    for (auto it = cbegin_it; it != cend_it; ++it) {
        /* Determine carry to next limb */
        apy_limb_t carry_tmp = *it < carry;
        *it -= carry;
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

//! Subtract two limb vectors in place: dest -= src, where len(dest) == len(src)
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_subtraction_same_length(
    RANDOM_ACCESS_ITERATOR_IN dest_begin,
    RANDOM_ACCESS_ITERATOR_IN dest_end,
    RANDOM_ACCESS_ITERATOR_IN src_begin
)
{
    assert(dest_begin != dest_end);

    apy_limb_t carry = 0;
    auto dest_it = dest_begin;
    auto src_it = src_begin;

    for (; dest_it != dest_end; ++dest_it, ++src_it) {
        sub_single_limbs_with_carry(*dest_it, *src_it, &*dest_it, carry, &carry);
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

//! Subtract two limb vectors of length two: dest = src0 - src1, where len(dest) ==
//! len(src0) == len(src1) == 2
[[maybe_unused]] static APY_INLINE apy_limb_t apy_subtraction_length_two(
    apy_limb_t* dest, const apy_limb_t* src0, const apy_limb_t* src1
)
{
    apy_limb_t carry = (src0[0] < src1[0]);
    dest[0] = src0[0] - src1[0];

    sub_single_limbs_with_carry(src0[1], src1[1], &dest[1], carry, &carry);
    return carry;
}

// Shift

//! Left-shift limb vector in place
apy_limb_t apy_inplace_left_shift(apy_limb_t*, const std::size_t, unsigned int);
//! Left-shift limb vector in place
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_left_shift(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    const unsigned int shift_amount
)
{
    assert(cbegin_it != cend_it);
    assert(shift_amount > 0);
    assert(shift_amount < APY_LIMB_SIZE_BITS);

    const unsigned int overlap = APY_LIMB_SIZE_BITS - shift_amount;
    auto it = cend_it;
    --it;
    apy_limb_t high_limb = *it;
    const apy_limb_t retval = high_limb >> overlap;
    apy_limb_t low_limb = (high_limb << shift_amount);
    for (; it != cbegin_it; --it) {
        high_limb = *(it - 1);
        *it = low_limb | (high_limb >> overlap);
        low_limb = (high_limb << shift_amount);
    }
    *cbegin_it = low_limb;

    return retval;
}
//! Left-shift limb vector: dest == src << shift_amount
apy_limb_t apy_left_shift(
    apy_limb_t* dest,
    const apy_limb_t* src,
    const std::size_t limbs,
    const unsigned shift_amount
);

//! Right-shift limb vector in place
apy_limb_t apy_inplace_right_shift(apy_limb_t*, const std::size_t, unsigned int);
//! Right-shift limb vector in place
template <class RANDOM_ACCESS_ITERATOR_IN>
apy_limb_t apy_inplace_right_shift(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    unsigned int shift_amount
)
{
    assert(cbegin_it != cend_it);
    assert(shift_amount > 0);
    assert(shift_amount < APY_LIMB_SIZE_BITS);

    const unsigned int overlap = APY_LIMB_SIZE_BITS - shift_amount;
    apy_limb_t low_limb = *cbegin_it;
    const apy_limb_t retval = (low_limb << overlap);
    apy_limb_t high_limb = low_limb >> shift_amount;
    for (auto it = cbegin_it + 1; it != cend_it; ++it) {
        low_limb = *it;
        *(it - 1) = high_limb | (low_limb << overlap);
        high_limb = low_limb >> shift_amount;
    }
    *(cend_it - 1) = high_limb;

    return retval;
}

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
