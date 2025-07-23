#include "apycfloatarray.h"
#include "apycfixed.h"
#include "apycfloat_util.h"
#include "apyfloat_util.h"
#include "src/apytypes_common.h"
#include "src/apytypes_intrinsics.h"
#include "src/apytypes_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/stl/complex.h> // std::complex<double> (with nanobind support)
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

#include <cmath> // std::isnan, std::signbit
#include <initializer_list>

/* ********************************************************************************** *
 * *                         Non-Python accessible constructors                     * *
 * ********************************************************************************** */

APyCFloatArray::APyCFloatArray(
    const std::vector<std::size_t>& shape,
    exp_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : APyArray(shape, /* itemsize= */ 2)
    , exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias.value_or(ieee_bias(exp_bits)))
{
}

/* ********************************************************************************** *
 * *                             Python constructors                                * *
 * ********************************************************************************** */

APyCFloatArray::APyCFloatArray(
    const nb::typed<nb::sequence, nb::any>& sign_seq,
    const nb::typed<nb::sequence, nb::any>& exp_seq,
    const nb::typed<nb::sequence, nb::any>& man_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
    : APyArray(
          python_sequence_extract_shape<true>(sign_seq, "APyCFloatArray.__init__"),
          /* itemsize= */ 2
      )
    , exp_bits { check_exponent_format(exp_bits, "APyCFloatArray.__init__") }
    , man_bits { check_mantissa_format(man_bits, "APyCFloatArray.__init__") }
    , bias { bias.value_or(ieee_bias(exp_bits)) }
{
    constexpr std::string_view NAME = "APyCFloatArray.__init__";

    const auto& signs_shape = _shape;
    const auto exps_shape = python_sequence_extract_shape<true>(exp_seq, NAME);
    const auto mans_shape = python_sequence_extract_shape<true>(man_seq, NAME);
    if (!((signs_shape == exps_shape) && (signs_shape == mans_shape))) {
        throw std::domain_error(
            fmt::format(
                "{}: shape mismatch, sign: {}, exp: {}, man: {}",
                NAME,
                tuple_string_from_vec(signs_shape),
                tuple_string_from_vec(exps_shape),
                tuple_string_from_vec(mans_shape)
            )
        );
    }

    auto signs = python_sequence_walk<nb::int_, nb::bool_>(sign_seq, NAME);
    auto exps = python_sequence_walk<nb::int_>(exp_seq, NAME);
    auto mans = python_sequence_walk<nb::int_>(man_seq, NAME);

    // If the walked sequence of Python integers is
    assert(signs.size() == exps.size() && signs.size() == mans.size());
    assert(signs.size() == _nitems || signs.size() == 2 * _nitems);
    bool is_inner_dim_complex = (signs.size() == 2 * _nitems);

    std::size_t complex_multiplier = is_inner_dim_complex ? 1 : 2;
    for (std::size_t i = 0; i < signs.size(); ++i) {
        bool sign;
        if (nb::isinstance<nb::bool_>(signs[i])) {
            sign = static_cast<bool>(nb::cast<nb::bool_>(signs[i]));
        } else if (nb::isinstance<nb::int_>(signs[i])) {
            sign = static_cast<int>(nb::cast<nb::int_>(signs[i]));
        } else {
            APYTYPES_UNREACHABLE();
        }
        exp_t exp = static_cast<exp_t>(nb::cast<nb::int_>(exps[i]));
        man_t man = static_cast<man_t>(nb::cast<nb::int_>(mans[i]));

        _data[i * complex_multiplier] = { sign, exp, man };
    }
}

/* ********************************************************************************** *
 * *                            Static array creation                               * *
 * ********************************************************************************** */

APyCFloatArray APyCFloatArray::zeros(
    const PyShapeParam_t& shape,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    std::vector<std::size_t> new_shape = cpp_shape_from_python_shape_like(shape);
    return APyCFloatArray(new_shape, exp_bits, man_bits, bias);
}

APyCFloatArray APyCFloatArray::ones(
    const PyShapeParam_t& shape,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    return full(shape, APyCFloat::one(exp_bits, man_bits, bias));
}

APyCFloatArray APyCFloatArray::eye(
    const nb::int_& N,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<nb::int_> M,
    std::optional<exp_t> bias
)
{
    // Use N for both dimensions if M is not provided
    PyShapeTuple_t shape = PyShapeTuple_t(nb::make_tuple(N, M.value_or(N)));
    return diagonal(shape, APyCFloat::one(exp_bits, man_bits, bias));
}

APyCFloatArray APyCFloatArray::identity(
    const nb::int_& N,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    return eye(N, exp_bits, man_bits, std::nullopt, bias);
}

APyCFloatArray APyCFloatArray::arange(
    const nb::object& start,
    const nb::object& stop,
    const nb::object& step,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyCFloatArray.arange");
    check_mantissa_format(man_bits, "APyCFloatArray.arange");

    const std::vector<APyFixed> apy_vals = ::arange(start, stop, step);
    APyCFloatArray result({ apy_vals.size() }, exp_bits, man_bits, bias);

    for (std::size_t i = 0; i < apy_vals.size(); i++) {
        auto&& fp = APyFloat::from_fixed(apy_vals[i], exp_bits, man_bits, bias);
        result._data[2 * i + 0] = fp.get_data();
        result._data[2 * i + 1] = { 0, 0, 0 };
    }

    return result;
}

//! Create an `APyCFloatArray` tensor object initialized with values from a sequence
//! of complex-like numbers
APyCFloatArray APyCFloatArray::from_numbers(
    const nb::typed<nb::sequence, nb::any>& number_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    if (nb::isinstance<nb::ndarray<>>(number_seq)) {
        // Sequence is NDArray. Initialize using `from_array`
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(number_seq);
        return from_array(ndarray, exp_bits, man_bits, bias);
    }

    APyCFloatArray result(
        python_sequence_extract_shape(number_seq, "APyCFloatArray.from_complex"),
        exp_bits,
        man_bits,
        bias
    );

    const auto py_objs = python_sequence_walk<
        std::complex<double>,
        nb::float_,
        nb::int_,
        APyFixed,
        APyFloat,
        APyCFixed,
        APyCFloat>(number_seq, "APyCFloatArray.from_float");

    for (std::size_t i = 0; i < result._nitems; i++) {
        if (nb::isinstance<std::complex<double>>(py_objs[i])) {
            std::complex<double> cplx = nb::cast<std::complex<double>>(py_objs[i]);
            double re = cplx.real();
            double im = cplx.imag();
            auto&& fp_real = APyFloat::from_double(re, exp_bits, man_bits, result.bias);
            auto&& fp_imag = APyFloat::from_double(im, exp_bits, man_bits, result.bias);
            result._data[2 * i + 0] = fp_real.get_data();
            result._data[2 * i + 1] = fp_imag.get_data();
        } else if (nb::isinstance<nb::float_>(py_objs[i])) {
            double val = static_cast<double>(nb::cast<nb::float_>(py_objs[i]));
            auto&& fp = APyFloat::from_double(val, exp_bits, man_bits, result.bias);
            result._data[2 * i] = fp.get_data();
        } else if (nb::isinstance<nb::int_>(py_objs[i])) {
            auto&& obj = nb::cast<nb::int_>(py_objs[i]);
            auto&& fp = APyFloat::from_integer(obj, exp_bits, man_bits, result.bias);
            result._data[2 * i] = fp.get_data();
        } else if (nb::isinstance<APyFixed>(py_objs[i])) {
            auto&& fx = nb::cast<APyFixed>(py_objs[i]);
            auto&& fp = APyFloat::from_fixed(fx, exp_bits, man_bits, bias);
            result._data[2 * i] = fp.get_data();
        } else if (nb::isinstance<APyFloat>(py_objs[i])) {
            auto&& obj = nb::cast<APyFloat>(py_objs[i]);
            auto&& fp = obj.cast(exp_bits, man_bits, bias, QuantizationMode::RND_CONV);
            result._data[2 * i] = fp.get_data();
        } else if (nb::isinstance<APyCFixed>(py_objs[i])) {
            auto&& cfx = nb::cast<APyCFixed>(py_objs[i]);
            auto&& re = cfx.get_real();
            auto&& im = cfx.get_imag();
            auto&& fp_real = APyFloat::from_fixed(re, exp_bits, man_bits, result.bias);
            auto&& fp_imag = APyFloat::from_fixed(im, exp_bits, man_bits, result.bias);
            result._data[2 * i + 0] = fp_real.get_data();
            result._data[2 * i + 1] = fp_imag.get_data();
        } else if (nb::isinstance<APyCFloat>(py_objs[i])) {
            auto&& obj = nb::cast<APyCFloat>(py_objs[i]);
            auto&& cfp = obj.cast(exp_bits, man_bits, bias, QuantizationMode::RND_CONV);
            result._data[2 * i + 0] = cfp.real();
            result._data[2 * i + 1] = cfp.imag();
        } else {
            APYTYPES_UNREACHABLE();
        }
    }

    return result;
}

APyCFloatArray APyCFloatArray::from_array(
    const nb::ndarray<nb::c_contig>& ndarray,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyCFloatArray.from_array");
    check_mantissa_format(man_bits, "APyCFloatArray.from_array");

    assert(ndarray.ndim() > 0);
    std::vector<std::size_t> shape(ndarray.ndim(), 0);
    for (std::size_t i = 0; i < ndarray.ndim(); i++) {
        shape[i] = ndarray.shape(i);
    }

    APyCFloatArray result(shape, exp_bits, man_bits, bias);
    result._set_values_from_ndarray(ndarray);
    return result;
}

void APyCFloatArray::_set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
    // Scalar casters
    APyFloat re_caster(11, 52, 1023);
    APyFloat im_caster(11, 52, 1023);

#define CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE(__TYPE__)                             \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                double re = static_cast<std::complex<double>>(view.data()[i]).real();  \
                double im = static_cast<std::complex<double>>(view.data()[i]).imag();  \
                re_caster.set_data(                                                    \
                    { sign_of_double(re),                                              \
                      exp_t(exp_of_double(re)),                                        \
                      man_of_double(re) }                                              \
                );                                                                     \
                im_caster.set_data(                                                    \
                    { sign_of_double(im),                                              \
                      exp_t(exp_of_double(im)),                                        \
                      man_of_double(im) }                                              \
                );                                                                     \
                APyFloat re_fp = re_caster.cast_from_double(exp_bits, man_bits, bias); \
                APyFloat im_fp = im_caster.cast_from_double(exp_bits, man_bits, bias); \
                _data[2 * i + 0] = re_fp.get_data();                                   \
                _data[2 * i + 1] = im_fp.get_data();                                   \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

#define CHECK_AND_SET_VALUES_FROM_FLOATING_NPTYPE(__TYPE__)                            \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                double val = static_cast<double>(view.data()[i]);                      \
                re_caster.set_data(                                                    \
                    { sign_of_double(val),                                             \
                      exp_t(exp_of_double(val)),                                       \
                      man_of_double(val) }                                             \
                );                                                                     \
                APyFloat fp = re_caster.cast_from_double(exp_bits, man_bits, bias);    \
                _data[2 * i + 0] = fp.get_data();                                      \
                _data[2 * i + 1] = { 0, 0, 0 };                                        \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

#define CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(__TYPE__)                                 \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                /* TODO: This should probably not cast to `double`... */               \
                double val = static_cast<double>(view.data()[i]);                      \
                re_caster.set_data(                                                    \
                    { sign_of_double(val),                                             \
                      exp_t(exp_of_double(val)),                                       \
                      man_of_double(val) }                                             \
                );                                                                     \
                APyFloat fp = re_caster.cast_from_double(exp_bits, man_bits, bias);    \
                _data[2 * i + 0] = fp.get_data();                                      \
                _data[2 * i + 1] = { 0, 0, 0 };                                        \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

    CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE(std::complex<double>);
    CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE(std::complex<float>);
    CHECK_AND_SET_VALUES_FROM_FLOATING_NPTYPE(double);
    CHECK_AND_SET_VALUES_FROM_FLOATING_NPTYPE(float);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int64_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int32_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int16_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int8_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint64_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint32_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint16_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint8_t);

