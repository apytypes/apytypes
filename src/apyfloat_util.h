#ifndef _APYFLOAT_UTIL_H
#define _APYFLOAT_UTIL_H

#include "apytypes_common.h"

man_t quantize_mantissa(
    man_t man, std::uint8_t bits_to_quantize, bool sign, QuantizationMode quantization
);

//! Fast integer power by squaring.
man_t ipow(man_t base, unsigned int n);

#endif // _APYFLOAT_UTIL_H
