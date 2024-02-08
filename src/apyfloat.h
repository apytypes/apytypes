#ifndef _APYFLOAT_H
#define _APYFLOAT_H

#include <cstdint>
#include <optional>
#include <string>

#include "apytypes_common.h"

/*
 * Scalar floating-point type, vectorized version still needed.
 * Currently the IEEE 754-Standard is followed but it would also be
 * of interest to have an option to disable it and support more customization.
 */

class APyFloat {
public:
    /* ******************************************************************************
     * * Constructors                                                               *
     * ******************************************************************************
     */

    explicit APyFloat(
        bool sign,
        exp_t exp,
        man_t man,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    // This constructor is not exposed to Python
    APyFloat(
        const APyFloatData& data,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ******************************************************************************
     * * Methods for conversions                                                    *
     * ******************************************************************************
     */

    static APyFloat from_double(
        double value,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<RoundingMode> rounding_mode = std::nullopt
    );
    double to_double() const;
    operator double() const;

    static APyFloat from_bits(
        unsigned long long bits,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );
    unsigned long long to_bits() const;

    APyFloat resize(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<RoundingMode> rounding_mode = std::nullopt
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

    static APyFloat abs(const APyFloat& x);
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

    inline bool get_sign() const { return sign; }
    inline man_t get_man() const { return man; }
    inline exp_t get_exp() const { return exp; }
    inline exp_t get_bias() const { return bias; }
    inline std::uint8_t get_man_bits() const { return man_bits; }
    inline std::uint8_t get_exp_bits() const { return exp_bits; }
    inline APyFloatData get_data() const { return { sign, exp, man }; }

    static inline exp_t ieee_bias(std::uint8_t exp_bits)
    {
        return (1ULL << (exp_bits - 1)) - 1;
    }

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
    APyFloat(
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ******************************************************************************
     * * Helper functions                                                           *
     * ******************************************************************************
     */
    APyFloat& update_from_bits(unsigned long long bits);
    APyFloat& update_from_double(
        double value, std::optional<RoundingMode> rounding_mode = std::nullopt
    );

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
    inline man_t man_mask() const { return ((1ULL << man_bits) - 1); }
    inline man_t leading_one() const { return (1ULL << man_bits); }
    inline man_t leading_bit() const
    {
        return (static_cast<man_t>(is_normal()) << man_bits);
    }
};

#endif // _APYFLOAT_H
