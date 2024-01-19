#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

/*
    The selected rounding mode is shared for all arithmetic operations,
    and can thus be stored as a variable inside the source file.
*/

enum class APyFixedRoundingMode {
    TRN,           // !< Truncation
    RND,           // !< Rounding, ties toward plus inf (most common fixed-point rounding)
    RND_TO_ZERO,   // !< Rounding, ties toward zero
    RND_AWAY_ZERO, // !< Rounding, ties away from zero
    RND_CONV,      // !< Rounding, ties toward even quantization setps
    RND_CONV_ODD,  // !< Rounding, ties toward odd quantization steps
    STOCHASTIC_WEIGHTED,
    STOCHASTIC_RANDOM
};

enum class APyFixedOverflowMode {
    TWOS_OVERFLOW, // !< Two's complement overflowing
    SATURATE       // !< Saturation on overflow
};

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
