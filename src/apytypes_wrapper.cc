#include "apytypes_common.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_accumulator_context(py::module& m);
void bind_common(py::module& m);
void bind_context_manager(py::module& m);
void bind_fixed(py::module& m);
void bind_fixed_array(py::module& m);
void bind_float(py::module& m);
void bind_float_array(py::module& m);
void bind_float_context(py::module& m);

PYBIND11_MODULE(_apytypes, m)
{
    bind_common(m);
    bind_fixed(m);
    bind_fixed_array(m);
    bind_float(m);
    bind_float_array(m);
    bind_context_manager(m);
    bind_float_context(m);
    bind_accumulator_context(m);
}

void bind_common(py::module& m)
{
    py::enum_<RoundingMode>(m, "RoundingMode")
        /*
         * Base rounding modes
         */
        .value("TRN", RoundingMode::TRN)
        .value("TRN_ZERO", RoundingMode::TRN_ZERO)
        .value("TRN_INF", RoundingMode::TRN_INF)
        .value("RND", RoundingMode::RND)
        .value("RND_ZERO", RoundingMode::RND_ZERO)
        .value("RND_INF", RoundingMode::RND_INF)
        .value("RND_MIN_INF", RoundingMode::RND_MIN_INF)
        .value("RND_CONV", RoundingMode::RND_CONV)
        .value("RND_CONV_ODD", RoundingMode::RND_CONV_ODD)
        .value("JAM", RoundingMode::JAM)
        .value("STOCHASTIC_WEIGHTED", RoundingMode::STOCHASTIC_WEIGHTED)
        .value("STOCHASTIC_EQUAL", RoundingMode::STOCHASTIC_EQUAL)

        /*
         * Floating point aliases
         */
        .value("TO_NEG", RoundingMode::TRN)
        .value("TO_ZERO", RoundingMode::TRN_ZERO)
        .value("TO_POS", RoundingMode::TRN_INF)
        .value("TIES_ZERO", RoundingMode::RND_ZERO)
        .value("TIES_AWAY", RoundingMode::RND_INF)
        .value("TIES_EVEN", RoundingMode::RND_CONV)
        .value("TIES_ODD", RoundingMode::RND_CONV_ODD);

    py::enum_<OverflowMode>(m, "OverflowMode")
        .value("WRAP", OverflowMode::WRAP)
        .value("SAT", OverflowMode::SAT);

    m.def("set_rounding_mode", &set_rounding_mode);
    m.def("get_rounding_mode", &get_rounding_mode);
    m.def("set_rounding_seed", &set_rounding_seed);
    m.def("get_rounding_seed", &get_rounding_seed);
}
