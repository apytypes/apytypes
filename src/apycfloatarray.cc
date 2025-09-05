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
    const nb::typed<nb::iterable, nb::any>& sign_seq,
    const nb::typed<nb::iterable, nb::any>& exp_seq,
    const nb::typed<nb::iterable, nb::any>& man_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
    : APyArray(
          python_iterable_extract_shape<true>(sign_seq, "APyCFloatArray.__init__"),
          /* itemsize= */ 2
      )
    , exp_bits { check_exponent_format(exp_bits, "APyCFloatArray.__init__") }
    , man_bits { check_mantissa_format(man_bits, "APyCFloatArray.__init__") }
    , bias { bias.value_or(ieee_bias(exp_bits)) }
{
    constexpr std::string_view NAME = "APyCFloatArray.__init__";

    const auto& signs_shape = _shape;
    const auto exps_shape = python_iterable_extract_shape<true>(exp_seq, NAME);
    const auto mans_shape = python_iterable_extract_shape<true>(man_seq, NAME);
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

    auto signs = python_iterable_walk<nb::int_, nb::bool_>(sign_seq, NAME);
    auto exps = python_iterable_walk<nb::int_>(exp_seq, NAME);
    auto mans = python_iterable_walk<nb::int_>(man_seq, NAME);

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

//! Create an `APyCFloatArray` tensor object initialized with values from a sequence
//! of complex-like numbers
APyCFloatArray APyCFloatArray::from_numbers(
    const nb::typed<nb::iterable, nb::any>& number_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyCFloatArray.from_complex");
    check_mantissa_format(man_bits, "APyCFloatArray.from_complex");

    if (nb::isinstance<nb::ndarray<>>(number_seq)) {
        // Sequence is NDArray. Initialize using `from_array`
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(number_seq);
        return from_array(ndarray, exp_bits, man_bits, bias);
    }

    APyCFloatArray result(
        python_iterable_extract_shape(number_seq, "APyCFloatArray.from_complex"),
        exp_bits,
        man_bits,
        bias
    );

    const auto py_objs = python_iterable_walk<
        std::complex<double>,
        nb::float_,
        nb::int_,
        APyFixed,
        APyFloat,
        APyCFixed,
        APyCFloat>(number_seq, "APyCFloatArray.from_complex");

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
        return fmt::format("({}, {})", cbegin_it->man, (cbegin_it + 1)->man);
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

nb::ndarray<nb::numpy, std::complex<double>> APyCFloatArray::to_numpy(
    std::optional<nb::object> dtype, std::optional<bool> copy
) const
{
    (void)dtype;

    if (!copy.value_or(true)) {
        throw nb::value_error("APyCFloatArray.to_numpy: copy must be True");
    }

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

APyCFloatArray APyCFloatArray::operator+(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    auto add = ComplexFloatingPointAdder<1, 0, 1>(spec(), rhs.spec(), res.spec(), qntz);
    add(&_data[0], &rhs._data[0], &res._data[0], res._nitems);
    return res;
}

APyCFloatArray APyCFloatArray::operator+() const { return *this; }

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

    // Perform subtraction
    auto sub = FloatingPointSubtractor<>(spec(), rhs.spec(), res.spec(), qntz);
    sub(&_data[0], &rhs._data[0], &res._data[0], 2 * res._nitems);

    return res;
}

APyCFloatArray APyCFloatArray::operator-(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform subtraction
    ComplexFloatingPointSubtractor<1, 0, 1> sub(spec(), rhs.spec(), res.spec(), qntz);
    sub(&_data[0], &rhs._data[0], &res._data[0], res._nitems);
    return res;
}

APyCFloatArray APyCFloatArray::operator-() const
{
    APyCFloatArray res(*this);
    for (auto&& fp : res._data) {
        fp.sign = !fp.sign;
    }
    return res;
}

APyCFloatArray APyCFloatArray::rsub(const APyCFloat& lhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, lhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, lhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), lhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform subtraction
    ComplexFloatingPointSubtractor<0, 1, 1> sub(lhs.spec(), spec(), res.spec(), qntz);
    sub(&lhs._data[0], &_data[0], &res._data[0], _nitems);

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

    // Perform multiplication
    auto mul = ComplexFloatingPointMultiplier<>(spec(), rhs.spec(), res.spec(), qntz);
    mul(&_data[0], &rhs._data[0], &res._data[0], res._nitems);

    return res;
}

APyCFloatArray APyCFloatArray::operator*(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform multiplication
    auto mul
        = ComplexFloatingPointMultiplier<1, 0, 1>(spec(), rhs.spec(), res.spec(), qntz);
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

    // Perform division
    auto div = ComplexFloatingPointDivider<>(spec(), rhs.spec(), res.spec(), qntz);
    div(&_data[0], &rhs._data[0], &res._data[0], res._nitems);

    return res;
}

APyCFloatArray APyCFloatArray::operator/(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform subtraction
    ComplexFloatingPointDivider<1, 0, 1> div(spec(), rhs.spec(), res.spec(), qntz);
    div(&_data[0], &rhs._data[0], &res._data[0], res._nitems);
    return res;
}

APyCFloatArray APyCFloatArray::rdiv(const APyCFloat& lhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, lhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, lhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), lhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyCFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform division
    ComplexFloatingPointDivider<0, 1, 1> div(lhs.spec(), spec(), res.spec(), qntz);
    div(&lhs._data[0], &_data[0], &res._data[0], _nitems);

    return res;
}

