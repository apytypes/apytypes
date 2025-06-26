/*
 * APyCFixed: Dynamic arbitrary complex-valued fixed-point data type.
 */

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

#include "apycfixed.h"
#include "apycfixed_util.h"
#include "apycfixedarray.h"
#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfloat.h"
#include "apytypes_mp.h"
#include "apytypes_util.h"
#include "python_util.h"

// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/stl/complex.h>
#include <variant>
namespace nb = nanobind;

// Standard header includes
#include <algorithm> // std::copy, std::max, std::transform, etc...
#include <cassert>   // assert()
#include <cmath>     // std::isinf, std::isnan
#include <cstring>   // std::memcpy
#include <iterator>  // std::back_inserter
#include <optional>  // std::optional
#include <string>    // std::string
#include <vector>    // std::vector, std::swap

#include <fmt/format.h>

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

APyCFixed::APyCFixed(
    const nb::int_& python_long_int_bit_pattern,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyCFixed(int_bits, frac_bits, bits)
{
    auto real = python_long_to_limb_vec(python_long_int_bit_pattern, _data.size() / 2);
    std::copy(std::begin(real), std::end(real), real_begin());
    _overflow_twos_complement(real_begin(), real_end(), _bits, _int_bits);
}

APyCFixed::APyCFixed(
    const nb::tuple& python_long_int_bit_pattern,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyCFixed(int_bits, frac_bits, bits)
{
    if (python_long_int_bit_pattern.size() == 0) {
        throw nb::value_error(
            "APyCFixed.__init__: tuple initializer with zero elements"
        );
    }
    if (python_long_int_bit_pattern.size() == 1) {
        // Single element, real part
        nb::int_ real_int;
        if (nb::try_cast<nb::int_>(python_long_int_bit_pattern[0], real_int)) {
            auto real = python_long_to_limb_vec(real_int, _data.size() / 2);
            std::copy(std::begin(real), std::end(real), real_begin());
            _overflow_twos_complement(real_begin(), real_end(), _bits, _int_bits);
        } else {
            throw nb::value_error(
                "APyCFixed.__init__: tuple initializer with non-integer element"
            );
        }
        return;
    } else if (python_long_int_bit_pattern.size() == 2) {
        // Two elements, real followed by imaginary part
        nb::int_ real_int, imag_int;
        if (nb::try_cast<nb::int_>(python_long_int_bit_pattern[0], real_int)) {
            auto real = python_long_to_limb_vec(real_int, _data.size() / 2);
            std::copy(std::begin(real), std::end(real), real_begin());
            _overflow_twos_complement(real_begin(), real_end(), _bits, _int_bits);
        } else {
            throw nb::value_error(
                "APyCFixed.__init__: tuple initializer with non-integer element"
            );
        }
        if (nb::try_cast<nb::int_>(python_long_int_bit_pattern[1], imag_int)) {
            auto imag = python_long_to_limb_vec(imag_int, _data.size() / 2);
            std::copy(std::begin(imag), std::end(imag), imag_begin());
            _overflow_twos_complement(imag_begin(), imag_end(), _bits, _int_bits);
        } else {
            throw nb::value_error(
                "APyCFixed.__init__: tuple initializer with non-integer element"
            );
        }
        return;
    } else {
        throw nb::value_error(
            "APyCFixed.__init__: tuple initializer with more than two elements"
        );
    }
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyCFixed::APyCFixed(
    std::optional<int> int_bits, std::optional<int> frac_bits, std::optional<int> bits
)
    : _bits { bits_from_optional(bits, int_bits, frac_bits) }
    , _int_bits { int_bits.has_value() ? *int_bits : *bits - *frac_bits }
    , _data(2 * bits_to_limbs(_bits), 0)
{
}

APyCFixed::APyCFixed(int bits, int int_bits)
    : _bits { bits }
    , _int_bits { int_bits }
    , _data(2 * bits_to_limbs(_bits), 0)
{
}

template <typename _IT>
APyCFixed::APyCFixed(int bits, int int_bits, _IT begin, _IT end)
    : APyCFixed(bits, int_bits)
{
    assert(std::distance(begin, end) >= 0);
    assert(std::distance(begin, end) <= 2 * ptrdiff_t(bits_to_limbs(bits)));

    // Copy data into resulting vector
    std::copy(begin, end, _data.begin());

    // Two's-complements overflow bits outside of the range
    _overflow_twos_complement(real_begin(), real_end(), _bits, _int_bits);
    _overflow_twos_complement(imag_begin(), imag_end(), _bits, _int_bits);
}

APyCFixed::APyCFixed(int bits, int int_bits, std::initializer_list<apy_limb_t> list)
    : APyCFixed(bits, int_bits, std::begin(list), std::end(list))
{
}

/* ********************************************************************************** *
 * *                            Arithmetic member functions                         * *
 * ********************************************************************************** */

template <class base_op, class ripple_carry_op>
inline APyCFixed APyCFixed::_apycfixed_base_add_sub(const APyCFixed& rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    APyCFixed result(res_bits, res_int_bits);
    auto lhs_shift = unsigned(res_frac_bits - frac_bits());
    auto rhs_shift = unsigned(res_frac_bits - rhs.frac_bits());

    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        // Result bits fits in a single limb. Use native operation
        result._data[0] = base_op {}( // real
            _data[0] << lhs_shift,
            rhs._data[0] << rhs_shift
        );
        result._data[1] = base_op {}( // imag
            _data[1] << lhs_shift,
            rhs._data[1] << rhs_shift
        );
    } else {
        // Resulting number of bits is more than one limb. Use ripple-carry operation
        APyCFixed op(res_bits, res_int_bits);
        _cast_no_quantize_no_overflow(
            real_begin(), real_end(), result.real_begin(), result.real_end(), lhs_shift
        );
        _cast_no_quantize_no_overflow(
            imag_begin(), imag_end(), result.imag_begin(), result.imag_end(), lhs_shift
        );
        _cast_no_quantize_no_overflow(
            rhs.real_begin(), rhs.real_end(), op.real_begin(), op.real_end(), rhs_shift
        );
        _cast_no_quantize_no_overflow(
            rhs.imag_begin(), rhs.imag_end(), op.imag_begin(), op.imag_end(), rhs_shift
        );
        ripple_carry_op {}(
            &*result.real_begin(),  // dst
            &*result.real_begin(),  // src1
            &*op.real_begin(),      // src2
            result._data.size() / 2 // limb_vector_length
        );
        ripple_carry_op {}(
            &*result.imag_begin(),  // dst
            &*result.imag_begin(),  // src1
            &*op.imag_begin(),      // src2
            result._data.size() / 2 // limb_vector_length
        );
    }

    return result;
}

APyCFixed APyCFixed::operator+(const APyCFixed& rhs) const
{
    return _apycfixed_base_add_sub<std::plus<>, apy_add_n_functor<>>(rhs);
}

APyCFixed APyCFixed::operator-(const APyCFixed& rhs) const
{
    return _apycfixed_base_add_sub<std::minus<>, apy_sub_n_functor<>>(rhs);
}

APyCFixed APyCFixed::operator*(const APyCFixed& rhs) const
{
    const int res_int_bits = 1 + int_bits() + rhs.int_bits();
    const int res_bits = 1 + bits() + rhs.bits();

    // Result fixed-point number
    APyCFixed result(res_bits, res_int_bits);

    // Single-limb result specialization
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        // TODO: Rewrite using SIMD MulComplex
        result._data[0] = apy_limb_signed_t(_data[0]) * apy_limb_signed_t(rhs._data[0])
            - apy_limb_signed_t(_data[1]) * apy_limb_signed_t(rhs._data[1]); // real
        result._data[1] = apy_limb_signed_t(_data[1]) * apy_limb_signed_t(rhs._data[0])
            + apy_limb_signed_t(_data[0]) * apy_limb_signed_t(rhs._data[1]); // imag

        return result; // early exit
    }
    // Double limb result specialization
    if (unsigned(res_bits) <= 2 * APY_LIMB_SIZE_BITS) {
        if (unsigned(bits()) <= APY_LIMB_SIZE_BITS
            && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
            complex_multiplication_1_1_2(&result._data[0], &_data[0], &rhs._data[0]);
            return result;
        } else {
#if (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
            __int128 re0, im0, re1, im1;
            if (unsigned(bits()) > APY_LIMB_SIZE_BITS) {
                re0 = (__int128)_data[0]
                    | ((__int128)apy_limb_signed_t(_data[1]) << APY_LIMB_SIZE_BITS);
                im0 = (__int128)_data[2]
                    | ((__int128)apy_limb_signed_t(_data[3]) << APY_LIMB_SIZE_BITS);
                re1 = (__int128)apy_limb_signed_t(rhs._data[0]);
                im1 = (__int128)apy_limb_signed_t(rhs._data[1]);
            } else {
                re0 = (__int128)apy_limb_signed_t(_data[0]);
                im0 = (__int128)apy_limb_signed_t(_data[1]);
                re1 = (__int128)rhs._data[0]
                    | ((__int128)apy_limb_signed_t(rhs._data[1]) << APY_LIMB_SIZE_BITS);
                im1 = (__int128)rhs._data[2]
                    | ((__int128)apy_limb_signed_t(rhs._data[3]) << APY_LIMB_SIZE_BITS);
            }
            auto re_res = re0 * re1 - im0 * im1;
            auto im_res = re0 * im1 + im0 * re1;
            result._data[0] = apy_limb_t(re_res);
            result._data[1] = apy_limb_t(re_res >> APY_LIMB_SIZE_BITS);
            result._data[2] = apy_limb_t(im_res);
            result._data[3] = apy_limb_t(im_res >> APY_LIMB_SIZE_BITS);
            return result;
#endif
#else
            // Double limb result specialization
            std::int64_t re0, im0, re1, im1;
            if (unsigned(bits()) > APY_LIMB_SIZE_BITS) {
                re0 = (std::int64_t)_data[0]
                    | ((std::int64_t)apy_limb_signed_t(_data[1]) << APY_LIMB_SIZE_BITS);
                im0 = (std::int64_t)_data[2]
                    | ((std::int64_t)apy_limb_signed_t(_data[3]) << APY_LIMB_SIZE_BITS);
                re1 = (std::int64_t)apy_limb_signed_t(rhs._data[0]);
                im1 = (std::int64_t)apy_limb_signed_t(rhs._data[1]);
            } else {
                re0 = (std::int64_t)apy_limb_signed_t(_data[0]);
                im0 = (std::int64_t)apy_limb_signed_t(_data[1]);
                re1 = (std::int64_t)rhs._data[0]
                    | ((std::int64_t)apy_limb_signed_t(rhs._data[1])
                       << APY_LIMB_SIZE_BITS);
                im1 = (std::int64_t)rhs._data[2]
                    | ((std::int64_t)apy_limb_signed_t(rhs._data[3])
                       << APY_LIMB_SIZE_BITS);
            }
            auto re_res = re0 * re1 - im0 * im1;
            auto im_res = re0 * im1 + im0 * re1;
            result._data[0] = apy_limb_t(re_res);
            result._data[1] = apy_limb_t(re_res >> APY_LIMB_SIZE_BITS);
            result._data[2] = apy_limb_t(im_res);
            result._data[3] = apy_limb_t(im_res >> APY_LIMB_SIZE_BITS);
            return result;
#endif
        }
    }

    // Scratch data:
    // * op1_abs:       _data.size() / 2
    // * op2_abs:       rhs._data.size() / 2
    // * prod_imm:      2 + _data.size() + rhs._data.size()
    std::size_t scratch_size = 2 + (3 * _data.size() + 3 * rhs._data.size()) / 2;
    ScratchVector<apy_limb_t, 64> scratch(scratch_size);
    auto op1_abs_begin = std::begin(scratch);
    auto op2_abs_begin = op1_abs_begin + _data.size() / 2;
    auto prod_imm_begin = op2_abs_begin + rhs._data.size() / 2;

    // Perform the product
    complex_fixed_point_product(
        std::begin(_data),        // src1
        std::begin(rhs._data),    // src2
        std::begin(result._data), // dst
        _data.size() / 2,         // src1_limbs
        rhs._data.size() / 2,     // src2_limbs
        bits_to_limbs(res_bits),  // dst_limbs
        op1_abs_begin,            // op1_abs
        op2_abs_begin,            // op2_abs
        prod_imm_begin            // prod_abs
    );

    return result;
}

APyCFixed APyCFixed::operator/(const APyCFixed& rhs) const
{
    if (rhs.is_zero()) {
        PyErr_SetString(PyExc_ZeroDivisionError, "fixed-point division by zero");
        throw nb::python_error();
    }

    // Divider bits (denominator known to be positive)
    const int num_int_bits = 1 + int_bits() + rhs.int_bits();
    const int num_frac_bits = frac_bits() + rhs.frac_bits();
    const int div_bits = num_int_bits + num_frac_bits + rhs.bits();

    // Result bits
    const int res_int_bits = int_bits() + rhs.frac_bits() + 1;
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    APyCFixed result(res_bits, res_int_bits);

    // Single-limb divider specialization
    if (unsigned(div_bits) <= APY_LIMB_SIZE_BITS) {
        apy_limb_signed_t den
            = apy_limb_signed_t(rhs._data[0]) * apy_limb_signed_t(rhs._data[0])
            + apy_limb_signed_t(rhs._data[1]) * apy_limb_signed_t(rhs._data[1]);
        apy_limb_signed_t real
            = apy_limb_signed_t(_data[0]) * apy_limb_signed_t(rhs._data[0])
            + apy_limb_signed_t(_data[1]) * apy_limb_signed_t(rhs._data[1]);
        apy_limb_signed_t imag
            = apy_limb_signed_t(_data[1]) * apy_limb_signed_t(rhs._data[0])
            - apy_limb_signed_t(_data[0]) * apy_limb_signed_t(rhs._data[1]);

        result._data[0] = (apy_limb_signed_t(real << (rhs.bits())) / den);
        result._data[1] = (apy_limb_signed_t(imag << (rhs.bits())) / den);
        return result; // early exit
    }

    std::size_t src1_limbs = _data.size() / 2;
    std::size_t src2_limbs = rhs._data.size() / 2;
    std::size_t dst_limbs = result._data.size() / 2;
    std::size_t div_limbs = bits_to_limbs(div_bits);

    // Scratch data (size):
    // * op1_abs:   src1_limbs
    // * op2_abs:   src2_limbs
    // * prod_imm:  2 + 2 * src1_limbs + 2 * src2_limbs
    // * num_imm:   div_limbs
    // * den_imm:   2 * src2_limbs
    // * qte_imm:   div_limbs
    std::size_t scratch_limbs = 2 + 3 * src1_limbs + 5 * src2_limbs + 2 * div_limbs;
    ScratchVector<apy_limb_t, 64> scratch(scratch_limbs);

    auto op1_abs = std::begin(scratch);
    auto op2_abs = op1_abs + src1_limbs;
    auto prod_imm = op2_abs + src2_limbs;
    auto num_imm = prod_imm + 2 + 2 * src1_limbs + 2 * src2_limbs;
    auto den_imm = num_imm + div_limbs;
    auto qte_imm = den_imm + 2 * src2_limbs;

    complex_fixed_point_division(
        std::begin(_data),        // src1
        std::begin(rhs._data),    // src2
        std::begin(result._data), // dst
        src1_limbs,               // src1_limbs
        src2_limbs,               // src2_limbs
        dst_limbs,                // dst_limbs
        rhs.bits(),               // src2_bits
        div_limbs,                // div_limbs
        op1_abs,                  // op1_abs
        op2_abs,                  // op2_abs
        prod_imm,                 // prod_imm
        den_imm,                  // den_imm
        num_imm,                  // num_imm
        qte_imm                   // qte_imm
    );

    return result;
}

bool APyCFixed::operator==(const APyCFixed& rhs) const
{
    return (*this - rhs).is_zero();
}

bool APyCFixed::operator!=(const APyCFixed& rhs) const { return !(*this == rhs); }

bool APyCFixed::operator==(const APyFixed& rhs) const
{
    if (!limb_vector_is_zero(imag_begin(), imag_end())) {
        return false;
    } else {
        APyFixed real(_bits, _int_bits);
        std::copy(real_begin(), real_end(), std::begin(real._data));
        return real == rhs;
    }
}

bool APyCFixed::operator!=(const APyFixed& rhs) const { return !(*this == rhs); }

bool APyCFixed::operator==(double rhs) const
{
    APyFloat rhs_fp(
        sign_of_double(rhs), exp_of_double(rhs), man_of_double(rhs), 11, 52, 1023
    );
    return (*this == rhs_fp.to_fixed());
}

bool APyCFixed::operator!=(double rhs) const { return !(*this == rhs); }

bool APyCFixed::operator==(const std::complex<double>& rhs) const
{
    APyFloat rhs_real(
        sign_of_double(rhs.real()),
        exp_of_double(rhs.real()),
        man_of_double(rhs.real()),
        11,
        52,
        1023
    );
    APyFloat rhs_imag(
        sign_of_double(rhs.imag()),
        exp_of_double(rhs.imag()),
        man_of_double(rhs.imag()),
        11,
        52,
        1023
    );

    APyFixed lhs_real(_bits, _int_bits);
    APyFixed lhs_imag(_bits, _int_bits);
    std::copy(real_begin(), real_end(), std::begin(lhs_real._data));
    std::copy(imag_begin(), imag_end(), std::begin(lhs_imag._data));
    return (lhs_real == rhs_real.to_fixed()) && (lhs_imag == rhs_imag.to_fixed());
}

bool APyCFixed::operator!=(const std::complex<double>& rhs) const
{
    return !(*this == rhs);
}

bool APyCFixed::operator==(const nb::int_& rhs) const
{
    const std::vector<apy_limb_t> limb_vec = python_long_to_limb_vec(rhs);
    APyFixed rhs_fixed(
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        APY_LIMB_SIZE_BITS * limb_vec.size(),
        limb_vec
    );
    return *this == rhs_fixed;
}

bool APyCFixed::operator!=(const nb::int_& rhs) const { return !(*this == rhs); }

APyCFixed APyCFixed::operator<<(const int shift_val) const
{
    // Left-shift in place
    APyCFixed result(_bits, _int_bits, std::begin(_data), std::end(_data));
    result._int_bits += shift_val;
    return result;
}

APyCFixed APyCFixed::operator>>(const int shift_val) const
{
    // Left-shift in place
    APyCFixed result(_bits, _int_bits, std::begin(_data), std::end(_data));
    result._int_bits -= shift_val;
    return result;
}

APyCFixed& APyCFixed::operator<<=(const int shift_val)
{
    // Left-shift in place
    _int_bits += shift_val;
    return *this;
}

APyCFixed& APyCFixed::operator>>=(const int shift_val)
{
    // Left-shift in place
    _int_bits -= shift_val;
    return *this;
}

/* ********************************************************************************** *
 * *                        Other public member functions                           * *
 * ********************************************************************************** */

std::string APyCFixed::repr() const
{
    std::stringstream ss {};
    ss << "APyCFixed((";
    ss << bit_pattern_to_string_dec() << "), ";
    ss << "bits=" << bits() << ", ";
    ss << "int_bits=" << int_bits() << ")";
    return ss.str();
}

std::string APyCFixed::bit_pattern_to_string_dec() const
{
    std::stringstream ss {};

    std::vector<apy_limb_t> real(real_begin(), real_end());
    std::vector<apy_limb_t> imag(imag_begin(), imag_end());
    if (bits() % APY_LIMB_SIZE_BITS) {
        apy_limb_t and_mask = (apy_limb_t(1) << (bits() % APY_LIMB_SIZE_BITS)) - 1;
        real.back() &= and_mask;
        imag.back() &= and_mask;
    }

    // Double-dabble for binary-to-BCD conversion
    ss << bcds_to_string(double_dabble(real)) << ", ";
    ss << bcds_to_string(double_dabble(imag));

    return ss.str();
}

std::string APyCFixed::to_string(int base) const
{
    switch (base) {
    case 10:
        return to_string_dec();
        break;
    default:
        auto msg = fmt::format("APyCFixed.__str__: base={} is not supported", base);
        throw nb::value_error(msg.c_str());
        break;
    }
}

std::string APyCFixed::to_string_dec() const
{
    double re = fixed_point_to_double(real_begin(), real_end(), frac_bits());
    double im = fixed_point_to_double(imag_begin(), imag_end(), frac_bits());
    if (im < 0) {
        return fmt::format("({}{}j)", re, im);
    } else {
        return fmt::format("({}+{}j)", re, im);
    }
}

std::string APyCFixed::to_string_hex() const
{
    throw NotImplementedException("APyCFixed::to_string_hex()");
}

std::string APyCFixed::to_string_oct() const
{
    throw NotImplementedException("APyCFixed::to_string_oct()");
}

bool APyCFixed::is_zero() const noexcept
{
    // Both real and imaginary part has to be zero
    return limb_vector_is_zero(std::begin(_data), std::end(_data));
}

nb::tuple APyCFixed::to_bits() const
{
    return nb::make_tuple(
        python_limb_vec_to_long(
            real_begin(), real_end(), false, bits() % APY_LIMB_SIZE_BITS
        ),
        python_limb_vec_to_long(
            imag_begin(), imag_end(), false, bits() % APY_LIMB_SIZE_BITS
        )
    );
}

APyCFixed APyCFixed::cast(
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<int> bits
) const
{
    // Sanitize the input (bit-specifier validity tested in `bits_from_optional_cast()`)
    const auto [new_bits, new_int_bits]
        = bits_from_optional_cast(bits, int_bits, frac_bits, _bits, _int_bits);

    const APyFixedCastOption cast_option = get_fixed_cast_mode();
    const auto quantization_mode = quantization.value_or(cast_option.quantization);
    const auto overflow_mode = overflow.value_or(cast_option.overflow);

    // Result that temporarily can hold all the necessary bits
    APyCFixed result(std::max(new_bits, _bits), new_int_bits);

    // Real part
    fixed_point_cast_unsafe(
        real_begin(),
        real_end(),
        result.real_begin(),
        result.real_end(),
        _bits,
        _int_bits,
        new_bits,
        new_int_bits,
        quantization_mode,
        overflow_mode
    );

    // Imaginary part
    fixed_point_cast_unsafe(
        imag_begin(),
        imag_end(),
        std::begin(result._data) + bits_to_limbs(new_bits),
        std::begin(result._data) + bits_to_limbs(new_bits) + result._data.size() / 2,
        _bits,
        _int_bits,
        new_bits,
        new_int_bits,
        quantization_mode,
        overflow_mode
    );

    result._bits = new_bits;
    result._data.resize(2 * bits_to_limbs(new_bits));
    return result;
}

std::complex<double> APyCFixed::to_complex() const
{
    return std::complex<double>(
        fixed_point_to_double(real_begin(), real_end(), frac_bits()),
        fixed_point_to_double(imag_begin(), imag_end(), frac_bits())
    );
}

/* ********************************************************************************** *
 *                       Static conversion from other types                         * *
 * ********************************************************************************** */

APyCFixed APyCFixed::from_number(
    const nb::object& py_obj,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    if (nb::isinstance<nb::int_>(py_obj)) {
        const nb::int_& val = nb::cast<nb::int_>(py_obj);
        return APyCFixed::from_integer(val, int_bits, frac_bits, bits);
    } else if (nb::isinstance<nb::float_>(py_obj)) {
        double val = static_cast<double>(nb::cast<nb::float_>(py_obj));
        return APyCFixed::from_double(val, int_bits, frac_bits, bits);
    } else if (nb::isinstance<APyFixed>(py_obj)) {
        const APyFixed& fx = nb::cast<APyFixed>(py_obj);
        APyCFixed result(int_bits, frac_bits, bits);
        fixed_point_cast_unsafe(
            std::begin(fx._data),
            std::end(fx._data),
            result.real_begin(),
            result.real_end(),
            fx._bits,
            fx._int_bits,
            result._bits,
            result._int_bits,
            QuantizationMode::RND_INF,
            OverflowMode::WRAP
        );
        return result;
    } else if (nb::isinstance<APyFloat>(py_obj)) {
        const auto fx = nb::cast<APyFloat>(py_obj).to_fixed();
        APyCFixed result(int_bits, frac_bits, bits);
        fixed_point_cast_unsafe(
            std::begin(fx._data),
            std::end(fx._data),
            result.real_begin(),
            result.real_end(),
            fx._bits,
            fx._int_bits,
            result._bits,
            result._int_bits,
            QuantizationMode::RND_INF,
            OverflowMode::WRAP
        );
        return result;
    } else if (nb::isinstance<APyCFixed>(py_obj)) { // One should really use cast
        const APyCFixed& fx = nb::cast<APyCFixed>(py_obj);
        return fx.cast(
            int_bits, frac_bits, QuantizationMode::RND_INF, OverflowMode::WRAP, bits
        );
    } else if (nb::isinstance<std::complex<double>>(py_obj)) {
        std::complex<double> cplx = nb::cast<std::complex<double>>(py_obj);
        return APyCFixed::from_complex(cplx, int_bits, frac_bits, bits);
    } else {
        // Last resort, try casting the Python object to a `std::complex`. This is
        // useful since the Python type `numpy.complex128` does not match any nanobind
        // type.
        std::complex<double> cplx;
        if (nb::try_cast<std::complex<double>>(py_obj, cplx)) {
            return APyCFixed::from_complex(cplx, int_bits, frac_bits, bits);
        } else {
            const nb::type_object type = nb::cast<nb::type_object>(py_obj.type());
            const nb::str type_string = nb::str(type);
            throw std::domain_error(
                std::string("Non-supported type: ") + type_string.c_str()
            );
        }
    }
}

APyCFixed APyCFixed::from_double(
    double value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    APyCFixed result(int_bits, frac_bits, bits);
    if (result._data.size() == 2) {
        unsigned shift = 64 - (result._bits & (64 - 1));
        int frac_bits = result.frac_bits();
        result._data[0] = fixed_point_from_double_single_limb(value, frac_bits, shift);
    } else {
        assert(result._data.size() >= 4);
        assert(result._data.size() % 2 == 0);
        fixed_point_from_double(
            value,
            result.real_begin(),
            result.real_end(),
            result._bits,
            result._int_bits
        );
    }
    return result;
}

APyCFixed APyCFixed::from_integer(
    const nb::int_& value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    // Extract things bit widths
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;

    APyCFixed result(res_bits, res_int_bits);
    auto real = python_long_to_limb_vec(value, result._data.size() / 2);
    std::copy(std::begin(real), std::end(real), result.real_begin());

    // Adjust the number
    if (result.frac_bits() > 0) {
        limb_vector_lsl(result.real_begin(), result.real_end(), result.frac_bits());
    } else { /* result.frac_bits() <= 0 */
        limb_vector_asr(result.real_begin(), result.real_end(), -result.frac_bits());
    }

    // Two's-complements overflow bits outside of the range
    _overflow_twos_complement(
        result.real_begin(), result.real_end(), res_bits, res_int_bits
    );

    return result;
}

APyCFixed APyCFixed::from_complex(
    std::complex<double> value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    APyCFixed result(int_bits, frac_bits, bits);
    double real = value.real();
    double imag = value.imag();
    if (result._data.size() == 2) {
        unsigned shift = 64 - (result._bits & (64 - 1));
        int frac_bits = result.frac_bits();
        result._data[0] = fixed_point_from_double_single_limb(real, frac_bits, shift);
        result._data[1] = fixed_point_from_double_single_limb(imag, frac_bits, shift);
    } else {
        assert(result._data.size() >= 4);
        assert(result._data.size() % 2 == 0);
        fixed_point_from_double(
            real, result.real_begin(), result.real_end(), result._bits, result._int_bits
        );
        fixed_point_from_double(
            imag, result.imag_begin(), result.imag_end(), result._bits, result._int_bits
        );
    }
    return result;
}

APyCFixed APyCFixed::from_apyfixed(
    const APyFixed& value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    APyCFixed result(int_bits, frac_bits, bits);
    std::copy(std::begin(value._data), std::end(value._data), result.real_begin());
    return result;
}

APyCFixed APyCFixed::one(int bits, int int_bits)
{
    std::size_t frac_bits = bits - int_bits;
    std::size_t limb_index = frac_bits / APY_LIMB_SIZE_BITS;
    std::size_t bit_offset = frac_bits % APY_LIMB_SIZE_BITS;

    std::size_t num_limbs = 2 * (limb_index + 1);
    ScratchVector<apy_limb_t> data(num_limbs, apy_limb_t(0));

    // Set the specified bit to 1
    data[limb_index] |= apy_limb_t(1) << bit_offset;

    return APyCFixed(bits, int_bits, std::begin(data), std::end(data));
}

//! Unary negation
APyCFixed APyCFixed::operator-() const
{
    APyCFixed result(_bits, _int_bits);
    limb_vector_negate(real_begin(), real_end(), result.real_begin());
    limb_vector_negate(imag_begin(), imag_end(), result.imag_begin());
    return result;
}

//! Unary negation
APyCFixed APyCFixed::operator~() const
{
    APyCFixed result(_bits, _int_bits);
    std::transform(_data.cbegin(), _data.cend(), result._data.begin(), std::bit_not {});
    return result;
}

APyFixed APyCFixed::get_real() const
{
    APyFixed result(_bits, _int_bits);
    std::copy(real_begin(), real_end(), std::begin(result._data));
    return result;
}

APyFixed APyCFixed::get_imag() const
{
    APyFixed result(_bits, _int_bits);
    std::copy(imag_begin(), imag_end(), std::begin(result._data));
    return result;
}

bool APyCFixed::is_identical(
    const std::variant<const APyCFixed*, const APyCFixedArray*>& other
) const
{
    if (!std::holds_alternative<const APyCFixed*>(other)) {
        return false;
    } else {
        auto&& other_scalar = *std::get<const APyCFixed*>(other);
        return bits() == other_scalar.bits() && int_bits() == other_scalar.int_bits()
            && *this == other_scalar;
    }
}
