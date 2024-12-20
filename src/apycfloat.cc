#include "apycfloat.h"
#include "apycfixed.h"
#include "apycfloat_util.h"
#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apytypes_common.h"
#include "ieee754.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/variant.h>

#include <fmt/format.h>
#include <variant>

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyCFloat::APyCFloat(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , _data {}
{
}

APyCFloat::APyCFloat(
    const APyFloatData& real_data,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    exp_t bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , _data { real_data }
{
}

APyCFloat::APyCFloat(
    const APyFloatData& real_data,
    const APyFloatData& imag_data,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    exp_t bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , _data { real_data, imag_data }
{
}

/* ********************************************************************************** *
 * *                       Python accessible constructions                          * *
 * ********************************************************************************** */

APyCFloat::APyCFloat(
    const nb::int_& exp_bits,
    const nb::int_& man_bits,
    const std::optional<nb::int_>& bias
)
    : exp_bits(check_exponent_format(static_cast<int>(exp_bits), "APyCFloat.__init__"))
    , man_bits(check_mantissa_format(static_cast<int>(man_bits), "APyCFloat.__init__"))
    , bias(bias.has_value() ? static_cast<int>(*bias) : ieee_bias(APyCFloat::exp_bits))
    , _data {}
{
}

APyCFloat::APyCFloat(
    const std::variant<nb::bool_, nb::int_>& sign,
    const nb::int_& exp,
    const nb::int_& man,
    const nb::int_& exp_bits,
    const nb::int_& man_bits,
    const std::optional<nb::int_>& bias
)
    : APyCFloat(exp_bits, man_bits, bias)
{
    exp_t real_exp = static_cast<exp_t>(exp);
    man_t real_man = static_cast<man_t>(man);
    bool real_sign;
    if (std::holds_alternative<nb::bool_>(sign)) {
        real_sign = nb::cast<bool>(std::get<nb::bool_>(sign));
    } else { /* std::holds_alternative<nb::int_>(sign) */
        real_sign = nb::cast<int>(std::get<nb::int_>(sign));
    }
    real() = { real_sign, real_exp, real_man };
}

APyCFloat::APyCFloat(
    const nb::tuple& sign_tuple,
    const nb::tuple& exp_tuple,
    const nb::tuple& man_tuple,
    const nb::int_& exp_bits,
    const nb::int_& man_bits,
    const std::optional<nb::int_>& bias
)
    : APyCFloat(exp_bits, man_bits, bias)
{
    // Sanitize input: all tuples have equally many items
    if (sign_tuple.size() != exp_tuple.size()
        || sign_tuple.size() != man_tuple.size()) {
        throw nb::value_error(
            "APyCFloat.__init__: different sized initialization tuples"
        );
    }
    std::size_t n_tuple_elements = sign_tuple.size();

    // Sanitize input: at least a single element in all tuples
    if (n_tuple_elements == 0) {
        throw nb::value_error(
            "APyCFloat.__init__: less than one element in initialization tuples"
        );
    }

    // Sanitize input: no more than two values in all tuples
    if (n_tuple_elements > 2) {
        throw nb::value_error(
            "APyCFloat.__init__: more than two elements in initializetion tuples"
        );
    }

    // Sign extraction helper (extract from `nb::int_` or `nb::bool_`)
    auto GET_SIGN = [](auto&& obj) -> bool {
        nb::bool_ b_sign;
        nb::int_ i_sign;
        if (nb::try_cast<nb::bool_>(obj, b_sign)) {
            return bool(b_sign);
        } else if (nb::try_cast<nb::int_>(obj, i_sign)) {
            return bool(int(i_sign));
        } else {
            throw nb::value_error("APyCFloat.__init__: sign is non-bool/non-integer");
        }
    };

    bool re_sign, im_sign;
    nb::int_ re_exp, re_man, im_exp, im_man;

    re_sign = GET_SIGN(sign_tuple[0]);
    if (!nb::try_cast<nb::int_>(exp_tuple[0], re_exp)) {
        throw nb::value_error("APyCFloat.__init__: exponent is non-integer");
    }
    if (!nb::try_cast<nb::int_>(man_tuple[0], re_man)) {
        throw nb::value_error("APyCFloat.__init__: mantissa is non-integer");
    }
    real() = { re_sign, static_cast<exp_t>(re_exp), static_cast<man_t>(re_man) };

    if (n_tuple_elements == 2) {
        // Two elements in all tuples, also initialize the imaginary part
        im_sign = GET_SIGN(sign_tuple[1]);
        if (!nb::try_cast<nb::int_>(exp_tuple[1], im_exp)) {
            throw nb::value_error("APyCFloat.__init__: exponent is non-integer");
        }
        if (!nb::try_cast<nb::int_>(man_tuple[1], im_man)) {
            throw nb::value_error("APyCFloat.__init__: mantissa is non-integer");
        }
        imag() = { im_sign, static_cast<exp_t>(im_exp), static_cast<man_t>(im_man) };
    }
}

/* ********************************************************************************** *
 * *                      Static conversion from other types                        * *
 * ********************************************************************************** */

APyCFloat APyCFloat::from_number(
    const nb::object& py_obj, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    if (nb::isinstance<nb::int_>(py_obj)) {
        const nb::int_& py_int = nb::cast<nb::int_>(py_obj);
        return APyCFloat::from_integer(py_int, exp_bits, man_bits, bias);
    } else if (nb::isinstance<nb::float_>(py_obj)) {
        double d = static_cast<double>(nb::cast<nb::float_>(py_obj));
        return APyCFloat::from_double(d, exp_bits, man_bits, bias);
    } else if (nb::isinstance<std::complex<double>>(py_obj)) {
        std::complex<double> c = nb::cast<std::complex<double>>(py_obj);
        return APyCFloat::from_complex(c, exp_bits, man_bits, bias);
    } else if (nb::isinstance<APyFloat>(py_obj)) {
        exp_t res_bias = bias.value_or(ieee_bias(exp_bits));
        APyFloatSpec res_spec { std::uint8_t(exp_bits),
                                std::uint8_t(man_bits),
                                res_bias };
        APyFloat c = nb::cast<APyFloat>(py_obj);
        if (res_spec != c.spec()) {
            c = c.cast(exp_bits, man_bits, res_bias, QuantizationMode::RND_CONV);
        }
        return APyCFloat(c.get_data(), exp_bits, man_bits, res_bias);
    } else {
        const nb::type_object type = nb::cast<nb::type_object>(py_obj.type());
        const nb::str type_string = nb::str(type);
        throw std::domain_error(
            std::string("Non supported type: ") + type_string.c_str()
        );
    }
}

APyCFloat APyCFloat::from_double(
    double value, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyCFloat.from_float");
    check_mantissa_format(man_bits, "APyCFloat.from_float");

    APyFloat real(
        sign_of_double(value), exp_of_double(value), man_of_double(value), 11, 52, 1023
    );

    const exp_t res_bias = bias.value_or(ieee_bias(exp_bits));
    return APyCFloat(
        real.cast_from_double(exp_bits, man_bits, res_bias).get_data(),
        exp_bits,
        man_bits,
        res_bias
    );
}

APyCFloat APyCFloat::from_integer(
    const nb::int_& value, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyCFloat.from_integer");
    check_mantissa_format(man_bits, "APyCFloat.from_integer");

    const exp_t res_bias = bias.value_or(ieee_bias(exp_bits));
    return APyCFloat(
        APyFloat::from_integer(value, exp_bits, man_bits, res_bias).get_data(),
        exp_bits,
        man_bits,
        res_bias
    );
}

APyCFloat APyCFloat::from_complex(
    std::complex<double> value, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyCFloat.from_complex");
    check_mantissa_format(man_bits, "APyCFloat.from_complex");

    const double real = value.real();
    const double imag = value.imag();
    const APyFloat apy_real(
        sign_of_double(real), exp_of_double(real), man_of_double(real), 11, 52, 1023
    );
    const APyFloat apy_imag(
        sign_of_double(imag), exp_of_double(imag), man_of_double(imag), 11, 52, 1023
    );

    const exp_t res_bias = bias.value_or(ieee_bias(exp_bits));
    return APyCFloat(
        apy_real.cast_from_double(exp_bits, man_bits, res_bias).get_data(),
        apy_imag.cast_from_double(exp_bits, man_bits, res_bias).get_data(),
        exp_bits,
        man_bits,
        res_bias
    );
}

APyCFloat APyCFloat::from_fixed(
    const APyCFixed& fixed, int exp_bits, int man_bits, std::optional<exp_t> opt_bias
)
{
    check_exponent_format(exp_bits, "APyCFloat.from_fixed");
    check_mantissa_format(man_bits, "APyCFloat.from_fixed");

    const exp_t bias = opt_bias.value_or(ieee_bias(exp_bits));
    APyFloatData re = floating_point_from_fixed_point(
        fixed.real_cbegin(), // src_cbegin
        fixed.real_cend(),   // src_cend
        fixed.bits(),        // bits
        fixed.int_bits(),    // int_bits
        exp_bits,            // exp_bits
        man_bits,            // man_bits
        bias                 // bias
    );
    APyFloatData im = floating_point_from_fixed_point(
        fixed.imag_cbegin(), // src_cbegin
        fixed.imag_cend(),   // src_cend
        fixed.bits(),        // bits
        fixed.int_bits(),    // int_bits
        exp_bits,            // exp_bits
        man_bits,            // man_bits
        bias                 // bias
    );

    return APyCFloat(re, im, exp_bits, man_bits, bias);
}

APyCFloat APyCFloat::from_fixed(
    const APyFixed& fixed, int exp_bits, int man_bits, std::optional<exp_t> opt_bias
)
{
    check_exponent_format(exp_bits, "APyCFloat.from_fixed");
    check_mantissa_format(man_bits, "APyCFloat.from_fixed");

    const exp_t bias = opt_bias.value_or(ieee_bias(exp_bits));
    APyFloatData re = floating_point_from_fixed_point(
        std::begin(fixed._data), // src_cbegin_it
        std::end(fixed._data),   // src_cend_it
        fixed.bits(),            // bits
        fixed.int_bits(),        // int_bits
        exp_bits,                // exp_bits
        man_bits,                // man_bits
        bias                     // bias
    );

    return APyCFloat(re, exp_bits, man_bits, bias);
}

/* ********************************************************************************** *
 * *                        Binary comparison operators                             * *
 * ********************************************************************************** */

bool APyCFloat::operator==(const APyCFloat& rhs) const
{
    return APyFloat(real(), exp_bits, man_bits, bias)
        == APyFloat(rhs.real(), rhs.exp_bits, rhs.man_bits, rhs.bias)
        && APyFloat(imag(), exp_bits, man_bits, bias)
        == APyFloat(rhs.imag(), rhs.exp_bits, rhs.man_bits, rhs.bias);
}

bool APyCFloat::operator!=(const APyCFloat& rhs) const { return !(*this == rhs); }

bool APyCFloat::operator==(const APyFloat& rhs) const
{
    return ::is_zero(imag()) && APyFloat(real(), exp_bits, man_bits, bias) == rhs;
}

bool APyCFloat::operator!=(const APyFloat& rhs) const { return !(*this == rhs); }

bool APyCFloat::operator==(double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this == rhs_fp);
}

bool APyCFloat::operator!=(double rhs) const { return !(*this == rhs); }

bool APyCFloat::operator==(const APyCFixed& rhs) const
{
    if (is_max_exponent(real(), spec()) || is_max_exponent(imag(), spec())) {
        return false;
    }

    return get_real().to_fixed() == rhs.get_real()
        && get_imag().to_fixed() == rhs.get_imag();
}

bool APyCFloat::operator!=(const APyCFixed& rhs) const { return !(*this == rhs); }

bool APyCFloat::operator==(const APyFixed& rhs) const
{
    if (is_max_exponent(real(), spec())) {
        return false;
    }

    return ::is_zero(imag()) && (get_real().to_fixed() == rhs);
}

bool APyCFloat::operator!=(const APyFixed& rhs) const { return !(*this == rhs); }

/* ********************************************************************************** *
 * *                        Binary arithmetic operators                             * *
 * ********************************************************************************** */

APyCFloat APyCFloat::operator+(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyCFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    // Perform the addition
    FloatingPointAdder<> add(spec(), rhs.spec(), res.spec(), qntz);
    add(_data, rhs._data, res._data, /* nitems = */ 2);

    return res;
}

APyCFloat APyCFloat::operator-(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyCFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    // Perform the subtraction
    FloatingPointSubtractor<> sub(spec(), rhs.spec(), res.spec(), qntz);
    sub(_data, rhs._data, res._data, /* nitems = */ 2);

    return res;
}

APyCFloat APyCFloat::operator*(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyCFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    // Perform the product
    FloatingPointComplexMultiplier<> complex_mul(spec(), rhs.spec(), res.spec(), qntz);
    complex_mul(_data, rhs._data, res._data);

    return res;
}

APyCFloat APyCFloat::operator/(const APyCFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyCFloat res(res_exp_bits, res_man_bits, res_bias);
    QuantizationMode qntz = get_float_quantization_mode();

    // Perform the division
    FloatingPointComplexDivider<> complex_div(spec(), rhs.spec(), res.spec(), qntz);
    complex_div(_data, rhs._data, res._data);

    return res;
}

APyCFloat APyCFloat::operator-() const
{
    APyFloatData real_data = real();
    APyFloatData imag_data = imag();
    real_data.sign ^= 1;
    imag_data.sign ^= 1;
    return APyCFloat(real_data, imag_data, exp_bits, man_bits, bias);
}

/* ********************************************************************************** *
 * *                        Other public member functions                           * *
 * ********************************************************************************** */

APyCFloat APyCFloat::cast(
    std::optional<int> new_exp_bits,
    std::optional<int> new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    const auto actual_exp_bits = new_exp_bits.value_or(exp_bits);
    const auto actual_man_bits = new_man_bits.value_or(man_bits);

    check_exponent_format(actual_exp_bits, "APyCFloat.cast");
    check_mantissa_format(actual_man_bits, "APyCFloat.cast");

    return checked_cast(
        actual_exp_bits,
        actual_man_bits,
        new_bias.value_or(ieee_bias(actual_exp_bits)),
        quantization.value_or(get_float_quantization_mode())
    );
}

APyCFloat APyCFloat::checked_cast(
    std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias, QuantizationMode qntz
) const
{
    const auto qntz_func = get_qntz_func(qntz);
    APyCFloat res(exp_bits, man_bits, bias);
    res.real() = floating_point_cast(real(), spec(), res.spec(), qntz, qntz_func);
    res.imag() = floating_point_cast(imag(), spec(), res.spec(), qntz, qntz_func);
    return res;
}

std::string APyCFloat::repr() const
{
    return fmt::format(
        "APyCFloat(sign=({}, {}), exp=({}, {}), man=({}, {}), exp_bits={}, "
        "man_bits={}{})",
        real().sign ? '1' : '0',
        imag().sign ? '1' : '0',
        real().exp,
        imag().exp,
        real().man,
        imag().man,
        exp_bits,
        man_bits,
        bias == ieee_bias(exp_bits) ? "" : fmt::format(", bias={}", bias)
    );
}

std::complex<double> APyCFloat::to_complex() const
{
    return std::complex<double>(
        APyFloat(real(), exp_bits, man_bits, bias).to_double(),
        APyFloat(imag(), exp_bits, man_bits, bias).to_double()
    );
}

std::string APyCFloat::to_string(int base) const
{
    switch (base) {
    case 8:
        return to_string_oct();
        break;
    case 16:
        return to_string_hex();
        break;
    case 10:
        return to_string_dec();
        break;
    default:
        throw nb::value_error(
            fmt::format("APyCFloat::to_string(base={}): base is not supported", base)
                .c_str()
        );
        break;
    }
}

std::string APyCFloat::to_string_dec() const
{
    // NOTE:
    // Python, unlike C++, unconditionally encodes the string of a floating-point NaN
    // without a minus sign.
    const std::complex<double> cplx = to_complex();
    const auto& re_str
        = is_nan(real(), spec()) ? "nan" : fmt::format("{}", cplx.real());

    if (is_nan(imag(), spec())) {
        return fmt::format("({}+nanj)", re_str);
    } else {
        return fmt::format("({}{}{}j)", re_str, imag().sign ? "" : "+", cplx.imag());
    }
}

std::string APyCFloat::to_string_hex() const
{
    throw NotImplementedException("APyCFloat::to_string_hex()");
}

std::string APyCFloat::to_string_oct() const
{
    throw NotImplementedException("APyCFloat::to_string_oct()");
}

bool APyCFloat::is_identical(const APyCFloat& other, bool ignore_zero_sign) const
{
    if (ignore_zero_sign) {
        if (is_zero() && other.is_zero()) {
            return spec() == other.spec();
        } else if (::is_zero(real()) && ::is_zero(other.real())) {
            return spec() == other.spec() && imag() == other.imag();
        } else if (::is_zero(imag()) && ::is_zero(other.imag())) {
            return spec() == other.spec() && real() == other.real();
        }
    }

    return spec() == other.spec() && real() == other.real() && imag() == other.imag();
}

APyFloat APyCFloat::get_real() const
{
    APyFloat result(exp_bits, man_bits, bias);
    result.set_data(real());
    return result;
}

APyFloat APyCFloat::get_imag() const
{
    APyFloat result(exp_bits, man_bits, bias);
    result.set_data(imag());
    return result;
}
