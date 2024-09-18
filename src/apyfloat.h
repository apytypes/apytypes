#ifndef _APYFLOAT_H
#define _APYFLOAT_H

#include <nanobind/nanobind.h>

#include <cstdint>
#include <optional>
#include <string>

#include "apyfixed.h"
#include "apytypes_common.h"

#include "../extern/mini-gmp/mini-gmp.h"

class APyFloat {
public:
    /* ******************************************************************************
     * * Constructors                                                               *
     * ******************************************************************************
     */

    // These constructors are not exposed to Python

    //! Constructor with optional bias, all fields are set.
    //! If no bias is given, an IEEE-like bias will be used.
    explicit APyFloat(
        bool sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Constructor setting all fields, no optionals
    APyFloat(
        bool sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias
    );
    //! Constructor setting data fields using a struct, mostly used by the APyFloatArray
    //! class
    APyFloat(
        const APyFloatData& data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias
    );
    //! Constructor where only the format is specified, with an optional bias. Data
    //! fields are initialized to zero. If no bias is given, an IEEE-like bias will be
    //! used.
    APyFloat(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Constructor only specifying the format, data fields are initialized to zero
    APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias);

    /* ******************************************************************************
     * * Methods for conversions                                                    *
     * ******************************************************************************
     */
    //! Factory function for Python interface
    static void create_in_place(
        APyFloat* apyfloat,
        int sign,
        exp_t exp,
        man_t man,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create APyFloat from double
    static APyFloat from_number(
        const nb::object& py_obj,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Create APyFloat from double
    static APyFloat from_double(
        double value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Create APyFloat from Python integer
    static APyFloat from_integer(
        const nb::int_ value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Create APyFloat from APyFixed
    static APyFloat from_fixed(
        APyFixed value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    //! Cast to double
    double to_double() const;
    operator double() const;

    //! Create APyFloat from bit-representation
    static APyFloat from_bits(
        nanobind::int_ python_long_int_bit_pattern,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Convert the underlying bit pattern to a Python long integer
    nanobind::int_ to_bits() const;

    //! Cast method exposed to Python
    APyFloat cast(
        std::optional<int> exp_bits,
        std::optional<int> man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    //! Internal cast method when format is given fully
    APyFloat _cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    //! Internal cast method when format and quantization mode is given
    APyFloat _cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        QuantizationMode quantization
    ) const;

    //! Core cast method when it is known that the bit widths are not the same
    APyFloat _checked_cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        QuantizationMode quantization
    ) const;

    //! Change the number of mantissa bits. The number is assumed not be NaN or Inf.
    //! The exponent is updated in case of carry.
    void cast_mantissa(std::uint8_t man_bits, QuantizationMode quantization);

    //! Decrease (not increase) the number of mantissa bits.
    //! The number is assumed not be NaN or Inf.
    //! The exponent is updated in case of carry.
    void cast_mantissa_shorter(std::uint8_t man_bits, QuantizationMode quantization);

    //! Change the number of mantissa bits. The number is assumed not be NaN or Inf.
    //! The exponent is assumed to be 0 and is updated in case of carry.
    void
    cast_mantissa_subnormal(std::uint8_t man_bits_delta, QuantizationMode quantization);

    //! Change the number of mantissa and exponent bits for cases where it is known that
    //! quantization does not happen, i.e., the resulting number of bits is not shorter.
    APyFloat cast_no_quant(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    ) const;

    //! Change the number of mantissa and exponent bits for cases where it is known that
    //! quantization does not happen, i.e., the resulting number of bits is not shorter.
    APyFloat
    cast_no_quant(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias) const;

    //! Simplified casting when the input is known to correspond to a double.
    //! Values too large will become infinity, and not saturate depending on the
    //! quantization mode.
    APyFloat
    cast_from_double(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias) const;

    //! Simplified casting when the input is known that the result will correspond to a
    //! double.
    APyFloat _cast_to_double() const;

    //! String representation
    std::string str() const;
    //! Python representation
    std::string repr() const;

    //! LaTeX representation
    std::string latex() const;

    /* ******************************************************************************
     * * Arithmetic operators                                                       *
     * ******************************************************************************
     */

    APyFloat operator+(const APyFloat& rhs) const;
    APyFloat operator-(const APyFloat& rhs) const;
    APyFloat operator-() const;
    APyFloat operator*(const APyFloat& rhs) const;
    APyFloat operator/(const APyFloat& rhs) const;
    APyFloat& operator+=(const APyFloat& rhs);

    /* ******************************************************************************
     * * Mathematical functions                                                     *
     * ******************************************************************************
     */

    // Absolute value
    APyFloat abs() const;
    //! Power function with another APyFloat as the exponent
    static APyFloat pow(const APyFloat& x, const APyFloat& y);
    //! Power function with integer exponent
    static APyFloat pown(const APyFloat& x, int n);

    /* ******************************************************************************
     * * Binary logic operators                                                     *
     * ******************************************************************************
     */

    APyFloat operator&(const APyFloat& rhs) const;
    APyFloat operator|(const APyFloat& rhs) const;
    APyFloat operator^(const APyFloat& rhs) const;
    APyFloat operator~() const;

    /* ******************************************************************************
     * * Binary comparison operators                                                *
     * ******************************************************************************
     */

    bool operator==(const APyFloat& rhs) const;
    bool operator!=(const APyFloat& rhs) const;
    bool operator<=(const APyFloat& rhs) const;
    bool operator<(const APyFloat& rhs) const;
    bool operator>=(const APyFloat& rhs) const;
    bool operator>(const APyFloat& rhs) const;
    bool operator==(const double rhs) const;
    bool operator!=(const double rhs) const;
    bool operator<=(const double rhs) const;
    bool operator<(const double rhs) const;
    bool operator>=(const double rhs) const;
    bool operator>(const double rhs) const;
    bool operator==(const APyFixed& rhs) const;
    bool operator!=(const APyFixed& rhs) const;
    bool operator<=(const APyFixed& rhs) const;
    bool operator<(const APyFixed& rhs) const;
    bool operator>=(const APyFixed& rhs) const;
    bool operator>(const APyFixed& rhs) const;

    /* ******************************************************************************
     * * Non-computational functions                                                *
     * ******************************************************************************
     */

    //! True if and only if value is normal (not zero, subnormal, infinite, or NaN).
    APY_INLINE bool is_normal() const
    {
        return !is_zero_exponent() && !is_max_exponent();
    }

    //! True if and only if value is zero, subnormal, or normal.
    APY_INLINE bool is_finite() const { return is_subnormal() || !is_max_exponent(); }

    //! True if and only if value is subnormal.
    APY_INLINE bool is_subnormal() const { return is_zero_exponent() && man != 0; }

    //! True if and only if value is zero.
    APY_INLINE bool is_zero() const { return is_zero_exponent() && man == 0; }

    //! True if and only if value is NaN.
    APY_INLINE bool is_nan() const { return man != 0 && is_max_exponent(); }

    //! True if and only if value is infinite.
    APY_INLINE bool is_inf() const { return man == 0 && is_max_exponent(); }

    //! True if and only if value is infinite or NaN.
    APY_INLINE bool is_max_exponent() const { return exp == max_exponent(); }

    //! True if and only if value is zero or subnormal.
    APY_INLINE bool is_zero_exponent() const { return exp == 0; }

    //! Return the stored sign
    APY_INLINE bool get_sign() const { return sign; }
    //! Return the stored mantissa, i.e. without leading one
    APY_INLINE man_t get_man() const { return man; }
    //! Return the stored exponent, i.e. with bias added
    APY_INLINE exp_t get_exp() const { return exp; }
    //! Return the bias
    APY_INLINE exp_t get_bias() const { return bias; }
    //! Return the bit width of the mantissa field
    APY_INLINE std::uint8_t get_man_bits() const { return man_bits; }
    //! Return the bit width of the exponent field
    APY_INLINE std::uint8_t get_exp_bits() const { return exp_bits; }
    //! Return the bit width of the entire floating-point format
    APY_INLINE std::uint8_t get_bits() const { return man_bits + exp_bits + 1; }
    //! Return all data fields packed in a struct
    APY_INLINE APyFloatData get_data() const { return { sign, exp, man }; }
    //! Return the mantissa with potential leading one
    APY_INLINE man_t true_man() const
    {
        return ((static_cast<man_t>(is_normal()) << man_bits) | man);
    }
    //! Return the mantissa with potential leading one
    APY_INLINE std::int64_t true_exp() const
    {
        return (std::int64_t)exp - (std::int64_t)bias + is_subnormal();
    }

    //! Set the data fields using a struct
    APY_INLINE void set_data(const APyFloatData& data)
    {
        sign = data.sign;
        exp = data.exp;
        man = data.man;
    }
    //! Set the sign
    APY_INLINE void set_sign(bool new_sign) { sign = new_sign; }

    //! Calculate the IEEE-like bias based on the number of exponent bits
    APY_INLINE static exp_t ieee_bias(std::uint8_t exp_bits)
    {
        return (1ULL << (exp_bits - 1)) - 1;
    }

    //! Set floating-point object to positive or negative zero
    APY_INLINE void set_to_zero(bool new_sign)
    {
        sign = new_sign;
        exp = 0;
        man = 0;
    }

    //! Set floating-point object to zero
    APY_INLINE void set_to_zero()
    {
        exp = 0;
        man = 0;
    }

    //! Set floating-point object to infinity
    APY_INLINE void set_to_inf()
    {
        exp = max_exponent();
        man = 0;
    }

    //! Set floating-point object to NaN
    APY_INLINE void set_to_nan()
    {
        exp = max_exponent();
        man = 1;
    }

    //! Factory method for creating a floating-point object in the same format
    //! with a value of positive or negative zero.
    APyFloat construct_zero(std::optional<bool> new_sign = std::nullopt) const;
    //! Factory method for creating a floating-point object in the same format
    //! with a value of positive or negative infinity.
    APyFloat construct_inf(std::optional<bool> new_sign = std::nullopt) const;
    //! Factory method for creating a floating-point object in the same format
    //! with a value of NaN.
    APyFloat
    construct_nan(std::optional<bool> new_sign = std::nullopt, man_t payload = 1) const;

    //! Test if two floating-point numbers are identical, i.e., has the same value, and
    //! the same format
    bool is_identical(const APyFloat& other) const;

    //! Convert to a fixed-point object
    APyFixed to_fixed() const;

    /* ******************************************************************************
     * * Convenience methods                                                        *
     * ******************************************************************************
     */
    //! Convenience method when target format is known to correspond to a
    //! double-precision floating-point
    APyFloat
    cast_to_double(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Convenience method when target format is known to correspond to a
    //! single-precision floating-point
    APyFloat
    cast_to_single(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Convenience method when target format is known to correspond to a half-precision
    //! floating-point
    APyFloat
    cast_to_half(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Convenience method when target format is known to correspond to a 16-bit brain
    //! float
    APyFloat
    cast_to_bfloat16(std::optional<QuantizationMode> quantization = std::nullopt) const;
    //! Get the smallest floating-point number in the same format that compares greater.
    APyFloat next_up() const;
    //! Get the largest floating-point number in the same format that compares less.
    APyFloat next_down() const;

private:
    std::uint8_t exp_bits, man_bits;
    exp_t bias;
    bool sign;
    exp_t exp; // Biased exponent
    man_t man; // Hidden one

    /* ******************************************************************************
     * * Non-Python accessible constructors                                         *
     * ******************************************************************************
     */

    APyFloat() = default;

    /* ******************************************************************************
     * * Helper functions                                                           *
     * ******************************************************************************
     */

    //! Update data fields based on a bit pattern
    APyFloat& update_from_bits(nanobind::int_ python_long_int_bit_pattern);

    //! Create a bit mask for the exponent field
    APY_INLINE exp_t exp_mask() const { return ((1ULL << exp_bits) - 1); }
    //! Calculate the maximum value for the exponent field
    APY_INLINE exp_t max_exponent() const
    {
        return ((1ULL << exp_bits) - 1);
    } // Max exponent with bias
    //! Calculate the IEEE-like bias for this particular format
    APY_INLINE exp_t ieee_bias() const { return ieee_bias(exp_bits); }
    //! Create a bit mask for the mantissa field
    APY_INLINE man_t man_mask() const { return (1ULL << man_bits) - 1; }
    //! Create a bit mask for the leading one
    APY_INLINE man_t leading_one() const { return (1ULL << man_bits); }

    //! Create a floating-point object that is normalized
    APyFloat normalized() const;

    //! Test if two floating-point numbers are identical, i.e., has the same value, and
    //! the same format
    APY_INLINE bool same_type_as(const APyFloat& other) const
    {
        return man_bits == other.man_bits && exp_bits == other.exp_bits
            && bias == other.bias;
    }
};

#endif // _APYFLOAT_H
