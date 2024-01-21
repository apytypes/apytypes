#include "apyfloat_contextmanager.h"

RoundingContext::RoundingContext(const RoundingMode& new_mode, std::optional<std::uint64_t> new_seed)
    : new_mode(new_mode)
    , prev_mode(get_rounding_mode())
    , new_seed(new_seed.value_or(get_rounding_seed()))
    , prev_seed(get_rounding_seed())
{
    if (new_seed.has_value() && new_mode != RoundingMode::STOCHASTIC_WEIGHTED && new_mode != RoundingMode::STOCHASTIC_EQUAL) {
        throw std::domain_error("Seed for rounding was given for a non-stochastic rounding mode.");
    }
}

void RoundingContext::enter_context() { 
    set_rounding_mode(new_mode);
    set_rounding_seed(new_seed);
}

void RoundingContext::exit_context() { 
    set_rounding_mode(prev_mode); 
    set_rounding_seed(prev_seed);
}
