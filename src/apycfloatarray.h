#ifndef _APYCFLOATARRAY_H
#define _APYCFLOATARRAY_H

#include "apyarray.h"
#include "apycfloat.h"
#include "apytypes_fwd.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::ndarray
#include <nanobind/stl/complex.h> // std::complex<double> (with nanobind support)
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

// Standard includes
#include <cstdint>

class APyCFloatArray : public APyArray<APyFloatData, APyCFloatArray> {

    /* ****************************************************************************** *
     * *                       APyCFloatArray data fields                           * *
     * ****************************************************************************** */

private:
    // Bit specifiers and exponent bias
    std::uint8_t exp_bits;
    std::uint8_t man_bits;
    exp_t bias;

    /* ****************************************************************************** *
     * *                          Python constructors                               * *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyCFloatArray` objects. At lesast the
    //! bit-specifiers and shape has to be set during construction.
    APyCFloatArray() = delete;

    explicit APyCFloatArray(
        const nb::typed<nb::iterable, nb::any>& sign_seq,
        const nb::typed<nb::iterable, nb::any>& exp_seq,
        const nb::typed<nb::iterable, nb::any>& man_seq,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                     Non-Python accessible constructors                     * *
     * ****************************************************************************** */

public:
    //! Constructor specifying only the shape and format of the array
    APyCFloatArray(
        const std::vector<std::size_t>& shape,
        exp_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Name of this array type (used when throwing errors)
    static constexpr auto ARRAY_NAME = std::string_view("APyCFloatArray");

    APyCFloat create_scalar() const { return APyCFloat(exp_bits, man_bits, bias); }

    APyCFloatArray create_array(const std::vector<std::size_t>& shape) const
    {
        return APyCFloatArray(shape, exp_bits, man_bits, bias);
    }

    static APyCFloatArray
    create_array_static(const std::vector<std::size_t>& shape, const APyCFloat& fp)
    {
        return APyCFloatArray(
            shape, fp.get_exp_bits(), fp.get_man_bits(), fp.get_bias()
        );
    }

    //! Test if two floating-point vectors have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyCFloatArray& other) const noexcept
    {
        return spec() == other.spec();
    }

    //! Test if `*this` has the same bit specifiers as another `APyCFloat`
    APY_INLINE bool is_same_spec(const APyCFloat& other) const noexcept
    {
        return spec() == other.spec();
    }

    //! Retrieve the bit specification
    APY_INLINE APyFloatSpec spec() const noexcept
    {
        return { exp_bits, man_bits, bias };
    }

    /* ****************************************************************************** *
     * *                        Arithmetic member functions                         * *
     * ****************************************************************************** */
public:
    APyCFloatArray operator+(const APyCFloatArray& rhs) const;
    APyCFloatArray operator+(const APyCFloat& rhs) const;
    APyCFloatArray operator+() const;
    APyCFloatArray operator-(const APyCFloatArray& rhs) const;
    APyCFloatArray operator-(const APyCFloat& rhs) const;
    APyCFloatArray operator-() const;
    APyCFloatArray operator*(const APyCFloatArray& rhs) const;
    APyCFloatArray operator*(const APyCFloat& rhs) const;
    APyCFloatArray operator/(const APyCFloatArray& rhs) const;
    APyCFloatArray operator/(const APyCFloat& rhs) const;
    APyCFloatArray operator<<(const int shift_val) const;
    APyCFloatArray operator>>(const int shift_val) const;
    APyCFloatArray& operator<<=(const int shift_val);
    APyCFloatArray& operator>>=(const int shift_val);
    APyCFloatArray rsub(const APyCFloat& rhs) const;
    APyCFloatArray rdiv(const APyCFloat& rhs) const;

    //! Sum over one or more axes.
    std::variant<APyCFloatArray, APyCFloat>
    sum(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative sum over one or more axes.
    APyCFloatArray cumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Sum over one or more axes, treating Nan as 0.
    std::variant<APyCFloatArray, APyCFloat>
    nansum(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative sum over one or more axes, treating Nan as 0.
    APyCFloatArray nancumsum(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes.
    std::variant<APyCFloatArray, APyCFloat>
    prod(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes.
    APyCFloatArray cumprod(std::optional<nb::int_> axis = std::nullopt) const;

    //! Multiplication over one or more axes, treating Nan as 0
    std::variant<APyCFloatArray, APyCFloat>
    nanprod(const std::optional<PyShapeParam_t>& axis = std::nullopt) const;

    //! Cumulative multiplication over one or more axes, treating Nan as 0
    APyCFloatArray nancumprod(std::optional<nb::int_> axis = std::nullopt) const;

    /*!
     * Matrix multiplication. If both arguments are 2-D tensors, this method performs
     * the ordinary matrix multiplication. If input dimensions are greater than 2, this
     * method performs stacked matrix multiplications, where the dimensions of last two
     * dimensions are treated as matrices.
     */
    std::variant<APyCFloatArray, APyCFloat> matmul(const APyCFloatArray& rhs) const;

    /*!
     * Evaluate the inner between two vectors. This method assumes that the shape
     * of both `*this` and `rhs` are equally long. Anything else is undefined
     * behaviour.
     */
    APyCFloat checked_inner_product(const APyCFloatArray& rhs) const;

    /*!
     * Evaluate the matrix product between two 2D matrices. This method assumes that
     * the shape of `*this` and `rhs` have been checked to match a 2D matrix
     * multiplication.
     */
    APyCFloatArray checked_2d_matmul(const APyCFloatArray& rhs) const;

    //! Perform a linear convolution with `other` using `mode`
    APyCFloatArray convolve(const APyCFloatArray& other, const std::string& mode) const;

    /* ****************************************************************************** *
     * *                          Public member functions                           * *
     * ****************************************************************************** */

public:
    //! Retrieve the string representation
    std::string repr() const;

    //! Retrieve a string of the stored values in this array.
    std::string to_string(int base = 10) const;
    std::string to_string_dec() const;

    //! Convert to a NumPy array
    nanobind::ndarray<nanobind::numpy, std::complex<double>> to_numpy(
        std::optional<nb::object> dtype = std::nullopt,
        std::optional<bool> copy = std::nullopt
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

    //! Test if two floating-point arrays are identical, i.e., has the same values, and
    //! the same format
    bool is_identical(const nb::object& other, bool ignore_zero_sign = false) const;

    /* ****************************************************************************** *
     * *                           Static array creation                            * *
     * ****************************************************************************** */

public:
    //! Create an `APyCFloatArray` initialized with zeros
    static APyCFloatArray zeros(
        const PyShapeParam_t& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyCFloatArray` initialized with ones
    static APyCFloatArray ones(
        const PyShapeParam_t& shape,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyCFloatArray` with ones on the diagonal and zeros elsewhere
    static APyCFloatArray
    eye(const nb::int_& N,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<nb::int_> M,
        std::optional<exp_t> bias = std::nullopt);

    //! Create a square `APyCFloatArray` with ones on the diagonal and zeros elsewhere
    static APyCFloatArray identity(
        const nb::int_& N,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyCFloatArray` tensor object initialized with values from a sequence
    //! of `complex`
    static APyCFloatArray from_complex(
        const nb::typed<nb::iterable, nb::any>& cplx_seq,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyCFloatArray` tensor object initialized with values from a sequence
    //! of complex-like numbers
    static APyCFloatArray from_numbers(
        const nb::typed<nb::iterable, nb::any>& number_seq,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create an `APyCFloatArray` tensor object initialized with values from an ndarray
    static APyCFloatArray from_array(
        const nb::ndarray<nb::c_contig>& ndarray,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

private:
    //! Set data fields based on an ndarray of doubles
    void _set_values_from_ndarray(const nanobind::ndarray<nanobind::c_contig>& ndarray);

    /* ****************************************************************************** *
     * *                           Other member functions                           * *
     * ****************************************************************************** */
public:
    //! Return a copy of the tensor with the elements resized.
    APyCFloatArray cast(
        std::optional<int> exp_bits,
        std::optional<int> man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    //! Internal cast method when format and quantization mode is given.
    APyCFloatArray _cast(
        std::uint8_t new_exp_bits,
        std::uint8_t new_man_bits,
        exp_t new_bias,
        QuantizationMode quantization
    ) const;
};

#endif
