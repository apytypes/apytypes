#include "apyfloat_util.h"

/* Helper functions */
man_t ipow(man_t base, unsigned int n)
{
    // Solution from
    // https://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
    man_t result = 1;
    for (;;) {
        if (n & 1) {
            result *= base;
        }

        n >>= 1;

        if (!n) {
            break;
        }

        base *= base;
    }

    return result;
}

APyFixed ipow_apyfixed(APyFixed base, unsigned int n)
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
