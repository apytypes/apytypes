#include "apycfloat.h"
#include "apyfloat_util.h"
#include "ieee754.h"

#include "fmt/core.h"

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyCFloat::APyCFloat(std::uint8_t exp_bits, std::uint8_t man_bits, exp_t bias)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias)
    , real {}
    , imag {}
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
    , real { real_data }
    , imag {}
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
    , real { real_data }
    , imag { imag_data }
{
}

/* ********************************************************************************** *
 * *                      Static conversion from other types                        * *
 * ********************************************************************************** */

APyCFloat APyCFloat::from_number(
    const nb::object& py_obj, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    if (nb::isinstance<nb::int_>(py_obj)) {
        return APyCFloat::from_integer(
            nb::cast<nb::int_>(py_obj), exp_bits, man_bits, bias
        );
    } else if (nb::isinstance<nb::float_>(py_obj)) {
        const double d = static_cast<double>(nb::cast<nb::float_>(py_obj));
        return APyCFloat::from_double(d, exp_bits, man_bits, bias);
    } else if (nb::isinstance<std::complex<double>>(py_obj)) {
        const std::complex<double> c = nb::cast<std::complex<double>>(py_obj);
        return APyCFloat::from_complex(c, exp_bits, man_bits, bias);
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
    const nb::int_ value, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
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
    const double real = value.real();
    const double imag = value.real();
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

/* ********************************************************************************** *
 * *                        Other public member functions                           * *
 * ********************************************************************************** */

std::string APyCFloat::repr() const
{
    return fmt::format(
        "APyCFloat(sign=({}, {}), exp=({}, {}), man=({}, {}), exp_bits={}, "
        "man_bits={}{})",
        real.sign ? '1' : '0',
        imag.sign ? '1' : '0',
        real.exp,
        imag.exp,
        real.man,
        imag.man,
        exp_bits,
        man_bits,
        bias == ieee_bias(exp_bits) ? "" : fmt::format(", bias={}", bias)
    );
}
