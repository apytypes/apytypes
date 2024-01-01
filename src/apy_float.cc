#include <iostream>
#include <limits>
#include <math.h>
#include "apy_common.h"
#include "apy_float.h"
#include "apy_util.h"

APyFloat::APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits, double value /*= 0*/) :
        exp_bits(exp_bits), man_bits(man_bits), sign(std::signbit(value)), bias(ieee_bias()) {

    switch (std::fpclassify(value)) {
        case FP_ZERO:
            *this = construct_zero(sign);
            return;
        case FP_INFINITE:
            *this = construct_inf(sign);
            break;
        case FP_NAN:
            *this = construct_nan(sign, 1);
        case FP_NORMAL:
            break; // Continue below switch statement
        case FP_SUBNORMAL:
        default:
            throw NotImplementedException();
    }

    // If no errors occur, std::frexp returns the value f_norm in the range (-1, -0.5], [0.5, 1)
    // and stores an integer value in *exp such that (f_norm * 2^d_exp) == num.
    int d_exp;
    double f_norm = std::abs(std::frexp(value, &d_exp));

    // f_norm contains the leading 1 so it should be shifted to the left (and exponent incremented)
    man = static_cast<man_t>(f_norm * (1 << (man_bits + 1)));
    man &= (1 << man_bits) - 1;
    exp = d_exp + bias - 1;
}

APyFloat::APyFloat(bool sign, std::int64_t exp, std::int64_t man, std::uint8_t exp_bits, std::uint8_t man_bits) : 
        sign(sign), exp(exp), bias(ieee_bias()), man(man), exp_bits(exp_bits), man_bits(man_bits) { 
}

APyFloat APyFloat::operator+(APyFloat y) const {
    if (get_rounding_mode() != RoundingMode::TIES_TO_EVEN) {
        throw NotImplementedException();
    }

    APyFloat x = *this;
    
    if (!(x.is_finite() && y.is_finite()) || (x.bias != y.bias)) {
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

    man_t lowY; // Used to update the sticky bit position T
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
        ++res.exp; 
        if (res.exp == max_exponent()) {
            res.man = 0; // Must be 0 because otherwise it would be NaN
            return res;
        }
    } else if (highR & (1 << (res.man_bits+3))) { // No carry
        M = (highR >> 3);
        G = (highR >> 2) & 1;
        T = (highR & 0x3) != 0;
    } else { // Cancellation occured
        G = 0; // TODO: Handle rounding correctly for n=6
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
    B = G & (M | T);
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

APyFloat APyFloat::operator*(const APyFloat &y) const {
    if (get_rounding_mode() != RoundingMode::TIES_TO_EVEN) {
        throw NotImplementedException();
    }
    
    const bool res_sign = sign ^ y.sign;

    if ((is_nan() || y.is_nan())
        || (is_inf() && y.is_zero())
        || (is_zero() && y.is_inf())) {
        return construct_nan(res_sign);
    }

    if ((is_inf() && y.is_finite())
        || (is_finite() && y.is_inf())) {
        return construct_inf(res_sign);
    }

    if (is_zero() || y.is_zero()) {
        return construct_zero(res_sign);
    }

    if (is_subnormal() || y.is_subnormal() || (bias != y.bias)) {
        throw NotImplementedException();
    }

    APyFloat res(std::max(exp_bits, y.exp_bits), std::max(man_bits, y.man_bits));
    res.sign = res_sign;

    // Conditionally add leading one's
    man_t mx = (is_normal() << man_bits) | man;
    man_t my = (y.is_normal() << y.man_bits) | y.man;
    
    res.exp = exp + y.exp - y.bias;
    
    man_t highR = mx * my;

    // Calculate rounding bit
    const man_t man_mask = ((1 << res.man_bits) - 1);
    man_t M, G, T, B;

    if (highR >= (1 << (2*res.man_bits+1))) { // Carry
        M = highR >> (res.man_bits+1);
        G = (highR >> (res.man_bits-1)) & 1;
        T = (highR & (man_mask<< 1 | 1)) != 0;
        B = G & (M | T);
        ++res.exp;
        if (res.exp == max_exponent()) {
            return res.construct_inf(res.sign);
        }
    } else {
        M = highR >> (res.man_bits);
        G = (highR >> (res.man_bits-1)) & 1;
        T = (highR & (man_mask<< 1 | 1)) != 0;
        B = G & (M | T);
        if (res.exp == max_exponent()) {
            return res.construct_inf(res.sign);
        }
    }

    M &= man_mask;
    res.man = M + B;
    // Check for potential carry and round again if needed
    if (res.man > man_mask) {
        res.man >>= 1;
        ++res.exp;
        throw NotImplementedException();
    }
    //res.man = (highR >> (res.man_bits)) & man_mask;

    return res;
}

APyFloat APyFloat::operator-() const {
    auto res = *this;
    res.sign = !sign;
    return res;
}

bool APyFloat::operator==(const APyFloat &rhs) const {
    if (is_nan() || rhs.is_nan()) {
        return false;
    } else {
        return (exp == rhs.exp) && (man == rhs.man) 
                && (exp_bits == rhs.exp_bits) && (man_bits == rhs.man_bits) 
                && (sign == rhs.sign);

    }
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

APyFloat::exp_t APyFloat::ieee_bias() const {
    return (1 << (exp_bits-1)) - 1;
}

/*
    Calculate the maximum exponent based on the IEEE 754-Standard.
*/
APyFloat::exp_t APyFloat::max_exponent() const {
    return (1 << exp_bits) - 1;
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

APyFloat APyFloat::construct_zero(bool sign) const {
    return APyFloat(sign, 0, 0, exp_bits, man_bits);
}

APyFloat APyFloat::construct_inf(bool sign) const {
    return construct_nan(sign, 0);
}

APyFloat APyFloat::construct_nan(bool sign, APyFloat::man_t payload /*= 1*/) const {
    return APyFloat(sign, max_exponent(), payload, exp_bits, man_bits);
}

std::string APyFloat::repr() const {
    std::string str = "fp<"
                    + std::to_string(exp_bits)
                    + ","
                    + std::to_string(man_bits)
                    + (sign ? ">(-" : ">(");
    
    if (is_nan()) {
        return str + "NaN)";
    } else if (is_inf()) {
        return str + "inf)";
    }

    str += "2**"
        + std::to_string(exp - bias - man_bits)
        + "*"
        + std::to_string((is_normal() << man_bits) | man)
        + ")";

    return str;
}

APyFloat::operator double() const {
    if (is_inf()) {
        return (is_sign_neg() ? -1.0 : 1.0) * std::numeric_limits<double>::infinity();
    } else if (is_nan()) {
        return (is_sign_neg() ? -1.0 : 1.0) * std::numeric_limits<double>::quiet_NaN();
    }

    const auto mantissa = (is_normal() << man_bits) | man;
    const auto exponent = is_subnormal() ? 0 : exp - bias - man_bits;

    return (sign ? -1.0 : 1.0) * mantissa / (1 << -exponent);
}