/*
 * Utility functions
 */

#ifndef _APYTYPES_UTIL_H
#define _APYTYPES_UTIL_H

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/variant.h>

#include <fmt/format.h>

#include <algorithm>        // std::find, std::unique, etc...
#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <cstdint>          // std::int64_t
#include <functional>       // std::bit_not
#include <initializer_list> // std::initializer_list
#include <iterator>         // std::distance
#include <numeric>          // std::accumulate, std::multiplies
#include <optional>         // std::optional, std::nullopt
#include <regex>            // std::regex, std::regex_replace
#include <sstream>          // std::stringstream
#include <string>           // std::string
#include <tuple>            // std::tuple
#include <type_traits>      // std::is_same_v
#include <vector>           // std::vector

#include "apytypes_common.h"
#include "apytypes_fwd.h"
#include "apytypes_mp.h"

// TODO: Remove me
#include "format.h"

/*!
 * Type trait removing const-volatile qualifiers and reference of `T`. Equivalent to the
 * C++20 trait `std::remove_cvref`.
 */
template <typename T> struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

/*!
 * Type trait removing const-volatile qualifiers and reference of `T`. Equivalent to the
 * C++20 trait `std::remove_cvref_t`.
 */
template <typename T> using remove_cvref_t = typename remove_cvref<T>::type;

/*!
 * Macro for splitting a single `std::uint64_t` into comma-separated limbs
 */
#if COMPILER_LIMB_SIZE == 64
#define UINT64_TO_LIMB(x) apy_limb_t(x)
#else
#define UINT64_TO_LIMB(x)                                                              \
    apy_limb_t(std::uint64_t(x)), apy_limb_t(std::uint64_t(x) >> 32)
#endif

//! Quickly evaluate how many limbs are required to store a `bits` bit word.
//! Undefined behaviour when `bits` is equal to zero.
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
bits_to_limbs(std::size_t bits)
{
    static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);
    assert(bits > 0);

    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        return ((bits - 1) >> 6) + 1;
    } else { /* APY_LIMB_SIZE_BITS == 32 */
        return ((bits - 1) >> 5) + 1;
    }
}

//! Count the number of significant limbs in unsigned limb vector
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
significant_limbs(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    assert(begin <= end);

    auto is_non_zero = [](auto n) { return n != 0; };
    auto back_non_zero_it = std::find_if(
        std::reverse_iterator(end), std::reverse_iterator(begin), is_non_zero
    );
    return std::distance(begin, back_non_zero_it.base());
}

//! Return the number of leading zeros of a limb vector
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
limb_vector_leading_zeros(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    assert(begin <= end);

    auto is_non_zero = [](auto n) { return n != 0; };
    auto rev_non_zero_it = std::find_if(
        std::reverse_iterator(end), std::reverse_iterator(begin), is_non_zero
    );
    std::size_t zero_limbs = std::distance(rev_non_zero_it.base(), end);
    if (rev_non_zero_it.base() == begin) {
        // All limbs are zero limbs
        return APY_LIMB_SIZE_BITS * zero_limbs;
    } else {
        // Some of the limbs are non-zero
        return APY_LIMB_SIZE_BITS * zero_limbs + leading_zeros(*rev_non_zero_it);
    }
}

//! Return the number of trailing zeros of a limb vector
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
limb_vector_trailing_zeros(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    assert(begin <= end);

    auto is_non_zero = [](auto n) { return n != 0; };
    auto non_zero_it = std::find_if(begin, end, is_non_zero);
    std::size_t zero_limbs = std::distance(begin, non_zero_it);
    if (non_zero_it == end) {
        // All limbs are zero limbs
        return APY_LIMB_SIZE_BITS * zero_limbs;
    } else {
        // Some of the limbs are non-zero
        return APY_LIMB_SIZE_BITS * zero_limbs + trailing_zeros_intrinsic(*non_zero_it);
    }
}

//! Return the number of leading signs of a limb vector, minus one.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
limb_vector_leading_ones(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    assert(begin <= end);

    auto is_not_all_ones = [](auto n) { return n != apy_limb_t(-1); };
    auto rev_not_all_ones_it = std::find_if(
        std::reverse_iterator(end), std::reverse_iterator(begin), is_not_all_ones
    );
    std::size_t all_ones_limbs = std::distance(rev_not_all_ones_it.base(), end);
    if (rev_not_all_ones_it.base() == begin) {
        // All limbs are ones limbs
        return APY_LIMB_SIZE_BITS * all_ones_limbs;
    } else {
        // Some of the limbs are not all ones
        return APY_LIMB_SIZE_BITS * all_ones_limbs + leading_ones(*rev_not_all_ones_it);
    }
}

