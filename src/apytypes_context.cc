#include "apytypes_context.h"
#include "apyfloat_util.h"
#include "apytypes_common.h"

// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
namespace nb = nanobind;

/* ********************************************************************************** *
 * *                      Floating-point quantization context                       * *
 * ********************************************************************************** */

APyFloatQuantizationContext::APyFloatQuantizationContext(
    const QuantizationMode& new_mode, std::optional<std::uint64_t> seed
)
    : context_mode { new_mode }
    , context_seed { seed.value_or(std::random_device {}()) }
    , context_engine { context_seed }
{
    // Previous state must be captured on `enter_context()`
}

void APyFloatQuantizationContext::enter_context()
{
    // Copy old state
    prev_mode = get_float_quantization_mode();
    prev_seed = get_rnd64_fp_seed();
    prev_engine = get_rnd64_fp_engine();

    // Set new state
    set_float_quantization_mode(context_mode);
    set_rnd64_fp_seed(context_seed);
    set_rnd64_fp_engine(context_engine);
}

void APyFloatQuantizationContext::exit_context()
{
    set_float_quantization_mode(prev_mode);
    set_rnd64_fp_seed(prev_seed);
    set_rnd64_fp_engine(prev_engine);
}

/* ********************************************************************************** *
 * *                            Fixed-point cast context                            * *
 * ********************************************************************************** */

APyFixedCastContext::APyFixedCastContext(
    std::optional<QuantizationMode> quantization, std::optional<OverflowMode> overflow
)
    : context_qntz_mode(quantization)
    , context_overflow_mode(overflow)
{
    if (!quantization.has_value() && !overflow.has_value()) {
        std::string msg = "Quantization mode or overflow mode must be specified";
        throw nb::value_error(msg.c_str());
    }

    // Previous state must be captured on `enter_context()`
}

void APyFixedCastContext::enter_context()
{
    // Capture the previous cast mode
    previous_mode = get_fixed_cast_mode();

    // Setup the new cast context which *may* be dependent on the current cast mode
    APyFixedCastOption context_mode {
        context_qntz_mode.value_or(previous_mode.quantization),
        context_overflow_mode.value_or(previous_mode.overflow)
    };
    set_fixed_cast_mode(context_mode);
}
void APyFixedCastContext::exit_context() { set_fixed_cast_mode(previous_mode); }

/* ********************************************************************************** *
 * *                       Floating-point accumulator context                       * *
 * ********************************************************************************** */

APyFloatAccumulatorContext::APyFloatAccumulatorContext(
    std::optional<int> exp_bits,
    std::optional<int> man_bits,
    std::optional<exp_t> bias,
    std::optional<QuantizationMode> quantization
)
{
    // Extract the input
    APyFloatAccumulatorOption new_mode {};

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

    // Setup the context mode
    context_mode = new_mode;

    // Previous mode must be captured on `enter_context()`
}

void APyFloatAccumulatorContext::enter_context()
{
    previous_mode = get_accumulator_mode_float();
    set_accumulator_mode_float(context_mode);
}
void APyFloatAccumulatorContext::exit_context()
{
    set_accumulator_mode_float(previous_mode);
}

/* ********************************************************************************** *
 * *                         Fixed-point accumulator context                        * *
 * ********************************************************************************** */

APyFixedAccumulatorContext::APyFixedAccumulatorContext(
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<int> bits
)
{
    APyFixedAccumulatorOption new_mode {};
    new_mode.bits = bits_from_optional(bits, int_bits, frac_bits);
    new_mode.int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    new_mode.quantization = quantization.value_or(QuantizationMode::TRN);
    new_mode.overflow = overflow.value_or(OverflowMode::WRAP);

    // Set the current mode
    context_mode = new_mode;

    // Previous mode must be captured on `enter_context()`
}

void APyFixedAccumulatorContext::enter_context()
{
    previous_mode = get_accumulator_mode_fixed();
    set_accumulator_mode_fixed(context_mode);
}

void APyFixedAccumulatorContext::exit_context()
{
    set_accumulator_mode_fixed(previous_mode);
}
