/*
 * Array/Tensor type for arbitrary precision fixed-point data.
 */

#ifndef _APYFIXED_ARRAY_H
#define _APYFIXED_ARRAY_H

#include <pybind11/pybind11.h> // pybind11::object
#include <pybind11/pytypes.h>  // pybind11::sequence

#include "apytypes_common.h"
#include "apytypes_util.h"

#include <cstddef>  // std::size_t
#include <limits>   // std::numeric_limits<>::is_iec559
#include <optional> // std::optional, std::nullopt
#include <ostream>  // std::ostream
#include <string>   // std::string
#include <vector>   // std::vector

// GMP should be included after all other includes
#include <gmp.h>

class APyFixedArray {
    int _bits;
    int _int_bits;
    std::vector<std::size_t> _shape;
    std::vector<mp_limb_t> _data;

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    // No default constructed APyFixed types
    APyFixedArray() = delete;

    explicit APyFixedArray(
        const pybind11::sequence& bit_pattern_sequence,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                     Non-Python accessible constructors                     * *
     * ****************************************************************************** */

    //! Constructor: specify only shape, size, and zero data on construction
    explicit APyFixedArray(
        const std::vector<std::size_t>& shape,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                       Binary arithmetic operators                          * *
     * ****************************************************************************** */

    APyFixedArray operator+(const APyFixedArray& rhs) const;
    APyFixedArray operator-(const APyFixedArray& rhs) const;
    APyFixedArray operator*(const APyFixedArray& rhs) const;
    APyFixedArray operator/(const APyFixedArray& rhs) const;
    APyFixedArray operator<<(int shift_val) const;
    APyFixedArray operator>>(int shift_val) const;

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

    //! Python `__repr__()` function
    std::string repr() const;

    //! Retrieve the `bits` specifier for this APyFixedArray
    int bits() const noexcept { return _bits; }

    //! Retrieve the `int_bits` specifier for this APyFixedArray
    int int_bits() const noexcept { return _int_bits; }

    //! Retrieve the `frac_bits` specifier for this APyFixedArray
    int frac_bits() const noexcept { return _bits - _int_bits; }

    /* ****************************************************************************** *
     * *                          Private member functions                          * *
     * ****************************************************************************** */

    //! Construct a new `APyFixedArray` tensor object with the same `shape` and
    //! fixed-point values as `*this`, but with a new word-length. The underlying
    //! bit-pattern of each tensor element are copied into place, meaning that lowering
    //! the number of fractional bits may result in truncation, and lowering the number
    //! of integer bits may result in two's complement overflowing.
    APyFixedArray _bit_resize(int new_bits, int new_int_bits) const;

    //! Retrieve the number of limbs per scalar in `*this` `APyFixedArray` tensor
    //! object.
    std::size_t _scalar_limbs() const { return bits_to_limbs(_bits); }
};

#endif // _APYFIXED_ARRAY_H
