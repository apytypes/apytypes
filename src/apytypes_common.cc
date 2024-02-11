#include "apytypes_common.h"
#include "apytypes_util.h"
#include <random>
#include <stdexcept>

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
    if (new_seed.has_value() && new_mode != QuantizationMode::STOCHASTIC_WEIGHTED
        && new_mode != QuantizationMode::STOCHASTIC_EQUAL) {
        throw std::domain_error(
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
    std::optional<QuantizationMode> quantization_mode,
    std::optional<OverflowMode> overflow_mode
)
{
    // Sanitize the input bits
    int acc_bits, acc_int_bits;
    set_bit_specifiers_from_optional(acc_bits, acc_int_bits, bits, int_bits, frac_bits);
    bit_specifier_sanitize_bits(acc_bits, acc_int_bits);

    // Store the previous accumulator mode
    previous_mode = global_accumulator_option;

    // Set the current mode
    QuantizationMode acc_quantization_mode
        = quantization_mode.value_or(QuantizationMode::TRN);
    OverflowMode acc_overflow_mode = overflow_mode.value_or(OverflowMode::WRAP);
    current_mode = { acc_bits, acc_int_bits, acc_quantization_mode, acc_overflow_mode };
}

void AccumulatorContext::enter_context() { global_accumulator_option = current_mode; }
void AccumulatorContext::exit_context() { global_accumulator_option = previous_mode; }