#undef CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE
#undef CHECK_AND_SET_VALUES_FROM_FLOATING_NPTYPE
#undef CHECK_AND_SET_VALUES_FROM_INT_NPTYPE

    // None of the `CHECK_AND_SET_VALUES_FROM_*_NPTYPE` succeeded. Unsupported type,
    // throw an error. If possible, it would be nice to show a string representation of
    // the `dtype`. Seems hard to achieve with nanobind, but please fix this if you
    // find out how this can be achieved.
    throw nb::type_error(
        "APyCFloatArray.from_array: unsupported `dtype`, expecting complex or float "
        "or integer"
    );
}

/* ********************************************************************************** *
 * *                            Public member functions                             * *
 * ********************************************************************************** */

std::string APyCFloatArray::repr() const
{
    const auto sign_fmt = [](auto cbegin_it, auto) -> std::string {
        int real_sign = int(cbegin_it->sign);
        int imag_sign = int((cbegin_it + 1)->sign);
        return fmt::format("({}, {})", real_sign, imag_sign);
    };
    const auto exp_fmt = [](auto cbegin_it, auto) -> std::string {
        return fmt::format("({}, {})", cbegin_it->exp, (cbegin_it + 1)->exp);
    };
    const auto man_fmt = [](auto cbegin_it, auto) -> std::string {
        return fmt::format("({}, {})", cbegin_it->man, (cbegin_it + 1)->exp);
    };

    std::initializer_list<formatter_t> formatters { sign_fmt, exp_fmt, man_fmt };
    std::vector<std::string> kw_args { fmt::format("exp_bits={}", exp_bits),
                                       fmt::format("man_bits={}", man_bits) };
    if (bias != ieee_bias(exp_bits)) {
        kw_args.push_back(fmt::format("bias={}", bias));
    }

    return array_repr(formatters, kw_args);
}

