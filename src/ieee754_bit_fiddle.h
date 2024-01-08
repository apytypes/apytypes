#ifndef _IEEE754_BIT_FIDDLING_H
#define _IEEE754_BIT_FIDDLING_H

/* C-HEADER
    Helper functions for reading bits of binary32 and binary64 numbers.
    Since type punning is allowed in C, this can be achived in a constistent
    manner together with the system header ieee754.h.
*/

struct float_bits
{
    unsigned int sign;
    int exp;
    unsigned long long man;
};

// Get the value of the sign bit
unsigned int sign_of_double(double d);

// Get the unbiased exponent
int exp_of_double(double d);

// Get the mantissa without the leading one
unsigned long long man_of_double(double d);

struct float_bits bits_of_double(double d);

// Get the value of the sign bit
unsigned int sign_of_float(float d);

// Get the unbiased exponent
int exp_of_float(float d);

// Get the mantissa without the leading one
unsigned int man_of_float(float d);

struct float_bits bits_of_float(float d);

#endif //_IEEE754_BIT_FIDDLING_H