//! Perform arithmetic right shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_asr(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_amnt
)
{
    assert(it_begin < it_end);

    // Return early if no shift or no vector
    if (!shift_amnt) {
        return;
    }

    apy_limb_t sign_limb = apy_limb_signed_t(*(it_end - 1)) >> (APY_LIMB_SIZE_BITS - 1);
    std::size_t vec_size = std::distance(it_begin, it_end);
    unsigned limb_skip = shift_amnt / APY_LIMB_SIZE_BITS;
    if (limb_skip >= vec_size) {
        std::fill(it_begin, it_end, sign_limb);
        return; // early return
    } else if (limb_skip) {
        for (auto it = it_begin; it < it_end - limb_skip; ++it) {
            *it = *(it + limb_skip);
        }
        for (auto it = it_end - limb_skip; it < it_end; ++it) {
            *it = sign_limb;
        }
    }

    // Perform the in-limb shifting
    unsigned limb_shift = shift_amnt % APY_LIMB_SIZE_BITS;
    if (limb_shift) {
        apy_inplace_right_shift(
            it_begin,  // dst/src
            it_end,    // limb vector end
            limb_shift // shift amount
        );

        // Sign extend the most significant bits
        if (sign_limb) {
            *(it_end - 1)
                |= ~((apy_limb_t(1) << (APY_LIMB_SIZE_BITS - limb_shift)) - 1);
        }
    }
}

//! Perform logical right shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_lsr(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_amnt
)
{
    assert(it_begin < it_end);

    // Return early if no shift or no vector
    if (!shift_amnt) {
        return;
    }

    std::size_t vec_size = std::distance(it_begin, it_end);
    unsigned limb_skip = shift_amnt / APY_LIMB_SIZE_BITS;
    if (limb_skip >= vec_size) {
        std::fill(it_begin, it_end, 0);
        return; // early return
    } else if (limb_skip) {
        for (auto it = it_begin; it < it_end - limb_skip; ++it) {
            *it = *(it + limb_skip);
        }
        for (auto it = it_end - limb_skip; it < it_end; ++it) {
            *it = 0;
        }
    }

    unsigned limb_shift = shift_amnt % APY_LIMB_SIZE_BITS;
    // Perform the in-limb shifting
    if (limb_shift) {
        apy_inplace_right_shift(
            it_begin,  // dst/src
            it_end,    // limb vector end
            limb_shift // shift amount
        );
    }
}

//! Perform logical left shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
static APY_INLINE void limb_vector_lsl_inner(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    unsigned int limb_skip,
    unsigned int limb_shift
)
{
    assert(it_begin < it_end);

    if (limb_skip) {
        for (auto it = it_end - 1; it != it_begin + limb_skip - 1; --it) {
            *it = *(it - limb_skip);
        }
        for (auto it = it_begin; it != it_begin + limb_skip; ++it) {
            *it = 0;
        }
    }

    // Perform the in-limb shifting
    if (limb_shift) {
        apy_inplace_left_shift(
            it_begin,  // src/dst
            it_end,    // limb vector size
            limb_shift // shift amount
        );
    }
}

//! Perform logical left shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_lsl(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_amnt
)
{
    assert(it_begin < it_end);

    // Return early if no shift or no vector
    if (!shift_amnt) {
        return;
    }

    unsigned int limb_skip = shift_amnt / APY_LIMB_SIZE_BITS;
    if (std::ptrdiff_t(limb_skip) >= std::distance(it_begin, it_end)) {
        std::fill(it_begin, it_end, 0);
        return; // early return
    }
    unsigned limb_shift = shift_amnt % APY_LIMB_SIZE_BITS;
    limb_vector_lsl_inner(it_begin, it_end, limb_skip, limb_shift);
}

