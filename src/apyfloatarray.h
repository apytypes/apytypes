#ifndef _APYFLOAT_ARRAY_H
#define _APYFLOAT_ARRAY_H

#include "apyfloat.h"
#include "apytypes_common.h"
#include <cstdint>
#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::ndarray
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
#include <optional>
#include <vector>

class APyFloatArray {
public:
    //! Constructor taking a sequence of signs, biased exponents, and mantissas.
    //! If no bias is given, an IEEE-like bias will be used.
    explicit APyFloatArray(
        const nanobind::sequence& sign_seq,
        const nanobind::sequence& exp_seq,
        const nanobind::sequence& man_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Vector with values
    std::vector<APyFloatData> data;
    //! Number of exponent bits
    std::uint8_t exp_bits;
    //! Number of mantissa bits
    std::uint8_t man_bits;
    //! Exponent bias
    exp_t bias;

    /* ****************************************************************************** *
     * *                       Binary arithmetic operators                          * *
     * ****************************************************************************** */

    APyFloatArray operator+(const APyFloatArray& rhs) const;
    APyFloatArray operator+(const APyFloat& rhs) const;
    APyFloatArray operator-(const APyFloatArray& rhs) const;
    APyFloatArray operator-(const APyFloat& rhs) const;
    APyFloatArray operator-() const;
    APyFloatArray operator*(const APyFloatArray& rhs) const;
    APyFloatArray operator*(const APyFloat& rhs) const;
    APyFloatArray operator/(const APyFloatArray& rhs) const;
    APyFloatArray operator/(const APyFloat& rhs) const;
    //! Absolute value
    APyFloatArray abs() const;
    //! Elementwise division with floating-point scalar
    APyFloatArray rtruediv(const APyFloat& rhs) const;

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

    //! Factory function for Python interface
    static void create_in_place(
        APyFloatArray* apyfloatarray,
        const nanobind::sequence& sign_seq,
        const nanobind::sequence& exp_seq,
        const nanobind::sequence& man_seq,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` tensor object initialized with values from a sequence
    //! of `doubles`
    static APyFloatArray from_double(
        const nanobind::sequence& double_seq,
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

    //! Set data fields based on an and-array of doubles
    void _set_values_from_ndarray(const nanobind::ndarray<nanobind::c_contig>& ndarray);

    /* ****************************************************************************** *
     *                     Static methods for array initialization                    *
     * ****************************************************************************** */

    static APyFloatArray zeros(
        const nb::tuple& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    static APyFloatArray ones(
        const nb::tuple& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    static APyFloatArray
    eye(const nb::int_& N,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<nb::int_> M,
        std::optional<exp_t> bias = std::nullopt);

    static APyFloatArray identity(
        const nb::int_& N,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyFloatArray` initialized with a specified fill value
    static APyFloatArray full(const nb::tuple& shape, const APyFloat& fill_value);

    //! Create an `APyFloatArray` with a specified diagonal value
    static APyFloatArray diagonal(const nb::tuple& shape, const APyFloat& fill_value);

    //! Create an `APyFloatArray` with evenly spaced values within a given interval
    static APyFloatArray arange(
        const nb::object& start,
        const nb::object& stop,
        const nb::object& step,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

    //! Reshape into a new shape.
    APyFloatArray reshape(nb::tuple new_shape) const;

    //! Reshape flatten into 1d shape.
    APyFloatArray flatten() const;

    //! Same as flatten as for now.
    APyFloatArray ravel() const;

    //! Broadcast to a new shape.
    APyFloatArray broadcast_to(const std::vector<std::size_t> shape) const;

    //! Return a copy where all axes of size 1 is removed.
    APyFloatArray
    squeeze(std::optional<std::variant<nb::int_, nb::tuple>> axis = std::nullopt) const;

    //! Perform a linear convolution with `other` using `mode`
    APyFloatArray convolve(const APyFloatArray& other, const std::string& mode) const;

    //! Sum over one or more axes.
    std::variant<APyFloatArray, APyFloat>
    sum(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Cumulative sum over one or more axes.
    APyFloatArray cumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Sum over one or more axes, treating Nan as 0.
    std::variant<APyFloatArray, APyFloat>
    nansum(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Cumulative sum over one or more axes, treatíng Nan as 0.
    APyFloatArray nancumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes.
    std::variant<APyFloatArray, APyFloat>
    prod(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes.
    APyFloatArray cumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes, treating Nan as 0
    std::variant<APyFloatArray, APyFloat>
    nanprod(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes, treating Nan as 0
    APyFloatArray nancumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Return the maximum of an array or the maximum along an axis.
    std::variant<APyFloatArray, APyFloat>
    max(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Return the minimum of an array or the minimum along an axis.
    std::variant<APyFloatArray, APyFloat>
    min(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Return the maximum of an array or the maximum along an axis. Ignoring nan
    std::variant<APyFloatArray, APyFloat>
    nanmax(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Return the minimum of an array or the minimum along an axis. Ignoring nan
    std::variant<APyFloatArray, APyFloat>
    nanmin(std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt) const;

    //! Python-exposed `broadcast_to`
    APyFloatArray broadcast_to_python(const std::variant<nb::tuple, nb::int_> shape
    ) const;

    //! Swaps the positions of two axes in the array.
    APyFloatArray swapaxes(nb::int_ axis1, nb::int_ axis2) const;

    //! Transposition function. For a 1-D array, Return an exact copy of `*this`. For
    //! a 2-D array, Return the matrix transposition of `*this`.
    APyFloatArray transpose(std::optional<nb::tuple> axes = std::nullopt) const;

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
    APY_INLINE exp_t get_bias() const { return bias; }
    //! Return the bit width of the mantissa field.
    APY_INLINE std::uint8_t get_man_bits() const { return man_bits; }
    //! Return the bit width of the exponent field.
    APY_INLINE std::uint8_t get_exp_bits() const { return exp_bits; }
    //! Return the bit width of the entire floating-point format.
    APY_INLINE std::uint8_t get_bits() const { return exp_bits + man_bits + 1; }

    //! Shape of the array
    nanobind::tuple python_get_shape() const;

    //! Number of dimensions
    size_t get_ndim() const;

    //! Length of the array
    size_t get_size() const;

    const std::vector<std::size_t>& get_shape() const noexcept { return shape; }

    /*!
     * Test if two `APyFloatArray` objects are identical. Two `APyFloatArray` objects
     * are considered identical if, and only if:
     *   * They represent exactly the same tensor shape
     *   * They store the exact same floating-point values in all tensor elements
     *   * They have the exact same sized fields
     */
    bool is_identical(const APyFloatArray& other) const;

    //! Test if two `APyFloatArray` objects have the same format
    APY_INLINE bool same_type_as(const APyFloatArray& other) const
    {
        return man_bits == other.man_bits && exp_bits == other.exp_bits
            && bias == other.bias;
    }

    //! Test if the `APyFloatArray` object has the same format as a `APyFloat` object
    APY_INLINE bool same_type_as(const APyFloat& other) const
    {
        return man_bits == other.get_man_bits() && exp_bits == other.get_exp_bits()
            && bias == other.get_bias();
    }

    /* ****************************************************************************** *
     * *            `__getitem__` and `__setitem__` family of methods               * *
     * ****************************************************************************** */

    //! Top-level `__getitem__` function
    std::variant<APyFloatArray, APyFloat>
    get_item(std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple> key) const;

    std::variant<APyFloatArray, APyFloat> get_item_integer(std::ptrdiff_t idx) const;

    std::variant<APyFloatArray, APyFloat>
    get_item_tuple(std::vector<std::variant<nb::int_, nb::slice>> tuple) const;

    APyFloatArray get_item_slice(nb::slice slice) const;

    std::vector<APyFloatArray> get_item_slice_nested(nb::slice slice) const;

    std::vector<std::size_t> get_item_tuple_shape(
        const std::vector<std::variant<nb::int_, nb::slice>>& tuple,
        const std::vector<std::variant<nb::int_, nb::slice>>& remaining
    ) const;

    std::vector<std::variant<nb::int_, nb::slice>>
    get_item_to_cpp_tuple(const nb::tuple& key) const;

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
    nanobind::ndarray<nanobind::numpy, double> to_numpy() const;

    /* ******************************************************************************
     * * Convenience methods                                                        *
     * ******************************************************************************
     */
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

private:
    //! Default constructor
    APyFloatArray() = default;
    //! Constructor specifying only the shape and format of the array
    APyFloatArray(
        const std::vector<std::size_t>& shape,
        exp_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    std::vector<std::size_t> shape;

    /*!
     * Evaluate the inner between two vectors. This method assumes that the the shape
     * of both `*this` and `rhs` are equally long. Anything else is undefined
     * behaviour.
     */
    APyFloat checked_inner_product(
        const APyFloatArray& rhs,
        std::optional<APyFloatAccumulatorOption> accumulator_mode,
        const std::uint8_t max_exp_bits,
        const std::uint8_t max_man_bits
    ) const;

    /*!
     * Evaluate the matrix product between two 2D matrices. This method assumes that
     * the shape of `*this` and `rhs` have been checked to match a 2D matrix
     * multiplication.
     */
    APyFloatArray checked_2d_matmul(const APyFloatArray& rhs) const;
    //! Compute the sum of all elements
    APyFloat vector_sum(const QuantizationMode quantization) const;
    /*!
     * Perform hadamard multiplication of `*this` and `rhs`, and store result in `res`.
     * This method assumes that the shape of `*this` and `rhs` are equal,
     * anything else is undefined behaviour.
     */
    void hadamard_multiplication(
        const APyFloatData* rhs,
        const uint8_t rhs_exp_bits,
        const uint8_t rhs_man_bits,
        const exp_t rhs_bias,
        APyFloatArray& res,
        const QuantizationMode quantization
    ) const;

    /*
     * Friend functions
     */
    template <
        typename RANDOM_ACCESS_ITERATOR_IN,
        typename RANDOM_ACCESS_ITERATOR_INOUT,
        typename APYFLOAT_TYPE>
    friend void float_product(
        RANDOM_ACCESS_ITERATOR_IN src1,
        RANDOM_ACCESS_ITERATOR_IN src2,
        RANDOM_ACCESS_ITERATOR_INOUT dst,
        const APYFLOAT_TYPE& x, // Floating point src1
        const APYFLOAT_TYPE& y  // Floating point src2
    );

    // internal function for the prod,sum,nanprod and nansum functions
    std::variant<APyFloatArray, APyFloat> prod_sum_function(
        void (*pos_func)(std::size_t, std::size_t, std::size_t, APyFloatArray&, APyFloatArray&, APyFloat&, APyFloat&),
        std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt
    ) const;

    // internal function for the cumprod,cumsum,nancumprod and nancumsum functions
    APyFloatArray cumulative_prod_sum_function(
        void (*pos_func)(std::size_t, std::size_t, std::size_t, APyFloatArray&, APyFloatArray&, APyFloat&, APyFloat&),
        std::optional<nb::int_> axis = std::nullopt
    ) const;

    std::variant<APyFloatArray, APyFloat> max_min_helper_function(
        bool (*comp_func)(APyFloat&, APyFloat&),
        std::optional<std::variant<nb::tuple, nb::int_>> axis = std::nullopt
    ) const;
};

#endif
