#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

#include <cstdint>  // std::uint32_t, uint64_t
#include <optional> // std::optional
#include <tuple>    // std::tuple

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

/* ********************************************************************************** *
 * *                          Context management for APyTypes                       * *
 * ********************************************************************************** */

class ContextManager {
public:
    virtual ~ContextManager() = default;
    virtual void enter_context() = 0;
    virtual void exit_context() = 0;
};

/* ********************************************************************************** *
 * *                          Rounding context for APyFloat                         * *
 * ********************************************************************************** */

/*
 * This allows the user to choose a rounding mode for all operations performed inside
 * the runtime context. The rounding mode will be changed back to whatever it was
 * before when the context ends.
 *
 * Python example using nested contexts:
 *
 * ```
 * with RoundingContext(RoundingMode.TO_NEG):
 *     # Operations now round towards negative infinity
 *     ...
 *     with RoundingContext(RoundingMode.TO_POS):
 *         # Operations now round towards positive infinity
 *         ...
 *     # Operations now round towards negative infinity again
 *
 * # Rounding mode now reverted back to what was used before
 * ```
 */

class RoundingContext : public ContextManager {
public:
    RoundingContext(const RoundingMode& new_mode);
    void enter_context() override;
    void exit_context() override;

private:
    RoundingMode new_mode, prev_mode;
};

// Set the global roudning mdoe
void set_rounding_mode(RoundingMode mode);

// Retrieve the global rounding mode
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

/* ********************************************************************************** *
 * *                          Accumulator context for APyFixed                      * *
 * ********************************************************************************** */

// Accumulator type
struct AccumulatorOption {
    int bits;
    int int_bits;
    RoundingMode rounding_mode;
    OverflowMode overflow_mode;
};

// Accumulator context
class AccumulatorContext : public ContextManager {
public:
    AccumulatorContext(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<RoundingMode> rounding_mode = std::nullopt,
        std::optional<OverflowMode> overflow_mode = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    std::optional<AccumulatorOption> current_mode, previous_mode;
};

// Retrieve the global accumulator mode
std::optional<AccumulatorOption> get_accumulator_mode();

#endif // _APYTYPES_COMMON_H