//! Test if positive integer value in limb vector is greater than or equal to a
//! non-negative power-of-two (>= 2 ^ `n` for unsigned `n`)
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_gte_pow2(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned n
)
{
    assert(it_begin <= it_end);

    unsigned bit_idx = n % APY_LIMB_SIZE_BITS;
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    std::size_t n_limbs = std::distance(it_begin, it_end);

    if (limb_idx >= n_limbs) {
        // The power-of-two is outside of the limb vector range
        return false;
    }

    // In the first limb, test for `bit_idx` up-to `APY_LIMB_SIZE_BITS - 1`
    apy_limb_t mask = ~((apy_limb_t(1) << bit_idx) - 1);
    if (mask & it_begin[limb_idx]) {
        return true;
    }

    // In the remaining limbs, test if any bit at all is set
    for (std::size_t i = limb_idx + 1; i < n_limbs; i++) {
        if (it_begin[i]) {
            return true;
        }
    }

    return false;
}

//! Test if the two's complement value of `src1` is smaller than that of `src2`
template <class RANDOM_ACCESS_ITERATOR1, class RANDOM_ACCESS_ITERATOR2>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_signed_less_than(
    RANDOM_ACCESS_ITERATOR1 src1, RANDOM_ACCESS_ITERATOR2 src2, std::size_t limbs
)
{
    static_assert(std::is_same_v<remove_cvref_t<decltype(*src1)>, apy_limb_t>);
    static_assert(std::is_same_v<remove_cvref_t<decltype(*src2)>, apy_limb_t>);
    assert(limbs > 0);

    if (src1[limbs - 1] != src2[limbs - 1]) {
        return apy_limb_signed_t(src1[limbs - 1]) < apy_limb_signed_t(src2[limbs - 1]);
    } else {
        for (std::size_t i = limbs - 1; i--;) {
            if (src1[i] != src2[i]) {
                bool is_negative = apy_limb_signed_t(src1[limbs - 1]) < 0;
                return is_negative
                    ? apy_limb_signed_t(src1[i]) < apy_limb_signed_t(src2[i])
                    : apy_limb_t(src1[i]) < apy_limb_t(src2[i]);
            }
        }
    }
    return false;
}

//! Add a power-of-two (2 ^ `n`) onto a limb vector. Return carry out.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_add_pow2(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned n
)
{
    assert(it_begin < it_end);

    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    std::size_t limbs = std::distance(it_begin, it_end);
    if (limb_idx < limbs) {
        apy_limb_t term_limb = apy_limb_t(1) << (n % APY_LIMB_SIZE_BITS);
        return apy_inplace_addition_single_limb(
            it_begin + limb_idx, // dst
            it_end,              // dst end
            term_limb            // src2
        );
    }
    return 0;
}

//! Return the `bits` specifier from user provided optional bit specifiers.
//! Throws `nb::value_error` if the resulting number of bits is less than or equal to
//! zero, or if not exactly two of three bit specifiers are set.
[[maybe_unused, nodiscard]] static APY_INLINE int bits_from_optional(
    std::optional<int> bits, std::optional<int> int_bits, std::optional<int> frac_bits
)
{
    int n_bit_spec = bits.has_value() + int_bits.has_value() + frac_bits.has_value();
    if (n_bit_spec != 2) {
        throw nanobind::value_error(
            "Fixed-point bit specification needs exactly two of three bit specifiers "
            "(`bits`, `int_bits`, `frac_bits`) set"
        );
    }

    int result = bits.has_value() ? *bits : *int_bits + *frac_bits;
    if (result <= 0) {
        throw nanobind::value_error(
            "Fixed-point bit specification needs a positive integer bit-size (>= 1 bit)"
        );
    }

    return result;
}

//! Return a two-tuple of ints (`bits`, `int_bits`) for a set of optional
//! bit-specifiers used in the casting context. Throws `nanobind::value_error` if
//! parameters do not add up, or if resulting `bits` is smaller than or equal to zero.
[[maybe_unused, nodiscard]] static APY_INLINE std::tuple<int, int>
bits_from_optional_cast(
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    int current_bits,
    int current_int_bits
)
{
    std::optional<std::tuple<int, int>> result = std::nullopt;
    int n_bit_spec = bits.has_value() + int_bits.has_value() + frac_bits.has_value();
    if (n_bit_spec == 1 && !bits.has_value()) {
        if (int_bits.has_value()) {
            result = { current_bits - current_int_bits + *int_bits, *int_bits };
        } else { /* frac_bits.has_value() */
            result = { current_int_bits + *frac_bits, current_int_bits };
        }
    } else if (n_bit_spec == 2) {
        if (bits.has_value()) {
            result = { *bits, int_bits.has_value() ? *int_bits : *bits - *frac_bits };
        } else { /* int_bits.has_value() && frac_bits.has_value() */
            result = { *int_bits + *frac_bits, *int_bits };
        }
    }

    if (!result.has_value() || std::get<0>(*result) <= 0) {
        throw nanobind::value_error(
            "Fixed-point casting bit specification needs exactly one of `int_bits` or "
            "`frac_bits` set, or exactly two of three specifiers (`bits`, `int_bits`, "
            "`frac_bits`) set"
        );
    }

    return *result;
}

