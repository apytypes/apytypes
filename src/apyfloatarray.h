#ifndef _APYFLOAT_ARRAY_H
#define _APYFLOAT_ARRAY_H

#include "apyarray.h"
#include "apyfloat.h"
#include "apytypes_common.h"
#include "apytypes_fwd.h"
#include "array_utils.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::ndarray
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

#include <cstdint>
#include <optional>
#include <vector>

//! An N-dimensional array class for `APyFloat` objects
class APyFloatArray : public APyArray<APyFloatData, APyFloatArray> {

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    //! Constructor taking a sequence of signs, biased exponents, and mantissas.
    //! If no bias is given, an IEEE-like bias will be used.
    explicit APyFloatArray(
        const nanobind::iterable& sign_seq,
        const nanobind::iterable& exp_seq,
        const nanobind::iterable& man_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                       APyFloatArray data fields                            * *
     * ****************************************************************************** */

    //! Number of exponent bits
    std::uint8_t exp_bits;
    //! Number of mantissa bits
    std::uint8_t man_bits;
    //! Exponent bias
    exp_t bias;

    /* ****************************************************************************** *
     * *                     Non-Python accessible constructors                     * *
     * ****************************************************************************** */

public:
    //! Constructor specifying only the shape and format of the array
    APyFloatArray(
        const std::vector<std::size_t>& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

private:
    //! Default constructor (not available)
    APyFloatArray() = delete;

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Name of this array type (used when throwing errors)
    static constexpr auto ARRAY_NAME = std::string_view("APyFloatArray");

    //! Create an `APyFloat` with the same bit specifiers as `*this`
    APyFloat create_scalar() const { return APyFloat(exp_bits, man_bits, bias); }

    //! Create an `APyFloatArray` with the same shape and bit specifiers as `*this`
    APyFloatArray create_array(const std::vector<std::size_t>& shape) const
    {
        return APyFloatArray(shape, exp_bits, man_bits, bias);
    }

    //! Create an `APyFloatArray` with a given shape and bit specifiers
    static APyFloatArray
    create_array_static(const std::vector<std::size_t>& shape, const APyFloat& fp)
    {
        return APyFloatArray(
            shape, fp.get_exp_bits(), fp.get_man_bits(), fp.get_bias()
        );
    }

    //! Test if two floating-point vectors have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyFloatArray& other) const noexcept
    {
        return spec() == other.spec();
    }

    //! Test if `*this` has the same bit specifiers as another `APyFloat`
    APY_INLINE bool is_same_spec(const APyFloat& other) const noexcept
    {
        return spec() == other.spec();
    }

    //! Retrieve the bit specification
    APY_INLINE APyFloatSpec spec() const noexcept
    {
        return { exp_bits, man_bits, bias };
    }

    /* ****************************************************************************** *
     * *                       Elementary arithmetic operators                      * *
     * ****************************************************************************** */

public:
    //! Element-wise addition of two floating-point arrays
    APyFloatArray operator+(const APyFloatArray& rhs) const;
    //! Element-wise addition of a floating-point array and a floating-point scalar
    APyFloatArray operator+(const APyFloat& rhs) const;
    //! Unary plus
    APY_INLINE APyFloatArray operator+() const { return *this; };
    //! Element-wise subtraction of two floating-point arrays
    APyFloatArray operator-(const APyFloatArray& rhs) const;
    //! Element-wise subtraction of a floating-point array and a floating-point scalar
    APyFloatArray operator-(const APyFloat& rhs) const;
    //! Unary minus
    APyFloatArray operator-() const;
    //! Element-wise multiplication of two floating-point arrays
    APyFloatArray operator*(const APyFloatArray& rhs) const;
    //! Element-wise multiplication of a floating-point array and a floating-point
    //! scalar
    APyFloatArray operator*(const APyFloat& rhs) const;
    //! Element-wise division of two floating-point arrays
    APyFloatArray operator/(const APyFloatArray& rhs) const;
    //! Element-wise division of a floating-point array and a floating-point scalar
    APyFloatArray operator/(const APyFloat& rhs) const;

    //! Absolute value
    APyFloatArray abs() const;
    //! Elementwise R-division with floating-point scalar
    APyFloatArray rdiv(const APyFloat& lhs) const;

    //! Elementwise R-subtraction with floating-point scalar
    APyFloatArray rsub(const APyFloat& lhs) const;

    //! Elementwise logic not
    APyFloatArray operator~() const;

    /*!
     * Matrix multiplication. If both arguments are 2-D tensors, this method performs
     * the ordinary matrix multiplication. If input dimensions are greater than 2, this
     * method performs stacked matrix multiplications, where the dimensions of last two
     * dimensions are treated as matrices.
     */
    std::variant<APyFloatArray, APyFloat> matmul(const APyFloatArray& rhs) const;

    //! Python `__repr__()` function
    std::string repr() const;

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

public:
    //! Factory function for Python interface
    static void create_in_place(
        APyFloatArray* apyfloatarray,
        const nb::typed<nb::iterable, nb::any>& sign_seq,
        const nb::typed<nb::iterable, nb::any>& exp_seq,
        const nb::typed<nb::iterable, nb::any>& man_seq,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` tensor object initialized with values from a sequence
    //! of numbers
    static APyFloatArray from_numbers(
        const nb::typed<nb::iterable, nb::any>& number_seq,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` tensor object initialized with values from an ndarray
    static APyFloatArray from_array(
        const nanobind::ndarray<nanobind::c_contig>& ndarray,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` tensor object initialized from bit-representation
    static APyFloatArray from_bits(
        const nb::iterable& python_bit_patterns,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Set data fields based on an ndarray of bit patterns
    void _set_bits_from_ndarray(const nanobind::ndarray<nanobind::c_contig>& ndarray);

    //! Set data fields based on an ndarray of doubles
    void _set_values_from_ndarray(const nanobind::ndarray<nanobind::c_contig>& ndarray);

    /* ****************************************************************************** *
     *                           Conversion to other types                            *
     * ****************************************************************************** */
public:
    //! Retrieve a string of the stored values in this array.
    std::string to_string(int base = 10) const;
    std::string to_string_dec() const;

    /* ****************************************************************************** *
     *                     Static methods for array initialization                    *
     * ****************************************************************************** */

    //! Create an array of given shape and bit specifiers, filled with zeros.
    static APyFloatArray zeros(
        const PyShapeParam_t& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an array of given shape and bit specifiers, filled with ones.
    static APyFloatArray ones(
        const PyShapeParam_t& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create a 2-D array with ones on the diagonal and zeros elsewhere.
    static APyFloatArray
    eye(const nb::int_& N,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<nb::int_> M,
        std::optional<exp_t> bias = std::nullopt);

    //! Create the identity matrix, i.e. a square array with ones on the main diagonal.
    static APyFloatArray identity(
        const nb::int_& N,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` with evenly spaced values within a given interval.
    static APyFloatArray arange(
        const nb::object& start,
        const nb::object& stop,
        const nb::object& step,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` with all values within a given interval.
    static APyFloatArray fullrange(
        const nb::object& start,
        const nb::object& stop,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

    //! Perform a linear convolution with `other` using `mode`
    APyFloatArray convolve(const APyFloatArray& other, const std::string& mode) const;

    //! Sum over one or more axes.
    std::variant<APyFloatArray, APyFloat>
    sum(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative sum over one or more axes.
    APyFloatArray cumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Sum over one or more axes, treating Nan as 0.
    std::variant<APyFloatArray, APyFloat>
    nansum(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative sum over one or more axes, treating Nan as 0.
    APyFloatArray nancumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes.
    std::variant<APyFloatArray, APyFloat>
    prod(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes.
    APyFloatArray cumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes, treating Nan as 0
    std::variant<APyFloatArray, APyFloat>
    nanprod(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes, treating Nan as 0
    APyFloatArray nancumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Return the maximum of an array or the maximum along an axis.
    std::variant<APyFloatArray, APyFloat>
    max(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Return the minimum of an array or the minimum along an axis.
    std::variant<APyFloatArray, APyFloat>
    min(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Return the maximum of an array or the maximum along an axis. Ignoring nan
    std::variant<APyFloatArray, APyFloat>
    nanmax(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Return the minimum of an array or the minimum along an axis. Ignoring nan
    std::variant<APyFloatArray, APyFloat>
    nanmin(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Return a copy of the tensor with the elements resized.
    APyFloatArray cast(
        std::optional<int> exp_bits,
        std::optional<int> man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    //! Internal cast method when format is given fully.
    APyFloatArray _cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    //! Internal cast method when format and quantization mode is given.
    APyFloatArray _cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        QuantizationMode quantization
    ) const;

    //! Change the number of mantissa and exponent bits for cases where it is known that
    //! quantization does not happen, i.e., the resulting number of bits is not shorter.
    APyFloatArray cast_no_quant(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    ) const;
    //! Return the bias.
    APY_INLINE exp_t get_bias() const noexcept { return bias; }
    //! Return the bit width of the mantissa field.
    APY_INLINE std::uint8_t get_man_bits() const noexcept { return man_bits; }
    //! Return the bit width of the exponent field.
    APY_INLINE std::uint8_t get_exp_bits() const noexcept { return exp_bits; }
    //! Return the bit width of the entire floating-point format.
    APY_INLINE std::uint8_t get_bits() const noexcept
    {
        return exp_bits + man_bits + 1;
    }

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
        std::size_t dim, std::vector<APyFloatData>::const_iterator& it
    ) const;

    //! Convert to a NumPy array
    nanobind::ndarray<nanobind::numpy, double> to_numpy(
        std::optional<nb::object> dtype = std::nullopt,
        std::optional<bool> copy = std::nullopt
    ) const;

    //! Test if two floating-point arrays are identical, i.e., has the same values, and
    //! the same format
    bool is_identical(const nb::object& other, bool ignore_zero_sign = false) const;

    /* ****************************************************************************** *
     * *                          Convenience methods                               * *
     * ****************************************************************************** */

public:
    //! Convenience method when target format is known to correspond to a
    //! double-precision floating-point
    APyFloatArray
    cast_to_double(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Convenience method when target format is known to correspond to a
    //! single-precision floating-point
    APyFloatArray
    cast_to_single(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Convenience method when target format is known to correspond to a half-precision
    //! floating-point
    APyFloatArray
    cast_to_half(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Convenience method when target format is known to correspond to a 16-bit brain
    //! float
    APyFloatArray
    cast_to_bfloat16(std::optional<QuantizationMode> quantization = std::nullopt) const;

    //! Create an `APyFloatArray` tensor object initialized from bit-representation
    static APyFloatArray _from_bits_ndarray(
        const nb::ndarray<nb::c_contig>& ndarray,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /*!
     * Evaluate the inner between two vectors. This method assumes that the shape
     * of both `*this` and `rhs` are equally long. Anything else is undefined
     * behaviour.
     */
    APyFloat checked_inner_product(const APyFloatArray& rhs) const;

    /*!
     * Evaluate the matrix product between two 2D matrices. This method assumes that
     * the shape of `*this` and `rhs` have been checked to match a 2D matrix
     * multiplication.
     */
    APyFloatArray checked_2d_matmul(const APyFloatArray& rhs) const;
};

#endif
