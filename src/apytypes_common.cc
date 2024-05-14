// Python object access through Pybind
#include <nanobind/nanobind.h>
namespace nb = nanobind;

#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include <random>

/* ********************************************************************************** *
 * *                          Quantization context for APyFloat * *
 * ********************************************************************************** */

// Global quantization mode
static QuantizationMode global_quantization_mode_float = QuantizationMode::RND_CONV;

// Get the global quantization mode
QuantizationMode get_float_quantization_mode()
{
    return global_quantization_mode_float;
}

void set_float_quantization_mode(QuantizationMode mode)
{
    global_quantization_mode_float = mode;
}

APyFloatQuantizationContext::APyFloatQuantizationContext(
    const QuantizationMode& new_mode, std::optional<std::uint64_t> new_seed
)
    : new_mode(new_mode)
    , prev_mode(get_float_quantization_mode())
    , new_seed(new_seed.value_or(get_float_quantization_seed()))
    , prev_seed(get_float_quantization_seed())
{
    if (new_seed.has_value() && new_mode != QuantizationMode::STOCH_WEIGHTED
        && new_mode != QuantizationMode::STOCH_EQUAL) {
        throw nb::value_error(
            "Seed for quantization was given for a non-stochastic quantization mode."
        );
    }
}

void APyFloatQuantizationContext::enter_context()
{
    set_float_quantization_mode(new_mode);
    set_float_quantization_seed(new_seed);
}

void APyFloatQuantizationContext::exit_context()
{
    set_float_quantization_mode(prev_mode);
    set_float_quantization_seed(prev_seed);
}

/* ********************************************************************************** *
 * *                          Random number engine for APyFloat                     * *
 * ********************************************************************************** */

// This creates a random seed on every program start.
std::uint64_t quantization_seed = std::random_device {}();

// A random number engine is used instead of purely std::random_device so that runs can
// be reproducible.
std::mt19937_64 gen64(quantization_seed);

void set_float_quantization_seed(std::uint64_t seed)
{
    quantization_seed = seed;
    gen64.seed(seed);
}

std::uint64_t get_float_quantization_seed() { return quantization_seed; }

std::uint64_t random_number_float() { return gen64(); }

/* ********************************************************************************** *
 * *                      Cast context for APyFixed                                 * *
 * ********************************************************************************** */

// Global accumulator option (default value: std::nullopt)
static APyFixedCastOption global_cast_option_fixed
    = { QuantizationMode::TRN, OverflowMode::WRAP };

APyFixedCastContext::APyFixedCastContext(
    std::optional<QuantizationMode> quantization, std::optional<OverflowMode> overflow
)
{
    if (!quantization.has_value() && !overflow.has_value()) {
        throw nb::value_error(
            "Either quantization mode or overflow mode must be specified."
        );
    }

    // Store the previous accumulator mode
    previous_mode = global_cast_option_fixed;

    // Extract the input
    APyFixedCastOption new_mode;

    new_mode.quantization = quantization.value_or(previous_mode.quantization);
    new_mode.overflow = overflow.value_or(previous_mode.overflow);

    // Set the current mode
    current_mode = new_mode;
}

void APyFixedCastContext::enter_context() { global_cast_option_fixed = current_mode; }
void APyFixedCastContext::exit_context() { global_cast_option_fixed = previous_mode; }

APyFixedCastOption get_fixed_cast_mode() { return global_cast_option_fixed; }

/* ********************************************************************************** *
 * *                      Accumulator context for APyFixedArray                     * *
 * ********************************************************************************** */

// Global accumulator option (default value: std::nullopt)
static std::optional<APyFixedAccumulatorOption> global_accumulator_option_fixed;

// Retrieve the global accumulator mode
std::optional<APyFixedAccumulatorOption> get_accumulator_mode_fixed()
{
    return global_accumulator_option_fixed;
}

APyFixedAccumulatorContext::APyFixedAccumulatorContext(
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<int> bits
)
{
    // Store the previous accumulator mode
    previous_mode = global_accumulator_option_fixed;

    // Extract the input
    APyFixedAccumulatorOption new_mode
        = get_accumulator_mode_fixed().value_or(APyFixedAccumulatorOption {});

    new_mode.bits = bits_from_optional(bits, int_bits, frac_bits);
    new_mode.int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    new_mode.quantization = quantization.value_or(QuantizationMode::TRN);
    new_mode.overflow = overflow.value_or(OverflowMode::WRAP);

    // Set the current mode
    current_mode = new_mode;
}

void APyFixedAccumulatorContext::enter_context()
{
    global_accumulator_option_fixed = current_mode;
}
void APyFixedAccumulatorContext::exit_context()
{
    global_accumulator_option_fixed = previous_mode;
}

/* ********************************************************************************** *
 * *                      Accumulator context for APyFloatArray                     * *
 * ********************************************************************************** */

// Global accumulator option (default value: std::nullopt)
static std::optional<APyFloatAccumulatorOption> global_accumulator_option_float;

// Retrieve the global accumulator mode
std::optional<APyFloatAccumulatorOption> get_accumulator_mode_float()
{
    return global_accumulator_option_float;
}

APyFloatAccumulatorContext::APyFloatAccumulatorContext(
    std::optional<int> exp_bits,
    std::optional<int> man_bits,
    std::optional<exp_t> bias,
    std::optional<QuantizationMode> quantization
)
{
    // Store the previous accumulator mode§
    previous_mode = global_accumulator_option_float;

    // Extract the input
    APyFloatAccumulatorOption new_mode
        = get_accumulator_mode_float().value_or(APyFloatAccumulatorOption {});

    if (!exp_bits.has_value() || !man_bits.has_value()) {
        throw nb::value_error(
            "Both the exponent bits and mantissa bits must be specified."
        );
    }

    check_exponent_format(exp_bits.value());
    check_mantissa_format(man_bits.value());

    new_mode.exp_bits = exp_bits.value();
    new_mode.man_bits = man_bits.value();
    new_mode.bias = bias;
    new_mode.quantization = quantization.value_or(get_float_quantization_mode());

    // Set the current mode
    current_mode = new_mode;
}

void APyFloatAccumulatorContext::enter_context()
{
    global_accumulator_option_float = current_mode;
}
void APyFloatAccumulatorContext::exit_context()
{
    global_accumulator_option_float = previous_mode;
}