//! Test if two's complement value in a limb vector is negative
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_is_negative(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it
)
{
    (void)cbegin_it;
    assert(cbegin_it < cend_it);
    return apy_limb_signed_t(*std::prev(cend_it)) < 0;
}

//! Test if the stored value of a limb vector is zero (all limbs are zero)
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool
limb_vector_is_zero(RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it)
{
    assert(cbegin_it <= cend_it);
    return std::all_of(cbegin_it, cend_it, [](auto n) { return n == 0; });
}

//! Reduce the first `n` bits in a limb vector over bitwise `or`. Return bool.
//! Unconditionally Return `false` when `n == 0`. Undefined behaviour if `n` is
//! greater than the number of bits in the limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_or_reduce(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n
)
{
    (void)cend_it;
    assert(cbegin_it < cend_it);
    assert(std::distance(cbegin_it, cend_it) >= (n ? bits_to_limbs(n) : 0));

    // The full limbs can be reduced as full integers
    const unsigned full_limbs = n / APY_LIMB_SIZE_BITS;
    for (auto limb_it = cbegin_it; limb_it != cbegin_it + full_limbs; ++limb_it) {
        if (*limb_it != 0) {
            return true;
        }
    }

    const unsigned last_limb_bits = n % APY_LIMB_SIZE_BITS;
    // The last limb must be masked
    if (last_limb_bits) {
        apy_limb_t last_limb = *(cbegin_it + full_limbs);
        apy_limb_t limb_mask = (apy_limb_t(1) << last_limb_bits) - 1;
        if (last_limb & limb_mask) {
            return true;
        }
    }

    return false;
}

//! Test if the `n` -th bit (zero indexed) is set in a limb vector. Undefined behaviour
//! if bit `n` is located outside the limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_test_bit(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n
)
{
    (void)cend_it;
    assert(cbegin_it < cend_it);
    assert(std::distance(cbegin_it, cend_it) >= (n ? bits_to_limbs(n) : 0));

    unsigned bit_idx = n % APY_LIMB_SIZE_BITS;
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    apy_limb_t mask = apy_limb_t(1) << bit_idx;
    apy_limb_t limb = cbegin_it[limb_idx];
    return mask & limb;
}

//! Set the `n` -th bit (zero indexed) of a limb vector to `bit`. Undefined behaviour if
//! bit `n` is located outside the limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_set_bit(
    RANDOM_ACCESS_ITERATOR begin_it, RANDOM_ACCESS_ITERATOR end_it, unsigned n, bool bit
)
{
    (void)end_it;
    assert(begin_it < end_it);

    unsigned bit_idx = n % APY_LIMB_SIZE_BITS;
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    apy_limb_t bit_mask = apy_limb_t(1) << bit_idx;
    apy_limb_t bit_unmask = ~bit_mask;
    begin_it[limb_idx]
        = (begin_it[limb_idx] & bit_unmask) | (apy_limb_t(bit) << bit_idx);
}

//! Compute two's complement negative value and place onto `res_out`
template <class RANDOM_ACCESS_ITERATOR_IN, class RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_negate(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    RANDOM_ACCESS_ITERATOR_OUT res_it
)
{
    assert(cbegin_it < cend_it);
    return apy_negate(cbegin_it, cend_it, res_it);
}

//! Take the two's complement negative value of a limb vector inplace
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_negate_inplace(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it, RANDOM_ACCESS_ITERATOR_IN cend_it
)
{
    assert(cbegin_it < cend_it);
    return apy_inplace_negate(cbegin_it, cend_it);
}

//! Add an LSB to a limb vector inplace
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_add_one_lsb_inplace(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it, RANDOM_ACCESS_ITERATOR_IN cend_it
)
{
    assert(cbegin_it < cend_it);
    return apy_inplace_add_one_lsb(cbegin_it, cend_it);
}

