#include "apytypes_common.h"
#include "apyfloat_util.h"
#include "apytypes_util.h"

// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
namespace nb = nanobind;

#include <random>

/* ********************************************************************************** *
 * *                        Thread local states for APyTypes                        * *
 * ********************************************************************************** */

// Quantization mode for floating-point arithmetic
thread_local static QuantizationMode qntz_mode_fl = QuantizationMode::RND_CONV;

// Get the thread-local floating-point quantization mode
QuantizationMode get_float_quantization_mode() { return qntz_mode_fl; }

// Set the thread-local floating-point quantization mode
void set_float_quantization_mode(QuantizationMode mode) { qntz_mode_fl = mode; }

/* ********************************************************************************** *
 * *            Random number engines for APyTypes stochastic quantization          * *
 * ********************************************************************************** */

// Seed used for the current stochastic rounding engine
thread_local static std::uint64_t rnd64_fx_seed = std::random_device {}();
thread_local static std::uint64_t rnd64_fp_seed = std::random_device {}();

// Default random-number generators for stochastic quantization (uniform distribution)
thread_local static std::mt19937_64 default_mt19937_fx { rnd64_fx_seed };
thread_local static std::mt19937_64 default_mt19937_fp { rnd64_fp_seed };
static std::uint64_t default_rnd64_fx() { return default_mt19937_fx(); }
static std::uint64_t default_rnd64_fp() { return default_mt19937_fp(); }

// Current (thread local) stochastic rounding floating-point random number engine
thread_local static std::function<std::uint64_t()> rnd64_fx_ptr = default_rnd64_fx;
thread_local static std::function<std::uint64_t()> rnd64_fp_ptr = default_rnd64_fp;

// Reset the default stochastic quantization random number generators
void rst_default_rnd64_fx(std::uint64_t seed)
{
    rnd64_fx_seed = seed;
    default_mt19937_fx.seed(seed);
}
void rst_default_rnd64_fp(std::uint64_t seed)
{
    rnd64_fp_seed = seed;
    default_mt19937_fp.seed(seed);
}

// Retrieve the seed used to initialize the active random number engine
std::uint64_t get_rnd64_fx_seed() { return rnd64_fx_seed; }
std::uint64_t get_rnd64_fp_seed() { return rnd64_fp_seed; }

// Generate a 64-bit random number using the current random-number engines
std::uint64_t rnd64_fx() { return rnd64_fx_ptr(); }
std::uint64_t rnd64_fp() { return rnd64_fp_ptr(); }

/* ********************************************************************************** *
 * *                          Quantization context for APyFloat                     * *
 * ********************************************************************************** */

APyFloatQuantizationContext::APyFloatQuantizationContext(
    const QuantizationMode& new_mode, std::optional<std::uint64_t> seed
)
    : prev_mode { get_float_quantization_mode() }
    , new_mode { new_mode }
    , prev_seed { rnd64_fp_seed }
    , new_seed { seed.value_or(std::random_device {}()) }
    , prev_engine { rnd64_fp_ptr }
    , default_engine(new_seed)
    , new_engine(std::bind(default_engine))
{
}

void APyFloatQuantizationContext::enter_context()
{
    set_float_quantization_mode(new_mode);
    rnd64_fp_seed = new_seed;
    rnd64_fp_ptr = new_engine;
}

void APyFloatQuantizationContext::exit_context()
{
    set_float_quantization_mode(prev_mode);
    rnd64_fp_seed = prev_seed;
    rnd64_fp_ptr = prev_engine;
}

/* ********************************************************************************** *
 * *                      Cast context for APyFixed                                 * *
 * ********************************************************************************** */

// Global fixed-point cast option, default value: { TRN, WRAP }
thread_local static APyFixedCastOption global_cast_option_fixed
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
thread_local static std::optional<APyFixedAccumulatorOption>
    global_accumulator_option_fixed = std::nullopt;

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
thread_local static std::optional<APyFloatAccumulatorOption>
    global_accumulator_option_float = std::nullopt;

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

    check_exponent_format(exp_bits.value(), "APyFloatAccumulatorContext");
    check_mantissa_format(man_bits.value(), "APyFloatAccumulatorContext");

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
