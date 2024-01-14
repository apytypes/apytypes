#include "apy_common.h"

RoundingMode rounding_mode = RoundingMode::TIES_TO_EVEN;

void set_rounding_mode(RoundingMode mode) { rounding_mode = mode; }

RoundingMode get_rounding_mode() { return rounding_mode; }
