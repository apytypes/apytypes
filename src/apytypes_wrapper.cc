#include "apytypes_common.h"
#include "apytypes_simd.h"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

void bind_accumulator_context(nb::module_& m);
void bind_cast_context(nb::module_& m);
void bind_cfixed(nb::module_& m);
void bind_cfixed_array(nb::module_& m);
void bind_cfloat(nb::module_& m);
void bind_cfloat_array(nb::module_& m);
void bind_common(nb::module_& m);
void bind_context_manager(nb::module_& m);
void bind_fixed(nb::module_& m);
void bind_fixed_array(nb::module_& m);
void bind_float(nb::module_& m);
void bind_float_array(nb::module_& m);
void bind_quantization_context(nb::module_& m);

NB_MODULE(_apytypes, m)
{
    bind_common(m);
    bind_cfixed(m);
    bind_cfixed_array(m);
    bind_cfloat(m);
    bind_cfloat_array(m);
    bind_fixed(m);
    bind_fixed_array(m);
    bind_float(m);
    bind_float_array(m);
    bind_context_manager(m);
    bind_quantization_context(m);
    bind_accumulator_context(m);
    bind_cast_context(m);

    /*
     * Nanobind leak warnings are enabled by `NANOBIND_LEAK_WARNINGS` pre-processor
     * macro. Specified in meson.build.
     */
#if !defined(NANOBIND_LEAK_WARNINGS)
#error "CPP macro `NANOBIND_LEAK_WARNINGS` not specified"
#else
#if NANOBIND_LEAK_WARNINGS == 1
    nb::set_leak_warnings(true);
#else
    nb::set_leak_warnings(false);
#endif
#endif

} /* end NB_MODULE(_apytypes, m) */

void bind_common(nb::module_& m)
{
    nb::enum_<QuantizationMode>(m, "QuantizationMode")
        /*
         * Floating point aliases, must come before base modes
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
            "TO_AWAY",
            QuantizationMode::TRN_AWAY,
            R"pbdoc(Alias. Truncate away from zero.)pbdoc"
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
        )
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
            R"pbdoc(Unbiased magnitude truncation (round towards zero).)pbdoc"
        )
        .value(
            "TRN_INF",
            QuantizationMode::TRN_INF,
            R"pbdoc(Round towards positive infinity.)pbdoc"
        )
        .value(
            "TRN_AWAY",
            QuantizationMode::TRN_AWAY,
            R"pbdoc(Truncate away from zero.)pbdoc"
        )
        .value(
            "TRN_MAG",
            QuantizationMode::TRN_MAG,
            R"pbdoc(
            Magnitude truncation (round towards zero), fixed-point friendly variant (add
            sign-bit).
            )pbdoc"
        )
        .value(
            "RND",
            QuantizationMode::RND,
            R"pbdoc(
            Round to nearest, ties towards positive infinity (standard 'round' for
            fixed-point).
            )pbdoc"
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
            R"pbdoc(
            Unbiased jamming/von Neumann rounding. Set LSB to 1 unless the previous LSB
            and all the removed bits are 0.
            )pbdoc"
        )
        .value(
            "STOCH_WEIGHTED",
            QuantizationMode::STOCH_WEIGHTED,
            R"pbdoc(
            Stochastic rounding. Probability depends on the bits to remove.
            )pbdoc"
        )
        .value(
            "STOCH_EQUAL",
            QuantizationMode::STOCH_EQUAL,
            R"pbdoc(Stochastic rounding with equal probability.)pbdoc"
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
            R"pbdoc(
            Saturate to the closest of most positive and most negative value.
            )pbdoc"
        )
        .value(
            "NUMERIC_STD",
            OverflowMode::NUMERIC_STD,
            R"pbdoc(
            Remove MSBs, but keep the most significant bit. As ieee.numeric_std resize
            for signed.
            )pbdoc"
        );

    m.def(
         "set_float_quantization_mode",
         &set_float_quantization_mode,
         nb::arg("mode"),
         R"pbdoc(
        Set current quantization context.

        Parameters
        ----------
        mode : :class:`QuantizationMode`
            The quantization mode to use.

        See Also
        --------
        get_float_quantization_mode
        )pbdoc"
    )
        .def("get_float_quantization_mode", &get_float_quantization_mode, R"pbdoc(
        Get current quantization context.

        Returns
        -------
        :class:`QuantizationMode`

        See Also
        --------
        set_float_quantization_mode
        )pbdoc")
        .def(
            "set_float_quantization_seed",
            &rst_default_rnd64_fp,
            nb::arg("seed"),
            R"pbdoc(
            Reset the floating-point default stochastic quantization engine.

            Parameters
            ----------
            seed : :class:`int`
                The quantization seed passed to the default engine.
        )pbdoc"
        )
        .def(
            "set_fixed_quantization_seed",
            &rst_default_rnd64_fx,
            nb::arg("seed"),
            R"pbdoc(
            Reset the fixed-point default stochastic quantization engine.

            Parameters
            ----------
            seed : :class:`int`
                The quantization seed passed to the default engine.
            )pbdoc"
        )
        .def(
            "get_float_quantization_seed",
            &get_rnd64_fp_seed,
            R"pbdoc(
            Retrieve the currently used floating-point quantization seed.

            Returns
            -------
            :class:`int`

            )pbdoc"
        )
        .def(
            "get_fixed_quantization_seed",
            &get_rnd64_fx_seed,
            R"pbdoc(
            Retrieve the currently used fixed-point quantization seed.

            Returns
            -------
            :class:`int`

            )pbdoc"
        )

        /* Get the APyTypes SIMD version string */
        .def("_get_simd_version_str", &simd::get_simd_version_str);
}
