/*
 * Array/Tensor type for arbitrary precision fixed-point data.
 */

#ifndef _APYFIXED_ARRAY_H
#define _APYFIXED_ARRAY_H

#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::array_t
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

#include "apybuffer.h"
#include "apyfixed.h"
#include "apytypes_common.h"
#include "apytypes_util.h"

#include <cstddef>  // std::size_t
#include <limits>   // std::numeric_limits<>::is_iec559
#include <optional> // std::optional, std::nullopt
#include <string>   // std::string
#include <vector>   // std::vector

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

class APyFixedArray : public APyBuffer<mp_limb_t> {

    /* ****************************************************************************** *
     *                        APyFixedArray C++ assumptions                           *
     * ****************************************************************************** */

    static_assert(
        (sizeof(mp_limb_t) == 8 || sizeof(mp_limb_t) == 4),
        "The GMP `mp_limb_t` data type is either 64 bit or 32 bit. Any other limb size "
        "is unsupported. This assumption should hold true always, according to the GMP "
        "documentation"
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
     *                         APyFixedArray data fields                              *
     * ****************************************************************************** */

    int _bits;
    int _int_bits;

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed APyFixedArray types
    APyFixedArray() = delete;

    explicit APyFixedArray(
        const nanobind::sequence& bit_pattern_sequence,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                     Non-Python accessible constructors                     * *
     * ****************************************************************************** */

    //! Constructor: specify only shape and word-length. Zero data on construction
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
    APyFixedArray operator+(const APyFixed& rhs) const;
    APyFixedArray operator-(const APyFixedArray& rhs) const;
    APyFixedArray operator-(const APyFixed& rhs) const;
    APyFixedArray operator*(const APyFixedArray& rhs) const;
    APyFixedArray operator*(const APyFixed& rhs) const;
    APyFixedArray operator/(const APyFixedArray& rhs) const;
    APyFixedArray operator<<(const int shift_val) const;
    APyFixedArray operator>>(const int shift_val) const;
    APyFixedArray& operator<<=(const int shift_val);
    APyFixedArray& operator>>=(const int shift_val);
    APyFixedArray rsub(const APyFixed& rhs) const;

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
    nanobind::tuple shape() const;

    //! Number of dimensions
    size_t ndim() const;

    //! Retrieve a single item
    std::variant<APyFixedArray, APyFixed> get_item(std::size_t idx) const;

    //! Number of bits
    int bits() const noexcept { return _bits; }

    //! Number of integer bits
    int int_bits() const noexcept { return _int_bits; }

    //! Number of fractional bits
    int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Convert to a NumPy array
    nanobind::ndarray<nanobind::numpy, double> to_numpy() const;

    //! Length of the array
    size_t size() const;

    //! Elementwise absolute value
    APyFixedArray abs() const;

    //! Elementwise unary negation
    APyFixedArray operator-() const;

    /*!
     * Construct a new `APyFixedArray` tensor object with the same `shape` and
     * fixed-point values as `*this`, but with a new word-length. The underlying
     * bit-pattern of each tensor element are copied into place, meaning that lowering
     * the number of fractional bits may result in quantization, and lowering the number
     * of integer bits may result in overflowing. Supports quantization and oveflow
     * options on narrowing casts.
     */
    APyFixedArray cast(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        QuantizationMode quantization = QuantizationMode::TRN,
        OverflowMode overflow = OverflowMode::WRAP,
        std::optional<int> frac_bits = std::nullopt
    ) const;

    APyFixedArray resize(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        QuantizationMode quantization = QuantizationMode::TRN,
        OverflowMode overflow = OverflowMode::WRAP,
        std::optional<int> frac_bits = std::nullopt
    ) const;

    /*!
     * Test if `*this` is identical to another `APyFixedArray`. Two `APyFixedArray`
     * objects are considered identical if, and only if:
     *   * They represent exatly the same tensor shape
     *   * They store the exact same fixed-point values in all tensor elements
     *   * They have the exact same bit format (`bits`, `int_bits`, and `frac_bits`)
     */
    bool is_identical(const APyFixedArray& other) const;

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

    //! Create an `APyFixedArray` tensor object initialized with values from a sequence
    //! of `doubles`
    static APyFixedArray from_double(
        const nanobind::sequence& double_seq,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

private:
    /* ****************************************************************************** *
     * *                          Private member functions                          * *
     * ****************************************************************************** */

    /*!
     * Perform hadamard product between `*this` and `rhs` using scratch memories. Store
     * the result in the vector pointed to by `res_begin`. This method assumes that the
     * shape of `*this` and `rhs` are equally long. Anyghing else is undefined
     * behaviour.
     */
    void _checked_hadamard_product(
        const APyFixedArray& rhs,                 // rhs
        std::vector<mp_limb_t>::iterator res_out, // output iterator
        std::vector<mp_limb_t>& prod_scratch,     // scratch: product result
        std::vector<mp_limb_t>& op1_scratch,      // scratch: absolute value operand 1
        std::vector<mp_limb_t>& op2_scratch       // scratch: absolute value operand 2
    ) const;

    /*!
     * Evaluate the inner product between `*this` and `rhs` using scratch memories.
     * Store the result in the vector pointed to by `res_begin`. This method assumes
     * that the the shape of both `*this` and `rhs` are equally long. Anything else is
     * undefined behaviour.
     */
    void _checked_inner_product(
        const APyFixedArray& rhs,             // rhs
        APyFixedArray& result,                // result
        APyFixedArray& hadamard_scratch,      // scratch: hadamard product
        std::vector<mp_limb_t>& prod_scratch, // scratch: product result
        std::vector<mp_limb_t>& op1_scratch,  // scratch: absolute value operand 1
        std::vector<mp_limb_t>& op2_scratch,  // scratch: absolute value operand 2
        std::optional<AccumulatorOption> mode // optional accumulation mode
    ) const;

    /*!
     * Evaluate the matrix product between two 2D matrices. This method assumes that
     * the shape of `*this` and `rhs` have been checked to match a 2D matrix-matrix or
     * matrix-vector multiplication. Anything else is undefined behaviour.
     */
    APyFixedArray _checked_2d_matmul(
        const APyFixedArray& rhs,             // rhs
        std::optional<AccumulatorOption> mode // optional accumulation mode
    ) const;

    APyFixedArray _cast_correct_wl(int new_bits, int new_int_bits) const;

    /*!
     * The internal cast method used to place cast data onto a pair of iterators.
     * See `APyFixedArray::cast` for example usage.
     * Importantly:
     *   * The `caster` must be an `APyFixed` object with same bit specifier as `*this`
     *   * If `new_bits` > `_bits`, the output must contain pad limbs
     */
    void _cast(
        std::vector<mp_limb_t>::iterator it_begin,
        std::vector<mp_limb_t>::iterator it_end,
        APyFixed& caster,
        int new_bits,
        int new_int_bits,
        QuantizationMode quantization,
        OverflowMode overflow
    ) const;

    /*!
     * Set the underlying bit values of `*this` from a NumPy NDArray object of integers.
     * This member function assumes that the shape of `*this` and `ndarray` are equal.
     */
    void _set_bits_from_numpy_ndarray(const nanobind::ndarray<nanobind::numpy>& ndarray
    );

    /*
     * Set the values of `*this` from a NumPy NDArray object of floats/integers. This
     * member function assumes that the shape of `*this` and `ndarray` are equal. The
     * elements in `ndarray` are explicitly converted to `double` before being copied
     * into `*this`.
     */
    void
    _set_values_from_numpy_ndarray(const nanobind::ndarray<nanobind::numpy>& ndarray);
};

#endif // _APYFIXED_ARRAY_H
