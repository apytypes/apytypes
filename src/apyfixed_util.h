#ifndef _APYFIXED_UTIL_H
#define _APYFIXED_UTIL_H

#include "apytypes_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

mp_limb_t twos_complement_overflow(mp_limb_t value, int bits);

mp_limb_t get_data_from_double(double value, int bits, int frac_bits);

#endif // _APYFIXED_UTIL_H
