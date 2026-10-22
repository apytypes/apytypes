#include "apycfixed.h"
#include "apyfixed.h"
#include "apyufixed.h"
#include "nanobind_util.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>

#include <type_traits>

namespace nb = nanobind;

template <typename OP, typename L_TYPE>
static APyFixed R_OP(const APyUFixed& rhs, const L_TYPE& lhs)
{
    const APyFixed fixed_rhs = rhs.to_apyfixed();
    const APyFixed fixed_lhs = [&]() {
        if constexpr (std::is_floating_point_v<L_TYPE>) {
            return APyFixed::from_double(
                lhs, fixed_rhs.int_bits(), fixed_rhs.frac_bits()
            );
        } else {
            return APyFixed::from_integer(
                lhs, fixed_rhs.int_bits(), fixed_rhs.frac_bits()
            );
        }
    }();
    return OP()(fixed_lhs, fixed_rhs);
}

template <typename OP, typename R_TYPE>
static APyFixed L_OP(const APyUFixed& lhs, const R_TYPE& rhs)
{
    const APyFixed fixed_lhs = lhs.to_apyfixed();
    const APyFixed fixed_rhs = [&]() {
        if constexpr (std::is_floating_point_v<R_TYPE>) {
            return APyFixed::from_double(
                rhs, fixed_lhs.int_bits(), fixed_lhs.frac_bits()
            );
        } else {
            return APyFixed::from_integer(
                rhs, fixed_lhs.int_bits(), fixed_lhs.frac_bits()
            );
        }
    }();
    return OP()(fixed_lhs, fixed_rhs);
}

static APyCFixed apyufixed_to_apycfixed(const APyUFixed& value)
{
    const APyFixed backing = value.to_apyfixed();
    return APyCFixed::from_apyfixed(backing, backing.int_bits(), backing.frac_bits());
}

static APyCFixed
complex_to_apycfixed_with_spec(std::complex<double> value, const APyUFixed& spec_source)
{
    return APyCFixed::from_complex(
        value, spec_source.int_bits() + 1, spec_source.frac_bits()
    );
}

