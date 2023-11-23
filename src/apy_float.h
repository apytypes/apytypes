#ifndef _APY_FLOAT_H
#define _APY_FLOAT_H

#include <cstdint>
#include <string>

/*
 * Scalar floating-point type, vectorized version still needed.
 */

class APyFloat {
    public:
    // Constructors
    explicit APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, double value /*= 0*/);

    // Overloaded operators
    APyFloat operator+(const APyFloat &rhs) const;
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

    void normalize();
    std::string repr() const;

    private:
    // It is maybe better to combine the sign, hidden one, and mantissa into one field using APyFixed
    std::uint8_t exp_bits, man_bits;
    std::int64_t exp, man, bias; // The mantissa does not contain the 1
    bool sign;

    APyFloat(bool sign, std::int64_t exp, std::int64_t man, std::uint8_t exp_bits, std::uint8_t man_bits);
};

#endif // _APY_FLOAT_H