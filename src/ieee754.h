/*
 * C++-compatible bit fiddling with IEEE-754 floating-point numbers. Heavily inspired by
 * the GLIBC `ieee754.h`. Portable and relies on C++17.
 *
 * Author: Mikael Henriksson (2024)
 */

#ifndef _CXX_IEEE754_FIDDLE
#define _CXX_IEEE754_FIDDLE

#include "apy_util.h"
#include <cstdint>  // uint64_t
#include <cstring>  // std::memcpy
#include <limits>   // std::numiric_limits

// Test target machine native endianness in pure C++. Optimized away to single `true`
// or `false`, depending on endianness, on all tested machines using most C++17
// compatible compilers (godbolt.org):
// * All GCC version supporting C++17
// * All Clang version supporting C++17
// * All versions of MSVC available at compiler explorer...
static inline bool _MACHINE_IS_NATIVE_LITTLE_ENDIAN()
{
    uint32_t deadbeef = 0xDEADBEEF;
    uint8_t arr[4];
    std::memcpy(arr, &deadbeef, sizeof(deadbeef));
    return arr[0] == uint8_t(0xEF) ? true : false;
}

static inline uint64_t type_pun_double_to_uint64_t(double d)
{
    // These functions are only compatible with IEEE-754 double-precision `double`s
    static_assert(std::numeric_limits<double>::is_iec559);

    // The *only* way to legaly and portably type-pun in C++: with `std::memcpy`
    uint64_t double_pun;
    std::memcpy(&double_pun, &d, sizeof(double_pun));
    return double_pun;
}

static inline bool sign_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
         return bool(double_pun & (uint64_t(1) << 63));
    } else {  // std::endiand::native == std::endian::big
        return bool(double_pun & (uint64_t(1) << 0));
    }
}

// Returns the *biased* exponent of a `double` in a `int64_t` 
// Return value range: [0, 2048)
static inline int64_t exponent_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        return (double_pun << 1) >> 53;
    } else {  // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

// Returns significand/mantissa of a `double` (*without* the hidden one) in a `uint64_t`
// Return value range: [0, 4503599627370496)
static inline uint64_t mantissa_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        return double_pun & 0x000FFFFFFFFFFFFF;
    } else {  // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

#endif  // _CXX_IEEE754_FIDDLE
