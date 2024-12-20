/*
 * Forward declarations of the important APyTypes C++ types, and their relationship.
 */

#ifndef _APYTYPES_FWD
#define _APYTYPES_FWD

#include <cstdint>
#include <tuple>

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

/* ********************************************************************************** *
 * *                            APyTypes type relationships                         * *
 * ********************************************************************************** */

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

    // Dereference (to act like iterators)
    const APyFloatData& operator*() const noexcept { return *this; };
    const APyFloatData* operator->() const noexcept { return this; };
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

#endif
