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
    explicit APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, double value /*= 0*/);

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

    private:
    // It is perhaps better to combine the sign, hidden one, and mantissa into one field using APyFixed
    std::uint8_t exp_bits, man_bits;
    man_t man; // Hidden one
    exp_t exp, bias;
    bool sign;

    APyFloat() = default;
    APyFloat(bool sign, std::int64_t exp, std::int64_t man, std::uint8_t exp_bits, std::uint8_t man_bits);

    exp_t max_exponent() const;
};

#endif // _APY_FLOAT_H