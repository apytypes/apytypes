/*
 * Array/tensor type for arbitrary precision complex-valued fixed-point formats.
 */

#ifndef _APYCFIXEDARRAY_H
#define _APYCFIXEDARRAY_H

#include "apyarray.h"
#include "apycfixed.h"
#include "apyfixedarray.h"
#include "apytypes_common.h"

#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::array_t
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

class APyCFixedArray : public APyArray<apy_limb_t, APyCFixedArray> {

    /* ****************************************************************************** *
     * *                      APyCFixedArray C++ assumptions                        * *
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
        "implementation defined. APyCFixed relies heavily on arithmetic right shift."
    );
    static_assert(
        (std::numeric_limits<double>::is_iec559),
        "We assume IEEE-754 double-precision floating-point types."
    );

    /* ****************************************************************************** *
     * *                       APyCFixedArray data fields                           * *
     * ****************************************************************************** */

private:
    int _bits;
    int _int_bits;

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Name of this array type (used when throwing errors)
    static constexpr auto ARRAY_NAME = std::string_view("APyCFixedArray");

    APyCFixed create_scalar() const { return APyCFixed(_bits, _int_bits); }

    APyCFixedArray create_array(const std::vector<std::size_t>& shape) const
    {
        return APyCFixedArray(shape, _bits, _int_bits);
    }

    static APyCFixedArray
    create_array_static(const std::vector<std::size_t>& shape, const APyCFixed& fix)
    {
        return APyCFixedArray(shape, fix._bits, fix._int_bits);
    }

    //! Test if two fixed-point vectors have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyCFixedArray& other) const noexcept
    {
        return _bits == other._bits && _int_bits == other._int_bits;
    }

    //! Test if `*this` has the same bit specifiers as another `APyCFixed`
    APY_INLINE bool is_same_spec(const APyCFixed& other) const noexcept
    {
        return _bits == other._bits && _int_bits == other._int_bits;
    }

