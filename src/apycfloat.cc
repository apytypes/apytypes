#include "apycfloat.h"
#include "apyfixed.h"
#include "apyfloat_util.h"

APyCFloat APyCFloat::from_number(
    const nb::object& py_obj, int exp_bits, int man_bits, std::optional<exp_t> bias
)
{
    return APyCFloat();
    // check_exponent_format(exp_bits);
    // check_mantissa_format(man_bits);

    // if (nb::isinstance<nb::int_>(py_obj)) {
    //     return APyCFloat::from_integer(
    //         nb::cast<nb::int_>(py_obj), exp_bits, man_bits, bias
    //     );
    // } else if (nb::isinstance<nb::float_>(py_obj)) {
    //     const auto d = static_cast<double>(nb::cast<nb::float_>(py_obj));
    //     return APyCFloat::from_double(d, exp_bits, man_bits, bias);
    // } else {
    //     const nb::type_object type = nb::cast<nb::type_object>(py_obj.type());
    //     const nb::str type_string = nb::str(type);
    //     throw std::domain_error(
    //         std::string("Non supported type: ") + type_string.c_str()
    //     );
    // }
}

// APyCFloat APyCFloat::from_double(
//     double value, int exp_bits, int man_bits, std::optional<exp_t> bias
// )
// {
//     APyCFloat apytypes_double(
//         sign_of_double(value), exp_of_double(value), man_of_double(value), 11, 52,
//         1023
//     );
//     return apytypes_double.cast_from_double(
//         exp_bits, man_bits, bias.value_or(APyCFloat::ieee_bias(exp_bits))
//     );
// }
//
// APyCFloat APyCFloat::from_integer(
//     const nb::int_ value, int exp_bits, int man_bits, std::optional<exp_t> bias
// )
// {
//     APyFixed apyfixed = APyFixed::from_unspecified_integer(value);
//     // Custom version of 'from_fixed' since we know certain properties
//
//     const exp_t actual_bias = bias.value_or(APyCFloat::ieee_bias(exp_bits));
//
//     if (apyfixed.is_zero()) {
//         return APyCFloat(0, 0, 0, exp_bits, man_bits, actual_bias);
//     }
//
//     APyCFloat res(exp_bits, man_bits, actual_bias);
//
//     // Get sign
//     if (apyfixed.is_negative()) {
//         res.sign = true;
//         apyfixed = apyfixed.abs();
//     } else {
//         res.sign = false;
//     }
//
//     // Calculate exponent
//     const exp_t target_exp
//         = apyfixed.bits() - apyfixed.leading_zeros() - 1; // The bit width minus 1
//     std::uint64_t tmp_exp = target_exp + res.bias;
//
//     // Calculate mantissa
//     apyfixed >>= target_exp;
//     apyfixed = apyfixed.cast(3, man_bits, QuantizationMode::RND_CONV);
//
//     // Check for overflow
//     if (apyfixed.positive_greater_than_equal_pow2(1)) {
//         ++tmp_exp;
//         apyfixed >>= 1;
//     }
//
//     // Check for overflow
//     if (tmp_exp >= res.max_exponent()) {
//         res.exp = res.max_exponent();
//         res.man = 0;
//     } else {
//         res.exp = tmp_exp;
//         // Remove leading one
//         apyfixed = apyfixed - fx_one;
//         res.man = apyfixed.get_lsbs();
//     }
//
//     return res;
// }
