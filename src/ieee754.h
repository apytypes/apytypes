/*
 * C++-compatible bit fiddling with IEEE-754 floating-point numbers. Heavily inspired by
 * the GLIBC `ieee754.h`. Portable and relies on C++17.
 *
 * Author: Mikael Henriksson (2024)
 */

#ifndef _CXX_IEEE754_FIDDLE
#define _CXX_IEEE754_FIDDLE

#include "apytypes_util.h"
#include <cstdint> // uint64_t
#include <cstring> // std::memcpy
#include <limits>  // std::numiric_limits

/*
 * For an easy C++17 compatible byte-swap expression, see:
 * https://en.cppreference.com/w/cpp/language/fold
 */

//! Test target machine native endianness in pure C++. Optimized away to single `true`
//! or `false`, depending on endianness, on all tested machines using most C++17
//! compatible compilers (godbolt.org):
//! * All GCC version supporting C++17
//! * All Clang version supporting C++17
//! * All versions of MSVC available at compiler explorer...
[[maybe_unused]] static APY_INLINE bool _MACHINE_IS_NATIVE_LITTLE_ENDIAN()
{
    uint32_t deadbeef = 0xDEADBEEF;
    uint8_t arr[4];
    std::memcpy(arr, &deadbeef, sizeof(deadbeef));
    return arr[0] == uint8_t(0xEF) ? true : false;
}

[[maybe_unused]] static APY_INLINE uint64_t type_pun_double_to_uint64_t(double d)
{
    // These functions are only compatible with IEEE-754 double-precision `double`s
    static_assert(std::numeric_limits<double>::is_iec559);

    // The *only* C++17 way to legaly and portably type-pun in C++: with `std::memcpy`
    uint64_t double_pun;
    std::memcpy(&double_pun, &d, sizeof(double_pun));
    return double_pun;
}

[[maybe_unused]] static APY_INLINE double type_pun_uint64_t_to_double(uint64_t num)
{
    // These functions are only compatible with IEEE-754 double-precision `double`s
    static_assert(std::numeric_limits<double>::is_iec559);

    // The *only* C++17 way to legaly and portably type-pun in C++: with `std::memcpy`
    double d;
    std::memcpy(&d, &num, sizeof(d));
    return d;
}

[[maybe_unused]] static APY_INLINE bool sign_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        return bool(double_pun & (uint64_t(1) << 63));
    } else { // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

//! Returns the *biased* exponent of a `double` in a `int64_t`.
//! Return value range: [0, 2048)
[[maybe_unused]] static APY_INLINE int64_t exp_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        return (double_pun << 1) >> 53;
    } else { // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

//! Returns significand/mantissa of a `double` (without the hidden one) in a `uint64_t`.
//! Range of returned value: [0, 4503599627370496)
[[maybe_unused]] static APY_INLINE uint64_t man_of_double(double d)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        return double_pun & 0x000FFFFFFFFFFFFF;
    } else { // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

//! [Un]set the sign of a `double` type from a `bool`
[[maybe_unused]] static APY_INLINE void set_sign_of_double(double& d, bool sign)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        double_pun &= 0x7FFFFFFFFFFFFFFF;
        double_pun |= (uint64_t(sign) << 63);
    } else {
        throw NotImplementedException();
    }
    d = type_pun_uint64_t_to_double(double_pun);
}

//! Set the exponent part of a `double` from `int64_t`.
//! Domain of argument `int64_t`: [0, 2048)
[[maybe_unused]] static APY_INLINE void set_exp_of_double(double& d, int64_t exp)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        double_pun &= 0x800FFFFFFFFFFFFF;
        double_pun |= 0x7FF0000000000000 & (uint64_t(exp) << 52);
    } else {
        throw NotImplementedException();
    }
    d = type_pun_uint64_t_to_double(double_pun);
}

//! Set the mantissa poart of a `double` from `uint64_t`.
//! Domain of argument `uint64_t`: [0, 4503599627370496)
[[maybe_unused]] static APY_INLINE void set_man_of_double(double& d, uint64_t man)
{
    uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if (_MACHINE_IS_NATIVE_LITTLE_ENDIAN()) {
        double_pun &= 0xFFF0000000000000;
        double_pun |= 0x000FFFFFFFFFFFFF & man;
    } else {
        throw NotImplementedException();
    }
    d = type_pun_uint64_t_to_double(double_pun);
}

#endif // _CXX_IEEE754_FIDDLE
