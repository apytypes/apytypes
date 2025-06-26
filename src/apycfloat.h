#ifndef _APYCFLOAT_H
#define _APYCFLOAT_H

#include "apyfloat.h"
#include "nanobind/nanobind.h"

#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/variant.h>

class APyCFloat {

    /* ****************************************************************************** *
     * *                          APyCFloat data fields                             * *
     * ****************************************************************************** */

private:
    // Bit specifiers and exponent bias
    std::uint8_t exp_bits;
    std::uint8_t man_bits;
    exp_t bias;

    // Data of the `APyCFloat`
    APyFloatData _data[2]; // { real, imag }

    // Access real/imag
public:
    const APyFloatData& real() const noexcept { return _data[0]; }
    const APyFloatData& imag() const noexcept { return _data[1]; }
    APyFloatData& real() noexcept { return _data[0]; }
    APyFloatData& imag() noexcept { return _data[1]; }

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Copy `n` items from `it` into `*this`
    template <typename RANDOM_ACCESS_ITERATOR>
    void
    copy_n_from(RANDOM_ACCESS_ITERATOR src_it, [[maybe_unused]] std::size_t n) noexcept
    {
        using iterator_value_type = typename RANDOM_ACCESS_ITERATOR::value_type;
        static_assert(std::is_same_v<iterator_value_type, APyFloatData>);
        assert(n == 2);

        real() = src_it[0];
        imag() = src_it[1];
    }

    //! Copy `n` items from `*this` into `it`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_to(
        RANDOM_ACCESS_ITERATOR dst_it, [[maybe_unused]] std::size_t n
    ) const noexcept
    {
        using iterator_value_type = typename RANDOM_ACCESS_ITERATOR::value_type;
        static_assert(std::is_same_v<iterator_value_type, APyFloatData>);
        assert(n == 2);

        dst_it[0] = real();
        dst_it[1] = imag();
    }

    //! Test if two floating-point numbers have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyCFloat& other) const
    {
        return man_bits == other.man_bits && exp_bits == other.exp_bits
            && bias == other.bias;
    }

    //! Retrieve the bit specification
    APY_INLINE APyFloatSpec spec() const noexcept
    {
        return { exp_bits, man_bits, bias };
    }

    /* ****************************************************************************** *
     * *                              Constructors                                  * *
     * ****************************************************************************** */

public:
    //! Constructor only specifying the format, data fields are initialized to zero
    APyCFloat(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias);

    //! Constructor setting data fields using a struct
    APyCFloat(
        const APyFloatData& real_data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias
    );

    //! Constructor setting data fields using a struct
    APyCFloat(
        const APyFloatData& real_data,
        const APyFloatData& imag_data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias
    );

    /* ****************************************************************************** *
     * *                       Python exposed constructors                          * *
     * ****************************************************************************** */

    //! Zero-initialization from Python type bit-specification
    APyCFloat(
        const nb::int_& exp_bits,
        const nb::int_& man_bits,
        const std::optional<nb::int_>& bias
    );

    //! Python constructor setting the real field
    APyCFloat(
        const std::variant<nb::bool_, nb::int_>& sign,
        const nb::int_& exp,
        const nb::int_& man,
        const nb::int_& exp_bits,
        const nb::int_& man_bits,
        const std::optional<nb::int_>& bias
    );

    //! Python constructor setting both real and imaginary field
    APyCFloat(
        const nb::typed<
            nb::tuple,
            std::variant<nb::bool_, nb::int_>,
            std::variant<nb::bool_, nb::int_>>& sign,
        const nb::typed<nb::tuple, nb::int_, nb::int_>& exp,
        const nb::typed<nb::tuple, nb::int_, nb::int_>& man,
        const nb::int_& exp_bits,
        const nb::int_& man_bits,
        const std::optional<nb::int_>& bias
    );

    /* ****************************************************************************** *
     * *                      Static conversion from other types                    * *
     * ****************************************************************************** */

