#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

#include <cstdint>  // std::uint32_t, uint64_t
#include <optional> // std::optional
#include <string>   // std::string
#include <tuple>    // std::tuple

#include "apytypes_util.h"

//! Quantization modes in APyTypes
enum class QuantizationMode {
    TRN,            // !< Truncation, quantization toward minus infinity
    TRN_INF,        // !< Truncation, quantization toward plus infinity
    TRN_ZERO,       // !< Unbiased magnitude truncation, quantization toward zero
    TRN_AWAY,       // !< Truncation away from zero
    TRN_MAG,        // !< Magnitude truncation, quantization toward zero
    RND,            // !< Round to nearest, ties toward plus inf
    RND_ZERO,       // !< Round to nearest, ties toward zero
    RND_INF,        // !< Round to nearest, ties away from zero
    RND_MIN_INF,    // !< Round to nearest, ties toward minus infinity
    RND_CONV,       // !< Round to nearest, ties toward even quantization setps
    RND_CONV_ODD,   // !< Round to nearest, ties toward odd quantization steps
    JAM,            // !< Jamming
    JAM_UNBIASED,   // !< Unbiased jamming
    STOCH_WEIGHTED, // !< Weighted stochastic quantization
    STOCH_EQUAL     // !< Stochastic quantization with equal probability
};

//! Overflowing modes in APyTypes
enum class OverflowMode {
    WRAP,        // !< Drop bits left of the MSB (two's complement overflowing)
    SAT,         // !< Saturate on overflow
    NUMERIC_STD, // !< Drop bits left of the MSB, but keep the most significant bit
};

/* ********************************************************************************** *
 * *                          Context management for APyTypes                       * *
 * ********************************************************************************** */
// Base class defining the interface for context managers
class ContextManager {
public:
    virtual ~ContextManager() = default;
    virtual void enter_context() = 0;
    virtual void exit_context() = 0;
};

/* ********************************************************************************** *
 * *                          Quantization context for APyFloat                     * *
 * ********************************************************************************** */

/*
 * This allows the user to choose a quantization mode for all operations performed
 * inside the runtime context. The quantization mode will be changed back to whatever it
 * was before when the context ends.
 *
 * Python example using nested contexts:
 *
 * ```
 * with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
 *     # Operations now round towards negative infinity
 *     ...
 *     with APyFloatQuantizationContext(QuantizationMode.TO_POS):
 *         # Operations now round towards positive infinity
 *         ...
 *     # Operations now round towards negative infinity again
 *
 * # Quantization mode now reverted back to what was used before
 * ```
 */

class APyFloatQuantizationContext : public ContextManager {
public:
    APyFloatQuantizationContext(
        const QuantizationMode& new_mode,
        std::optional<std::uint64_t> new_seed = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    QuantizationMode new_mode, prev_mode;
    std::uint64_t new_seed, prev_seed;
};

//! Set the global quantization mode for APyFloat
void set_float_quantization_mode(QuantizationMode mode);

//! Return the global quantization mode for APyFloat
QuantizationMode get_float_quantization_mode();

//! Set the global seed for stochastic quantization for APyFloat
void set_float_quantization_seed(std::uint64_t);

//! Get the global seed for stochastic quantization for APyFloat
std::uint64_t get_float_quantization_seed();

//! Return a random 64-bit number from the random number engine used for APyFloat
std::uint64_t random_number_float();

using exp_t = std::uint32_t;
using man_t = std::uint64_t;

struct APyFloatData {
    bool sign; //! Sign bit
    exp_t exp; //! Biased exponent
    man_t man; //! Mantissa with hidden one
    bool operator==(const APyFloatData& other) const
    {
        return std::make_tuple(sign, exp, man)
            == std::make_tuple(other.sign, other.exp, other.man);
    }
};

/* ********************************************************************************** *
 * *                          Cast context for APyFixed                             * *
 * ********************************************************************************** */

struct APyFixedCastOption {
    QuantizationMode quantization; //! Quantization mode to use for cast operations
    OverflowMode overflow;         //! Overflow mode to use for cast operations
};

class APyFixedCastContext : public ContextManager {
public:
    APyFixedCastContext(
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    APyFixedCastOption current_mode, previous_mode;
};

//! Return the global cast mode for APyFixed
APyFixedCastOption get_fixed_cast_mode();

/* ********************************************************************************** *
 * *                          Accumulator context for APyFixed                      * *
 * ********************************************************************************** */

// Accumulator type for APyFixed
struct APyFixedAccumulatorOption {
    int bits;     //! Total number of bits to use for accumulator
    int int_bits; //! Number of integer bits to use for accumulator
    QuantizationMode
        quantization;      //! Quantization mode to use for multiplication result
    OverflowMode overflow; //! Overflow mode to apply after each accumulation
};

// Accumulator context
class APyFixedAccumulatorContext : public ContextManager {
public:
    APyFixedAccumulatorContext(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    std::optional<APyFixedAccumulatorOption> current_mode, previous_mode;
};

//! Return the global accumulator mode for APyFixed
std::optional<APyFixedAccumulatorOption> get_accumulator_mode_fixed();

/* ********************************************************************************** *
 * *                          Accumulator context for APyFloat                      * *
 * ********************************************************************************** */

// Accumulator type for APyFloat
struct APyFloatAccumulatorOption {
    std::uint8_t exp_bits;
    std::uint8_t man_bits;
    std::optional<exp_t> bias;
    QuantizationMode quantization;
};

// Accumulator context
class APyFloatAccumulatorContext : public ContextManager {
public:
    APyFloatAccumulatorContext(
        std::optional<int> = std::nullopt,
        std::optional<int> = std::nullopt,
        std::optional<exp_t> = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    std::optional<APyFloatAccumulatorOption> current_mode, previous_mode;
};

//! Return the global accumulator mode for APyFloat
std::optional<APyFloatAccumulatorOption> get_accumulator_mode_float();

#endif // _APYTYPES_COMMON_H
