/*
 * Forward declarations of the important APyTypes C++ types, and their relationships.
 */

#ifndef _APYTYPES_FWD
#define _APYTYPES_FWD

#include <cstddef>
#include <cstdint>
#include <tuple>

/*
 * Conditional inlining of utility functions if profiling `_APY_PROFILING`
 */
#ifdef _APY_PROFILING
#define APY_INLINE
#else
#define APY_INLINE inline
#endif

/* ********************************************************************************** *
 * *                     The fundamental APyTypes limb type                         * *
 * ********************************************************************************** */

#if !defined(COMPILER_LIMB_SIZE)
#error "C Macro `COMPILER_LIMB_SIZE` not specified. Must be set during compilation."
#else /* defined(COMPILER_LIMB_SIZE) */
#if COMPILER_LIMB_SIZE == 32
using apy_limb_t = std::uint32_t;
using apy_limb_signed_t = std::int32_t;
#elif COMPILER_LIMB_SIZE == 64
using apy_limb_t = std::uint64_t;
using apy_limb_signed_t = std::int64_t;
#elif COMPILER_LIMB_SIZE == NATIVE
#if SIZE_MAX == 4294967295ull /* 32-bit system detected */
using apy_limb_t = std::uint32_t;
using apy_limb_signed_t = std::int32_t;
#undef COMPILER_LIMB_SIZE
#define COMPILER_LIMB_SIZE 32
#elif SIZE_MAX == 18446744073709551615ull /* 64-bit system detected */
using apy_limb_t = std::uint64_t;
using apy_limb_signed_t = std::int64_t;
#undef COMPILER_LIMB_SIZE
#define COMPILER_LIMB_SIZE 64
#else
#error "Cannot detect native target architecture word size."
#endif
#else
#error "C Macro `COMPILER_LIMB_SIZE` must be one of `32`, `64` or `NATIVE`"
#endif

#endif

//! Number of bits in a byte, as defined by POSIX
#define POSIX_CHAR_BITS 8

//! Number of bytes in a limb
constexpr std::size_t APY_LIMB_SIZE_BYTES = sizeof(apy_limb_t);
constexpr apy_limb_t APY_NUMBER_MASK = ~((apy_limb_t)0);

//! Number of bits in a limb
constexpr std::size_t APY_LIMB_SIZE_BITS = POSIX_CHAR_BITS * APY_LIMB_SIZE_BYTES;

//! Number of bits in a half limb
constexpr std::size_t APY_HALF_LIMB_SIZE_BITS = APY_LIMB_SIZE_BITS / 2;

//! Mask for lower half of a limb
constexpr apy_limb_t APY_LOWER_LIMB_MASK
    = ((apy_limb_t)1 << APY_HALF_LIMB_SIZE_BITS) - 1;
constexpr apy_limb_t endian_test = (((apy_limb_t)1) << (APY_LIMB_SIZE_BITS - 7)) - 1;

#define HOST_ENDIAN (*(signed char*)&endian_test)

constexpr std::size_t NIBBLES_PER_LIMB = 2 * APY_LIMB_SIZE_BYTES;
constexpr std::size_t BITS_PER_NIBBLE = 4;

// Only used for asserts
constexpr apy_limb_t APY_LIMB_MSBWEIGHT = ((apy_limb_t)1 << (APY_LIMB_SIZE_BITS - 1));

using apy_size_t = std::ptrdiff_t;

/* ********************************************************************************** *
 * *                            APyTypes type relationships                         * *
 * ********************************************************************************** */

// Scalar types
class APyFixed;
class APyFloat;
class APyCFixed;
class APyCFloat;

// Array types
class APyFixedArray;
class APyFloatArray;
class APyCFixedArray;
class APyCFloatArray;

template <typename T> struct scalar_variant;
template <typename T> struct array_variant;

// APyFixed
template <> struct scalar_variant<APyFixedArray> {
    using type = APyFixed;
};
template <> struct scalar_variant<APyFixed> {
    using type = APyFixed;
};
template <> struct array_variant<APyFixedArray> {
    using type = APyFixedArray;
};
template <> struct array_variant<APyFixed> {
    using type = APyFixedArray;
};

// APyFloat
template <> struct scalar_variant<APyFloatArray> {
    using type = APyFloat;
};
template <> struct scalar_variant<APyFloat> {
    using type = APyFloat;
};
template <> struct array_variant<APyFloatArray> {
    using type = APyFloatArray;
};
template <> struct array_variant<APyFloat> {
    using type = APyFloatArray;
};

// APyCFixed
template <> struct scalar_variant<APyCFixedArray> {
    using type = APyCFixed;
};
template <> struct scalar_variant<APyCFixed> {
    using type = APyCFixed;
};
template <> struct array_variant<APyCFixedArray> {
    using type = APyCFixedArray;
};
template <> struct array_variant<APyCFixed> {
    using type = APyCFixedArray;
};

// APyCFloat
template <> struct scalar_variant<APyCFloatArray> {
    using type = APyCFloat;
};
template <> struct scalar_variant<APyCFloat> {
    using type = APyCFloat;
};
template <> struct array_variant<APyCFloatArray> {
    using type = APyCFloatArray;
};
template <> struct array_variant<APyCFloat> {
    using type = APyCFloatArray;
};

template <typename T> using scalar_variant_t = typename scalar_variant<T>::type;
template <typename T> using array_variant_t = typename array_variant<T>::type;

/* ********************************************************************************** *
 * *                           APyFloat C++ data types                              * *
 * ********************************************************************************** */

using exp_t = std::uint32_t;
using man_t = std::uint64_t;

struct APyFloatData {
    bool sign; //! Sign bit
    exp_t exp; //! Biased exponent
    man_t man; //! Mantissa with hidden one
    bool operator==(const APyFloatData& other) const noexcept
    {
        return std::make_tuple(sign, exp, man)
            == std::make_tuple(other.sign, other.exp, other.man);
    }
    bool operator!=(const APyFloatData& other) const noexcept
    {
        return !(*this == other);
    }

    // // Dereference (to act like iterators)
    // const APyFloatData& operator*() const noexcept { return *this; };
    // const APyFloatData* operator->() const noexcept { return this; };
};

struct APyFloatSpec {
    //! Number of exponent bits
    std::uint8_t exp_bits;
    //! Number of mantissa bits
    std::uint8_t man_bits;
    //! Exponent bias
    exp_t bias;

    bool operator==(const APyFloatSpec& other) const noexcept
    {
        return std::make_tuple(exp_bits, man_bits, bias)
            == std::make_tuple(other.exp_bits, other.man_bits, other.bias);
    }
    bool operator!=(const APyFloatSpec& other) const noexcept
    {
        return !(*this == other);
    }
};

/* ********************************************************************************** *
 * *                           APyFixed C++ data types                              * *
 * ********************************************************************************** */

struct APyFixedSpec {
    int bits;
    int int_bits;
};

#endif
