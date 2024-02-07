#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

/*!
 * Available rounding modes for APyFixed and APyFloat
 */
enum class RoundingMode {
    TRN,                 // !< Truncation
    TRN_INF,             // !< Jamming
    TRN_ZERO,            // !< Magnitude truncation
    RND,                 // !< Rounding, ties toward plus inf
    RND_ZERO,            // !< Rounding, ties toward zero
    RND_INF,             // !< Rounding, ties away from zero
    RND_MIN_INF,         // !< Rounding, ties toward minus infinity
    RND_CONV,            // !< Rounding, ties toward even quantization setps
    RND_CONV_ODD,        // !< Rounding, ties toward odd quantization steps
    JAM,                 // !< Jamming
    STOCHASTIC_WEIGHTED, // !< Weighted stochastic rounding
    STOCHASTIC_EQUAL     // !< Stochastic rounding with equal probability
};

/*!
 * Available overflowing modes for APyFixed and APyFloat
 */
enum class OverflowMode {
    WRAP, // Drop bits left of the MSB (two's complement overflowing)
    SAT   // Saturate on overflow
};

void set_rounding_mode(RoundingMode);
RoundingMode get_rounding_mode();

#endif // _APYTYPES_COMMON_H
