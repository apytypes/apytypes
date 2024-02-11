#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

#include <cstdint>  // std::uint32_t, uint64_t
#include <optional> // std::optional
#include <tuple>    // std::tuple

/*!
 * Available quantization modes for APyFixed and APyFloat
 */
enum class QuantizationMode {
    TRN,                 // !< Truncation
    TRN_INF,             // !< Jamming
    TRN_ZERO,            // !< Magnitude truncation
    RND,                 // !< Quantization, ties toward plus inf
    RND_ZERO,            // !< Quantization, ties toward zero
    RND_INF,             // !< Quantization, ties away from zero
    RND_MIN_INF,         // !< Quantization, ties toward minus infinity
    RND_CONV,            // !< Quantization, ties toward even quantization setps
    RND_CONV_ODD,        // !< Quantization, ties toward odd quantization steps
    JAM,                 // !< Jamming
    JAM_UNBIASED,        // !< Unbiased jamming
    STOCHASTIC_WEIGHTED, // !< Weighted stochastic quantization
    STOCHASTIC_EQUAL     // !< Stochastic quantization with equal probability
};

/*!
 * Available overflowing modes for APyFixed and APyFloat
 */
enum class OverflowMode {
    WRAP,        // !< Drop bits left of the MSB (two's complement overflowing)
    SAT,         // !< Saturate on overflow
    NUMERIC_STD, // !< Drop bits left of the MSB, but keep the most significant bit
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
 * *                          Quantization context for APyFloat * *
 * ********************************************************************************** */

/*
 * This allows the user to choose a quantization mode for all operations performed
 * inside the runtime context. The quantization mode will be changed back to whatever it
 * was before when the context ends.
 *
 * Python example using nested contexts:
 *
 * ```
 * with QuantizationContext(QuantizationMode.TO_NEG):
 *     # Operations now round towards negative infinity
 *     ...
 *     with QuantizationContext(QuantizationMode.TO_POS):
 *         # Operations now round towards positive infinity
 *         ...
 *     # Operations now round towards negative infinity again
 *
 * # Quantization mode now reverted back to what was used before
 * ```
 */

class QuantizationContext : public ContextManager {
public:
    QuantizationContext(
        const QuantizationMode& new_mode,
        std::optional<std::uint64_t> new_seed = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    QuantizationMode new_mode, prev_mode;
    std::uint64_t new_seed, prev_seed;
};

// Set the global quantization mdoe
void set_quantization_mode(QuantizationMode mode);

// Retrieve the global quantization mode
QuantizationMode get_quantization_mode();

// Set the global seed for stochastic quantization
void set_quantization_seed(std::uint64_t);

// Get the global seed for stochastic quantization
std::uint64_t get_quantization_seed();

// Retreive a random 64-bit number
std::uint64_t random_number();

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
    QuantizationMode quantization_mode;
    OverflowMode overflow_mode;
};

// Accumulator context
class AccumulatorContext : public ContextManager {
public:
    AccumulatorContext(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization_mode = std::nullopt,
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