    //! Retrieve the bit specification
    APY_INLINE APyFixedSpec spec() const noexcept { return { _bits, _int_bits }; }

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyCFixedArray` objects. At least the
    //! bit-specifiers and shape has to be set during construction.
    APyCFixedArray() = delete;

    explicit APyCFixedArray(
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
    explicit APyCFixedArray(
        const std::vector<std::size_t>& shape, int bits, int int_bits
    );

    //! Constructor: specify shape and word-length and steal the data from vector
    explicit APyCFixedArray(
        const std::vector<std::size_t>& shape, int bits, int int_bits, vector_type&& v
    );

    //! Constructor: specify only shape and word-length. Zero data on construction.
    explicit APyCFixedArray(
        const std::vector<std::size_t>& shape,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                        Arithmetic member functions                         * *
     * ****************************************************************************** */
private:
    //! Base addition/subtraction routine for `APyCFixedArray`
    template <class ripple_carry_op, class simd_op, class simd_shift_op>
    inline APyCFixedArray _apycfixedarray_base_add_sub(const APyCFixedArray& rhs) const;

    //! Base addition/subtraction routine for `APyCFixedArray` with `APyCFixed`
    template <class op, class ripple_carry_op>
    inline APyCFixedArray _apycfixed_base_add_sub(const APyCFixed& rhs) const;

public:
    APyCFixedArray operator+(const APyCFixedArray& rhs) const;
    APyCFixedArray operator+(const APyCFixed& rhs) const;
    APyCFixedArray operator-(const APyCFixedArray& rhs) const;
    APyCFixedArray operator-(const APyCFixed& rhs) const;
    APyCFixedArray operator*(const APyCFixedArray& rhs) const;
    APyCFixedArray operator*(const APyCFixed& rhs) const;
    APyCFixedArray operator/(const APyCFixedArray& rhs) const;
    APyCFixedArray operator/(const APyCFixed& rhs) const;
    APyCFixedArray operator<<(const int shift_val) const;
    APyCFixedArray operator>>(const int shift_val) const;
    APyCFixedArray& operator<<=(const int shift_val);
    APyCFixedArray& operator>>=(const int shift_val);
    APyCFixedArray rsub(const APyCFixed& rhs) const;
    APyCFixedArray rdiv(const APyCFixed& rhs) const;

    //! Elementwise unary negation
    APyCFixedArray operator-() const;

    //! Elementwise unary positive
    APY_INLINE APyCFixedArray operator+() const { return *this; };

    //! Elementwise logic not
    APyCFixedArray operator~() const;

    /*!
     * Matrix multiplication. If both arguments are 2-D tensors, this method performs
     * the ordinary matrix multiplication. If input dimensions are greater than 2, this
     * method performs stacked matrix multiplications, where the dimensions of last two
     * dimensions are treated as matrices.
     */
    std::variant<APyCFixedArray, APyCFixed> matmul(const APyCFixedArray& rhs) const;

    /*!
     * Evaluate the inner between two vectors. This method assumes that the shape
     * of both `*this` and `rhs` are equally long. Anything else is undefined
     * behaviour.
     */
    APyCFixed checked_inner_product(
        const APyCFixedArray& rhs, std::optional<APyFixedAccumulatorOption> mode
    ) const;

    /*!
     * Evaluate the matrix product between two 2D matrices. This method assumes that
     * the shape of `*this` and `rhs` have been checked to match a 2D matrix
     * multiplication.
     */
    APyCFixedArray checked_2d_matmul(
        const APyCFixedArray& rhs, std::optional<APyFixedAccumulatorOption> mode
    ) const;

    //! Perform a linear convolution with `other` using `mode`
    APyCFixedArray convolve(const APyCFixedArray& other, const std::string& mode) const;

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

public:
    //! Number of bits
    APY_INLINE int bits() const noexcept { return _bits; }

    //! Number of integer bits
    APY_INLINE int int_bits() const noexcept { return _int_bits; }

    //! Number of fractional bits
    APY_INLINE int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Retrieve the string representation
    std::string repr() const;

    //! Convert to a NumPy array
    nb::ndarray<nb::numpy, std::complex<double>> to_numpy(
        std::optional<nb::object> dtype = std::nullopt,
        std::optional<bool> copy = std::nullopt
    ) const;

    //! Sum over one or more axes.
    std::variant<APyCFixedArray, APyCFixed>
    sum(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative sum over one or more axes.
    APyCFixedArray cumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes.
    std::variant<APyCFixedArray, APyCFixed>
    prod(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes.
    APyCFixedArray cumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Retrieve real part
    APyFixedArray get_real() const;

    //! Retrieve imaginary part
    APyFixedArray get_imag() const;

    /*!
     * Construct a new `APyCFixedArray` tensor object with the same `shape` and
     * fixed-point values as `*this`, but with a new word-length. The underlying
     * bit-pattern of each tensor element are copied into place, meaning that lowering
     * the number of fractional bits may result in quantization, and lowering the number
     * of integer bits may result in overflowing. Supports quantization and overflow
     * options on narrowing casts.
     */
    APyCFixedArray cast(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    ) const;

    /* ****************************************************************************** *
     * *                           Static array creation                            * *
     * ****************************************************************************** */

public:
    //! Create an `APyCFixedArray` tensor object initialized with values from a sequence
    //! of `complex`
    static APyCFixedArray from_complex(
        const nb::typed<nb::iterable, nb::any>& cplx_seq,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyCFixedArray` tensor object initialized with values from a sequence
    //! of numbers
    static APyCFixedArray from_numbers(
        const nb::typed<nb::iterable, nb::any>& number_seq,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyCFixedArray` tensor object initialized with values from an ndarray
    static APyCFixedArray from_array(
        const nb::ndarray<nb::c_contig>& ndarray,
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
     * *                    Static methods for array initialization                 * *
     * ****************************************************************************** */

    //! Create an `APyCFixedArray` initialized with zeros
    static APyCFixedArray zeros(
        const PyShapeParam_t& shape,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyCFixedArray` initialized with ones
    static APyCFixedArray ones(
        const PyShapeParam_t& shape,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyCFixedArray` with ones on the diagonal and zeros elsewhere
    static APyCFixedArray
    eye(const nb::int_& N,
        std::optional<nb::int_> M = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt);

    //! Create a square `APyCFixedArray` with ones on the diagonal and zeros elsewhere
    static APyCFixedArray identity(
        const nb::int_& N,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create an `APyCFixedArray` with evenly spaced values within a given interval
    static APyCFixedArray arange(
        const nb::object& start,
        const nb::object& stop,
        const nb::object& step,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     * *                          Private member functions                          * *
     * ****************************************************************************** */

private:
    auto real_begin() noexcept { return std::begin(_data); }
    auto real_begin() const noexcept { return std::cbegin(_data); }
    auto real_cbegin() const noexcept { return std::cbegin(_data); }

    auto imag_begin() noexcept { return std::begin(_data) + _itemsize / 2; }
    auto imag_begin() const noexcept { return std::cbegin(_data) + _itemsize / 2; }
    auto imag_cbegin() const noexcept { return std::cbegin(_data) + _itemsize / 2; }

    /*!
     * Set the underlying bit values of `*this` from a NDArray object of integers. This
     * member function assumes that the shape of `*this` and `ndarray` are equal.
     */
    void _set_bits_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray);

    /*!
     * Set the values of `*this` from a NDArray object of floats/integers. This member
     * function assumes that the shape of `*this` and `ndarray` are equal. The elements
     * in `ndarray` are explicitly converted to `double` (or `std::complex<double>`)
     * before being copied into `*this`.
     */
    void _set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray);
};

#endif