std::string APyCFloatArray::to_string_dec() const
{
    const auto formatter = [spec = spec()](auto cbegin_it, auto) -> std::string {
        const APyFloatData& re = *(cbegin_it + 0);
        const APyFloatData& im = *(cbegin_it + 1);
        return complex_floating_point_to_str_dec(re, im, spec);
    };

    return array_format(formatter, 88, false);
}

std::string APyCFloatArray::to_string(int base) const
{
    switch (base) {
    case 10:
        return to_string_dec();
    default:
        auto msg
            = fmt::format("APyCFloatArray.__str__: base={} is not supported", base);
        throw nb::value_error(msg.c_str());
    }
}

nb::ndarray<nb::numpy, std::complex<double>> APyCFloatArray::to_numpy() const
{
    // Dynamically allocate data to be passed to Python
    std::complex<double>* result_data = new std::complex<double>[_nitems];
    auto fp_re = APyFloat(exp_bits, man_bits, bias);
    auto fp_im = APyFloat(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < _nitems; i++) {
        fp_re.set_data(_data[2 * i + 0]);
        fp_im.set_data(_data[2 * i + 1]);
        result_data[i] = std::complex<double> { fp_re.to_double(), fp_im.to_double() };
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept {
        delete[] (std::complex<double>*)p;
    });

    return nb::ndarray<nb::numpy, std::complex<double>>(
        result_data, ndim(), &_shape[0], owner
    );
}

