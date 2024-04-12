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
