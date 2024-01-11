/*
 * C++-compatible bit fiddling with IEEE-754 floating-point numbers. Heavily inspired by
 * the GLIBC `ieee754.h`. Portable, but requires C++-20.
 *
 * Author: Mikael Henriksson (2024)
 */

#ifndef _CXX_IEEE754_FIDDLE
#define _CXX_IEEE754_FIDDLE

#include "apy_util.h"
#include <bit>      // std::endian
#include <cstdint>  // uint64_t
#include <cstring>  // std::memcpy
#include <limits>   // std::numiric_limits

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
    if constexpr (std::endian::native == std::endian::little) {
         return bool(double_pun & (uint64_t(1) << 63));
    } else {  // std::endiand::native == std::endian::big
        return bool(double_pun & (uint64_t(1) << 0));
    }
}

// Returns the *biased* exponent of a `double` in a `int64_t` 
// Return value range: [0, 1023)
static inline int64_t exp_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (std::endian::native == std::endian::little) {
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
    if constexpr (std::endian::native == std::endian::little) {
        return double_pun & 0x000FFFFFFFFFFFFF;
    } else {  // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

#endif  // _CXX_IEEE754_FIDDLE