bool APyCFloatArray::is_identical(const nb::object& other, bool ignore_zero_sign) const
{
    if (!nb::isinstance<APyCFloatArray>(other)) {
        return false;
    } else {
        auto&& other_arr = nb::cast<APyCFloatArray>(other);
        if (!is_same_spec(other_arr) || _shape != other_arr._shape) {
            return false;
        }

        for (std::size_t i = 0; i < _data.size(); i++) {
            // If `ignore_zero_sign` is set, ignore the sign bit if results equals zero
            if (ignore_zero_sign && is_zero(_data[i]) && is_zero(other_arr._data[i])) {
                continue;
            }

            if (_data[i] != other_arr._data[i]) {
                return false;
            }
        }
    }

    return true;
}

/* ********************************************************************************** *
 * *                          Arithmetic member functions                           * *
 * ********************************************************************************** */

APyCFloatArray APyCFloatArray::operator+(const APyCFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::plus<>>(rhs, "__add__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    auto add = FloatingPointAdder<>(spec(), rhs.spec(), res.spec(), qntz);
    add(&_data[0], &rhs._data[0], &res._data[0], 2 * res._nitems);

    return res;
}

APyCFloatArray APyCFloatArray::operator-(const APyCFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::minus<>>(rhs, "__sub__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    auto sub = FloatingPointSubtractor<>(spec(), rhs.spec(), res.spec(), qntz);
    sub(&_data[0], &rhs._data[0], &res._data[0], 2 * res._nitems);

    return res;
}

APyCFloatArray APyCFloatArray::operator*(const APyCFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::multiplies<>>(rhs, "__mul__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    auto mul = ComplexFloatingPointMultiplier<>(spec(), rhs.spec(), res.spec(), qntz);
    mul(&_data[0], &rhs._data[0], &res._data[0], res._nitems);

    return res;
}

APyCFloatArray APyCFloatArray::operator/(const APyCFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::divides<>>(rhs, "__truediv__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    auto div = ComplexFloatingPointDivider<>(spec(), rhs.spec(), res.spec(), qntz);
    div(&_data[0], &rhs._data[0], &res._data[0], res._nitems);

    return res;
}
