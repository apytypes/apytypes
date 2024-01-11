#ifndef _APY_FLOAT_H
#define _APY_FLOAT_H

#include <cstdint>
#include <optional>
#include <string>

/*
 * Scalar floating-point type, vectorized version still needed.
 * Currently the IEEE 754-Standard is followed but it would also be
 * of interest to have an option to disable it and support more customization.
 * 
 * Static methods have 's' as a prefix inorder to avoid collision in Python, e.g. 'from_float' and 'sfrom_float'.
 */

using exp_t = std::uint32_t;
using man_t = std::uint64_t;

class APyFloat {
    public:
    // Constructors
    explicit APyFloat(bool sign,
                      exp_t exp,
                      man_t man,
                      std::uint8_t exp_bits,
                      std::uint8_t man_bits, 
                      std::optional<exp_t> bias = std::nullopt);

    // Conversions
    static APyFloat from_double(double value, std::uint8_t exp_bits, std::uint8_t man_bits);
    APyFloat& update_from_double(double value);
    double to_double() const;

    static APyFloat from_bits(unsigned long long bits, std::uint8_t exp_bits, std::uint8_t man_bits);
    APyFloat& update_from_bits(unsigned long long bits);
    unsigned long long to_bits() const;
    
    std::string str() const;
    std::string repr() const;
    std::string pretty_string() const;

    // Overloaded operators
    APyFloat operator+(APyFloat rhs) const;
    APyFloat operator-(const APyFloat &rhs) const;
    APyFloat operator-() const;
    APyFloat operator*(const APyFloat &rhs) const;
    APyFloat operator/(const APyFloat &rhs) const;
    bool operator==(const APyFloat &rhs) const;
    bool operator!=(const APyFloat &rhs) const;
    bool operator<=(const APyFloat &rhs) const;
    bool operator<(const APyFloat &rhs) const;
    bool operator>=(const APyFloat &rhs) const;
    bool operator>(const APyFloat &rhs) const;
    std::ostream &operator<<(std::ostream &os) const;
    operator double() const;

    bool is_normal() const;
    bool is_subnormal() const;
    bool is_zero() const;
    bool is_finite() const;
    bool is_nan() const;
    bool is_inf() const;
    bool is_sign_neg() const;

    // Getters
    bool get_sign() const { return sign; }
    man_t get_man() const { return man; }
    exp_t get_exp() const { return exp; }
    std::uint8_t get_man_bits() const { return man_bits; }
    std::uint8_t get_exp_bits() const { return exp_bits; }

    private:
    // It is perhaps better to combine the sign, hidden one, and mantissa into one field using APyFixed,
    // especially since the supported floating-point formats is limited by the underlying data type.
    std::uint8_t exp_bits, man_bits;
    exp_t bias;
    bool sign;
    exp_t exp;
    man_t man; // Hidden one

    APyFloat() = default;
    APyFloat(std::uint8_t exp_bits,
             std::uint8_t man_bits,
             std::optional<exp_t> bias = std::nullopt);

    exp_t max_exponent() const;
    exp_t ieee_bias() const;
    APyFloat construct_zero(bool sign) const;
    APyFloat construct_inf(bool sign) const;
    APyFloat construct_nan(bool sign, man_t payload = 1) const;

    // Masks
    inline man_t man_mask() const { return ((1 << man_bits) - 1); }
    inline exp_t exp_mask() const { return ((1 << exp_bits) - 1); }
    inline exp_t leading_one() const { return (1 << man_bits); }
};

// This prevents pybind from confusing abs with std::abs.
namespace apy_types {
    APyFloat abs(const APyFloat &);
}

#endif // _APY_FLOAT_H