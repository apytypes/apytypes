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
    exp = d_exp ? (d_exp + bias) : 0;

    man = static_cast<man_t>(f_norm * (1 << (man_bits + 1)));
    man &= (1 << man_bits) - 1;
}

APyFloat::APyFloat(bool sign, std::int64_t exp, std::int64_t man, std::uint8_t exp_bits, std::uint8_t man_bits) : 
        sign(sign), exp(exp), man(man), exp_bits(exp_bits), man_bits(man_bits) { 
    bias = (1 << (exp_bits - 1)) - 1;
}

APyFloat APyFloat::operator+(APyFloat y) const {
    APyFloat x = *this;
    
    if (!(x.is_finite() && y.is_finite())) {
        throw NotImplementedException();
    }

    APyFloat res;

    // Compute sign and swap operands if need to make sure |x| >= |y|
    const APyFloat xabs = abs(x);
    const APyFloat yabs = abs(y);

    if (xabs > yabs) {
        res.sign = x.sign;
    } else if (xabs < yabs) {
        res.sign = y.sign;
        std::swap(x, y);
    } else {
        res.sign = x.sign | y.sign;
    }

    res.exp = x.exp;
    res.exp_bits = x.exp_bits;
    res.bias = x.bias;
    res.man_bits = std::max(x.man_bits, y.man_bits);

    // Conditionally add leading one's
    const man_t nx = x.is_normal();
    const man_t ny = y.is_normal();
    man_t mx = (nx << x.man_bits) | x.man;
    man_t my = (ny << y.man_bits) | y.man;

    // Add room for guard bits
    mx <<= 3;
    my <<= 3;

    // Align mantissas
    exp_t delta = (x.exp - nx) - (y.exp - ny);
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
    if (x.sign != y.sign) {
        if (xabs == yabs) {
            return APyFloat(false, 0, 0, res.exp_bits, res.man_bits);
        }
        highR = mx - (highY | lowY);
    } else {
        highR = mx + (highY | lowY);
    }

    // Calculate rounding bit
    man_t M, G, T, B;
    const man_t man_mask = ((1 << res.man_bits) - 1);

    // TODO: Break out rounding and normalization to their own methods
    // Currently round-to-nearest-even is used
    if (highR & (1 << (res.man_bits+4))) { // Carry
        M = highR >> 4;
        G = (highR >> 3) & 1;
        T = (highR & 0x7) != 0;
        B = G & (M | T);
        ++res.exp; 
        if (res.exp == max_exponent()) {
            res.man = 0; // Must be 0 because otherwise it would be NaN
            return res;
        }
    } else if (highR & (1 << (res.man_bits+3))) { // No carry
        M = (highR >> 3);
        G = (highR >> 2) & 1;
        T = (highR & 0x3) != 0;
        B = G & (M | T);
    } else { // Cancellation occured
        B = 0; // TODO: Handle rounding correctly for n=6
        while (!(highR & (1 << (res.man_bits+3)))) {
            highR <<= 1;
            --res.exp;
            if (res.is_subnormal()) {
                break;
            }
        }
        M = (highR >> 3);
    }
    
    M &= man_mask;
    res.man = M + B;
    // Check for potential carry and round again if needed
    if (res.man > man_mask) {
        res.man >>= 1;
        ++res.exp;
    }
    return res;
}

APyFloat APyFloat::operator-(const APyFloat &y) const {
    return *this + (-y);
}

APyFloat APyFloat::operator-() const {
    auto res = *this;
    res.sign = !sign;
    return res;
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
    } else {
        return sign;
    }
}

/*
    Calculate the maximum exponent based on the IEEE 754-Standard.
*/
APyFloat::exp_t APyFloat::max_exponent() const {
    return (exp != (1 << exp_bits) - 1);
}

/*
    True if and only if x is normal (not zero, subnormal, infinite, or NaN).
*/
bool APyFloat::is_normal() const {
    return is_finite() && !is_subnormal();
}

/*
    True if and only if x is zero, subnormal or normal.
*/
bool APyFloat::is_finite() const {
    return is_subnormal() || exp != max_exponent();
}

/*
    True if and only if x is subnormal. Zero is also considered a subnormal number.
*/
bool APyFloat::is_subnormal() const {
    return exp == 0;
}

/*
    True if and only if x is zero.
*/
bool APyFloat::is_zero() const {
    return exp == 0 && man == 0;
}

/*
    True if and only if x is NaN.
*/
bool APyFloat::is_nan() const {
    return exp == max_exponent() && man != 0;
}

/*
    True if and only if x is infinite.
*/
bool APyFloat::is_inf() const {
    return exp == max_exponent() && man == 0;
}

/*
    True if and only if x has a negative sign. Applies to zeros and NaNs as well.
*/
bool APyFloat::is_sign_neg() const {
    return sign;
}

APyFloat APyFloat::abs(const APyFloat &f) const {
    return f.is_sign_neg() ? -f : f;
}

std::string APyFloat::repr() const {
    std::string str = "fp<"
                    + std::to_string(exp_bits)
                    + ", "
                    + std::to_string(man_bits)
                    + (sign ? ">(-" : ">(");
    
    if (is_nan()) {
        return str + "NaN)";
    } else if (is_inf()) {
        return str + "inf)";
    }

    str += "2**"
        + (is_subnormal() ? std::string("0") : std::to_string(exp - bias - man_bits - 1))
        + "*"
        + std::to_string((is_normal() << man_bits) | man)
        + ")";

    return str;
}

APyFloat::operator double() const {
    // TODO: Handle special numbers
    const auto mantissa = (is_normal() << man_bits) | man;
    const auto exponent = is_subnormal() ? 0 : exp - bias - man_bits - 1;

    return (sign ? -1.0 : 1.0) * mantissa / (1 << -exponent);
}