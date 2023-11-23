#include <iostream>
#include <math.h>
#include "apy_float.h"

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

APyFloat APyFloat::operator+(const APyFloat &rhs) const {
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
}

APyFloat APyFloat::operator-() const {
    return APyFloat(!sign, exp, man, exp_bits, man_bits);
}

bool APyFloat::operator==(const APyFloat &rhs) const {
    return (exp == rhs.exp) && (man == rhs.man) && (exp_bits == rhs.exp_bits) && (man_bits == rhs.man_bits) && (sign == rhs.sign);
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