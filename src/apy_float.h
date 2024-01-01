#ifndef _APY_FLOAT_H
#define _APY_FLOAT_H

#include <cstdint>
#include <string>

/*
 * Scalar floating-point type, vectorized version still needed.
 * Currently the IEEE 754-Standard is followed but it would also be
 * of interest to have an option to disable it and support more customization.
 */

class APyFloat {
    using man_t = std::uint64_t;
    using exp_t = std::int64_t;

    public:
    // Constructors
    explicit APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, double value = 0);

    // Overloaded operators
    APyFloat operator+(APyFloat rhs) const;
    APyFloat &operator+=(const APyFloat &rhs);
    APyFloat operator-(const APyFloat &rhs) const;
    APyFloat operator-() const;
    APyFloat &operator-=(const APyFloat &rhs);
    APyFloat operator*(const APyFloat &rhs) const;
    APyFloat &operator*=(const APyFloat &rhs);
    APyFloat operator/(const APyFloat &rhs) const;
    APyFloat &operator/=(const APyFloat &rhs);
    bool operator==(const APyFloat &rhs) const;
    bool operator<=(const APyFloat &rhs) const;
    bool operator<(const APyFloat &rhs) const;
    bool operator>=(const APyFloat &rhs) const;
    bool operator>(const APyFloat &rhs) const;
    bool operator!=(const APyFloat &rhs) const;
    std::ostream &operator<<(std::ostream &os) const;
    operator double() const;

    bool is_normal() const;
    bool is_subnormal() const;
    bool is_zero() const;
    bool is_finite() const;
    bool is_nan() const;
    bool is_inf() const;
    bool is_sign_neg() const;
    APyFloat abs(const APyFloat &) const;
    std::string repr() const;

    // Getters
    bool get_sign() const { return sign; }
    man_t get_man() const { return man; }
    exp_t get_exp() const { return exp; }

    private:
    // It is perhaps better to combine the sign, hidden one, and mantissa into one field using APyFixed,
    // especially since the supported floating-point formats is limited by the underlying data type.
    std::uint8_t exp_bits, man_bits;
    bool sign;
    man_t man; // Hidden one
    exp_t bias, exp;

    APyFloat() = default;
    APyFloat(bool sign, std::int64_t exp, std::int64_t man, std::uint8_t exp_bits, std::uint8_t man_bits);

    exp_t max_exponent() const;
    exp_t ieee_bias() const;
    APyFloat construct_zero(bool sign) const;
    APyFloat construct_inf(bool sign) const;
    APyFloat construct_nan(bool sign, man_t payload = 1) const;
};

#endif // _APY_FLOAT_H