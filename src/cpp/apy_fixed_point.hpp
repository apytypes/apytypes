#pragma once

#include <cstdint>
#include <string>

/*
 * Scalar fixed-point type, vectorized version still needed.
 */

class aPyFixed {
    public:
    // Constructor with optional conversion from double
    aPyFixed(std::uint8_t total_bits, std::uint8_t integer_bits /*= 1*/, double value /*= 0*/);

    std::string to_string() const;

    // Overloaded operators
    aPyFixed operator+(const aPyFixed &rhs) const;
    aPyFixed &operator+=(const aPyFixed &rhs);
    aPyFixed operator-(const aPyFixed &rhs) const;
    aPyFixed &operator-=(const aPyFixed &rhs);
    aPyFixed operator*(const aPyFixed &rhs) const;
    aPyFixed &operator*=(const aPyFixed &rhs);
    aPyFixed operator/(const aPyFixed &rhs) const;
    aPyFixed &operator/=(const aPyFixed &rhs);
    bool operator==(const aPyFixed &rhs) const;
    bool operator<=(const aPyFixed &rhs) const;
    bool operator<(const aPyFixed &rhs) const;
    bool operator>=(const aPyFixed &rhs) const;
    bool operator>(const aPyFixed &rhs) const;
    bool operator!=(const aPyFixed &rhs) const;
    std::ostream &operator<<(std::ostream &os) const;
    operator double() const;

    private:
    std::uint8_t total_bits;
    std::uint8_t frac_bits;
    std::int64_t data;

    aPyFixed(std::uint8_t total_bits, std::uint8_t frac_bits, std::int64_t data);
};