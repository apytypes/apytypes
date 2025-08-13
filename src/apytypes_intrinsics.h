#ifndef _APYTYPES_INTRINSICS
#define _APYTYPES_INTRINSICS

#include <cassert>
#include <cstddef>
#include <cstdint>

/*
 * Include Microsoft intrinsics if using Microsoft Visual C/C++ compiler
 */
#if defined(_MSC_VER)
#include <intrin.h>
#pragma intrinsic(_umul128)
#endif

#if defined(__x86_64__)
#if defined(__GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
static_assert(
    false,
    "add_single_limbs_with_carry(): No intrinsic available on your platform. "
    "Please open an issue at https://github.com/apytypes/apytypes/issues with "
    "information about the compiler and platform and we will be happy to add "
    "support for it."
);
#endif
#endif
#include "apytypes_fwd.h"

//! Mark code path as unreachable, allowing the compiler to make more aggressive
//! control-flow optimizations. Calling this macro in debug builds triggers an assertion
//! failure. Calling this macro in release builds causes undefined behaviour.
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
#define APYTYPES_UNREACHABLE()                                                         \
    assert(false);                                                                     \
    __assume(false)
#else // GCC, Clang
#define APYTYPES_UNREACHABLE()                                                         \
    assert(false);                                                                     \
    __builtin_unreachable()
#endif

//! Compute unsigned product of two `apy_limb_t`, obtaining two-limb product
[[maybe_unused, nodiscard]] static APY_INLINE std::tuple<apy_limb_t, apy_limb_t>
long_unsigned_mult(apy_limb_t src0, apy_limb_t src1)
{
#if (COMPILER_LIMB_SIZE == 32)
    std::uint64_t res = (std::uint64_t)src0 * (std::uint64_t)src1;
    apy_limb_t high_limb = apy_limb_t(res >> 32);
    return { high_limb, apy_limb_t(res) };
#elif (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
    /*
     * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++ compiler
     * (ICC).
     */
    unsigned __int128 res = (unsigned __int128)src0 * (unsigned __int128)src1;
    apy_limb_t high_limb = apy_limb_t(res >> 64);
    return { high_limb, apy_limb_t(res) };
#elif defined(_MSC_VER)
    // Microsoft Visual C/C++ compiler
    apy_limb_t high_limb;
    apy_limb_t low_limb = apy_limb_t(_umul128(src0, src1, &high_limb));
    return { high_limb, low_limb };
#else
    // No 128-bit multiplication intrinsic found. We could implement this function,
    // but fail for now so we can clearly see which systems are missing out on these
    // intrinsics.
    static_assert(
        false,
        "long_unsigned_mult(): No intrinsic available on your compiler. Please "
        "open an issue at https://github.com/apytypes/apytypes/issues with "
        "information about the compiler and platform and we will be happy to add "
        "support for it."
    );
#endif
#else
    static_assert(false, "COMPILER_LIMB_SIZE must be 32 or 64");
    return 0;
#endif
}

//! Compute signed product of two `apy_limb_t`, obtaining two-limb product
[[maybe_unused, nodiscard]] static APY_INLINE std::tuple<apy_limb_t, apy_limb_t>
long_signed_mult(apy_limb_t src0, apy_limb_t src1)
{
#if (COMPILER_LIMB_SIZE == 32)
    std::int64_t res
        = (std::int64_t)apy_limb_signed_t(src0) * (std::int64_t)apy_limb_signed_t(src1);
    apy_limb_t high_limb = apy_limb_t(res >> 32);
    return { high_limb, apy_limb_t(res) };
#elif (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
    /*
     * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++ compiler
     * (ICC).
     */
    __int128 res
        = (__int128)apy_limb_signed_t(src0) * (__int128)apy_limb_signed_t(src1);
    apy_limb_signed_t high_limb = apy_limb_signed_t(res >> 64);
    return { apy_limb_t(high_limb), apy_limb_t(res) };
#elif defined(_MSC_VER)
    // Microsoft Visual C/C++ compiler
    apy_limb_signed_t high_limb;
    apy_limb_signed_t low_limb = apy_limb_signed_t(
        _mul128(apy_limb_signed_t(src0), apy_limb_signed_t(src1), &high_limb)
    );
    return { apy_limb_t(high_limb), apy_limb_t(low_limb) };
#else
    // No 128-bit multiplication intrinsic found. We could implement this function,
    // but fail for now so we can clearly see which systems are missing out on these
    // intrinsics.
    static_assert(
        false,
        "long_signed_mult(): No intrinsic available on your compiler. Please "
        "open an issue at https://github.com/apytypes/apytypes/issues with "
        "information about the compiler and platform and we will be happy to add "
        "support for it."
    );
#endif
#else
    static_assert(false, "COMPILER_LIMB_SIZE must be 32 or 64");
    return 0;
#endif
}

