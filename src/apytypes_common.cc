#include "apytypes_common.h"
#include <stdexcept>

/*
 * Global rounding mode for floating-point number
 */
RoundingMode rounding_mode = RoundingMode::RND_CONV;

void set_rounding_mode(RoundingMode mode) { rounding_mode = mode; }

RoundingMode get_rounding_mode() { return rounding_mode; }
