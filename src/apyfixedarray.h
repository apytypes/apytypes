/*
 * Array/tensor type for arbitrary precision fixed-point formats.
 */

#ifndef _APYFIXEDARRAY_H
#define _APYFIXEDARRAY_H

#include "apyarray.h"
#include "apybuffer.h"
#include "apyfixed.h"
#include "apytypes_common.h"
#include "apytypes_util.h"

#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::array_t
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

#include <cstddef>     // std::size_t
#include <limits>      // std::numeric_limits<>::is_iec559
#include <optional>    // std::optional, std::nullopt
#include <set>         // std::set
#include <string>      // std::string
#include <string_view> // std::string_view
#include <vector>      // std::vector

class APyFixedArray : public APyArray<apy_limb_t, APyFixedArray> {

    /* ****************************************************************************** *
     * *                      APyFixedArray C++ assumptions                         * *
     * ****************************************************************************** */

    static_assert(
        (APY_LIMB_SIZE_BYTES == 8 || APY_LIMB_SIZE_BYTES == 4),
        "The `apy_limb_t` data type is either 64-bit or 32-bit. Any other limb size "
        "is unsupported. The size of limbs is specified during compilation with the C "
        "Macro `COMPILER_LIMB_SIZE`."
    );
    static_assert(
        (-1 >> 1 == -1),
        "Right shift applied to signed integral types performs *arithmetic* right "
        "shift. Arithmetic right shift of signed types is *the only* valid behaviour "
        "since C++20, but before C++20 the right shift of signed integral types is "
        "implementation defined. APyFixed relies heavily on arithmetic right shift."
    );
    static_assert(
        (std::numeric_limits<double>::is_iec559),
        "We assume IEEE-754 double-precision floating-point types."
    );

    /* ****************************************************************************** *
     * *                       APyFixedArray data fields                            * *
     * ****************************************************************************** */

private:
    int _bits;
    int _int_bits;

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Name of this array type (used when throwing errors)
    static constexpr auto ARRAY_NAME = std::string_view("APyFixedArray");

    APyFixed create_scalar() const { return APyFixed(_bits, _int_bits); }

    APyFixedArray create_array(const std::vector<std::size_t>& shape) const
    {
        return APyFixedArray(shape, _bits, _int_bits);
    }

    static APyFixedArray
    create_array_static(const std::vector<std::size_t>& shape, const APyFixed& fix)
    {
        return APyFixedArray(shape, fix._bits, fix._int_bits);
    }

    //! Test if two fixed-point vectors have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyFixedArray& other) const noexcept
    {
        return _bits == other._bits && _int_bits == other._int_bits;
    }

    //! Test if `*this` has the same bit specifiers as another `APyFixed`
    APY_INLINE bool is_same_spec(const APyFixed& other) const noexcept
    {
        return _bits == other._bits && _int_bits == other._int_bits;
    }