//! Compute addition between two apy_limb_t and a carry in, return carry out
static APY_INLINE void add_single_limbs_with_carry(
    apy_limb_t src0,
    apy_limb_t src1,
    apy_limb_t* dest,
    apy_limb_t carry_in,
    apy_limb_t* carry_out
)
{
// If using clang or new enough gcc
#if (defined(__GNUC__) && (__GNUC__ >= 14))                                            \
    || (defined(__clang__)                                                             \
        && ((__clang_major__ >= 4)                                                     \
            || ((__clang_major__ == 3) && (__clang_minor__ >= 3))))
    if constexpr (std::is_same_v<apy_limb_t, unsigned int> == true) {
        *dest = __builtin_addc(src0, src1, carry_in, (unsigned int*)carry_out);
        return;
    } else if constexpr (std::is_same_v<apy_limb_t, unsigned long int> == true) {
        *dest = __builtin_addcl(src0, src1, carry_in, (unsigned long int*)carry_out);
        return;
    } else if constexpr (std::is_same_v<apy_limb_t, unsigned long long int> == true) {
        *dest = __builtin_addcll(
            src0, src1, carry_in, (unsigned long long int*)carry_out
        );
        return;
    }
#endif
#if defined(__x86_64__)
// If x86_64 and the above does not hold
// At least GCC 5 required, but fine as we require C++17
// MSVC will also end up here
#if (COMPILER_LIMB_SIZE == 32)
    *carry_out = _addcarry_u32(carry_in, src0, src1, dest);
    return;
#elif (COMPILER_LIMB_SIZE == 64)
    *carry_out = _addcarry_u64(carry_in, src0, src1, (long long unsigned int*)dest);
    return;
#else
    static_assert(false, "COMPILER_LIMB_SIZE must be 32 or 64");
#endif
#endif
    apy_limb_t tmp_res = src0 + carry_in;
    *carry_out = (tmp_res < carry_in);
    tmp_res += src1;
    *carry_out += (tmp_res < src1);
    *dest = tmp_res;
}

//! Compute subtraction between two apy_limb_t and a carry in, return carry out
static APY_INLINE void sub_single_limbs_with_carry(
    apy_limb_t src0,
    apy_limb_t src1,
    apy_limb_t* dest,
    apy_limb_t carry_in,
    apy_limb_t* carry_out
)
{
// If using clang or new enough gcc
#if (defined(__GNUC__) && (__GNUC__ >= 14))                                            \
    || (defined(__clang__)                                                             \
        && ((__clang_major__ >= 4)                                                     \
            || ((__clang_major__ == 3) && (__clang_minor__ >= 3))))
    if constexpr (std::is_same_v<apy_limb_t, unsigned int> == true) {
        *dest = __builtin_subc(src0, src1, carry_in, (unsigned int*)carry_out);
        return;
    } else if constexpr (std::is_same_v<apy_limb_t, long unsigned int> == true) {
        *dest = __builtin_subcl(src0, src1, carry_in, (unsigned long int*)carry_out);
        return;
    } else if constexpr (std::is_same_v<apy_limb_t, long long unsigned int> == true) {
        *dest = __builtin_subcll(
            src0, src1, carry_in, (unsigned long long int*)carry_out
        );
        return;
    }
#endif
#if defined(__x86_64__)
// If x86_64 and the above does not hold
// At least GCC 5 required, but fine as we require C++17
// MSVC will also end up here
#if (COMPILER_LIMB_SIZE == 32)
    *carry_out = _subborrow_u32(carry_in, src0, src1, dest);
    return;
#elif (COMPILER_LIMB_SIZE == 64)
    *carry_out = _subborrow_u64(carry_in, src0, src1, (long long unsigned int*)dest);
    return;
#else
    static_assert(false, "COMPILER_LIMB_SIZE must be 32 or 64");
#endif
#endif
    apy_limb_t tmp_src1 = src1 + carry_in;
    *carry_out = (tmp_src1 < carry_in) + (src0 < tmp_src1);
    *dest = src0 - tmp_src1;
}

