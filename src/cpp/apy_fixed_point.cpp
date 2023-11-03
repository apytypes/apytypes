#include <algorithm>
#include <cmath>
#include <iostream>
#include "apy_fixed_point.hpp"

aPyFixed::aPyFixed(std::uint8_t total_bits, std::uint8_t integer_bits = 1, double value = 0.0)
        : total_bits(total_bits) {
        frac_bits = static_cast<std::uint8_t>(total_bits - integer_bits);
        // Different rounding modes should be added
        const double int_part = std::floor(value);
        const std::int64_t frac_part = static_cast<std::int64_t>((value - int_part) * (1 << frac_bits));
        data = (static_cast<std::int64_t>(int_part) << frac_bits) | frac_part;
}

std::string aPyFixed::to_string() const {
    std::string int_part(std::to_string(data >> frac_bits));
    std::int64_t denominator = 1 << frac_bits;
    std::int64_t numerator = data & (denominator - 1);

    return "fx<" + std::to_string(total_bits) + ", " + std::to_string(total_bits-frac_bits) + ">(" +
            int_part + " + " + std::to_string(numerator) + "/" + std::to_string(denominator) + ")";
}

aPyFixed::aPyFixed(std::uint8_t total_bits, std::uint8_t frac_bits, std::int64_t data)
        : total_bits(total_bits), frac_bits(frac_bits), data(data) { }

/*
 * Overloaded operators
 */
aPyFixed aPyFixed::operator+(const aPyFixed &rhs) const {
    const std::uint8_t res_frac_bits = std::max(frac_bits, rhs.frac_bits);
    const std::uint8_t res_total_bits = static_cast<std::uint8_t>(res_frac_bits + std::max(total_bits - frac_bits, rhs.total_bits - rhs.frac_bits) + 1);
    const std::uint8_t shift = static_cast<std::uint8_t>(std::abs(frac_bits - rhs.frac_bits));
    const std::int64_t res_data = (frac_bits > rhs.frac_bits) ? data + (rhs.data << shift) : (data << shift) + rhs.data;

    return aPyFixed(res_total_bits, res_frac_bits, res_data);
    
}

aPyFixed &aPyFixed::operator+=(const aPyFixed &rhs) {
    return *this;
}

aPyFixed aPyFixed::operator-(const aPyFixed &rhs) const {
    return aPyFixed(1);
}

aPyFixed &aPyFixed::operator-=(const aPyFixed &rhs) {
    return *this;
}

aPyFixed aPyFixed::operator*(const aPyFixed &rhs) const {
    return aPyFixed(1);
}

aPyFixed &aPyFixed::operator*=(const aPyFixed &rhs) {
    return *this;
}

aPyFixed aPyFixed::operator/(const aPyFixed &rhs) const {
    return aPyFixed(1);
}

aPyFixed &aPyFixed::operator/=(const aPyFixed &rhs) {
    return *this;
}

bool aPyFixed::operator==(const aPyFixed &rhs) const {
    return false;
}

bool aPyFixed::operator<=(const aPyFixed &rhs) const {
    return false;
}

bool aPyFixed::operator<(const aPyFixed &rhs) const {
    return false;
}

bool aPyFixed::operator>=(const aPyFixed &rhs) const {
    return false;
}

bool aPyFixed::operator>(const aPyFixed &rhs) const {
    return false;
}

bool aPyFixed::operator!=(const aPyFixed &rhs) const {
    return false;
}

std::ostream &aPyFixed::operator<<(std::ostream &os) const {
    return os << to_string();
}

aPyFixed::operator double() const {
    return static_cast<double>(data) / (1 << frac_bits);
}