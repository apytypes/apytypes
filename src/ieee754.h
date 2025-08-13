/*
 * C++-compatible bit fiddling with IEEE-754 floating-point numbers. Heavily inspired by
 * the GLIBC `ieee754.h`. Portable and relies on C++17.
 *
 * Author: Mikael Henriksson (2024)
 */

#ifndef _CXX_IEEE754_FIDDLE
#define _CXX_IEEE754_FIDDLE

#include "apytypes_util.h"
#include <cstdint> // std::uint64_t
#include <cstring> // std::memcpy
#include <limits>  // std::numeric_limits

/*
 * For an easy C++17 compatible byte-swap expression, see:
 * https://en.cppreference.com/w/cpp/language/fold
 */

//! Test target machine native endianness
[[maybe_unused]] static constexpr bool is_little_endian()
{
#if defined(_MSC_VER)
    // Microsoft Visual C/C++ compiler. Windows x86/64 systems are always little endian.
    // Assume for now that we don't compile for XBOX-360 or any other Microsoft
    // big-endian architectures.
    return true;
#elif defined(__GNUC__) && defined(__BYTE_ORDER__)
    /*
     * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++ compiler
     * (ICC). Macro __BYTE_ORDER__ is specified
     */
    return __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
#else
    // Failure: could not reliably detect the byte-order of the target architecture.
    static_assert(
        false,
        "apytype_util.h: `constexpr bool is_little_endian()` could not find native "
        "endianness"
    );
#endif
}

[[maybe_unused]] static APY_INLINE std::uint64_t type_pun_double_to_uint64_t(double d)
{
    // These functions are only compatible with IEEE-754 double-precision `double`s
    static_assert(std::numeric_limits<double>::is_iec559);

    // The *only* C++17 way to legally and portably type-pun in C++: with `std::memcpy`
    std::uint64_t double_pun;
    std::memcpy(&double_pun, &d, sizeof(double_pun));
    return double_pun;
}

[[maybe_unused]] static APY_INLINE double type_pun_uint64_t_to_double(std::uint64_t num)
{
    // These functions are only compatible with IEEE-754 double-precision `double`s
    static_assert(std::numeric_limits<double>::is_iec559);

    // The *only* C++17 way to legally and portably type-pun in C++: with `std::memcpy`
    double d;
    std::memcpy(&d, &num, sizeof(d));
    return d;
}

[[maybe_unused]] static APY_INLINE bool sign_of_double(double d)
{
    std::uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (is_little_endian()) {
        return bool(double_pun & (std::uint64_t(1) << 63));
    } else { // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

//! Return the biased exponent of a `double` in a `int64_t`.
//! Return value range: [0, 2048)
[[maybe_unused]] static APY_INLINE int exp_of_double(double d)
{
    std::uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (is_little_endian()) {
        return static_cast<int>((double_pun << 1) >> 53);
    } else { // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

//! Return significand/mantissa of a `double` (without the hidden one) in a `uint64_t`.
//! Range of returned value: [0, 4503599627370496)
[[maybe_unused]] static APY_INLINE std::uint64_t man_of_double(double d)
{
    std::uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (is_little_endian()) {
        return double_pun & 0x000FFFFFFFFFFFFF;
    } else { // std::endiand::native == std::endian::big
        throw NotImplementedException();
    }
}

//! [Un]set the sign of a `double` type from a `bool`
[[maybe_unused]] static APY_INLINE void set_sign_of_double(double& d, bool sign)
{
    std::uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (is_little_endian()) {
        double_pun &= 0x7FFFFFFFFFFFFFFF;
        double_pun |= (std::uint64_t(sign) << 63);
    } else {
        throw NotImplementedException();
    }
    d = type_pun_uint64_t_to_double(double_pun);
}

//! Set the exponent part of a `double` from `int`.
//! Domain of argument `exp`: [0, 2048)
[[maybe_unused]] static APY_INLINE void set_exp_of_double(double& d, int exp)
{
    std::uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (is_little_endian()) {
        double_pun &= 0x800FFFFFFFFFFFFF;
        double_pun |= 0x7FF0000000000000 & (std::uint64_t(exp) << 52);
    } else {
        throw NotImplementedException();
    }
    d = type_pun_uint64_t_to_double(double_pun);
}

//! Set the mantissa part of a `double` from `uint64_t`.
//! Domain of argument `uint64_t`: [0, 4503599627370496)
[[maybe_unused]] static APY_INLINE void set_man_of_double(double& d, std::uint64_t man)
{
    std::uint64_t double_pun = type_pun_double_to_uint64_t(d);
    if constexpr (is_little_endian()) {
        double_pun &= 0xFFF0000000000000;
        double_pun |= 0x000FFFFFFFFFFFFF & man;
    } else {
        throw NotImplementedException();
    }
    d = type_pun_uint64_t_to_double(double_pun);
}

#endif // _CXX_IEEE754_FIDDLE