/* ********************************************************************************** *
 * *                             Other member functions                             * *
 * ********************************************************************************** */

APyCFloatArray APyCFloatArray::cast(
    std::optional<int> new_exp_bits,
    std::optional<int> new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    const auto actual_exp_bits = new_exp_bits.value_or(exp_bits);
    const auto actual_man_bits = new_man_bits.value_or(man_bits);

    check_exponent_format(actual_exp_bits, "APyCFloatArray.cast");
    check_mantissa_format(actual_man_bits, "APyCFloatArray.cast");

    return _cast(
        actual_exp_bits,
        actual_man_bits,
        new_bias.value_or(APyFloat::ieee_bias(actual_exp_bits)),
        quantization.value_or(get_float_quantization_mode())
    );
}

APyCFloatArray APyCFloatArray::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    QuantizationMode quantization
) const
{
    // If same bit specifiers, return a copy of this.
    if (new_exp_bits == exp_bits && new_man_bits == man_bits && new_bias == bias) {
        return *this;
    }

    APyCFloatArray result(_shape, new_exp_bits, new_man_bits, new_bias);

    const exp_t SRC_MAX_EXP = (1ULL << exp_bits) - 1;
    const exp_t DST_MAX_EXP = (1ULL << new_exp_bits) - 1;
    const int SPEC_MAN_BITS_DELTA = new_man_bits - man_bits;
    const std::int64_t BIAS_DELTA = std::int64_t(bias) - std::int64_t(new_bias);

    // If longer word lengths, use simpler/faster method
    if (new_exp_bits >= exp_bits && new_man_bits >= man_bits) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = array_floating_point_cast_no_quant(
                _data[i],
                spec(),
                SRC_MAX_EXP,
                DST_MAX_EXP,
                SPEC_MAN_BITS_DELTA,
                BIAS_DELTA
            );
        }
        return result;
    }

    const auto quantization_func = get_qntz_func(quantization);
    const man_t SRC_LEADING_ONE = (1ULL << man_bits);
    const man_t DST_LEADING_ONE = (1ULL << new_man_bits);
    const man_t SRC_HIDDEN_ONE = (1ULL << man_bits);

    if (SPEC_MAN_BITS_DELTA >= 0) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = array_floating_point_cast_pos_man_delta(
                _data[i],
                spec(),
                result.spec(),
                quantization,
                quantization_func,
                SRC_MAX_EXP,
                DST_MAX_EXP,
                SRC_LEADING_ONE,
                DST_LEADING_ONE,
                SPEC_MAN_BITS_DELTA,
                SRC_HIDDEN_ONE,
                BIAS_DELTA
            );
        }
    } else {
        const int SPEC_MAN_BITS_DELTA_REV = -SPEC_MAN_BITS_DELTA;
        const man_t FINAL_STICKY = (1ULL << (SPEC_MAN_BITS_DELTA_REV - 1)) - 1;
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = array_floating_point_cast_neg_man_delta(
                _data[i],
                spec(),
                result.spec(),
                quantization,
                quantization_func,
                SRC_MAX_EXP,
                DST_MAX_EXP,
                SRC_LEADING_ONE,
                DST_LEADING_ONE,
                SPEC_MAN_BITS_DELTA_REV,
                SRC_HIDDEN_ONE,
                FINAL_STICKY,
                BIAS_DELTA
            );
        }
    }

    return result;
}

