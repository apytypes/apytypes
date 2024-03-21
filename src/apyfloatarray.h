
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
    explicit APyFloatArray(

        const nanobind::sequence& sign_seq,
        const nanobind::sequence& exp_seq,
        const nanobind::sequence& man_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

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
    APyFloatArray abs() const;
    APyFloatArray rtruediv(const APyFloat& rhs) const;

    /*!
     * Matrix mutliplication. If both arguments ar 2-D tensors, this method performs the
     * ordinary matrix multiplication. If input dimensions are greater than 2, this
     * method performs stacked matrix multiplications, where the dimensions of last two
     * dimensions are treated as matrices.
     */
    APyFloatArray matmul(const APyFloatArray& rhs) const;

    //! Python `__repr__()` function
    std::string repr() const;

    //! Shape of the array
    nanobind::tuple get_shape() const;

    //! Number of dimensions
    size_t get_ndim() const;

    //! Retrieve a single item
    std::variant<APyFloatArray, APyFloat> get_item(std::size_t idx) const;

    //! Length of the array
    size_t get_size() const;

    /*!
     * Test if two `APyFloatArray` objects are identical. Two `APyFloatArray` objects
     * are considered identical if, and only if:
     *   * They represent exatly the same tensor shape
     *   * They store the exact same floating-point values in all tensor elements
     *   * They have the exact same sized fields
     */
    bool is_identical(const APyFloatArray& other) const;

    //! Convert to a NumPy array
    nanobind::ndarray<nanobind::numpy, double> to_numpy() const;

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

    //! Create an `APyFloatArray` tensor object initialized with values from a sequence
    //! of `doubles`
    static APyFloatArray from_double(
        const nanobind::sequence& double_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    );

    void _set_values_from_numpy_ndarray(
        const nanobind::ndarray<nanobind::numpy>& ndarray, QuantizationMode quantization
    );

    //! Transposition function. For a 1-D array, returns an exact copy of `*this`. For
    //! a 2-D array, returns the matrix transposition of `*this`.
    APyFloatArray transpose() const;

    //! Returns a copy of the tensor with the elements resized.
    APyFloatArray cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    APyFloatArray cast_no_quant(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    ) const;

    APyFloatArray resize(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    inline exp_t get_bias() const { return bias; }
    inline std::uint8_t get_man_bits() const { return man_bits; }
    inline std::uint8_t get_exp_bits() const { return exp_bits; }

    enum class ArithmeticOperation { ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION };

    /* ******************************************************************************
     * * Convenience methods                                                        *
     * ******************************************************************************
     */
    APyFloatArray
    cast_to_double(std::optional<QuantizationMode> quantization = std::nullopt) const;
    APyFloatArray
    cast_to_single(std::optional<QuantizationMode> quantization = std::nullopt) const;
    APyFloatArray
    cast_to_half(std::optional<QuantizationMode> quantization = std::nullopt) const;
    APyFloatArray
    cast_to_bfloat16(std::optional<QuantizationMode> quantization = std::nullopt) const;

private:
    APyFloatArray() = default;
    APyFloatArray(
        const std::vector<std::size_t>& shape,
        exp_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    std::uint8_t exp_bits, man_bits;
    exp_t bias;
    std::vector<std::size_t> shape;
    std::vector<APyFloatData> data;

    //! Fold the `_shape` field over multiplication
    std::size_t fold_shape() const;

    /*!
     * Evaluate the inner between two vectors. This method assumes that the the shape
     * of both `*this` and `rhs` are equally long. Anything else is undefined
     * behaviour.
     */
    APyFloatArray checked_inner_product(const APyFloatArray& rhs) const;

    /*!
     * Evaluate the matrix product between two 2D matrices. This method assumes that
     * the shape of `*this` and `rhs` have been checked to match a 2D matrix
     * multiplication.
     */
    APyFloatArray checked_2d_matmul(const APyFloatArray& rhs) const;
};

#endif
