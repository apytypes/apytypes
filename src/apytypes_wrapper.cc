#include "apytypes_common.h"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

void bind_accumulator_context(nb::module_& m);
void bind_common(nb::module_& m);
void bind_context_manager(nb::module_& m);
void bind_fixed(nb::module_& m);
void bind_fixed_array(nb::module_& m);
void bind_float(nb::module_& m);
void bind_float_array(nb::module_& m);
void bind_float_context(nb::module_& m);

NB_MODULE(_apytypes, m)
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

void bind_common(nb::module_& m)
{
    nb::enum_<QuantizationMode>(m, "QuantizationMode")
        /*
         * Base quantization modes
         */
        .value(
            "TRN",
            QuantizationMode::TRN,
            R"pbdoc(Truncation (remove bits, round towards negative infinity)pbdoc"
        )
        .value(
            "TRN_ZERO",
            QuantizationMode::TRN_ZERO,
            R"pbdoc(Magnitude truncation (round towards zero).)pbdoc"
        )
        .value(
            "TRN_INF",
            QuantizationMode::TRN_INF,
            R"pbdoc(Round towards positive infinity.)pbdoc"
        )
        .value(
            "RND",
            QuantizationMode::RND,
            R"pbdoc(Round to nearest, ties towards postive infinity (standard 'round' for fixed-point).)pbdoc"
        )
        .value(
            "RND_ZERO",
            QuantizationMode::RND_ZERO,
            R"pbdoc(Round to nearest, ties toward zero.)pbdoc"
        )
        .value(
            "RND_INF",
            QuantizationMode::RND_INF,
            R"pbdoc(Round to nearest, ties away from zero.)pbdoc"
        )
        .value(
            "RND_MIN_INF",
            QuantizationMode::RND_MIN_INF,
            R"pbdoc(Round to nearest, ties toward negative infinity.)pbdoc"
        )
        .value(
            "RND_CONV",
            QuantizationMode::RND_CONV,
            R"pbdoc(Round to nearest, ties to even.)pbdoc"
        )
        .value(
            "RND_CONV_ODD",
            QuantizationMode::RND_CONV_ODD,
            R"pbdoc(Round to nearest, ties to odd.)pbdoc"
        )
        .value(
            "JAM",
            QuantizationMode::JAM,
            R"pbdoc(Jamming/von Neumann rounding. Set LSB to 1.)pbdoc"
        )
        .value(
            "JAM_UNBIASED",
            QuantizationMode::JAM_UNBIASED,
            R"pbdoc(Unbiased jamming/von Neumann rounding. Set LSB to 1 unless the previous LSB and all the removed bits are 0.)pbdoc"
        )
        .value(
            "STOCH_WEIGHTED",
            QuantizationMode::STOCH_WEIGHTED,
            R"pbdoc(Stochastic rounding. Probability depends on the bits to remove.)pbdoc"
        )
        .value(
            "STOCH_EQUAL",
            QuantizationMode::STOCH_EQUAL,
            R"pbdoc(Stochastic rounding with equal probability.)pbdoc"
        )

        /*
         * Floating point aliases
         */
        .value(
            "TO_NEG",
            QuantizationMode::TRN,
            R"pbdoc(Alias. Round towards negative infinity.)pbdoc"
        )
        .value(
            "TO_ZERO",
            QuantizationMode::TRN_ZERO,
            R"pbdoc(Alias. Round towards zero.)pbdoc"
        )
        .value(
            "TO_POS",
            QuantizationMode::TRN_INF,
            R"pbdoc(Alias. Round towards positive infinity.)pbdoc"
        )
        .value(
            "TIES_ZERO",
            QuantizationMode::RND_ZERO,
            R"pbdoc(Alias. Round to nearest, ties toward zero.)pbdoc"
        )
        .value(
            "TIES_AWAY",
            QuantizationMode::RND_INF,
            R"pbdoc(Alias. Round to nearest, ties away from zero.)pbdoc"
        )
        .value(
            "TIES_EVEN",
            QuantizationMode::RND_CONV,
            R"pbdoc(Alias. Round to nearest, ties to even.)pbdoc"
        )
        .value(
            "TIES_ODD",
            QuantizationMode::RND_CONV_ODD,
            R"pbdoc(Alias. Round to nearest, ties to odd.)pbdoc"
        )
        .value(
            "TIES_NEG",
            QuantizationMode::RND_MIN_INF,
            R"pbdoc(Alias. Round to nearest, ties towards negative infinity.)pbdoc"
        )
        .value(
            "TIES_POS",
            QuantizationMode::RND,
            R"pbdoc(Alias. Round to nearest, ties towards positive infinity.)pbdoc"
        );

    nb::enum_<OverflowMode>(m, "OverflowMode")
        .value(
            "WRAP",
            OverflowMode::WRAP,
            R"pbdoc(Two's complement overflow. Remove MSBs.)pbdoc"
        )
        .value(
            "SAT",
            OverflowMode::SAT,
            R"pbdoc(Saturate to the closest of most positive and most negative value.)pbdoc"
        )
        .value(
            "NUMERIC_STD",
            OverflowMode::NUMERIC_STD,
            R"pbdoc(Remove MSBs, but keep the most significant bit. As ieee.numeric_std resize for signed.)pbdoc"
        );

    m.def("set_quantization_mode", &set_quantization_mode);
    m.def("get_quantization_mode", &get_quantization_mode);
    m.def("set_quantization_seed", &set_quantization_seed);
    m.def("get_quantization_seed", &get_quantization_seed);
}
