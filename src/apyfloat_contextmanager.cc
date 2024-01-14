#include "apyfloat_contextmanager.h"

RoundingContext::RoundingContext(const RoundingMode& new_mode)
    : new_mode(new_mode)
    , prev_mode(get_rounding_mode())
{
}

void RoundingContext::enter_context() { set_rounding_mode(new_mode); }

void RoundingContext::exit_context() { set_rounding_mode(prev_mode); }
