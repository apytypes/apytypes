/*
 * Utility functions
 */

#include <cstddef>

// Perform fast integer ceil( n/64 )
static inline std::size_t idiv64_ceil_fast(std::size_t n) {
    if (n%64 == 0) {
        return n/64;
    } else {
        return n/64+1;
    }
}
