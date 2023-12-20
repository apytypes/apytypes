#include <iostream>
#include <math.h>
#include "apy_float.h"
#include "apy_util.h"

APyFloat::APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, double value = 0) :
        exp_bits(exp_bits), man_bits(man_bits) {
    if (value < 0) {
        sign = true;
        value = -value;
    } else {
        sign = false;
    }

    bias = (1 << (exp_bits - 1)) - 1;
    int d_exp;
    double f_norm = std::frexp(value, &d_exp);
    exp = d_exp + bias;

    man = static_cast<std::int64_t>(f_norm * (1 << (man_bits + 1)));
    man &= (1 << man_bits) - 1;
}

APyFloat::APyFloat(bool sign, std::int64_t exp, std::int64_t man, std::uint8_t exp_bits, std::uint8_t man_bits) : 
        sign(sign), exp(exp), man(man), exp_bits(exp_bits), man_bits(man_bits) { 
    bias = (1 << (exp_bits - 1)) - 1;
}

void swap(APyFloat &x, APyFloat &y) {
    APyFloat tmp = y;
    y = x;
    x = tmp;
}

APyFloat APyFloat::operator+(APyFloat y) const {
    APyFloat x = *this;
    APyFloat res;

    if (!(x.is_normal() && y.is_normal())) {
        throw NotImplementedException();
    }

    // Compute sign
    bool x_sign = x.sign, y_sign = y.sign;
    x = abs(x);
    y = abs(y);

    if (x > y) {
        res.sign = x_sign;
    } else if (x < y) {
        res.sign = y_sign;
        // Make sure |x| >= |y|
        swap(x, y);
    } else {
        res.sign = x_sign | y_sign;
    }

    res.exp = x.exp;
    res.exp_bits = x.exp_bits;

    // Conditionally add leading one's
    man_t mx = (x.is_normal() << man_bits) | man;
    man_t my = (y.is_normal() << y.man_bits) | y.man;

    // Add room for guard bits
    mx <<= 3;
    my <<= 3;

    // Align mantissas
    exp_t delta = x.exp - y.exp;
    const exp_t max_shift = y.man_bits + 4UL; // +4 to account for leading one and 3 guard bits 
    man_t highY = my >> std::min(max_shift, delta);

    man_t lowY; // Used to update the sticky bit position
    if (delta <= 3) {
        lowY = 0;
    } else if (delta >= max_shift) {
        lowY = 1;
    } else {
        lowY = (my << (max_shift-delta)) != 0;
    }

    // Perform addition/subtraction
    man_t highR;
    if (y.sign) {
        highR = mx - (highY | lowY);
    } else {
        highR = mx + (highY | lowY);
    }

    // Calculate rounding bit
    man_t M, G, T, B;

    if (highR & (1 << man_bits - 1)) { // Check for carry
        M = highR >> 4;
        G = (highR >> 3) & 1;
        T = (highR << 29) != 0;
        B = G & (M | T);

        //++x_exp; // Should also check for overflow
    } else if (false) { // No carry
        M = (highR >> 3) & 0;
        G = (highR >> 2) & 1;
        T = (highR << 30) != 0;
        B = G & (M | T);
    } else { // n >= 6 should only happend during subtraction
        throw std::runtime_error("n was out of range");
    }
    
    res.man = M & (1 << man_bits);
    return APyFloat(1, 1);
}

APyFloat APyFloat::operator-(const APyFloat &y) const {
    return *this + (-y);
}

/*APyFloat APyFloat::plus(const APyFloat &rhs) const {
    // Sign is not currently handled
    APyFloat res = *this;

    // Add hidden one
    uint64_t rhs_man = (1 << rhs.man_bits) | rhs.man;
    res.man |= 1 << res.man_bits;

    // Align mantissas
    const auto shift = std::abs(res.exp - rhs.exp);
    if (res.exp > rhs.exp) {
        rhs_man >>= shift;
    } else {
        res.man >>= shift;
    }

    res.man += rhs_man;

    res.exp_bits = std::max(res.exp_bits, rhs.exp_bits);
    res.exp = std::max(res.exp, rhs.exp);
    
    res.normalize();
    return res;
}*/

APyFloat APyFloat::operator-() const {
    return APyFloat(!sign, exp, man, exp_bits, man_bits);
}

bool APyFloat::operator==(const APyFloat &rhs) const {
    return (exp == rhs.exp) && (man == rhs.man) && (exp_bits == rhs.exp_bits) && (man_bits == rhs.man_bits) && (sign == rhs.sign);
}

bool APyFloat::operator>(const APyFloat &rhs) const {
    return !(*this < rhs);
}

bool APyFloat::operator<(const APyFloat &rhs) const {
    if (sign == rhs.sign) {
        if ((exp - bias) < (rhs.exp - rhs.bias)) {
            return true;
        } else if ((exp - bias) > (rhs.exp - rhs.bias)) {
            return false;
        } else {
            return man < rhs.man;
        }
    }
    throw NotImplementedException();
}

void APyFloat::normalize() {
    // Doesn't cover all cases atm
    const uint64_t norm_mask = ~((1 << (man_bits + 1)) - 1);
    while (man & norm_mask) {
        man >>= 1;
        ++exp;
    }
    man &= (1 << man_bits) - 1;
}

/*
    True if and only if x is normal (not zero, subnormal, infinite, or NaN).
*/
bool APyFloat::is_normal() const {
    return (exp != 0) && (exp != (1 << exp_bits) - 1);
}

std::string APyFloat::repr() const {
    return "fp<"
            + std::to_string(exp_bits)
            + ", "
            + std::to_string(man_bits)
            + (sign ? ">(-2^" : ">(2^")
            + std::to_string(exp - bias - man_bits - 1)
            + " x "
            + std::to_string(man + (1 << man_bits))
            + ")";
}

APyFloat::operator double() const {
    const auto mantissa = (1 << man_bits) + man;
    const auto exponent = exp - bias - man_bits - 1;

    return (sign ? -1.0 : 1.0) * mantissa / (1 << -exponent);
}

APyFloat APyFloat::abs(const APyFloat &f) const {
    return f.sign ? -f : f;
}