public:
    //! Create `APyCFloat` from Python object
    static APyCFloat from_number(
        const nb::object& py_obj,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create `APyCFloat` from double
    static APyCFloat from_double(
        double value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create `APyCFloat` from Python integer
    static APyCFloat from_integer(
        const nb::int_& value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create `APyCFloat` from Python complex
    static APyCFloat from_complex(
        std::complex<double> value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create `APyCFloat` from `APyCFixed`
    static APyCFloat from_fixed(
        const APyCFixed& value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create `APyCFloat` from `APyFixed`
    static APyCFloat from_fixed(
        const APyFixed& value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                         Binary comparison operators                        * *
     * ****************************************************************************** */

public:
    bool operator==(const APyCFloat& rhs) const;
    bool operator!=(const APyCFloat& rhs) const;

    bool operator==(const APyFloat& rhs) const;
    bool operator!=(const APyFloat& rhs) const;

    bool operator==(double rhs) const;
    bool operator!=(double rhs) const;

    bool operator==(const APyCFixed& rhs) const;
    bool operator!=(const APyCFixed& rhs) const;

    bool operator==(const APyFixed& rhs) const;
    bool operator!=(const APyFixed& rhs) const;

    /* ****************************************************************************** *
     * *                     Unary/binary arithmetic operators                      * *
     * ****************************************************************************** */

    // Unary negation
    APyCFloat operator+(const APyCFloat& rhs) const;
    APyCFloat operator-(const APyCFloat& rhs) const;
    APyCFloat operator*(const APyCFloat& rhs) const;
    APyCFloat operator/(const APyCFloat& rhs) const;

    // Unary negation
    APyCFloat operator-() const;
    APyCFloat operator+() const { return *this; };

    /* ****************************************************************************** *
     * *                       Non-computational functions                          * *
     * ****************************************************************************** */

public:
    //! Return the bit width of the mantissa field
    APY_INLINE std::uint8_t get_man_bits() const { return man_bits; }

    //! Return the bit width of the exponent field
    APY_INLINE std::uint8_t get_exp_bits() const { return exp_bits; }

    //! Return the bit width of the entire floating-point format
    APY_INLINE std::uint8_t get_bits() const { return man_bits + exp_bits + 1; }

    /* ****************************************************************************** *
     * *                                     Copy                                   * *
     * ****************************************************************************** */

    //! Copy scalar
    APyCFloat python_copy() const { return *this; }

    //! Deepcopy scalar (same as copy here)
    APyCFloat python_deepcopy(const nb::typed<nb::dict, nb::int_, nb::any>&) const
    {
        return *this;
    }

    /* ****************************************************************************** *
     * *                       Other public member functions                        * *
     * ****************************************************************************** */

public:
    //! Retrieve the Python string representation
    APyCFloat cast(
        std::optional<int> new_exp_bits,
        std::optional<int> new_man_bits,
        std::optional<exp_t> new_bias,
        std::optional<QuantizationMode> quantization
    ) const;

    //! Core cast method when it is known that the bit widths are not the same
    APyCFloat checked_cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        QuantizationMode quantization
    ) const;

    //! Retrieve the Python string representation
    std::string repr() const;

    //! Conversion to string
    std::string to_string(int base = 10) const;
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;

    //! Retrieve a `std::complex<double>`
    std::complex<double> to_complex() const;

    //! Test if two floating-point numbers are identical, i.e., has the same values, the
    //! same number of exponent bits, and the same number of mantissa bits
    bool is_identical(const nb::object& other, bool ignore_zero_sign = false) const;

    //! Retrieve real part
    APyFloat get_real() const;

    //! Retrieve imaginary part
    APyFloat get_imag() const;

    //! Retrieve the bias
    exp_t get_bias() const noexcept { return bias; }

    //! True if and only if value is zero.
    APY_INLINE bool is_zero() const
    {
        return real().exp == 0 && real().man == 0 && imag().exp == 0 && imag().man == 0;
    }

    //! Create an `APyCFloat` with the stored value one
    static APyCFloat
    one(std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt);

    /* ****************************************************************************** *
     * *                           Friends of `APyCFloat`                           * *
     * ****************************************************************************** */

    template <typename T, typename ARRAY_TYPE> friend class APyArray;
    friend class APyCFloatArray;
};

#endif