    //! Retrieve the bit specification
    APY_INLINE APyFixedSpec spec() const noexcept { return { _bits, _int_bits }; }

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyFixedArray` objects. At least the
    //! bit-specifiers and shape has to be set during construction.
    APyFixedArray() = delete;

    explicit APyFixedArray(
        const nb::typed<nb::iterable, nb::any>& bit_pattern_sequence,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                     Non-Python accessible constructors                     * *
     * ****************************************************************************** */

public:
    //! Constructor: specify only shape and word-length. Zero data on construction.
    explicit APyFixedArray(
        const std::vector<std::size_t>& shape, int bits, int int_bits
    );

    //! Constructor: specify shape and word-length and steal the data from vector
    explicit APyFixedArray(
        const std::vector<std::size_t>& shape, int bits, int int_bits, vector_type&& v
    );

    //! Constructor: specify only shape and word-length. Zero data on construction.
    explicit APyFixedArray(
        const std::vector<std::size_t>& shape,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                       Binary arithmetic operators                          * *
     * ****************************************************************************** */

private:
    //! Base addition/subtraction routine for `APyFixedArray`
    template <class ripple_carry_op, class simd_op, class simd_shift_op>
    inline APyFixedArray _apyfixedarray_base_add_sub(const APyFixedArray& rhs) const;

    //! Base addition/subtraction routine for `APyFixedArray` with `APyFixed`
    template <class ripple_carry_op, class simd_op_const, class simd_shift_op_const>
    inline APyFixedArray _apyfixed_base_add_sub(const APyFixed& rhs) const;

public:
    APyFixedArray operator+(const APyFixedArray& rhs) const;
    APyFixedArray operator+(const APyFixed& rhs) const;
    APyFixedArray operator-(const APyFixedArray& rhs) const;
    APyFixedArray operator-(const APyFixed& rhs) const;
    APyFixedArray operator*(const APyFixedArray& rhs) const;
    APyFixedArray operator*(const APyFixed& rhs) const;
    APyFixedArray operator/(const APyFixedArray& rhs) const;
    APyFixedArray operator/(const APyFixed& rhs) const;
    APyFixedArray operator<<(const int shift_val) const;
    APyFixedArray operator>>(const int shift_val) const;
    APyFixedArray& operator<<=(const int shift_val);
    APyFixedArray& operator>>=(const int shift_val);
    APyFixedArray rsub(const APyFixed& rhs) const;
    APyFixedArray rdiv(const APyFixed& rhs) const;

    //! Elementwise unary negation
    APyFixedArray operator-() const;

    //! Elementwise unary positive
    APY_INLINE APyFixedArray operator+() const { return *this; };

    //! Elementwise logic not
    APyFixedArray operator~() const;

    /*!
     * Matrix multiplication. If both arguments ar 2-D tensors, this method performs the
     * ordinary matrix multiplication. If input dimensions are greater than 2, this
     * method performs stacked matrix multiplications, where the dimensions of last two
     * dimensions are treated as matrices.
     */
    std::variant<APyFixedArray, APyFixed> matmul(const APyFixedArray& rhs) const;

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

    //! Perform a linear convolution with `other` using `mode`
    APyFixedArray convolve(const APyFixedArray& other, const std::string& mode) const;

    //! Sum over one or more axes.
    std::variant<APyFixedArray, APyFixed>
    sum(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative sum over one or more axes.
    APyFixedArray cumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes.
    std::variant<APyFixedArray, APyFixed>
    prod(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes.
    APyFixedArray cumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Return the maximum of an array or the maximum along an axis.
    std::variant<APyFixedArray, APyFixed>
    max(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Return the minimum of an array or the minimum along an axis.
    std::variant<APyFixedArray, APyFixed>
    min(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Python `__repr__()` function
    std::string repr() const;

    //! Number of bits
    APY_INLINE int bits() const noexcept { return _bits; }

    //! Number of integer bits
    APY_INLINE int int_bits() const noexcept { return _int_bits; }

    //! Number of fractional bits
    APY_INLINE int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Extract bit-pattern
    std::variant<
        nb::list,
        nb::ndarray<nb::numpy, uint64_t>,
        nb::ndarray<nb::numpy, uint32_t>,
        nb::ndarray<nb::numpy, uint16_t>,
        nb::ndarray<nb::numpy, uint8_t>>
    to_bits(bool numpy = false) const;

    //! Create an N-dimensional array containing bit-patterns.
    template <typename NB_ARRAY_TYPE, typename INT_TYPE>
    nb::ndarray<NB_ARRAY_TYPE, INT_TYPE> to_bits_ndarray() const;

    //! Create a nested Python list containing bit-patterns as Python integers.
    nb::list to_bits_python_recursive_descent(
        std::size_t dim,
        APyBuffer<apy_limb_t>::vector_type::const_iterator& it,
        bool vec_is_signed = false
    ) const;

    //! Extract bit-pattern as signed Python integers
    nb::list to_signed_bits() const;

    //! Convert to a NumPy array
    nanobind::ndarray<nanobind::numpy, double> to_numpy(
        std::optional<nb::object> dtype = std::nullopt,
        std::optional<bool> copy = std::nullopt
    ) const;

    //! Elementwise absolute value
    APyFixedArray abs() const;

    /*!
     * Construct a new `APyFixedArray` tensor object with the same `shape` and
     * fixed-point values as `*this`, but with a new word-length. The underlying
     * bit-pattern of each tensor element are copied into place, meaning that lowering
     * the number of fractional bits may result in quantization, and lowering the number
     * of integer bits may result in overflowing. Supports quantization and overflow
     * options on narrowing casts.
     */
    APyFixedArray cast(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    ) const;

    /* ****************************************************************************** *
     * *                     Static conversion from other types                     * *
     * ****************************************************************************** */

    //! Create an `APyFixedArray` tensor object initialized with values from a sequence
    //! of Python objects
    static APyFixedArray from_numbers(
        const nb::typed<nb::iterable, nb::any>& number_seq,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyFixedArray` tensor object initialized with values from an ndarray
    static APyFixedArray from_array(
        const nb::ndarray<nb::c_contig>& double_seq,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                    Static methods for array initialization                 * *
     * ****************************************************************************** */

    //! Create an `APyFixedArray` initialized with zeros
    static APyFixedArray zeros(
        const PyShapeParam_t& shape,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyFixedArray` initialized with ones
    static APyFixedArray ones(
        const PyShapeParam_t& shape,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyFixedArray` with ones on the diagonal and zeros elsewhere
    static APyFixedArray
    eye(const nb::int_& N,
        std::optional<nb::int_> M = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt);

    //! Create a square `APyFixedArray` with ones on the diagonal and zeros elsewhere
    static APyFixedArray identity(
        const nb::int_& N,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyFixedArray` with evenly spaced values within a given interval
    static APyFixedArray arange(
        const nb::object& start,
        const nb::object& stop,
        const nb::object& step,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyFixedArray` with all values within a given interval
    static APyFixedArray fullrange(
        const nb::object& start,
        const nb::object& stop,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                           Conversion to other types                            *
     * ****************************************************************************** */
public:
    //! Retrieve a string of the stored values in this array.
    std::string to_string(int base = 10) const;
    std::string to_string_dec() const;

    /* ****************************************************************************** *
     * *                          Private member functions                          * *
     * ****************************************************************************** */
private:
    /*!
     * Evaluate the 2D matrix product between `*this` and `rhs`, possibly using an
     * accumulator mode `mode`. This method assumes that the shape of `*this` and `rhs`
     * have been checked to match a 2D matrix-matrix or matrix-vector multiplication.
     * Anything else is undefined behaviour. Return result in a new `APyFixedArray`.
     */
    APyFixedArray _checked_2d_matmul(
        const APyFixedArray& rhs,                     // rhs
        std::optional<APyFixedAccumulatorOption> mode // optional accumulation mode
    ) const;

    /*!
     * Evaluate the inner product between `*this` and `rhs`, possibly using an
     * accumulator mode `mode`. This method assumes that the shape of both `*this`
     * and `rhs` are equally long. Anything else is undefined behaviour. Return result
     * in a new `APyFixedArray`.
     */
    APyFixed _checked_inner_product(
        const APyFixedArray& rhs,                     // rhs
        std::optional<APyFixedAccumulatorOption> mode // optional accumulation mode
    ) const;

    /*!
     * Set the underlying bit values of `*this` from a NDArray object of integers. This
     * member function assumes that the shape of `*this` and `ndarray` are equal.
     */
    void _set_bits_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray);

    /*!
     * Set the values of `*this` from a NDArray object of floats/integers. This member
     * function assumes that the shape of `*this` and `ndarray` are equal. The elements
     * in `ndarray` are explicitly converted to `double` before being copied into
     * `*this`.
     */
    void _set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray);
};

#endif // _APYFIXEDARRAY_H
