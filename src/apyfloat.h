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
    explicit APyFloat(
        bool sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    APyFloat(
        const APyFloatData& data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    APyFloat(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

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

    APyFloat cast_no_quant(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    ) const;

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

    APyFloat operator+(APyFloat rhs) const;
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

    //! Test if two floating-point numbers are identical, i.e., has the same value, and
    //! the same format
    bool is_identical(const APyFloat& other) const;

    /* ******************************************************************************
     * * Non-computational functions                                                *
     * ******************************************************************************
     */

    bool is_normal() const;
    bool is_subnormal() const;
    bool is_zero() const;
    bool is_finite() const;
    bool is_nan() const;
    bool is_inf() const;

    APY_INLINE bool get_sign() const { return sign; }
    APY_INLINE man_t get_man() const { return man; }
    APY_INLINE exp_t get_exp() const { return exp; }
    APY_INLINE exp_t get_bias() const { return bias; }
    APY_INLINE std::uint8_t get_man_bits() const { return man_bits; }
    APY_INLINE std::uint8_t get_exp_bits() const { return exp_bits; }
    APY_INLINE std::uint8_t get_bits() const { return man_bits + exp_bits + 1; }
    APY_INLINE APyFloatData get_data() const { return { sign, exp, man }; }
    APY_INLINE void set_data(const APyFloatData& data)
    {
        sign = data.sign;
        exp = data.exp;
        man = data.man;
    }

    static inline exp_t ieee_bias(std::uint8_t exp_bits)
    {
        return (1ULL << (exp_bits - 1)) - 1;
    }

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

    APyFloat construct_zero(std::optional<bool> new_sign = std::nullopt) const;
    APyFloat construct_inf(std::optional<bool> new_sign = std::nullopt) const;
    APyFloat
    construct_nan(std::optional<bool> new_sign = std::nullopt, man_t payload = 1) const;

    inline exp_t exp_mask() const { return ((1ULL << exp_bits) - 1); }
    inline exp_t max_exponent() const
    {
        return ((1ULL << exp_bits) - 1);
    } // Max exponent with bias
    inline exp_t ieee_bias() const { return ieee_bias(exp_bits); }
    inline man_t man_mask() const { return (1ULL << man_bits) - 1; }
    inline man_t leading_one() const { return (1ULL << man_bits); }
    inline man_t leading_bit() const
    {
        return (static_cast<man_t>(is_normal()) << man_bits);
    }

    APyFloat normalized() const;
    static void quantize_apymantissa(
        APyFixed& apyman,
        bool sign,
        int bits,
        std::optional<QuantizationMode> quantization = std::nullopt
    );
    static QuantizationMode
    translate_quantization_mode(QuantizationMode quantization, bool sign);

    int leading_zeros_apyfixed(APyFixed fx) const;
    APY_INLINE bool same_type_as(APyFloat other) const;
};

#endif // _APYFLOAT_H