//! Take the two's complement absolute value of a limb vector and place onto `res_out`.
//! Return `true` if the argument is negative, and `false` otherwise.
template <class RANDOM_ACCESS_ITERATOR_IN, class RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE bool limb_vector_abs(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    RANDOM_ACCESS_ITERATOR_OUT res_it
)
{
    bool is_negative = apy_limb_signed_t(*std::prev(cend_it)) < 0;
    if (is_negative) {
        limb_vector_negate(cbegin_it, cend_it, res_it);
    } else {
        std::copy(cbegin_it, cend_it, res_it);
    }
    return is_negative;
}

//! Test if the bits in a limb vector are all zeros starting from bit `n` (zero
//! indexed). Bit `n` is assumed to be located in the first limb.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE bool limb_vector_all_zeros(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n = 0
)
{
    apy_limb_t mask = ~((apy_limb_t(1) << n) - 1);
    if (*cbegin_it & mask) {
        // One or more bits in the masked first limb are non-zero
        return false;
    } else {
        // Test if remaining limbs are all full zero limbs
        for (auto it = cbegin_it + 1; it != cend_it; ++it) {
            if (*it != 0) {
                return false;
            }
        }
        return true; // All zeros
    }
}

//! Test if the bits in a limb vector are all ones starting from bit `n` (zero indexed).
//! Bit `n` is assumed to be located in the first limb.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE bool limb_vector_all_ones(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n = 0
)
{
    apy_limb_t mask = ~((apy_limb_t(1) << n) - 1);
    if (~*cbegin_it & mask) {
        // One or more bits in the masked first limb are non-ones
        return false;
    } else {
        // Test if remaining limbs are all full one limbs
        for (auto it = cbegin_it + 1; it != cend_it; ++it) {
            if (*it != apy_limb_t(-1)) {
                return false;
            }
        }
        return true; // All ones
    }
}

//! Copy limbs from `src` to `dst` and possibly sign extend the data in `dst`
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE void limb_vector_copy_n_sign_extend(
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    std::size_t src_n,
    RANDOM_ACCESS_ITERATOR_OUT dst_begin,
    std::size_t dst_n
)
{
    std::copy_n(src_begin, std::min(src_n, dst_n), dst_begin);
    if (src_n < dst_n) {
        bool is_negative = limb_vector_is_negative(src_begin, src_begin + src_n);
        std::fill_n(dst_begin + src_n, dst_n - src_n, is_negative ? -1 : 0);
    }
}

//! Copy limbs from `src` to `dst` and possibly sign extend the data in `dst`
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE void limb_vector_copy_sign_extend(
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    RANDOM_ACCESS_ITERATOR_IN src_end,
    RANDOM_ACCESS_ITERATOR_OUT dst_begin,
    RANDOM_ACCESS_ITERATOR_OUT dst_end
)
{
    assert(src_begin <= src_end);
    assert(dst_begin <= dst_end);
    std::size_t src_n = std::distance(src_begin, src_end);
    std::size_t dst_n = std::distance(dst_begin, dst_end);
    limb_vector_copy_n_sign_extend(src_begin, src_n, dst_begin, dst_n);
}

//! Read an unsigned 64-bit value from a limb vector. If `APY_LIMB_SIZE_BITS == 64`,
//! this results in a normal vector read without bounds checking. If `APY_LIMB_SIZE_BITS
//! == 32`, the second 32-bits limb is bound-checked and the result upper 32-bits are
//! zeroed if out-of-bounds.
template <typename VECTOR_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::uint64_t
limb_vector_to_uint64(const VECTOR_TYPE& limb_vec, std::size_t n)
{
    static_assert(APY_LIMB_SIZE_BITS == 32 || APY_LIMB_SIZE_BITS == 64);
    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        // No bound-checking for 64-bit limbs
        return std::uint64_t(limb_vec[n]);
    } else { /* APY_LIMB_SIZE_BITS == 32 */
        if (n + 1 < limb_vec.size()) {
            return std::uint64_t(limb_vec[n]) | (std::uint64_t(limb_vec[n + 1]) << 32);
        } else {
            return std::uint64_t(limb_vec[n]);
        }
    }
}

