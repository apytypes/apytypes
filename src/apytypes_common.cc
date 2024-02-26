#include "apytypes_common.h"
#include "apyfloat.h"
#include "apytypes_util.h"
#include <random>

/* ********************************************************************************** *
 * *                          Quantization context for APyFloat * *
 * ********************************************************************************** */

// Global quantization mode
static QuantizationMode global_quantization_mode = QuantizationMode::RND_CONV;

// Get the global quantization mode
QuantizationMode get_quantization_mode() { return global_quantization_mode; }

void set_quantization_mode(QuantizationMode mode) { global_quantization_mode = mode; }

QuantizationContext::QuantizationContext(
    const QuantizationMode& new_mode, std::optional<std::uint64_t> new_seed
)
    : new_mode(new_mode)
    , prev_mode(get_quantization_mode())
    , new_seed(new_seed.value_or(get_quantization_seed()))
    , prev_seed(get_quantization_seed())
{
    if (new_seed.has_value() && new_mode != QuantizationMode::STOCH_WEIGHTED
        && new_mode != QuantizationMode::STOCH_EQUAL) {
        throw py::value_error(
            "Seed for quantization was given for a non-stochastic quantization mode."
        );
    }
}

void QuantizationContext::enter_context()
{
    set_quantization_mode(new_mode);
    set_quantization_seed(new_seed);
}

void QuantizationContext::exit_context()
{
    set_quantization_mode(prev_mode);
    set_quantization_seed(prev_seed);
}

/* ********************************************************************************** *
 * *                          Random number engine                                  * *
 * ********************************************************************************** */

// This creates a random seed on every program start.
std::uint64_t quantization_seed = std::random_device {}();

// A random number engine is used instead of purely std::random_device so that runs can
// be reproducible.
std::mt19937_64 gen64(quantization_seed);

void set_quantization_seed(std::uint64_t seed)
{
    quantization_seed = seed;
    gen64.seed(seed);
}

std::uint64_t get_quantization_seed() { return quantization_seed; }

std::uint64_t random_number() { return gen64(); }

/* ********************************************************************************** *
 * *                      Accumulator context for APyFixedArray                     * *
 * ********************************************************************************** */

// Global accumulator option (default value: std::nullopt)
static std::optional<AccumulatorOption> global_accumulator_option;

// Retrieve the global accumulator mode
std::optional<AccumulatorOption> get_accumulator_mode()
{
    return global_accumulator_option;
}

AccumulatorContext::AccumulatorContext(
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<std::uint8_t> _exp_bits,
    std::optional<std::uint8_t> _man_bits,
    std::optional<exp_t> _bias
)
{
    // Check that APyFixed specific parameters are mixed with APyFloat parameters
    const bool any_apfixed_parameters = bits.has_value() || int_bits.has_value()
        || frac_bits.has_value() || overflow.has_value();
    const bool any_apfloat_parameters
        = _exp_bits.has_value() || _man_bits.has_value() || _bias.has_value();
    if (any_apfixed_parameters == any_apfloat_parameters) {
        throw py::value_error(
            "Invalid combination of parameters for accumulator context"
        );
    }

    AccumulatorOption new_mode = get_accumulator_mode().value_or(AccumulatorOption {});
    QuantizationMode acc_quantization;

    if (any_apfixed_parameters) {
        // Extract the input
        int acc_bits = bits_from_optional(bits, int_bits, frac_bits);
        int acc_int_bits = int_bits_from_optional(bits, int_bits, frac_bits);

        // Store the previous accumulator mode
        previous_mode = global_accumulator_option;

        acc_quantization = quantization.value_or(QuantizationMode::TRN);
        OverflowMode acc_overflow_mode = overflow.value_or(OverflowMode::WRAP);

        // Set the current mode
        new_mode.bits = acc_bits;
        new_mode.int_bits = acc_int_bits;
        new_mode.quantization = acc_quantization;
        new_mode.overflow = acc_overflow_mode;
    } else {
        new_mode.exp_bits = _exp_bits.value();
        new_mode.man_bits = _man_bits.value();
        new_mode.bias = _bias.value_or(APyFloat::ieee_bias(new_mode.exp_bits));
        acc_quantization = quantization.value_or(get_quantization_mode());
    }

    new_mode.quantization = acc_quantization;
    current_mode = new_mode;
}

void AccumulatorContext::enter_context() { global_accumulator_option = current_mode; }
void AccumulatorContext::exit_context() { global_accumulator_option = previous_mode; }
