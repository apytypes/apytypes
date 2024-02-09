#include "apytypes_common.h"
#include "apytypes_util.h"
#include <iostream>

/* ********************************************************************************** *
 * *                          Rounding context for APyFloat                         * *
 * ********************************************************************************** */

// Global rounding mode
static RoundingMode global_rounding_mode = RoundingMode::RND_CONV;

// Get the global rounding mode
RoundingMode get_rounding_mode() { return global_rounding_mode; }

void set_rounding_mode(RoundingMode mode) { global_rounding_mode = mode; }

RoundingContext::RoundingContext(const RoundingMode& new_mode)
    : new_mode(new_mode)
    , prev_mode(get_rounding_mode())
{
}

void RoundingContext::enter_context() { set_rounding_mode(new_mode); }
void RoundingContext::exit_context() { set_rounding_mode(prev_mode); }

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
    std::optional<RoundingMode> rounding_mode,
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
    RoundingMode acc_rounding_mode = rounding_mode.value_or(RoundingMode::TRN);
    OverflowMode acc_overflow_mode = overflow_mode.value_or(OverflowMode::WRAP);
    current_mode = { acc_bits, acc_int_bits, acc_rounding_mode, acc_overflow_mode };
}

void AccumulatorContext::enter_context() { global_accumulator_option = current_mode; }
void AccumulatorContext::exit_context() { global_accumulator_option = previous_mode; }
