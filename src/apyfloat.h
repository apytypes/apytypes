#ifndef _APYFLOAT_H
#define _APYFLOAT_H

#include <nanobind/nanobind.h>

#include <cstdint>
#include <optional>
#include <string>

#include "apyfixed.h"
#include "apytypes_common.h"

#include "../extern/mini-gmp/mini-gmp.h"

/*!
 * Sizes of APyFloat datatypes
 */
static constexpr std::size_t _MAN_T_SIZE_BYTES = sizeof(man_t);
static constexpr std::size_t _MAN_T_SIZE_BITS = 8 * _MAN_T_SIZE_BYTES;
static constexpr std::size_t _EXP_T_SIZE_BYTES = sizeof(exp_t);
static constexpr std::size_t _EXP_T_SIZE_BITS = 8 * _EXP_T_SIZE_BYTES;

static constexpr std::size_t _MAN_LIMIT_BITS = _MAN_T_SIZE_BITS - 3;
static constexpr std::size_t _EXP_LIMIT_BITS = _EXP_T_SIZE_BITS - 2;

class APyFloat {
public:
    /* ******************************************************************************
     * * Constructors                                                               *
     * ******************************************************************************
     */

    // These constructors are not exposed to Python
    explicit APyFloat(
        bool sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    APyFloat(
        bool sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias
    );
    APyFloat(
        const APyFloatData& data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    APyFloat(
        const APyFloatData& data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias
    );
    APyFloat(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias);

    /* ******************************************************************************
     * * Methods for conversions                                                    *
     * ******************************************************************************
     */
    // Factory function for Python interface
    static void create_in_place(
        APyFloat* apyfloat,
        int sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    static APyFloat from_double(
        double value,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    double to_double() const;
    operator double() const;

    static APyFloat from_bits(
        nanobind::int_ python_long_int_bit_pattern,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Convert the underlying bit pattern to a Python long integer
    nanobind::int_ to_bits() const;

    APyFloat cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    APyFloat _cast(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        exp_t bias,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    APyFloat _cast(
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
    void cast_mantissa_subnormal(std::uint8_t man_bits, QuantizationMode quantization);

    APyFloat cast_no_quant(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    ) const;

    APyFloat
    cast_no_quant(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias) const;

    APyFloat
    cast_from_double(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias) const;

    APyFloat _cast_to_double() const;

    APyFloat resize(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    ) const;

    std::string str() const;
    std::string repr() const;
    std::string pretty_string() const;

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

    /* ******************************************************************************
     * * Mathematical functions                                                     *
     * ******************************************************************************
     */

    APyFloat abs() const;
    static APyFloat pow(const APyFloat& x, const APyFloat& y);
    static APyFloat pown(const APyFloat& x, int n);

    /* ******************************************************************************
     * * Binary logic operators                                                     *
     * ******************************************************************************
     */

    APyFloat operator&(APyFloat& rhs);
    APyFloat operator|(APyFloat& rhs);
    APyFloat operator^(APyFloat& rhs);
    APyFloat operator~();

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
    bool operator==(const float rhs) const;
    bool operator!=(const float rhs) const;
    bool operator<=(const float rhs) const;
    bool operator<(const float rhs) const;
    bool operator>=(const float rhs) const;
    bool operator>(const float rhs) const;
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
    APY_INLINE bool is_normal() const { return !is_subnormal() && !is_max_exponent(); }

    //! True if and only if value is zero, subnormal, or normal.
    APY_INLINE bool is_finite() const { return is_subnormal() || !is_max_exponent(); }

    //! True if and only if value is subnormal. Zero is also considered a subnormal
    //! number.
    APY_INLINE bool is_subnormal() const { return exp == 0; }

    //! True if and only if value is zero.
    APY_INLINE bool is_zero() const { return is_subnormal() && man == 0; }

    //! True if and only if value is NaN.
    APY_INLINE bool is_nan() const { return man != 0 && is_max_exponent(); }

    //! True if and only if value is infinite.
    APY_INLINE bool is_inf() const { return man == 0 && is_max_exponent(); }

    //! True if and only if value is infinite or NaN.
    APY_INLINE bool is_max_exponent() const { return exp == max_exponent(); }

    APY_INLINE bool get_sign() const { return sign; }
    APY_INLINE man_t get_man() const { return man; }
    APY_INLINE exp_t get_exp() const { return exp; }
    APY_INLINE exp_t get_bias() const { return bias; }
    APY_INLINE std::uint8_t get_man_bits() const { return man_bits; }
    APY_INLINE std::uint8_t get_exp_bits() const { return exp_bits; }
    APY_INLINE std::uint8_t get_bits() const { return man_bits + exp_bits + 1; }
    APY_INLINE APyFloatData get_data() const { return { sign, exp, man }; }
    APY_INLINE man_t true_man() const { return leading_bit() | man; }
    APY_INLINE std::int64_t true_exp() const
    {
        return (std::int64_t)exp - (std::int64_t)bias + is_subnormal();
    }

    APY_INLINE void set_data(const APyFloatData& data)
    {
        sign = data.sign;
        exp = data.exp;
        man = data.man;
    }
    APY_INLINE void set_sign(bool new_sign) { sign = new_sign; }

    APY_INLINE static exp_t ieee_bias(std::uint8_t exp_bits)
    {
        return (1ULL << (exp_bits - 1)) - 1;
    }

    APyFloat construct_zero(std::optional<bool> new_sign = std::nullopt) const;
    APyFloat construct_inf(std::optional<bool> new_sign = std::nullopt) const;
    APyFloat
    construct_nan(std::optional<bool> new_sign = std::nullopt, man_t payload = 1) const;

    //! Test if two floating-point numbers are identical, i.e., has the same value, and
    //! the same format
    bool is_identical(const APyFloat& other) const;

    APyFixed to_fixed() const;

    /* ******************************************************************************
     * * Convenience methods                                                        *
     * ******************************************************************************
     */
    APyFloat
    cast_to_double(std::optional<QuantizationMode> quantization = std::nullopt) const;
    APyFloat
    cast_to_single(std::optional<QuantizationMode> quantization = std::nullopt) const;
    APyFloat
    cast_to_half(std::optional<QuantizationMode> quantization = std::nullopt) const;
    APyFloat
    cast_to_bfloat16(std::optional<QuantizationMode> quantization = std::nullopt) const;

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
    APyFloat& update_from_bits(nanobind::int_ python_long_int_bit_pattern);

    APY_INLINE exp_t exp_mask() const { return ((1ULL << exp_bits) - 1); }
    APY_INLINE exp_t max_exponent() const
    {
        return ((1ULL << exp_bits) - 1);
    } // Max exponent with bias
    APY_INLINE exp_t ieee_bias() const { return ieee_bias(exp_bits); }
    APY_INLINE man_t man_mask() const { return (1ULL << man_bits) - 1; }
    APY_INLINE man_t leading_one() const { return (1ULL << man_bits); }
    APY_INLINE man_t leading_bit() const
    {
        return (static_cast<man_t>(is_normal()) << man_bits);
    }

    APyFloat normalized() const;
    static void quantize_apymantissa(
        APyFixed& apyman, bool sign, int bits, QuantizationMode quantization
    );
    static QuantizationMode
    translate_quantization_mode(QuantizationMode quantization, bool sign);

    //! Test if two floating-point numbers are identical, i.e., has the same value, and
    //! the same format
    APY_INLINE bool same_type_as(const APyFloat& other) const
    {
        return man_bits == other.man_bits && exp_bits == other.exp_bits
            && bias == other.bias;
    }
};

#endif // _APYFLOAT_H
