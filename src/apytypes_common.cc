#include "apytypes_common.h"
#include <stdexcept>

RoundingMode rounding_mode = RoundingMode::TIES_TO_EVEN;

void set_rounding_mode(RoundingMode mode) { rounding_mode = mode; }

RoundingMode get_rounding_mode() { return rounding_mode; }

APyFixedRoundingMode translate_rounding_mode(RoundingMode mode)
{
    switch (mode) {
    case RoundingMode::TO_ZERO:
        return APyFixedRoundingMode::TRN;
    case RoundingMode::TIES_TO_EVEN:
        return APyFixedRoundingMode::RND_CONV;
    case RoundingMode::TIES_TO_AWAY:
        return APyFixedRoundingMode::RND;
    default:
        throw std::domain_error(
            "No corresponding rounding mode between APyFloat and APyFixed."
        );
    }
}

RoundingMode translate_rounding_mode(APyFixedRoundingMode mode)
{
    switch (mode) {
    case APyFixedRoundingMode::TRN:
        return RoundingMode::TO_ZERO;
    case APyFixedRoundingMode::RND_CONV:
        return RoundingMode::TIES_TO_EVEN;
    case APyFixedRoundingMode::RND:
        return RoundingMode::TIES_TO_AWAY;
    default:
        throw std::domain_error(
            "No corresponding rounding mode between APyFloat and APyFixed."
        );
    }
}
