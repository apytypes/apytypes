#ifndef _APYFIXED_UTIL_H
#define _APYFIXED_UTIL_H

#include "apyfixed.h"
#include "apytypes_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

//! Constant fixed-point one for convenience
static const APyFixed fx_one(2, 2, std::vector<mp_limb_t>({ 1 }));

//! Specialized method when only one limb is used
mp_limb_t get_data_from_double(double value, int bits, int frac_bits, int shift_amnt);

//! Fast integer power by squaring.
APyFixed ipow(APyFixed base, unsigned int n);

#endif // _APYFIXED_UTIL_H
