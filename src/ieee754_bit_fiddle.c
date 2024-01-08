#include <ieee754.h>
#include <limits.h>
#include "ieee754_bit_fiddle.h"

unsigned int sign_of_double(double d) {
    union ieee754_double ieee_d;
    ieee_d.d = d;

    return ieee_d.ieee.negative ? 1 : 0;
}

int exp_of_double(double d) {
    union ieee754_double ieee_d;
    ieee_d.d = d;

    if (ieee_d.ieee.exponent == 0) {
        return -1024;
    } else {
        return ieee_d.ieee.exponent - 1023;
    }
}

unsigned long long man_of_double(double d) {
    union ieee754_double ieee_d;
    ieee_d.d = d;
    const int shift = CHAR_BIT * sizeof(unsigned int);
    
    return ((unsigned long long)ieee_d.ieee.mantissa0 << shift) | ieee_d.ieee.mantissa1;
}

struct float_bits bits_of_double(double d) {
    struct float_bits bits = { sign_of_double(d), exp_of_double(d), man_of_double(d) };

    return bits;
}

unsigned int sign_of_float(float f) {
    union ieee754_float ieee_f;
    ieee_f.f = f;

    return ieee_f.ieee.negative ? 1 : 0;
}

int exp_of_float(float f) {
    union ieee754_float ieee_f;
    ieee_f.f = f;

    if (ieee_f.ieee.exponent == 0) {
        return -128;
    } else {
        return ieee_f.ieee.exponent - 127;
    }
}

unsigned int man_of_float(float f) {
    union ieee754_float ieee_f;
    ieee_f.f = f;

    return ieee_f.ieee.mantissa;
}

struct float_bits bits_of_float(float f) {
    struct float_bits bits = { sign_of_float(f), exp_of_float(f), man_of_float(f) };

    return bits;
}