#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

/*
    The selected rounding mode is shared for all arithmetic operations,
    and can thus be stored as a variable inside the source file.
*/

enum class RoundingMode {
    TO_POSITIVE,
    TO_NEGATIVE,
    TO_ZERO,
    TIES_TO_EVEN,
    TIES_TO_AWAY,
    JAMMING
};

void set_rounding_mode(RoundingMode);
RoundingMode get_rounding_mode();

#endif // _APYTYPES_COMMON_H
