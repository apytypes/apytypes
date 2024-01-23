#include "apytypes_common.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_common(py::module& m);
void bind_fixed(py::module& m);
void bind_float(py::module& m);
void bind_float_context(py::module& m);

PYBIND11_MODULE(_apytypes, m)
{
    bind_common(m);
    bind_fixed(m);
    bind_float(m);
    bind_float_context(m);
}

void bind_common(py::module& m)
{
    py::enum_<RoundingMode>(m, "RoundingMode")
        .value("TO_POSITIVE", RoundingMode::TO_POSITIVE)
        .value("TO_NEGATIVE", RoundingMode::TO_NEGATIVE)
        .value("TO_ZERO", RoundingMode::TO_ZERO)
        .value("TIES_TO_EVEN", RoundingMode::TIES_TO_EVEN)
        .value("TIES_TO_AWAY", RoundingMode::TIES_TO_AWAY)
        .value("JAMMING", RoundingMode::JAMMING);

    py::enum_<APyFixedRoundingMode>(m, "APyFixedRoundingMode")
        .value("TRN", APyFixedRoundingMode::TRN)
        .value("RND", APyFixedRoundingMode::RND)
        .value("RND_TO_ZERO", APyFixedRoundingMode::RND_TO_ZERO)
        .value("RND_AWAY_ZERO", APyFixedRoundingMode::RND_AWAY_ZERO)
        .value("RND_CONV", APyFixedRoundingMode::RND_CONV)
        .value("RND_CONV_ODD", APyFixedRoundingMode::RND_CONV_ODD);

    py::enum_<APyFixedOverflowMode>(m, "APyFixedOverflowMode")
        .value("TWOS_OVERFLOW", APyFixedOverflowMode::TWOS_OVERFLOW)
        .value("SATURATE", APyFixedOverflowMode::SATURATE);

    m.def("set_rounding_mode", &set_rounding_mode);
    m.def("get_rounding_mode", &get_rounding_mode);

    // APyTypes version number
    m.attr("__version__") = "0.0.1";
}