void bind_ufixed(nb::module_& m)
{
    nb::class_<APyUFixed>(m, "APyUFixed")
        .def(
            nb::init<
                nb::int_,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            nb::arg("bit_pattern"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none()
        )
        .def("copy", &APyUFixed::python_copy)
        .def("__copy__", &APyUFixed::python_copy)
        .def("__deepcopy__", &APyUFixed::python_deepcopy, nb::arg("memo"))
        .def("__getstate__", &APyUFixed::python_pickle)
        .def("__setstate__", &APyUFixed::python_unpickle)
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def(nb::self < nb::self)
        .def(nb::self <= nb::self)
        .def(nb::self > nb::self)
        .def(nb::self >= nb::self)
        .def(nb::self + nb::self, NB_NARG())
        .def(nb::self - nb::self, NB_NARG())
        .def(nb::self * nb::self, NB_NARG())
        .def(nb::self / nb::self, NB_NARG())
        .def(-nb::self)
        .def(+nb::self)
        .def(~nb::self)
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)
        .def(nb::self == nb::int_())
        .def(nb::self != nb::int_())
        .def(nb::self < nb::int_())
        .def(nb::self <= nb::int_())
        .def(nb::self > nb::int_())
        .def(nb::self >= nb::int_())
        .def(nb::self + nb::int_(), NB_NARG())
        .def(nb::self - nb::int_(), NB_NARG())
        .def(nb::self * nb::int_(), NB_NARG())
        .def(nb::self / nb::int_(), NB_NARG())
        .def("__radd__", R_OP<STD_ADD<>, nb::int_>, NB_OP())
        .def("__rsub__", R_OP<STD_SUB<>, nb::int_>, NB_OP())
        .def("__rmul__", R_OP<STD_MUL<>, nb::int_>, NB_OP())
        .def("__rtruediv__", R_OP<STD_DIV<>, nb::int_>, NB_OP())
        .def(nb::self == double())
        .def(nb::self != double())
        .def(nb::self < double())
        .def(nb::self <= double())
        .def(nb::self > double())
        .def(nb::self >= double())
        .def("__add__", L_OP<STD_ADD<>, double>, NB_OP())
        .def("__radd__", R_OP<STD_ADD<>, double>, NB_OP())
        .def("__sub__", L_OP<STD_SUB<>, double>, NB_OP())
        .def("__rsub__", R_OP<STD_SUB<>, double>, NB_OP())
        .def("__mul__", L_OP<STD_MUL<>, double>, NB_OP())
        .def("__rmul__", R_OP<STD_MUL<>, double>, NB_OP())
        .def("__truediv__", L_OP<STD_DIV<>, double>, NB_OP())
        .def("__rtruediv__", R_OP<STD_DIV<>, double>, NB_OP())
        .def(
            "__eq__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() == rhs;
            },
            NB_OP()
        )
        .def(
            "__ne__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() != rhs;
            },
            NB_OP()
        )
        .def(
            "__lt__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() < rhs;
            },
            NB_OP()
        )
        .def(
            "__le__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() <= rhs;
            },
            NB_OP()
        )
        .def(
            "__gt__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() > rhs;
            },
            NB_OP()
        )
        .def(
            "__ge__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() >= rhs;
            },
            NB_OP()
        )
        .def(
            "__add__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() + rhs;
            },
            NB_OP()
        )
        .def(
            "__radd__",
            [](const APyUFixed& rhs, const APyFixed& lhs) {
                return lhs + rhs.to_apyfixed();
            },
            NB_OP()
        )
        .def(
            "__sub__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() - rhs;
            },
            NB_OP()
        )
        .def(
            "__rsub__",
            [](const APyUFixed& rhs, const APyFixed& lhs) {
                return lhs - rhs.to_apyfixed();
            },
            NB_OP()
        )
        .def(
            "__mul__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() * rhs;
            },
            NB_OP()
        )
        .def(
            "__rmul__",
            [](const APyUFixed& rhs, const APyFixed& lhs) {
                return lhs * rhs.to_apyfixed();
            },
            NB_OP()
        )
        .def(
            "__truediv__",
            [](const APyUFixed& lhs, const APyFixed& rhs) {
                return lhs.to_apyfixed() / rhs;
            },
            NB_OP()
        )
        .def(
            "__rtruediv__",
            [](const APyUFixed& rhs, const APyFixed& lhs) {
                return lhs / rhs.to_apyfixed();
            },
            NB_OP()
        )
        .def(
            "__add__",
            [](const APyUFixed& self, std::complex<double> rhs) {
                return apyufixed_to_apycfixed(self)
                    + complex_to_apycfixed_with_spec(rhs, self);
            },
            NB_OP()
        )
        .def(
            "__radd__",
            [](const APyUFixed& self, std::complex<double> lhs) {
                return complex_to_apycfixed_with_spec(lhs, self)
                    + apyufixed_to_apycfixed(self);
            },
            NB_OP()
        )
        .def(
            "__sub__",
            [](const APyUFixed& self, std::complex<double> rhs) {
                return apyufixed_to_apycfixed(self)
                    - complex_to_apycfixed_with_spec(rhs, self);
            },
            NB_OP()
        )
        .def(
            "__rsub__",
            [](const APyUFixed& self, std::complex<double> lhs) {
                return complex_to_apycfixed_with_spec(lhs, self)
                    - apyufixed_to_apycfixed(self);
            },
            NB_OP()
        )
        .def(
            "__mul__",
            [](const APyUFixed& self, std::complex<double> rhs) {
                return apyufixed_to_apycfixed(self)
                    * complex_to_apycfixed_with_spec(rhs, self);
            },
            NB_OP()
        )
        .def(
            "__rmul__",
            [](const APyUFixed& self, std::complex<double> lhs) {
                return complex_to_apycfixed_with_spec(lhs, self)
                    * apyufixed_to_apycfixed(self);
            },
            NB_OP()
        )
        .def(
            "__truediv__",
            [](const APyUFixed& self, std::complex<double> rhs) {
                return apyufixed_to_apycfixed(self)
                    / complex_to_apycfixed_with_spec(rhs, self);
            },
            NB_OP()
        )
        .def(
            "__rtruediv__",
            [](const APyUFixed& self, std::complex<double> lhs) {
                return complex_to_apycfixed_with_spec(lhs, self)
                    / apyufixed_to_apycfixed(self);
            },
            NB_OP()
        )
        .def("__pow__", &APyUFixed::pown, NB_OP())
        .def("to_bits", &APyUFixed::to_bits)
        .def("__getitem__", &APyUFixed::get_bit_value, nb::arg("index"))
        .def("__getitem__", &APyUFixed::get_hdl_bit_range, nb::arg("index"))
        .def("to_fraction", &APyUFixed::to_fraction)
        .def("as_integer_ratio", &APyUFixed::as_integer_ratio)
        .def_prop_ro("bits", &APyUFixed::bits)
        .def_prop_ro("int_bits", &APyUFixed::int_bits)
        .def_prop_ro("frac_bits", &APyUFixed::frac_bits)
        .def_prop_ro("_is_negative", &APyUFixed::is_negative)
        .def_prop_ro("_is_positive", &APyUFixed::is_positive)
        .def("is_identical", &APyUFixed::is_identical, nb::arg("other"))
        .def_prop_ro("is_zero", &APyUFixed::is_zero)
        .def(
            "cast",
            &APyUFixed::cast,
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("quantization") = nb::none(),
            nb::arg("overflow") = nb::none(),
            nb::arg("bits") = nb::none()
        )
        .def_prop_ro("_vector_size", &APyUFixed::vector_size)
        .def("_repr_latex_", &APyUFixed::latex)
        .def_prop_ro("leading_ones", &APyUFixed::leading_ones)
        .def_prop_ro("leading_zeros", &APyUFixed::leading_zeros)
        .def_prop_ro("leading_fractional_zeros", &APyUFixed::leading_fractional_zeros)
        .def_prop_ro("leading_signs", &APyUFixed::leading_signs)
        .def_prop_ro("trailing_zeros", &APyUFixed::trailing_zeros)
        .def_prop_ro("real", &APyUFixed::python_copy)
        .def_prop_ro("imag", &APyUFixed::get_zero)
        .def("__abs__", &APyUFixed::abs)
        .def("__float__", &APyUFixed::operator double)
        .def("__repr__", &APyUFixed::repr)
        .def("__str__", &APyUFixed::to_string, nb::arg("base") = 10)
        .def("__lshift__", &APyUFixed::operator<<, NB_OP("shift_amnt"))
        .def("__rshift__", &APyUFixed::operator>>, NB_OP("shift_amnt"))
        .def_static(
            "from_float",
            &APyUFixed::from_number,
            nb::arg("value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none()
        )
        .def_static(
            "from_number",
            &APyUFixed::from_number,
            nb::arg("value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none()
        )
        .def_static(
            "from_str",
            &APyUFixed::from_string,
            nb::arg("string_value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("base") = 10,
            nb::arg("bits") = nb::none()
        );
}