//! Convert a limb vector into a 64-bit limb vector. This is useful to get consistent
//! pickling support between 64-bit and 32-bit platforms.
template <typename RANDOM_ACCESS_IT>
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<std::uint64_t>
limb_vector_to_u64_vec(RANDOM_ACCESS_IT begin_it, RANDOM_ACCESS_IT end_it)
{
    static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);
    assert(begin_it <= end_it);

    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        return std::vector<uint64_t>(begin_it, end_it);
    } else { /* APY_LIMB_SIZE_BITS == 32 */
        std::size_t src_nitems = std::distance(begin_it, end_it);
        std::vector<uint64_t> limb64_vec(src_nitems / 2 + src_nitems % 2);
        for (std::size_t i = 0; i < src_nitems / 2; i++) {
            std::uint64_t limb64_hi = std::uint64_t(*(begin_it + 2 * i + 1)) << 32;
            std::uint64_t limb64_lo = std::uint64_t(*(begin_it + 2 * i + 0)) << 0;
            limb64_vec[i] = limb64_hi | limb64_lo;
        }
        if (src_nitems % 2) {
            std::uint64_t limb64 = std::int64_t(apy_limb_signed_t(*(end_it - 1)));
            limb64_vec.back() = limb64;
        }
        return limb64_vec;
    }
}

template <typename VEC_RETURN_TYPE, typename RANDOM_ACCESS_IT>
[[maybe_unused, nodiscard]] static APY_INLINE VEC_RETURN_TYPE
limb_vector_from_u64_vec(RANDOM_ACCESS_IT begin_it, RANDOM_ACCESS_IT end_it)
{
    static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);
    assert(begin_it <= end_it);

    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        return VEC_RETURN_TYPE(begin_it, end_it);
    } else {
        std::size_t n_limbs = 2 * std::distance(begin_it, end_it);
        VEC_RETURN_TYPE res(n_limbs);
        for (std::size_t i = 0; i < n_limbs / 2; i++) {
            res[2 * i + 0] = (*(begin_it + i)) >> 0;
            res[2 * i + 1] = (*(begin_it + i)) >> 32;
        }
        return res;
    }
}

template <typename VEC_RETURN_TYPE, typename VEC_INPUT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE VEC_RETURN_TYPE
limb_vector_from_u64_vec(const VEC_INPUT_TYPE& vec)
{
    return limb_vector_from_u64_vec<VEC_RETURN_TYPE>(std::begin(vec), std::end(vec));
}

//! Construct a Python tuple-literal string from a vector of `T`. The type `T` must be
//! convertible to string through both a `std::stringstream` and `fmt::format({})`.
template <typename T> std::string tuple_string_from_vec(const std::vector<T>& vec)
{
    if (vec.size() == 0) {
        return "()";
    } else if (vec.size() == 1) {
        return fmt::format("({},)", vec[0]);
    } else {
        std::stringstream ss;
        ss << "(";
        for (auto& d : vec) {
            ss << d << ", ";
        }
        return ss.str().substr(0, ss.str().length() - 2).append(")");
    }
}

//! Fold a shape under multiplication
[[maybe_unused]] static APY_INLINE std::size_t fold_shape(
    std::vector<std::size_t>::const_iterator cbegin,
    std::vector<std::size_t>::const_iterator cend
)
{
    return std::accumulate(cbegin, cend, 1, std::multiplies {});
}

//! Fold a shape under multiplication
[[maybe_unused]] static APY_INLINE std::size_t
fold_shape(const std::vector<std::size_t>& shape)
{
    return fold_shape(std::cbegin(shape), std::cend(shape));
}

//! Compute the strides from a shape
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
strides_from_shape(const std::vector<std::size_t>& shape, std::size_t acc_base = 1)
{
    std::vector<std::size_t> strides(shape.size(), 0);
    for (std::size_t i = 0; i < shape.size(); i++) {
        strides[shape.size() - 1 - i] = std::accumulate(
            shape.crbegin(), shape.crbegin() + i, acc_base, std::multiplies {}
        );
    }
    return strides;
}

//! In-place multi-limb swap function
template <typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void multi_limb_swap(
    RANDOM_ACCESS_ITERATOR_INOUT a_it,
    RANDOM_ACCESS_ITERATOR_INOUT b_it,
    std::size_t itemsize
)
{
    for (std::size_t i = 0; i < itemsize; i++) {
        std::swap(*(a_it + i), *(b_it + i));
    }
}

