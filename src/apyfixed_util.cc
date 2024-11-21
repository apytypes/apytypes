#include "apyfixed_util.h"

#include <cmath>
#include <fmt/format.h>

APyFixed ipow(APyFixed base, unsigned int n)
{
    // Early exit for one of the most common cases
    if (n == 2) {
        return base * base;
    }
    // Because how APyFloat::pown is written, we know n will be >= 2,
    // this fact can probably be used to optimize this code further.

    APyFixed result = fx_one;
    for (;;) {
        if (n & 1) {
            result = result * base; // Until *= is implemented
        }

        n >>= 1;

        if (!n) {
            break;
        }

        base = base * base; // Until *= is implemented
    }

    return result;
}