//! Compute the number of trailing zeros in an integer
template <typename INT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t trailing_zeros(INT_TYPE n)
{
    static_assert(
        sizeof(INT_TYPE) == 8 || sizeof(INT_TYPE) == 4,
        "leading_zeros(INT_TYPE n): int type must be 32-bit or 64-bit"
    );
#if defined(__GNUC__)
    /*
     * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++ compiler
     * (ICC).
     */
    if constexpr (sizeof(INT_TYPE) == 8) {
        return n == 0 ? 0 : __builtin_ctzll(n);
    } else {
        return n == 0 ? 0 : __builtin_ctz(n);
    }
#elif defined(_MSC_VER)
    // Microsoft Visual C/C++ compiler
    unsigned long trailing_zero = 0;
    if constexpr (sizeof(INT_TYPE) == 8) {
        _BitScanForward64(&trailing_zero, n);
        return trailing_zero;
    } else {
        _BitScanForward(&trailing_zero, n);
        return trailing_zero;
    }
#else
    // No trailing zeros intrinsic found. We could implement this function using a
    // bit-counting while-loop, but fail for now so we can clearly see which systems are
    // missing out on these intrinsics.
    static_assert(
        false,
        "trailing_zeros(INT_TYPE n): No intrinsic available for your compiler. Please "
        "open an issue at https://github.com/apytypes/apytypes/issues with information "
        "about the compiler and platform and we will be happy to add support for it."
    );
#endif
}

//! Compute the number of leading zeros in a 32-bit or 64-bit integer of type `INT_TYPE`
template <typename INT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t leading_zeros(INT_TYPE n)
{
    static_assert(
        sizeof(INT_TYPE) == 8 || sizeof(INT_TYPE) == 4,
        "leading_zeros(INT_TYPE n): int type must be 32-bit or 64-bit"
    );
#if defined(__GNUC__)
    /*
     * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++ compiler
     * (ICC).
     */
    if constexpr (sizeof(INT_TYPE) == 8) {
        return n == 0 ? 64 : __builtin_clzll(n);
    } else {
        return n == 0 ? 32 : __builtin_clz(n);
    }
#elif defined(_MSC_VER)
    // Microsoft Visual C/C++ compiler
    if constexpr (sizeof(INT_TYPE) == 8) {
        return __lzcnt64(n);
    } else {
        return __lzcnt(n);
    }
#else
    // No leading zeros intrinsic found. We could implement this function using a
    // bit-counting while-loop, but fail for now so we can clearly see which systems are
    // missing out on these intrinsics.
    static_assert(
        false,
        "leading_zeros(INT_TYPE n): No intrinsic available for your compiler. Please "
        "open an issue at https://github.com/apytypes/apytypes/issues with information "
        "about the platform and we will be happy to add support for it."
    );
#endif
}

//! Compute number of leading ones in an integer
template <typename INT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t leading_ones(INT_TYPE n)
{
    return leading_zeros(~n);
}

//! Compute bit-width (`ceil(log2(1 + n))` for unsigned `n`) of an integer `n`.
template <typename INT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t bit_width(INT_TYPE n)
{
    return POSIX_CHAR_BITS * sizeof(INT_TYPE) - leading_zeros(n);
}

//! Compute number of trailing bits after most significant `1` in an integer
template <typename INT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
count_trailing_bits(INT_TYPE n)
{
    return bit_width(n) - 1;
}

#endif