//! In-place multi-limb reverse function
template <typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void multi_limb_reverse(
    RANDOM_ACCESS_ITERATOR_INOUT begin_it,
    RANDOM_ACCESS_ITERATOR_INOUT end_it,
    std::size_t itemsize
)
{
    assert(begin_it <= end_it);
    auto n_items = std::distance(begin_it, end_it) / itemsize;
    for (std::size_t i = 0; i < (n_items + 1) / 2; i++) {
        auto it1 = begin_it + (i * itemsize);
        auto it2 = begin_it + itemsize * (n_items - i - 1);
        multi_limb_swap(it1, it2, itemsize);
    }
}

//! Get the convolutional properties `len`, `n_left`, and `n_right` (in that order) from
//! a string of either "full", "same", or "valid". Throws a `nanobind::value_error` if
//! the string is anything else.
template <typename APY_ARRAY>
[[maybe_unused]] static APY_INLINE std::tuple<std::size_t, std::size_t, std::size_t>
get_conv_lengths(const ConvolutionMode mode, const APY_ARRAY& a, const APY_ARRAY& b)
{
    std::size_t len, n_left, n_right;
    switch (mode) {
    case ConvolutionMode::FULL:
        len = a->shape()[0] + b->shape()[0] - 1;
        n_left = b->shape()[0] - 1;
        n_right = b->shape()[0] - 1;
        break;
    case ConvolutionMode::SAME:
        len = a->shape()[0];
        n_left = b->shape()[0] / 2;
        n_right = b->shape()[0] - n_left - 1;
        break;
    case ConvolutionMode::VALID:
        len = a->shape()[0] - b->shape()[0] + 1;
        n_left = 0;
        n_right = 0;
        break;
    default:
        throw nanobind::value_error("Unknown convolution mode. Did you pass an 'int'?");
    }
    return { len, n_left, n_right };
}

//! Get the convolutional mode from a string of either "full", "same", or "valid".
//! Throws a `nanobind::value_error` if the string is anything else.
[[maybe_unused]] static APY_INLINE ConvolutionMode
get_conv_mode(const std::string& mode)
{
    if (mode == "full") {
        return ConvolutionMode::FULL;
    } else if (mode == "same") {
        return ConvolutionMode::SAME;
    } else if (mode == "valid") {
        return ConvolutionMode::VALID;
    } else {
        auto msg = fmt::format("mode='{}' not in 'full', 'same', or 'valid'", mode);
        throw nanobind::value_error(msg.c_str());
    }
}

//! Macro for creating a void-specialization state-less functor `FUNCTOR_NAME` from a
//! function `FUNC_NAME`. The void-specialization functor allows template argument
//! deduction to be performed once its function is called. Neat!
#define CREATE_FUNCTOR_FROM_FUNC(FUNCTOR_NAME, FUNC_NAME)                              \
    template <typename... T> struct FUNCTOR_NAME { };                                  \
    template <> struct FUNCTOR_NAME<> {                                                \
        template <typename... T> auto operator()(T... t) const                         \
        {                                                                              \
            return FUNC_NAME(t...);                                                    \
        }                                                                              \
    }

CREATE_FUNCTOR_FROM_FUNC(apy_add_n_functor, apy_addition_same_length);
CREATE_FUNCTOR_FROM_FUNC(apy_sub_n_functor, apy_subtraction_same_length);

CREATE_FUNCTOR_FROM_FUNC(apy_add_2_functor, apy_addition_length_two);
CREATE_FUNCTOR_FROM_FUNC(apy_sub_2_functor, apy_subtraction_length_two);

//! Type-dependant `false` value. Can be used as a `false` value in a static assertion
//! as `static_assert(always_false_v)` without triggering ill-formed behaviour.
template <typename> inline constexpr bool always_false_v = false;

//! Test if two types `T` and `U` are equal, ignoring references and const-volatile
//! qualifiers. The expression `is_same_type<T, U>::value` equals `true` if `T` and `V`
//! are equal types and `false` otherwise.
template <typename T, typename U> struct is_same_type {
    static constexpr bool value = std::is_same_v<
        std::remove_cv_t<std::remove_reference_t<T>>,
        std::remove_cv_t<std::remove_reference_t<U>>>;
};

template <typename T, typename U>
constexpr bool is_same_type_v = is_same_type<T, U>::value;

#endif // _APYTYPES_UTIL_H