std::variant<APyCFloatArray, APyCFloat>
APyCFloatArray::sum(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointAdder<> add(spec(), spec(), spec(), qntz);

    auto accumulate
        = [&](auto acc_it, auto src_it) { add(&*acc_it, &*src_it, &*acc_it); };

    return array_fold(
        axes,         // axes
        accumulate,   // fold function
        std::nullopt, // initial fold data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyCFloatArray APyCFloatArray::cumsum(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axes = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axes.has_value() && axes >= _ndim) {
        std::string msg = fmt::format(
            "APyCFloatArray.cumsum: axis {} out of range (ndim = {})", *axes, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointAdder<> add(spec(), spec(), spec(), qntz);
    auto accumulate
        = [&](auto acc_it, auto src_it) { add(&*acc_it, &*src_it, &*acc_it); };

    return array_fold_cumulative(
        axes,               // axes
        accumulate,         // fold function
        [](auto, auto) { }, // post-processing function
        std::nullopt,       // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyCFloatArray, APyCFloat>
APyCFloatArray::nansum(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointAdder<> add(spec(), spec(), spec(), qntz);

    auto accumulate = [&](auto acc_it, auto src_it) {
        if (!is_nan(*(src_it + 0), spec()) && !is_nan(*(src_it + 1), spec())) {
            add(&*acc_it, &*src_it, &*acc_it);
        }
    };

    return array_fold(
        axes,         // axes
        accumulate,   // fold function
        std::nullopt, // initial fold data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyCFloatArray APyCFloatArray::nancumsum(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axes = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axes.has_value() && axes >= _ndim) {
        std::string msg = fmt::format(
            "APyCFloatArray.cumsum: axis {} out of range (ndim = {})", *axes, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointAdder<> add(spec(), spec(), spec(), qntz);
    auto accumulate = [&](auto acc_it, auto src_it) {
        if (!is_nan(*(src_it + 0), spec()) && !is_nan(*(src_it + 1), spec())) {
            add(&*acc_it, &*src_it, &*acc_it);
        }
    };

    return array_fold_cumulative(
        axes,               // axes
        accumulate,         // fold function
        [](auto, auto) { }, // post-processing function
        std::nullopt,       // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyCFloatArray, APyCFloat>
APyCFloatArray::prod(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Fold function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointMultiplier<> mul(spec(), spec(), spec(), qntz);
    auto fold_func
        = [&](auto acc_it, auto src_it) { mul(&*acc_it, &*src_it, &*acc_it); };

    APyCFloat init_one = APyCFloat::one(exp_bits, man_bits, bias);
    return array_fold(
        axes,      // axes
        fold_func, // fold function
        init_one,  // initial fold data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyCFloatArray APyCFloatArray::cumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axes = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axes.has_value() && axes >= _ndim) {
        std::string msg = fmt::format(
            "APyCFloatArray.cumsum: axis {} out of range (ndim = {})", *axes, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Fold function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointMultiplier<> mul(spec(), spec(), spec(), qntz);
    auto fold_func
        = [&](auto acc_it, auto src_it) { mul(&*acc_it, &*src_it, &*acc_it); };

    APyCFloat init_one = APyCFloat::one(exp_bits, man_bits, bias);
    return array_fold_cumulative(
        axes,               // axes
        fold_func,          // fold function
        [](auto, auto) { }, // post-processing function
        init_one,           // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyCFloatArray, APyCFloat>
APyCFloatArray::nanprod(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Fold function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointMultiplier<> mul(spec(), spec(), spec(), qntz);
    auto fold_func = [&](auto acc_it, auto src_it) {
        if (!is_nan(*(src_it + 0), spec()) && !is_nan(*(src_it + 1), spec())) {
            mul(&*acc_it, &*src_it, &*acc_it);
        }
    };

    APyCFloat init_one = APyCFloat::one(exp_bits, man_bits, bias);
    return array_fold(
        axes,      // axes
        fold_func, // fold function
        init_one,  // initial fold data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyCFloatArray APyCFloatArray::nancumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axes = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axes.has_value() && axes >= _ndim) {
        std::string msg = fmt::format(
            "APyCFloatArray.cumsum: axis {} out of range (ndim = {})", *axes, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Fold function
    auto&& qntz = get_float_quantization_mode();
    ComplexFloatingPointMultiplier<> mul(spec(), spec(), spec(), qntz);
    auto fold_func = [&](auto acc_it, auto src_it) {
        if (!is_nan(*(src_it + 0), spec()) && !is_nan(*(src_it + 1), spec())) {
            mul(&*acc_it, &*src_it, &*acc_it);
        }
    };

    APyCFloat init_one = APyCFloat::one(exp_bits, man_bits, bias);
    return array_fold_cumulative(
        axes,               // axes
        fold_func,          // fold function
        [](auto, auto) { }, // post-processing function
        init_one,           // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyCFloatArray, APyCFloat>
APyCFloatArray::matmul(const APyCFloatArray& rhs) const
{
    if (ndim() == 1 && rhs.ndim() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out.
            // Perform the checked inner product.
            return checked_inner_product(rhs);
        }
    }
    if (ndim() == 2 && (rhs.ndim() == 2 || rhs.ndim() == 1)) {
        if (_shape[1] == rhs._shape[0]) {
            // Dimensionality for a standard 2D matrix multiplication checks out.
            // Perform the checked 2D matrix
            return checked_2d_matmul(rhs);
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(
        fmt::format(
            "APyCFloatArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
            tuple_string_from_vec(_shape),
            tuple_string_from_vec(rhs._shape)
        )
    );
}

// Evaluate the inner between two vectors. This method assumes that the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
APyCFloat APyCFloatArray::checked_inner_product(const APyCFloatArray& rhs) const
{
    const std::optional<APyFloatAccumulatorOption> mode = get_accumulator_mode_float();
    const QuantizationMode& qntz
        = mode.has_value() ? mode->quantization : get_float_quantization_mode();
    std::uint8_t res_exp_bits;
    std::uint8_t res_man_bits;
    exp_t res_bias;
    if (mode.has_value()) {
        res_exp_bits = mode->exp_bits;
        res_man_bits = mode->man_bits;
        res_bias = mode->bias.has_value() ? *mode->bias : ieee_bias(res_exp_bits);
    } else {
        res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        res_man_bits = std::max(man_bits, rhs.man_bits);
        res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    }

    APyCFloat result(res_exp_bits, res_man_bits, res_bias);

    auto inner_product
        = ComplexFloatingPointInnerProduct(spec(), rhs.spec(), result.spec(), qntz);

    // dst = A x b
    APyFloatData sum[2] = {};
    inner_product(
        &_data[0],     // src1, a: [1 x N]
        &rhs._data[0], // src2, b: [N x 1]
        sum,           // dst
        _shape[0]      // N
    );
    result._data[0] = sum[0];
    result._data[1] = sum[1];
    return result;
}

// Evaluate the matrix product between two 2D matrices. This method assumes that the
// shape of `*this` and `rhs` have been checked to match a 2d matrix multiplication.
APyCFloatArray APyCFloatArray::checked_2d_matmul(const APyCFloatArray& rhs) const
{
    // Resulting parameters
    const std::vector<std::size_t> res_shape = rhs._shape.size() > 1
        ? std::vector<std::size_t> { _shape[0], rhs._shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { _shape[0] };               // rhs is 1-D
    const std::optional<APyFloatAccumulatorOption> mode = get_accumulator_mode_float();
    const QuantizationMode& qntz
        = mode.has_value() ? mode->quantization : get_float_quantization_mode();
    std::uint8_t res_exp_bits;
    std::uint8_t res_man_bits;
    exp_t res_bias;
    if (mode.has_value()) {
        res_exp_bits = mode->exp_bits;
        res_man_bits = mode->man_bits;
        res_bias = mode->bias.has_value() ? *mode->bias : ieee_bias(res_exp_bits);
    } else {
        res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        res_man_bits = std::max(man_bits, rhs.man_bits);
        res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    }

    // Result array
    APyCFloatArray result(res_shape, res_exp_bits, res_man_bits, res_bias);

    // Current column from rhs, read once and cached for efficiency
    const auto res_cols = rhs._shape.size() > 1 ? rhs._shape[1] : 1;
    APyCFloatArray current_column(
        { rhs._shape[0] }, rhs.exp_bits, rhs.man_bits, rhs.bias
    );

    auto inner_product
        = ComplexFloatingPointInnerProduct(spec(), rhs.spec(), result.spec(), qntz);

    for (std::size_t x = 0; x < res_cols; x++) {
        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        for (std::size_t col = 0; col < rhs._shape[0]; col++) {
            current_column._data[2 * col + 0] = rhs._data[2 * (x + col * res_cols) + 0];
            current_column._data[2 * col + 1] = rhs._data[2 * (x + col * res_cols) + 1];
        }

        // dst = A x b
        inner_product(
            &_data[0],                // src1, A: [M x N]
            &current_column._data[0], // src2, b: [N x 1]
            &result._data[2 * x],     // dst
            _shape[1],                // N
            res_shape[0],             // M
            res_cols                  // DST_STEP
        );
    }

    return result;
}

//! Perform a linear convolution with `other` using `mode`
APyCFloatArray
APyCFloatArray::convolve(const APyCFloatArray& rhs, const std::string& conv_mode) const
{
    if (ndim() != 1 || rhs.ndim() != 1) {
        auto msg = fmt::format(
            "can only convolve 1D arrays (lhs.ndim = {}, rhs.ndim = {})",
            ndim(),
            rhs.ndim()
        );
        throw nanobind::value_error(msg.c_str());
    }

    // Find the shorter array of `*this` and `other` based on length.
    bool swap = _shape[0] < rhs._shape[0];

    // Make a reverse copy of the shorter array
    APyCFloatArray b_cpy = swap ? *this : rhs;
    multi_limb_reverse(std::begin(b_cpy._data), std::end(b_cpy._data), 2);

    // Let `a` be a pointer to the longer array, and let `b` be a pointer to the reverse
    // copy of the shorter array.
    const APyCFloatArray* a = swap ? &rhs : this;
    const APyCFloatArray* b = &b_cpy;

    // Extract convolution properties
    auto [len, n_left, n_right] = get_conv_lengths(conv_mode, a, b);

    // Create result vector
    const std::optional<APyFloatAccumulatorOption> mode = get_accumulator_mode_float();
    const QuantizationMode& qntz
        = mode.has_value() ? mode->quantization : get_float_quantization_mode();
    std::uint8_t res_exp_bits;
    std::uint8_t res_man_bits;
    exp_t res_bias;
    if (mode.has_value()) {
        res_exp_bits = mode->exp_bits;
        res_man_bits = mode->man_bits;
        res_bias = mode->bias.has_value() ? *mode->bias : ieee_bias(res_exp_bits);
    } else {
        res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        res_man_bits = std::max(man_bits, rhs.man_bits);
        res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    }
    APyCFloatArray result({ len }, res_exp_bits, res_man_bits, res_bias);

    // Loop working variables
    std::size_t n = b->_shape[0] - n_left;
    auto dst = std::begin(result._data);
    auto src1 = std::cbegin(a->_data);
    auto src2 = std::cbegin(b->_data) + 2 * n_left;

    const APyFloatSpec& lhs_spec = swap ? rhs.spec() : spec();
    const APyFloatSpec& rhs_spec = swap ? spec() : rhs.spec();
    const APyFloatSpec& res_spec = result.spec();
    auto inner_product
        = ComplexFloatingPointInnerProduct(lhs_spec, rhs_spec, res_spec, qntz);

    // `b` limits length of the inner product length
    for (std::size_t i = 0; i < n_left; i++) {
        inner_product(&*src1, &*src2, &*dst, n);
        src2 -= 2;
        dst += 2;
        n++;
    }

    // full inner product length
    for (std::size_t i = 0; i < a->_shape[0] - b->_shape[0] + 1; i++) {
        inner_product(&*src1, &*src2, &*dst, n);
        src1 += 2;
        dst += 2;
    }

    // `a` limits length of the inner product length
    for (std::size_t i = 0; i < n_right; i++) {
        n--;
        inner_product(&*src1, &*src2, &*dst, n);
        src1 += 2;
        dst += 2;
    }

    return result;
}
