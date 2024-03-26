#ifndef _APYTYPES_SIMD_H
#define _APYTYPES_SIMD_H

#include <vector>

#include "apybuffer.h"

/*
 * GMP should be included after all other includes
 */
#include "../extern/mini-gmp/mini-gmp.h"

namespace simd {

//! Retrieve a string for
std::string get_simd_version_str();

void limb_vector_shift_add(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t src1_shift_amount,
    std::size_t size
);

};

#endif
