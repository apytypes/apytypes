/*
 * Array/Tensor type for arbitrary precision fixed-point data.
 */

#ifndef _APYFIXED_ARRAY_H
#define _APYFIXED_ARRAY_H

#include <pybind11/numpy.h>    // pybind11::array_t
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
#include "../extern/mini-gmp/mini-gmp.h"

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

    /*!
     * Matrix mutliplication. If both arguments ar 2-D tensors, this method performs the
     * ordinary matrix multiplication. If input dimensions are greater than 2, this
     * method performs stacked matrix multiplications, where the dimensions of last two
     * dimensions are treated as matrices.
     */
    APyFixedArray matmul(const APyFixedArray& rhs) const;

    //! Transposition function. For a 1-D array, returns an exact copy of `*this`. For
    //! a 2-D array, returns the matrix transposition of `*this`.
    APyFixedArray transpose() const;

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

    //! Python `__repr__()` function
    std::string repr() const;

    //! Shape of the array
    pybind11::tuple shape() const;

    //! Number of dimension
    int ndim() const;

    //! Retrieve a single item
    APyFixedArray get_item(std::size_t idx) const;

    //! Retrieve `bits` specifier
    int bits() const noexcept { return _bits; }

    //! Retrieve `int_bits` specifier
    int int_bits() const noexcept { return _int_bits; }

    //! Retrieve `frac_bits` specifier
    int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Convert to a NumPy array
    pybind11::array_t<double> to_numpy() const;

    /*!
     * Test if two `APyFixedArray` objects are identical. Two `APyFixedArray` objects
     * are considered identical if, and only if:
     *   * They represent exatly the same tensor shape
     *   * They store the exact same fixed-point values in all tensor elements
     *   * They have the exact same bit specification (`bits`, `int_bits`, and
     *     `frac_bits` are all equal)
     */
    bool is_identical(const APyFixedArray& other) const;

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

    //! Create an `APyFixedArray` tensor object initialized with values from a sequence
    //! of `doubles`
    static APyFixedArray from_double(
        const pybind11::sequence& double_seq,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                          Private member functions                          * *
     * ****************************************************************************** */

    //! Construct a new `APyFixedArray` tensor object with the same `shape` and
    //! fixed-point values as `*this`, but with a new word-length. The underlying
    //! bit-pattern of each tensor element are copied into place, meaning that lowering
    //! the number of fractional bits may result in truncation, and lowering the number
    //! of integer bits may result in two's complement overflowing.
    APyFixedArray _bit_resize(int new_bits, int new_int_bits) const;

    //! The number of limbs per scalar in `*this` `APyFixedArray` tensor
    //! object.
    std::size_t _scalar_limbs() const { return bits_to_limbs(_bits); }

    //! Fold the `_shape` field over multiplication
    std::size_t _fold_shape() const;

    //! Evaluate the inner between two vectors. This method assumes that the the shape
    //! of both `*this` and `rhs` are equally long. Anything else is undefined
    //! behaviour.
    APyFixedArray _checked_inner_product(const APyFixedArray& rhs) const;

    //! Evaluate the matrix product between two 2D matrices. This method assumes that
    //! the shape of `*this` and `rhs` have been checked to match a 2D matrix
    //! multiplication.
    APyFixedArray _checked_2d_matmul(const APyFixedArray& rhs) const;
};

#endif // _APYFIXED_ARRAY_H
