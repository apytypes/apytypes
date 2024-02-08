#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

#include <cstdint>
#include <tuple>

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

using exp_t = std::uint32_t;
using man_t = std::uint64_t;

struct APyFloatData {
    bool sign;
    exp_t exp; // Biased exponent
    man_t man; // Hidden one
    bool operator==(const APyFloatData& other) const
    {
        return std::make_tuple(sign, exp, man)
            == std::make_tuple(other.sign, other.exp, other.man);
    }
};

#endif // _APYTYPES_COMMON_H
