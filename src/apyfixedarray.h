/*
 * Array/Tensor type for arbitrary precision fixed-point data.
 */

#ifndef _APYFIXED_ARRAY_H
#define _APYFIXED_ARRAY_H

#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::array_t
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

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
        "The GMP `mp_limb_t` data type is either 64-bit or 32-bit. Any other limb size "
        "is unsupported. This assumption should hold true always, according to the GMP "
        "documentation. The size of limbs is specified during compilation with the C "
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
     *                         APyFixedArray data fields                              *
     * ****************************************************************************** */

    int _bits;
    int _int_bits;

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyFixedArray` objects. At lesast the
    //! bit-specifiers and shape has to be set during construction.
    APyFixedArray() = delete;

    explicit APyFixedArray(
        const nb::sequence& bit_pattern_sequence,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                     Non-Python accessible constructors                     * *
     * ****************************************************************************** */

    //! Constructor: specify only shape and word-length. Zero data on construction.
    explicit APyFixedArray(
        const std::vector<std::size_t>& shape, int bits, int int_bits
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

    /*!
     * Matrix multiplication. If both arguments ar 2-D tensors, this method performs the
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

    //! Broadcast to a new shape
    APyFixedArray broadcast_to(const std::vector<std::size_t> shape) const;

    //! Python-exposed `broadcast_to`
    APyFixedArray broadcast_to_python(const std::variant<nb::tuple, nb::int_> shape
    ) const;

    //! Squeeze this array. Returns new `APyFixedArray`.
    APyFixedArray
    squeeze(std::optional<std::variant<nb::int_, nb::tuple>> axis = std::nullopt) const;

    //! Perform a linear convolution with `other` using `mode`
    APyFixedArray convolve(const APyFixedArray& other, const std::string& mode) const;

    //! Python `__repr__()` function
    std::string repr() const;

    APyFixedArray reshape(nb::tuple new_shape) const;

    //! Reshape flatten into 1d shape
    APyFixedArray flatten() const;

    //! same as flatten as for now
    APyFixedArray ravel() const;

    //! Shape of the array
    nb::tuple python_get_shape() const;

    //! Number of dimensions
    size_t ndim() const noexcept;

    //! Retrieve a single item
    std::variant<APyFixedArray, APyFixed> get_item(std::size_t idx) const;

    //! Number of bits
    APY_INLINE int bits() const noexcept { return _bits; }

    //! Number of integer bits
    APY_INLINE int int_bits() const noexcept { return _int_bits; }

    //! Number of fractional bits
    APY_INLINE int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Convert to a NumPy array
    nb::ndarray<nb::numpy, double> to_numpy() const;

    //! Length of the array
    size_t size() const noexcept;

    //! Elementwise absolute value
    APyFixedArray abs() const;

    //! Elementwise unary negation
    APyFixedArray operator-() const;

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

    /*!
     * Test if `*this` is identical to another `APyFixedArray`. Two `APyFixedArray`
     * objects are considered identical if, and only if:
     *   * They represent exactly the same tensor shape
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
        const nb::sequence& double_seq,
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

private:
    /* ****************************************************************************** *
     * *                          Private member functions                          * *
     * ****************************************************************************** */

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
     * accumulator mode `mode`. This method assumes that the the shape of both `*this`
     * and `rhs` are equally long. Anything else is undefined behaviour. Return result
     * in a new `APyFixedArray`.
     */
    APyFixedArray _checked_inner_product(
        const APyFixedArray& rhs,                     // rhs
        std::optional<APyFixedAccumulatorOption> mode // optional accumulation mode
    ) const;

    /*!
     * Evaluate the inner product between `*this` and `rhs` using scratch memories. This
     * method assumes that no accumulator context is being used. Store the
     * single-element (possibly multiple-limb) result in `result`. This method assumes
     * that the the shape of both `*this` and `rhs` are equally long. Anything else is
     * undefined behaviour.
     */
    void _checked_inner_product_full(
        const APyFixedArray& rhs,             // rhs
        APyFixedArray& result,                // result
        APyFixedArray& hadamard_scratch,      // scratch: hadamard product
        std::vector<mp_limb_t>& prod_scratch, // scratch: product result
        std::vector<mp_limb_t>& op1_scratch,  // scratch: absolute value operand 1
        std::vector<mp_limb_t>& op2_scratch   // scratch: absolute value operand 2
    ) const;

    /*!
     * Evaluate the inner product between `*this` and `rhs` using scratch memories, with
     * an accumulator option specified. Store the single-element (possibly
     * multiple-limb) result in `result`. This method assumes that the the shape of both
     * `*this` and `rhs` are equally long. Anything else is undefined behaviour.
     */
    void _checked_inner_product_acc(
        const APyFixedArray& rhs,             // rhs
        APyFixedArray& result,                // result
        APyFixedArray& hadamard_scratch,      // scratch: hadamard product
        std::vector<mp_limb_t>& prod_scratch, // scratch: product result
        std::vector<mp_limb_t>& op1_scratch,  // scratch: absolute value operand 1
        std::vector<mp_limb_t>& op2_scratch,  // scratch: absolute value operand 2
        const APyFixedAccumulatorOption& mode // accumulation mode
    ) const;

    /*!
     * Perform hadamard multiplication of `*this` and `rhs` using scratch memories.
     * Store the result in the vector pointed to by `res_out`. This method assumes that
     * the shape of `*this` and `rhs` are equally long. Anyghing else is undefined
     * behaviour.
     *
     * NOTE: Use `simd::vector_mul` if you know that the resulting number of bits fit in
     * a single limb.
     */
    template <typename RANDOM_ACCESS_ITERATOR>
    void _checked_hadamard_product(
        const APyFixedArray& rhs,             // rhs
        RANDOM_ACCESS_ITERATOR res_out,       // output iterator
        std::vector<mp_limb_t>& prod_scratch, // scratch: product result
        std::vector<mp_limb_t>& op1_scratch,  // scratch: absolute value operand 1
        std::vector<mp_limb_t>& op2_scratch   // scratch: absolute value operand 2
    ) const;

    /*!
     * Cast values to a longer (at least not shorter) word length. This code has moved
     * all conditions out of the for-loop to speed up the execution and instead there
     * are multiple for-loops doing similar operations.
     */
    template <typename RANDOM_ACCESS_ITERATOR>
    void _cast_correct_wl(
        RANDOM_ACCESS_ITERATOR output_it, int new_bits, int new_int_bits
    ) const;

    /*!
     * The internal cast method used to place cast data onto a pair of iterators.
     * See `APyFixedArray::cast` for example usage.
     * Importantly:
     *   * The `caster` must be an `APyFixed` object with same bit specifier as `*this`
     *   * If `new_bits` > `_bits`, the output must contain pad limbs
     */
    template <typename RANDOM_ACCESS_ITERATOR>
    void _cast(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        APyFixed& caster,
        int new_bits,
        int new_int_bits,
        QuantizationMode quantization,
        OverflowMode overflow
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

#endif // _APYFIXED_ARRAY_H
