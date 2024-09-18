#include "nanobind/nanobind.h"
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
#include <variant>
namespace nb = nanobind;

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

// Standard header includes
#include <algorithm> // std::copy, std::max, std::transform, etc...
#include <cstddef>   // std::size_t
#include <cstdint>   // std::int16, std::int32, std::int64, etc...
#include <ios>       // std::dec, std::hex
#include <optional>  // std::optional
#include <set>       // std::set
#include <sstream>   // std::stringstream
#include <stdexcept> // std::length_error
#include <string>    // std::string
#include <vector>    // std::vector, std::swap

#include "apybuffer.h"
#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"
#include "apytypes_common.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "broadcast.h"
#include "python_util.h"

#include <fmt/format.h>

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

APyFixedArray::APyFixedArray(
    const nb::sequence& bit_pattern_sequence,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyFixedArray(
          python_sequence_extract_shape(bit_pattern_sequence), int_bits, frac_bits, bits
      )
{
    // Specialized initialization for NDArray
    if (nb::isinstance<nb::ndarray<>>(bit_pattern_sequence)) {
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(bit_pattern_sequence);
        _set_bits_from_ndarray(ndarray);
        return; // initialization completed
    }

    // Caster for performing two's complement overflow
    APyFixed caster(_bits, _int_bits);

    // 1D vector of Python int object (`nb::int_` objects)
    std::vector<nb::object> python_ints
        = python_sequence_walk<nb::int_>(bit_pattern_sequence);

    for (std::size_t i = 0; i < _data.size() / _itemsize; i++) {

        std::vector<mp_limb_t> limb_vec
            = python_long_to_limb_vec(nb::cast<nb::int_>(python_ints[i]), _itemsize);

        // Two's complements overflowing using caster type
        std::copy_n(limb_vec.begin(), _itemsize, caster._data.begin());
        _overflow_twos_complement(
            caster._data.begin(), caster._data.end(), _bits, _int_bits
        );
        // caster._overflow_twos_complement(
        //     caster._data.begin(), caster._data.end(), _bits, _int_bits
        //);
        std::copy_n(
            caster._data.begin(),         // src
            _itemsize,                    // limbs to copy
            _data.begin() + i * _itemsize // dst
        );
    }
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyFixedArray::APyFixedArray(
    const std::vector<std::size_t>& shape, int bits, int int_bits
)
    : APyBuffer(shape, bits_to_limbs(bits))
    , _bits { bits }
    , _int_bits { int_bits }
{
}

APyFixedArray::APyFixedArray(
    const std::vector<std::size_t>& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyBuffer(shape, bits_to_limbs(bits_from_optional(bits, int_bits, frac_bits)))
    , _bits { bits.has_value() ? *bits : *int_bits + *frac_bits }
    , _int_bits { int_bits.has_value() ? *int_bits : *bits - *frac_bits }
{
}

/* ********************************************************************************** *
 * *                          Binary arithmetic operators                           * *
 * ********************************************************************************** */

template <class ripple_carry_op, class simd_op, class simd_shift_op>
inline APyFixedArray APyFixedArray::_apyfixedarray_base_add_sub(const APyFixedArray& rhs
) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting vector
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: Operands and result fit in single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        if (frac_bits() == rhs.frac_bits()) {
            // Operands have equally many fractional bits.
            simd_op {}(
                _data.begin(),
                rhs._data.begin(),
                result._data.begin(),
                result._data.size()
            );
        } else {
            auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());
            auto lhs_shift_amount = unsigned(res_frac_bits - frac_bits());
            simd_shift_op {}(
                _data.begin(),
                rhs._data.begin(),
                result._data.begin(),
                lhs_shift_amount,
                rhs_shift_amount,
                result._data.size()
            );
        }
        return result; // early exit
    }

    // Special case #2: Operands and result have equally many limbs
    if (result._itemsize == _itemsize && result._itemsize == rhs._itemsize) {
        const mp_limb_t* src1_ptr;
        const mp_limb_t* src2_ptr;
        if (frac_bits() == rhs.frac_bits()) {
            // Right-hand side and left-hand side have equally many fractional bits
            src1_ptr = &_data[0];
            src2_ptr = &rhs._data[0];
        } else if (frac_bits() <= rhs.frac_bits()) {
            // Right-hand side has more fractional bits. Upsize `*this`
            _cast_correct_wl(result._data.begin(), res_bits, res_int_bits);
            src1_ptr = &result._data[0];
            src2_ptr = &rhs._data[0];
        } else {
            // Left-hand side has more fractional bits. Upsize `rhs`
            rhs._cast_correct_wl(result._data.begin(), res_bits, res_int_bits);
            src1_ptr = &_data[0];
            src2_ptr = &result._data[0];
        }
        for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
            ripple_carry_op {}(
                &result._data[i], // dst
                &src1_ptr[i],     // src1
                &src2_ptr[i],     // src2
                result._itemsize  // limb vector length
            );
        }
        return result; // early exit
    }

    // Most general case: Works in any situation, but is slowest
    APyFixedArray imm(_shape, res_bits, res_int_bits);
    _cast_correct_wl(result._data.begin(), res_bits, res_int_bits);
    rhs._cast_correct_wl(imm._data.begin(), res_bits, res_int_bits);

    // Perform ripple-carry operation for each element
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        ripple_carry_op {}(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[i],    // src2
            result._itemsize  // limb vector length
        );
    }
    return result;
}

template <class ripple_carry_op, class simd_op_const, class simd_shift_op_const>
inline APyFixedArray APyFixedArray::_apyfixed_base_add_sub(const APyFixed& rhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting vector
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: Operands and result fit in single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        if (frac_bits() == rhs.frac_bits()) {
            // Operands have equally many fractional bits.
            simd_op_const {}(
                _data.begin(), rhs._data[0], result._data.begin(), result._data.size()
            );
        } else {
            auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());
            auto lhs_shift_amount = unsigned(res_frac_bits - frac_bits());
            simd_shift_op_const {}(
                std::begin(_data),                // src1
                rhs._data[0] << rhs_shift_amount, // constant
                std::begin(result._data),         // dst
                lhs_shift_amount,                 // src1 shift amount
                result._data.size()
            );
        }
        return result; // early exit
    }

    // Most general case: Works in any situation, but is slowest
    APyFixed imm(res_bits, res_int_bits);
    auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());
    _cast_correct_wl(result._data.begin(), res_bits, res_int_bits);
    rhs._cast_correct_wl(imm._data.begin(), imm._data.end(), rhs_shift_amount);
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        // Perform ripple-carry operation
        ripple_carry_op {}(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[0],    // src2
            result._itemsize  // limb vector length
        );
    }
    // Return result
    return result;
}

APyFixedArray APyFixedArray::operator+(const APyFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFixedArray.__add__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) + rhs.broadcast_to(broadcast_shape);
    }

    return _apyfixedarray_base_add_sub<
        mpn_add_n_functor<>,
        simd::add_functor<>,
        simd::shift_add_functor<>>(rhs);
}

APyFixedArray APyFixedArray::operator+(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<
        mpn_add_n_functor<>,
        simd::add_const_functor<>,
        simd::shift_add_const_functor<>>(rhs);
}

APyFixedArray APyFixedArray::operator-(const APyFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFixedArray.__sub__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) - rhs.broadcast_to(broadcast_shape);
    }

    return _apyfixedarray_base_add_sub<
        mpn_sub_n_functor<>,
        simd::sub_functor<>,
        simd::shift_sub_functor<>>(rhs);
}

APyFixedArray APyFixedArray::operator-(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<
        mpn_sub_n_functor<>,
        simd::sub_const_functor<>,
        simd::shift_sub_const_functor<>>(rhs);
}

// Scalar - Array
APyFixedArray APyFixedArray::rsub(const APyFixed& lhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(lhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(lhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Adjust binary point
    APyFixedArray result(_shape, res_bits, res_int_bits);
    _cast_correct_wl(result._data.begin(), res_bits, res_int_bits);
    auto lhs_shift_amount = unsigned(res_frac_bits - lhs.frac_bits());
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        mp_limb_t operand = lhs._data[0] << lhs_shift_amount;
        simd::vector_rsub_const(
            result._data.begin(), operand, result._data.begin(), result._data.size()
        );
    } else {
        APyFixed imm(res_bits, res_int_bits);
        lhs._cast_correct_wl(imm._data.begin(), imm._data.end(), lhs_shift_amount);

        // Perform subtraction
        for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
            mpn_sub_n(
                &result._data[i], // dst
                &imm._data[0],    // src1
                &result._data[i], // src2
                result._itemsize  // limb vector length
            );
        }
    }

    // Return result
    return result;
}

APyFixedArray APyFixedArray::operator*(const APyFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFixedArray.__mul__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) * rhs.broadcast_to(broadcast_shape);
    }

    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);

    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        simd::vector_mul(
            std::begin(_data),        // src1
            std::begin(rhs._data),    // src2
            std::begin(result._data), // dst
            result._data.size()       // elements
        );
    } else {
        fixed_point_hadamard_product(
            std::begin(_data),        // src1
            std::begin(rhs._data),    // src2
            std::begin(result._data), // dst
            _itemsize,                // src1_limbs
            rhs._itemsize,            // src2_limbs
            result._itemsize,         // dst_limbs
            _nitems                   // n_items
        );
    }

    return result;
}

APyFixedArray APyFixedArray::operator*(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: The resulting number of bits fit in a single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        simd::vector_mul_const(
            std::begin(_data),        // src1
            rhs._data[0],             // src2
            std::begin(result._data), // dst
            result._data.size()       // elements
        );
        return result; // early exit
    }

    // General case: This always works but is slower than the special cases.
    auto op2_begin = rhs._data.begin();
    auto op2_end = rhs._data.begin() + rhs.vector_size();
    bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
    std::vector<mp_limb_t> op2_abs(rhs.vector_size());
    limb_vector_abs(op2_begin, op2_end, op2_abs.begin());

    // Perform multiplication for each element in the tensor. `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    // most significant limbs are zero."
    std::vector<mp_limb_t> res_tmp_vec(_itemsize + rhs.vector_size(), 0);
    std::vector<mp_limb_t> op1_abs(_itemsize);
    auto op1_begin = _data.begin();
    for (std::size_t i = 0; i < _nitems; i++) {
        // Current working operands
        auto op1_end = op1_begin + _itemsize;

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;

        // Retrieve the absolute value of both operands, as required by GMP
        limb_vector_abs(op1_begin, op1_end, op1_abs.begin());

        // Perform the multiplication
        if (op1_abs.size() < op2_abs.size()) {
            mpn_mul(
                &res_tmp_vec[0], // dst
                &op2_abs[0],     // src1
                op2_abs.size(),  // src1 limb vector length
                &op1_abs[0],     // src2
                op1_abs.size()   // src2 limb vector length
            );
        } else {
            mpn_mul(
                &res_tmp_vec[0], // dst
                &op1_abs[0],     // src1
                op1_abs.size(),  // src1 limb vector length
                &op2_abs[0],     // src2
                op2_abs.size()   // src2 limb vector length
            );
        }

        // Handle sign
        if (result_sign) {
            limb_vector_negate(
                res_tmp_vec.begin(),
                res_tmp_vec.begin() + result._itemsize,
                result._data.begin() + (i + 0) * result._itemsize
            );
        } else {
            // Copy into resulting vector
            std::copy_n(
                res_tmp_vec.begin(),
                result._itemsize,
                result._data.begin() + (i + 0) * result._itemsize
            );
        }
        op1_begin = op1_end;
    }
    return result;
}

APyFixedArray APyFixedArray::operator/(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFixedArray.__div__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) / rhs.broadcast_to(broadcast_shape);
    }

    const int res_int_bits = int_bits() + rhs.frac_bits() + 1;
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: The resulting number of bits fit in a single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        simd::vector_shift_div_signed(
            std::begin(_data),        // src1 (numerator)
            std::begin(rhs._data),    // src2 (denominator)
            std::begin(result._data), // dst
            rhs.bits(),               // numerator shift amount
            _data.size()              // vector elements
        );
        return result; // early exit
    }

    // General case: This always works but is slower than the special cases.

    // Absolute value denominator
    ScratchVector<mp_limb_t> abs_den(rhs._itemsize);

    // Absolute value left-shifted numerator
    ScratchVector<mp_limb_t> abs_num(result._itemsize);

    for (std::size_t i = 0; i < _nitems; i++) {
        std::fill(std::begin(abs_num), std::end(abs_num), 0);
        if (limb_vector_is_zero(
                std::begin(rhs._data) + (i + 0) * rhs._itemsize,
                std::begin(rhs._data) + (i + 1) * rhs._itemsize
            )) {
            continue;
        }
        bool den_sign = limb_vector_abs(
            std::begin(rhs._data) + (i + 0) * rhs._itemsize,
            std::begin(rhs._data) + (i + 1) * rhs._itemsize,
            std::begin(abs_den)
        );
        bool num_sign = limb_vector_abs(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize,
            std::begin(abs_num)
        );
        limb_vector_lsl(abs_num.begin(), abs_num.end(), rhs.bits());

        // `mpn_tdiv_qr` requires the number of *significant* limbs in denominator
        std::size_t den_significant_limbs
            = significant_limbs(std::begin(abs_den), std::end(abs_den));
        mpn_div_qr(
            &result._data[i * result._itemsize], // Quotient
            &abs_num[0],                         // Numerator
            abs_num.size(),                      // Numerator limbs
            &abs_den[0],                         // Denominator
            den_significant_limbs                // Denominator significant limbs
        );

        // Negate result if negative
        if (num_sign ^ den_sign) {
            limb_vector_negate(
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                std::begin(result._data) + (i + 0) * result._itemsize
            );
        }
    }
    return result;
}

APyFixedArray APyFixedArray::operator/(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.frac_bits() + 1;
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: The resulting number of bits fit in a single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        simd::vector_shift_div_const_signed(
            std::begin(_data),        // src1 (numerator)
            rhs._data[0],             // src2 (constant denominator)
            std::begin(result._data), // dst
            rhs.bits(),               // numerator shift amount
            _data.size()              // vector elements
        );
        return result; // early exit
    }

    // General case: This always works but is slower than the special cases.

    // Absolute value denominator. `mpn_tdiv_qr` requires the number of *significant*
    // limbs in denominator
    ScratchVector<mp_limb_t> abs_den(rhs.vector_size());
    bool den_sign = limb_vector_abs(
        std::begin(rhs._data), std::end(rhs._data), std::begin(abs_den)
    );
    std::size_t den_significant_limbs
        = significant_limbs(std::begin(abs_den), std::end(abs_den));

    // Absolute value left-shifted numerator
    ScratchVector<mp_limb_t> abs_num(result._itemsize);

    for (std::size_t i = 0; i < _nitems; i++) {
        std::fill(std::begin(abs_num), std::end(abs_num), 0);
        bool num_sign = limb_vector_abs(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize,
            std::begin(abs_num)
        );
        limb_vector_lsl(abs_num.begin(), abs_num.end(), rhs.bits());

        mpn_div_qr(
            &result._data[i * result._itemsize], // Quotient
            &abs_num[0],                         // Numerator
            abs_num.size(),                      // Numerator limbs
            &abs_den[0],                         // Denominator
            den_significant_limbs                // Denominator significant limbs
        );

        // Negate result if negative
        if (num_sign ^ den_sign) {
            limb_vector_negate(
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                std::begin(result._data) + (i + 0) * result._itemsize
            );
        }
    }
    return result;
}

APyFixedArray APyFixedArray::rdiv(const APyFixed& rhs) const
{
    const int res_int_bits = rhs.int_bits() + frac_bits() + 1;
    const int res_frac_bits = rhs.frac_bits() + int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: The resulting number of bits fit in a single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        simd::vector_rdiv_const_signed(
            std::begin(_data),        // src2 (denominator)
            rhs._data[0] << bits(),   // src1 (constant numerator)
            std::begin(result._data), // dst
            _data.size()              // vector elements
        );
        return result;
    }

    // General case: This always works but is slower than the special cases.

    // Absolute value denominator
    ScratchVector<mp_limb_t> abs_den(_itemsize);

    // Absolute value left-shifted numerator. As `mpn_div_qr` alters the numerator on
    // call, we allocate twice it's size. The first half [ `0`, `result._itemsize` ) is
    // passed to `mpn_div_qr` and [ `result._item_size`, `2*result._itemsize` ) is used
    // to cache the left-shifted absolute numerator.
    ScratchVector<mp_limb_t> abs_num(2 * result._itemsize);
    bool num_sign = limb_vector_abs(
        std::begin(rhs._data),
        std::end(rhs._data),
        std::begin(abs_num) + result._itemsize
    );
    limb_vector_lsl(std::begin(abs_num) + result._itemsize, std::end(abs_num), bits());

    for (std::size_t i = 0; i < _nitems; i++) {
        bool den_sign = limb_vector_abs(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize,
            std::begin(abs_den)
        );
        std::copy_n(
            std::begin(abs_num) + result._itemsize,
            result._itemsize,
            std::begin(abs_num)
        );

        // `mpn_tdiv_qr` requires the number of *significant* limbs in denominator
        std::size_t den_significant_limbs
            = significant_limbs(std::begin(abs_den), std::end(abs_den));
        mpn_div_qr(
            &result._data[i * result._itemsize], // Quotient
            &abs_num[0],                         // Numerator
            result._itemsize,                    // Numerator limbs
            &abs_den[0],                         // Denominator
            den_significant_limbs                // Denominator significant limbs
        );

        // Negate result if negative
        if (num_sign ^ den_sign) {
            limb_vector_negate(
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                std::begin(result._data) + (i + 0) * result._itemsize
            );
        }
    }
    return result;
}

APyFixedArray APyFixedArray::operator<<(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixedArray result = *this;
    result._int_bits += shift_val;
    return result;
}

APyFixedArray APyFixedArray::operator>>(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixedArray result = *this;
    result._int_bits -= shift_val;
    return result;
}

APyFixedArray& APyFixedArray::operator<<=(const int shift_val)
{
    // Left shift in place
    _int_bits += shift_val;
    return *this;
}

APyFixedArray& APyFixedArray::operator>>=(const int shift_val)
{
    // Right shift in place
    _int_bits -= shift_val;
    return *this;
}

APyFixedArray APyFixedArray::matmul(const APyFixedArray& rhs) const
{
    if (ndim() == 1 && rhs.ndim() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out. Perform
            // the checked inner product.
            return _checked_inner_product(rhs, get_accumulator_mode_fixed());
        }
    }
    if (ndim() == 2 && (rhs.ndim() == 2 || rhs.ndim() == 1)) {
        if (_shape[1] == rhs._shape[0]) {
            // Dimensionality for a standard 2D matrix multiplication checks out.
            // Perform the checked 2D matrix
            return _checked_2d_matmul(rhs, get_accumulator_mode_fixed());
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(fmt::format(
        "APyFixedArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
        tuple_string_from_vec(_shape),
        tuple_string_from_vec(rhs._shape)
    ));
}

APyFixedArray APyFixedArray::swapaxes(nb::int_ axis1, nb::int_ axis2) const
{
    size_t _axis1 = ::get_normalized_axes(axis1, _ndim).front();
    size_t _axis2 = ::get_normalized_axes(axis2, _ndim).front();

    std::vector<size_t> new_axis(_ndim);
    std::iota(new_axis.begin(), new_axis.end(), 0);

    // Swap the specified axes
    std::swap(new_axis[_axis1], new_axis[_axis2]);

    std::vector<size_t> new_shape(_ndim);
    for (size_t i = 0; i < _ndim; ++i) {
        new_shape[i] = _shape[new_axis[i]];
    }

    APyFixedArray result(new_shape, bits(), int_bits());
    ::transpose_axes_and_copy_data(
        _data.begin(), result._data.begin(), _shape, new_axis, _itemsize
    );
    return result;
}

APyFixedArray APyFixedArray::transpose(std::optional<nb::tuple> axes) const
{
    switch (_ndim) {
    case 0:
    case 1:
        // Behave like `NumPy`, simply returns `*this` if single-dimensional
        return *this;
    case 2: {
        // Optimized code for dim == 2
        // Resulting array with shape dimensions
        APyFixedArray result(_shape, bits(), int_bits());
        std::reverse(result._shape.begin(), result._shape.end());
        // Copy data
        for (std::size_t y = 0; y < _shape[0]; y++) {
            for (std::size_t x = 0; x < _shape[1]; x++) {
                std::copy_n(
                    _data.begin() + (y * _shape[1] + x) * _itemsize,       // src
                    _itemsize,                                             // limbs
                    result._data.begin() + (x * _shape[0] + y) * _itemsize // dst
                );
            }
        }
        return result;
    }

    default: {
        std::vector<size_t> new_axis(_ndim);

        if (axes.has_value()) {
            std::variant<nb::tuple, nb::int_> axis_variant = axes.value();
            new_axis = get_normalized_axes(axis_variant, _ndim);
        } else {
            // reverse the order of axes by default
            std::iota(new_axis.begin(), new_axis.end(), 0);
            std::reverse(new_axis.begin(), new_axis.end());
        }

        std::vector<size_t> new_shape(_ndim);
        for (size_t i = 0; i < _ndim; ++i) {
            new_shape[i] = _shape[new_axis[i]];
        }

        APyFixedArray result(new_shape, bits(), int_bits());
        ::transpose_axes_and_copy_data(
            _data.begin(), result._data.begin(), _shape, new_axis, _itemsize
        );
        return result;
    }
    }
}

/* ********************************************************************************** *
 * *                               Other methods                                    * *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::broadcast_to(const std::vector<std::size_t> shape) const
{
    if (!is_broadcastable(_shape, shape)) {
        throw nb::value_error(
            fmt::format(
                "Operands could not be broadcast together with shapes: {}, {}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(shape)
            )
                .c_str()
        );
    }

    APyFixedArray result(shape, bits(), int_bits());
    broadcast_data_copy(_data.begin(), result._data.begin(), _shape, shape, _itemsize);
    return result;
}

APyFixedArray
APyFixedArray::squeeze(std::optional<std::variant<nb::int_, nb::tuple>> axis) const
{
    std::vector<std::size_t> shape = _shape;
    std::set<int> axis_set;
    if (axis.has_value()) {
        // given an int or tuple of ints, remove the specified dimensions if size is one
        // or throw an error
        auto ax = axis.value();
        nb::tuple axis_tuple;

        if (std::holds_alternative<nb::tuple>(ax)) {
            axis_tuple = std::get<nb::tuple>(ax);
        } else if (std::holds_alternative<nb::int_>(ax)) {
            int axis = int(std::get<nb::int_>(ax));
            axis_tuple = nb::make_tuple(axis);
        }
        for (auto ptr = axis_tuple.begin(); ptr != axis_tuple.end(); ptr++) {
            int axis_n = int(nanobind::cast<nb::int_>(*ptr));
            if (axis_n >= int(shape.size())) {
                throw nb::index_error(
                    "Specified axis with larger than number of dimensions in the "
                    "APyFixedArray"
                );
            }
            axis_set.insert(axis_n);
        }
        int cnt = 0;
        auto predicate = [&](std::size_t dim) {
            if (axis_set.find(cnt) != axis_set.end() && dim != 1) {
                throw nb::value_error(
                    "Cannot select an axis to squeeze that has size other than one"
                );
            }
            return axis_set.find(cnt++) != axis_set.end() && dim == 1;
        };
        shape.erase(std::remove_if(shape.begin(), shape.end(), predicate), shape.end());
    } else {
        // Given no specified axis, remove all dimensions of size one.
        auto predicate = [](std::size_t dim) { return dim == 1; };
        shape.erase(std::remove_if(shape.begin(), shape.end(), predicate), shape.end());
    }
    // if all dimensions where one, set the dimension to one, to avoid an error
    if (shape.size() == 0) {
        shape = { 1 };
    }

    // create the resulting Array and copy the data
    APyFixedArray result(shape, bits(), int_bits());
    std::copy_n(_data.begin(), _data.size(), result._data.begin());
    return result;
}

APyFixedArray APyFixedArray::cumulative_prod_sum_function(
    void (*pos_func)(
        std::size_t,
        std::size_t,
        std::size_t,
        APyFixedArray&,
        APyFixedArray&,
        std::size_t
    ),
    int (*int_bit_increase)(std::size_t, std::size_t),
    int (*frac_bit_increase)(std::size_t, std::size_t),
    std::optional<nb::int_> axis
) const
{
    // determine input value
    // if no input is received _shape.size() will be used
    std::size_t _axis = _shape.size();
    if (axis.has_value()) {
        _axis = std::size_t(axis.value());
        if (_axis >= _shape.size()) {
            throw nb::index_error(
                "specified axis outside number of dimensions in the APyFixedArray"
            );
        }
    }

    // initialize variables used for the summation or multiplication
    // and determine the new shape
    std::size_t elements = _nitems;
    std::size_t res_nitems = _nitems;
    std::vector<std::size_t> shape = { _nitems };
    if (_axis < _shape.size()) {
        elements = _shape.at(_axis);
        shape = _shape;
    }

    // calculate new bits
    const int res_int_bits = int_bits() + int_bit_increase(elements, int_bits());
    const int res_frac_bits = frac_bits() + frac_bit_increase(elements, frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // temporary vector and source vector.
    // the source vector will also contain the result after the summation or
    // multiplication are performed
    APyFixedArray temp(_shape, res_bits, res_int_bits);
    APyFixedArray source(_shape, bits(), int_bits());
    std::copy_n(_data.begin(), _data.size(), source._data.begin());

    // get the stride
    std::size_t stride = 1;
    if (_axis < _shape.size()) {
        stride = strides_from_shape(_shape)[_axis];
    }
    // perform the summation or multiplication and place result in 'result'
    for (std::size_t i = 0; i < temp._nitems; i++) {
        pos_func(i, stride, elements, source, temp, frac_bits());
    }
    // copy the elements into a new array to get the correct shape
    APyFixedArray result2(shape, res_bits, res_int_bits);
    std::copy_n(temp._data.begin(), res_nitems * temp._itemsize, result2._data.begin());
    return result2;
}
std::variant<APyFixedArray, APyFixed> APyFixedArray::prod_sum_function(
    void (*pos_func)(
        std::size_t,
        std::size_t,
        std::size_t,
        APyFixedArray&,
        APyFixedArray&,
        std::size_t
    ),
    int (*int_bit_increase)(std::size_t, std::size_t),
    int (*frac_bit_increase)(std::size_t, std::size_t),
    std::optional<std::variant<nb::tuple, nb::int_>> axis
) const
{
    // determine input value
    // if no value were given it will default to {_shape.size()}
    std::set<std::size_t> axes_set;
    if (axis.has_value()) {
        std::vector<std::size_t> axes_vector
            = cpp_shape_from_python_shape_like(axis.value());
        for (auto i : axes_vector) {
            if (i >= _shape.size()) {
                throw nb::index_error(
                    "specified axis outside number of dimensions in the APyFixedArray"
                );
            }
            axes_set.insert(i);
        }
    } else {
        axes_set.insert(_shape.size());
    }

    // determine new shape
    std::vector<std::size_t> shape = _shape;
    std::size_t cnt = 0;
    auto predicate
        = [&](std::size_t x) { return axes_set.find(cnt++) != axes_set.end(); };
    shape.erase(std::remove_if(shape.begin(), shape.end(), predicate), shape.end());

    // caclutlate how many elements are beign
    std::size_t elems
        = _nitems / std::accumulate(shape.begin(), shape.end(), 1, std::multiplies {});
    if (axes_set.find(_shape.size()) != axes_set.end()
        || axes_set.size() == _shape.size()) {
        elems = _nitems;
    }
    // calculate new bits
    const int res_int_bits = int_bits() + int_bit_increase(elems, int_bits());
    const int res_frac_bits = frac_bits() + frac_bit_increase(elems, frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // temporary vector and source vector.
    // the source vector will also contain the result after the summation or
    // multiplication are performed
    APyFixedArray temp(_shape, res_bits, res_int_bits);
    APyFixedArray source(_shape, bits(), int_bits());
    std::copy_n(_data.begin(), _data.size(), source._data.begin());

    // if no axis was given axis or if all the axes where specified this special case
    // can be used instead
    if (axes_set.find(_shape.size()) != axes_set.end()
        || axes_set.size() == _shape.size()) {

        for (std::size_t i = 0; i < _nitems; i++) {
            pos_func(i, 1, _nitems, source, temp, frac_bits());
        }
        APyFixed res(res_bits, res_int_bits);
        std::copy_n(temp._data.begin(), temp._itemsize, res._data.begin());
        return res;
    }
    // get strides for the summation or multiplication loop
    std::vector<std::size_t> strides = strides_from_shape(_shape);
    std::size_t elements = _nitems;
    // perform operations for the given axes
    for (std::size_t x = 0; x < _shape.size(); x++) {
        if (axes_set.find(x) == axes_set.end()) {
            continue;
        }
        // perform the operations for the current axis
        for (std::size_t i = 0; i < elements; i++) {
            pos_func(i, strides[x], _shape.at(x), source, temp, frac_bits());
        }
        // update new amount of elements in the list
        elements /= _shape.at(x);
        // copy the data to source and reset temp
        source = temp;
        temp._data.assign(temp._data.size(), 0);
    }
    // copy the data into an array with the correct shape
    APyFixedArray res(shape, res_bits, res_int_bits);
    std::copy_n(source._data.begin(), elements * source._itemsize, res._data.begin());
    return res;
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::sum(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto int_bit_increase = [](std::size_t elements, std::size_t bits) {
        return int(bit_width(elements - 1));
    };
    auto frac_bit_increase = [](std::size_t elements, std::size_t bits) { return 0; };
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFixedArray& src,
                       APyFixedArray& dst,
                       std::size_t frac_bits) {
        // calculate new position
        std::size_t pos_in_sec = i % (sec_length);
        std::size_t sec_pos = (i - i % (elements * sec_length)) / elements;
        // Perform ripple-carry operation on the limbs
        auto pos = pos_in_sec + sec_pos;
        mpn_add_n_functor<> {}(
            &dst._data.at(pos * src._itemsize), // dst
            &dst._data.at(pos * src._itemsize), // src1
            &src._data.at(i * src._itemsize),   // src2
            src._itemsize                       // limb vector length
        );
    };

    return prod_sum_function(pos_func, int_bit_increase, frac_bit_increase, axis);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::nansum(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    // since no Nan can exist in an APyFixedArray it is the same as sum
    return this->sum(axis);
}

APyFixedArray APyFixedArray::cumsum(std::optional<nb::int_> axis) const
{
    auto int_bit_increase = [](std::size_t elements, std::size_t bits) {
        return int(bit_width(elements - 1));
    };
    auto frac_bit_increase = [](std::size_t elements, std::size_t bits) { return 0; };
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFixedArray& src,
                       APyFixedArray& dst,
                       std::size_t frac_bits) {
        // calculate new position
        std::size_t pos = i - sec_length;
        if (i % (sec_length * elements) < sec_length) {
            pos = i;
        }
        // Perform ripple-carry operation on the limbs
        mpn_add_n_functor<> {}(
            &dst._data.at(i * src._itemsize),   // dst
            &dst._data.at(pos * src._itemsize), // src1
            &src._data.at(i * src._itemsize),   // src2
            src._itemsize                       // limb vector length
        );
    };
    return cumulative_prod_sum_function(
        pos_func, int_bit_increase, frac_bit_increase, axis
    );
}

APyFixedArray APyFixedArray::nancumsum(std::optional<nb::int_> axis) const
{
    // since no Nan can exist in an APyFixedArray it is the same as cumsum
    return this->cumsum(axis);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::prod(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto int_bit_increase = [](std::size_t elements, std::size_t int_bits) {
        return int((elements - 1) * int_bits);
    };
    auto frac_bit_increase = [](std::size_t elements, std::size_t frac_bits) {
        return int((elements - 1) * frac_bits);
    };
    auto pos_func = [](std::size_t i,
                       std::size_t stride,
                       std::size_t elems,
                       APyFixedArray& src,
                       APyFixedArray& dst,
                       std::size_t frac_bits) {
        // calculate new position
        auto pos = i % stride + (i - i % (elems * stride)) / elems;
        // itemsizes of the different numbers
        std::size_t i_sz_1 = src._itemsize;
        std::size_t i_sz_2 = dst._itemsize;
        // if no axes where given and its the first elements we need to multiply with a
        // 1 instead of 0, since we know the resulting vector will at this point only
        // have 0s we can write over it.
        // The same if we are at the start of a multiplication chain when multiplying
        // over an axis.
        if ((elems == src._nitems && i == 0) || i % (stride * elems) < stride) {
            dst._data.at(pos * i_sz_2) = 1;
        }
        // setup for the multiplication
        auto op1_begin = src._data.begin() + i * i_sz_1;
        auto op2_begin = dst._data.begin() + pos * i_sz_2;
        auto op1_end = op1_begin + i_sz_1;
        auto op2_end = op2_begin + i_sz_2;

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;
        // Retrieve the absolute value of both operands, as required by GMP
        std::vector<mp_limb_t> op1_abs(i_sz_1);
        std::vector<mp_limb_t> op2_abs(i_sz_2);
        limb_vector_abs(op1_begin, op1_end, op1_abs.begin());
        limb_vector_abs(op2_begin, op2_end, op2_abs.begin());

        std::vector<mp_limb_t> tmp(i_sz_1 + i_sz_2, 0);

        // Perform the multiplication
        if (op1_abs.size() < op2_abs.size()) {
            mpn_mul(
                &tmp.at(0),     // dst
                &op2_abs.at(0), // src1
                op2_abs.size(), // src1 limb vector length
                &op1_abs.at(0), // src2
                op1_abs.size()  // src2 limb vector length
            );
        } else {
            mpn_mul(
                &tmp.at(0),     // dst
                &op1_abs.at(0), // src1
                op1_abs.size(), // src1 limb vector length
                &op2_abs.at(0), // src2
                op2_abs.size()  // src2 limb vector length
            );
        }

        if (result_sign) {
            limb_vector_negate(tmp.begin(), tmp.begin() + i_sz_2, op2_begin);
        } else {
            // Copy into resulting vector
            std::copy_n(tmp.begin(), i_sz_2, op2_begin);
        }
    };
    return prod_sum_function(pos_func, int_bit_increase, frac_bit_increase, axis);
}

APyFixedArray APyFixedArray::cumprod(std::optional<nb::int_> axis) const
{
    auto int_bit_increase = [](std::size_t elements, std::size_t int_bits) {
        return int((elements - 1) * int_bits);
    };
    auto frac_bit_increase = [](std::size_t elements, std::size_t frac_bits) {
        return int((elements - 1) * frac_bits);
    };
    auto pos_func = [](std::size_t i,
                       std::size_t stride,
                       std::size_t elems,
                       APyFixedArray& src,
                       APyFixedArray& dst,
                       std::size_t frac_bits) {
        std::size_t i_sz_1 = src._itemsize;
        std::size_t i_sz_2 = dst._itemsize;
        std::size_t pos = i - stride;
        // handle the case where the current number is the first in a multiplication
        // chain. when this is the case we need to shift it to the left to offset the
        // increase in fraction bits later on
        if (i % (stride * elems) < stride) {
            pos = i;
            dst._data.at(i * i_sz_2) = 1;
            limb_vector_lsl(
                dst._data.begin() + i * i_sz_2,
                dst._data.begin() + (i + 1) * i_sz_2,
                frac_bits * (elems - 1)
            );
        }
        auto op1_begin = src._data.begin() + i * i_sz_1;
        auto op2_begin = dst._data.begin() + pos * i_sz_2;
        auto op1_end = op1_begin + i_sz_1;
        auto op2_end = op2_begin + i_sz_2;

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;
        // Retrieve the absolute value of both operands, as required by GMP
        std::vector<mp_limb_t> op1_abs(i_sz_1);
        std::vector<mp_limb_t> op2_abs(i_sz_2);
        limb_vector_abs(op1_begin, op1_end, op1_abs.begin());
        limb_vector_abs(op2_begin, op2_end, op2_abs.begin());

        std::vector<mp_limb_t> tmp(i_sz_1 + i_sz_2, 0);

        // Perform the multiplication
        if (op1_abs.size() < op2_abs.size()) {
            mpn_mul(
                &tmp.at(0),     // dst
                &op2_abs.at(0), // src1
                op2_abs.size(), // src1 limb vector length
                &op1_abs.at(0), // src2
                op1_abs.size()  // src2 limb vector length
            );
        } else {
            mpn_mul(
                &tmp.at(0),     // dst
                &op1_abs.at(0), // src1
                op1_abs.size(), // src1 limb vector length
                &op2_abs.at(0), // src2
                op2_abs.size()  // src2 limb vector length
            );
        }

        // if we are inside a multiplication chain we have overcompensated on the first
        // number, so now we need to counteract that. This part can be optimized.
        if (i % (stride * elems) >= stride) {
            limb_vector_lsr(tmp.begin(), tmp.begin() + i_sz_2, frac_bits);
        }

        if (result_sign) {
            limb_vector_negate(
                tmp.begin(), tmp.begin() + i_sz_2, dst._data.begin() + i * i_sz_2
            );
        } else {
            // Copy into resulting vector
            std::copy_n(tmp.begin(), i_sz_2, dst._data.begin() + i * i_sz_2);
        }
    };
    return cumulative_prod_sum_function(
        pos_func, int_bit_increase, frac_bit_increase, axis
    );
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::nanprod(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    // since no Nan can exist in an APyFixedArray it is the same as prod
    return this->prod(axis);
}

APyFixedArray APyFixedArray::nancumprod(std::optional<nb::int_> axis) const
{
    // since no Nan can exist in an APyFixedArray it is the same as cumprod
    return this->cumprod(axis);
}

APyFixedArray
APyFixedArray::broadcast_to_python(const std::variant<nb::tuple, nb::int_> shape) const
{
    return broadcast_to(cpp_shape_from_python_shape_like(shape));
}

APyFixedArray
APyFixedArray::convolve(const APyFixedArray& other, const std::string& mode) const
{
    if (ndim() != 1 || other.ndim() != 1) {
        auto msg = fmt::format(
            "can only convolve 1D arrays (lhs.ndim = {}, rhs.ndim = {})",
            ndim(),
            other.ndim()
        );
        throw nanobind::value_error(msg.c_str());
    }

    // Find the shorter array of `*this` and `other` based on length.
    bool swap = _shape[0] < other._shape[0];

    // Make a reverse copy of the shorter array
    APyFixedArray b_cpy = swap ? *this : other;
    multi_limb_reverse(std::begin(b_cpy._data), std::end(b_cpy._data), b_cpy._itemsize);

    // Let `a` be a pointer to the longer array, and let `b` be a pointer to the reverse
    // copy of the shorter array.
    const APyFixedArray* a = swap ? &other : this;
    const APyFixedArray* b = &b_cpy;

    // Extract convolution properties
    auto [len, n_left, n_right] = get_conv_lengths(mode, a, b);

    // Handle accumulator context
    const int prod_bits = a->bits() + b->bits();
    const int prod_int_bits = a->int_bits() + b->int_bits();
    std::optional<APyFixedAccumulatorOption> acc_mode = get_accumulator_mode_fixed();
    auto dot = inner_product_func_from_acc_mode<APyBuffer<mp_limb_t>::vector_type>(
        prod_bits, prod_int_bits, acc_mode
    );

    // Result vector
    const int sum_bits = bit_width(b->_shape[0] - 1);
    const int res_bits = acc_mode ? acc_mode->bits : prod_bits + sum_bits;
    const int res_int_bits = acc_mode ? acc_mode->int_bits : prod_int_bits + sum_bits;
    APyFixedArray result({ len }, res_bits, res_int_bits);

    // Loop working variables
    std::size_t n = b->_shape[0] - n_left;
    auto dst = std::begin(result._data);
    auto src1 = std::cbegin(a->_data);
    auto src2 = std::cbegin(b->_data) + n_left * b->_itemsize;

    // `b` limits length of the inner product length
    for (std::size_t i = 0; i < n_left; i++) {
        dot(src1, src2, dst, a->_itemsize, b->_itemsize, result._itemsize, n);
        src2 -= b->_itemsize;
        dst += result._itemsize;
        n++;
    }

    // full inner product length
    for (std::size_t i = 0; i < a->_shape[0] - b->_shape[0] + 1; i++) {
        dot(src1, src2, dst, a->_itemsize, b->_itemsize, result._itemsize, n);
        src1 += a->_itemsize;
        dst += result._itemsize;
    }

    // `a` limits length of the inner product length
    for (std::size_t i = 0; i < n_right; i++) {
        n--;
        dot(src1, src2, dst, a->_itemsize, b->_itemsize, result._itemsize, n);
        src1 += a->_itemsize;
        dst += result._itemsize;
    }

    return result;
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::max(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto comp_func = [](APyFixed& lhs, APyFixed& rhs) { return lhs > rhs; };
    return max_min_helper_function(comp_func, axis);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::min(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto comp_func = [](APyFixed& lhs, APyFixed& rhs) { return lhs < rhs; };
    return max_min_helper_function(comp_func, axis);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::nanmax(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    return max(axis);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::nanmin(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    return min(axis);
}

// Return the maximum of an array or the maximum along an axis.
std::variant<APyFixedArray, APyFixed> APyFixedArray::max_min_helper_function(
    bool (*comp_func)(APyFixed&, APyFixed&),
    std::optional<std::variant<nb::tuple, nb::int_>> axis
) const
{
    // determine the input axes
    std::set<std::size_t> axes_set;
    if (axis.has_value()) {
        std::vector<std::size_t> axes_vector
            = cpp_shape_from_python_shape_like(axis.value());
        for (auto i : axes_vector) {
            if (i >= _shape.size()) {
                throw nb::index_error(
                    "specified axis outside number of dimensions in the APyFixedArray"
                );
            }
            axes_set.insert(i);
        }
    } else {
        axes_set.insert(_shape.size());
    }

    // special case where the maximum or minimum from the whole array is wanted
    if (axes_set.size() == _shape.size()
        || axes_set.find(_shape.size()) != axes_set.end()) {
        APyFixed res(_bits, _int_bits);
        std::copy_n(_data.begin(), _itemsize, res._data.begin());
        APyFixed temp(_bits, _int_bits);
        for (std::size_t i = _itemsize; i < _data.size(); i += _itemsize) {
            std::copy_n(_data.begin() + i, _itemsize, temp._data.begin());
            if (comp_func(temp, res)) {
                res = temp;
            }
        }
        return res;
    }

    std::size_t elements = _nitems;
    std::vector<std::size_t> res_shape;
    APyBuffer<mp_limb_t>::vector_type source_data = _data;
    APyBuffer<mp_limb_t>::vector_type temp_data(_data.size(), 0);
    std::vector<std::size_t> strides = strides_from_shape(_shape);
    APyFixed lhs_scalar(_bits, _int_bits);
    APyFixed rhs_scalar(_bits, _int_bits);

    // loop over one axis at a time
    for (std::size_t x = 0; x < _shape.size(); x++) {
        if (axes_set.find(x) == axes_set.end()) {
            res_shape.push_back(_shape[x]);
            continue;
        }
        // loop over an axis and get the maximum or minimum along it
        for (std::size_t i = 0; i < elements; i++) {
            std::size_t new_pos
                = i % strides[x] + (i - i % (strides[x] * _shape[x])) / _shape[x];
            if (i % (strides[x] * _shape[x]) < strides[x]) {
                std::copy_n(
                    source_data.begin() + i * _itemsize,
                    _itemsize,
                    temp_data.begin() + new_pos * _itemsize
                );
                continue;
            }

            std::copy_n(
                source_data.begin() + i * _itemsize, _itemsize, lhs_scalar._data.begin()
            );
            std::copy_n(
                temp_data.begin() + new_pos * _itemsize,
                _itemsize,
                rhs_scalar._data.begin()
            );
            if (comp_func(lhs_scalar, rhs_scalar)) {
                std::copy_n(
                    lhs_scalar._data.begin(),
                    _itemsize,
                    temp_data.begin() + new_pos * _itemsize
                );
            }
        }

        elements /= _shape[x];
        source_data = temp_data;
        temp_data.assign(elements, 0);
    }
    APyFixedArray result(res_shape, _bits, _int_bits);
    std::copy_n(source_data.begin(), elements, result._data.begin());
    return result;
}

std::string APyFixedArray::repr() const
{
    std::stringstream ss {};
    ss << "APyFixedArray([";
    if (_shape[0]) {
        // Setup hex printing which will properly display the BCD characters
        ss << std::hex;

        std::vector<mp_limb_t> data(_itemsize, 0);
        for (std::size_t offset = 0; offset < _data.size(); offset += _itemsize) {
            std::copy_n(_data.begin() + offset, _itemsize, data.begin());

            // Zero sign bits outside of bit-range
            if (bits() % _LIMB_SIZE_BITS) {
                mp_limb_t and_mask = (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
                data.back() &= and_mask;
            }

            // Double-dabble for binary-to-BCD conversion
            ss << bcds_to_string(double_dabble(data));
            ss << ", ";
        }

        ss.seekp(-2, ss.cur);
    }
    ss << "], shape=";
    ss << tuple_string_from_vec(_shape);
    ss << ", "
       << "bits=" << std::dec << bits() << ", "
       << "int_bits=" << std::dec << int_bits() << ")";
    return ss.str();
}

APyFixedArray APyFixedArray::reshape(nb::tuple new_shape) const
{
    std::vector<std::size_t> new_shape_cpp = reshape_from_tuple(new_shape, _nitems);
    APyFixedArray result = APyFixedArray(new_shape_cpp, _bits, _int_bits);
    std::copy_n(this->_data.begin(), _data.size(), result._data.begin());

    return result;
}

APyFixedArray APyFixedArray::flatten() const
{
    // Reuse the reshape function to flatten the array
    return this->reshape(nb::make_tuple(-1));
}

APyFixedArray APyFixedArray::ravel() const
{
    // same as flatten as for now
    return this->flatten();
}

// The shape of the array
nb::tuple APyFixedArray::python_get_shape() const
{
    nb::list result_list;
    for (std::size_t i = 0; i < _shape.size(); i++) {
        result_list.append(_shape[i]);
    }
    return nb::tuple(result_list);
}

// The dimension in the array
size_t APyFixedArray::ndim() const noexcept { return _shape.size(); }

size_t APyFixedArray::size() const noexcept { return _shape[0]; }

APyFixedArray APyFixedArray::abs() const
{
    // Increase word length of result by one
    const int res_int_bits = _int_bits + 1;
    const int res_bits = _bits + 1;

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = std::abs(mp_limb_signed_t(_data[i]));
        }
        return result;
    }
    auto it_begin = _data.begin();
    for (std::size_t i = 0; i < _nitems; i++) {
        auto it_end = it_begin + _itemsize;
        limb_vector_abs(it_begin, it_end, result._data.begin() + i * result._itemsize);
        it_begin = it_end;
    }
    return result;
}

APyFixedArray APyFixedArray::operator-() const
{
    // Increase word length of result by one
    const int res_int_bits = _int_bits + 1;
    const int res_bits = _bits + 1;

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = -_data[i];
        }
        return result;
    }
    // Sign-extend in case an additional limb is required
    _cast_correct_wl(result._data.begin(), res_bits, res_int_bits);
    auto it_begin = result._data.begin();
    for (std::size_t i = 0; i < _nitems; i++) {
        auto it_end = it_begin + result._itemsize;
        limb_vector_negate(it_begin, it_end, it_begin);
        it_begin = it_end;
    }
    return result;
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::get_item(std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple> key
) const
{
    if (std::holds_alternative<nb::int_>(key)) {

        // Key is of integer type
        return get_item_integer(static_cast<std::ptrdiff_t>(std::get<nb::int_>(key)));

    } else if (std::holds_alternative<nb::slice>(key)) {

        // Key is of slice type
        return get_item_slice(std::get<nb::slice>(key));

    } else if (std::holds_alternative<nb::ellipsis>(key)) {

        // Key is a single ellipsis, return a copy of `*this`
        return *this;

    } else { /* std::holds_alternative<nb::tuple>(key) */

        // Key is a tuple of slicing instructions
        return get_item_tuple(get_item_to_cpp_tuple(std::get<nb::tuple>(key)));
    }
}

std::variant<APyFixedArray, APyFixed> APyFixedArray::get_item_integer(std::ptrdiff_t idx
) const
{
    if (idx >= std::ptrdiff_t(_shape[0]) || idx < -std::ptrdiff_t(_shape[0])) {
        throw std::out_of_range(fmt::format(
            "APyFixedArray.__getitem__: index {} is out of bounds for axis 0 with size "
            "{}",
            idx,
            _shape[0]
        ));
    }

    // Adjust for negative index
    idx = idx < 0 ? idx + _shape[0] : idx;

    if (_ndim == 1) {
        // One dimension, return APyFixed
        APyFixed result(_bits, _int_bits);
        std::copy_n(_data.begin() + idx * _itemsize, _itemsize, result._data.begin());
        return result;
    } else {
        // New shape contains all dimensions except the very first one
        auto new_shape = std::vector<std::size_t>(_shape.begin() + 1, _shape.end());

        // Element stride is the new shape folded over multiplication
        std::size_t element_stride = fold_shape(new_shape);

        APyFixedArray result(new_shape, _bits, _int_bits);
        std::copy_n(
            _data.begin() + idx * _itemsize * element_stride,
            _itemsize * element_stride,
            result._data.begin()
        );
        return result;
    }
}

APyFixedArray APyFixedArray::get_item_slice(nb::slice slice) const
{
    auto [start, stop, step, len] = slice.compute(_shape[0]);

    // New resulting shape
    std::vector<std::size_t> new_shape = _shape;
    new_shape[0] = len;

    // Result fixed-point array
    APyFixedArray result(new_shape, _bits, _int_bits);

    auto size = _itemsize * fold_shape(std::begin(_shape) + 1, std::end(_shape));
    std::ptrdiff_t src_i = start;
    std::ptrdiff_t dst_i = 0;
    if (step < 0) {
        // Copy data into result and return (negative src step size)
        for (; src_i > stop; src_i += step, dst_i++) {
            std::copy_n(
                std::begin(_data) + src_i * size,       // src
                size,                                   // elements to copy
                std::begin(result._data) + dst_i * size // dst
            );
        }
    } else { /* step >= 0 */
        // Copy data into result and return (positive src step size)
        for (; src_i < stop; src_i += step, dst_i++) {
            std::copy_n(
                std::begin(_data) + src_i * size,       // src
                size,                                   // elements to copy
                std::begin(result._data) + dst_i * size // dst
            );
        }
    }

    return result;
}

std::vector<APyFixedArray> APyFixedArray::get_item_slice_nested(nb::slice slice) const
{
    auto [start, stop, step, len] = slice.compute(_shape[0]);

    // The shape of the sliced vectors
    std::vector<std::size_t> new_shape(std::begin(_shape) + 1, std::end(_shape));
    if (!new_shape.size()) {
        new_shape = { 1 };
    }

    // Result fixed-point array
    std::vector<APyFixedArray> result;

    auto size = fold_shape(new_shape) * _itemsize;
    APyFixedArray tmp(new_shape, _bits, _int_bits);
    if (step < 0) {
        // Copy data into result and return (negative src step size)
        for (std::ptrdiff_t src_i = start; src_i > stop; src_i += step) {
            std::copy_n(std::begin(_data) + src_i * size, size, std::begin(tmp._data));
            result.push_back(tmp);
        }
    } else {
        // Copy data into result and return (positive src step size)
        for (std::ptrdiff_t src_i = start; src_i < stop; src_i += step) {
            std::copy_n(std::begin(_data) + src_i * size, size, std::begin(tmp._data));
            result.push_back(tmp);
        }
    }
    return result;
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::get_item_tuple(std::vector<std::variant<nb::int_, nb::slice>> tuple
) const
{
    // Return everything on an empty tuple
    if (tuple.size() == 0) {
        return *this;
    }

    std::variant<nb::int_, nb::slice> current = tuple[0];
    std::vector<std::variant<nb::int_, nb::slice>> remaining(
        std::begin(tuple) + 1, std::end(tuple)
    );

    if (std::holds_alternative<nb::int_>(current)) {

        /*
         * Current tuple element is an integer
         */
        std::ptrdiff_t key = static_cast<std::ptrdiff_t>(std::get<nb::int_>(current));
        std::variant<APyFixedArray, APyFixed> result = get_item_integer(key);
        if (!remaining.size()) {
            return result; // is always APyFixed
        } else {
            return std::get<APyFixedArray>(result).get_item_tuple(remaining);
        }

    } else { /* std::holds_alternative<nb::slice>(current) */

        /*
         * Current tuple element is a slice
         */
        APyFixedArray result(get_item_tuple_shape(tuple, remaining), _bits, _int_bits);
        nb::slice slice = std::get<nb::slice>(current);
        auto arrays = get_item_slice_nested(slice);

        for (std::size_t i = 0; i < arrays.size(); i++) {
            auto v = arrays[i].get_item_tuple(remaining);
            if (std::holds_alternative<APyFixed>(v)) {
                auto fix = std::get<APyFixed>(v);
                std::copy_n(
                    std::begin(fix._data),
                    _itemsize,
                    std::begin(result._data) + i * _itemsize
                );
            } else {
                auto fixarray = std::get<APyFixedArray>(v);
                std::copy_n(
                    std::begin(fixarray._data),
                    fixarray._nitems * _itemsize,
                    std::begin(result._data) + i * fixarray._nitems * _itemsize
                );
            }
        }

        return result;
    }
}

std::vector<std::size_t> APyFixedArray::get_item_tuple_shape(
    const std::vector<std::variant<nb::int_, nb::slice>>& tuple,
    const std::vector<std::variant<nb::int_, nb::slice>>& remaining
) const
{
    // Compute the resulting array shape
    std::vector<std::size_t> result_shape;
    for (std::size_t i = 0; i < tuple.size(); i++) {
        auto&& element = tuple[i];
        if (std::holds_alternative<nb::int_>(element)) {
            continue;
        } else { /* std::holds_alternative<nb::slice>(v) */
            auto [_, __, ___, len] = std::get<nb::slice>(element).compute(_shape[i]);
            result_shape.push_back(len);
        }
    }
    for (std::size_t i = remaining.size() + 1; i < _shape.size(); i++) {
        result_shape.push_back(_shape[i]);
    }
    return result_shape;
}

std::vector<std::variant<nb::int_, nb::slice>>
APyFixedArray::get_item_to_cpp_tuple(const nb::tuple& key) const
{
    if (key.size() > _ndim) {
        // The key tuple must have fewer elements than this array has number of
        // dimensions
        std::string msg = fmt::format(
            "APyFixedArray.__getitem__: key tuple size (={}) > ndim (={})",
            key.size(),
            _ndim
        );
        throw nb::value_error(msg.c_str());
    }

    nb::int_ integer;
    nb::slice slice;
    nb::ellipsis ellipsis;
    bool ellipsis_found = false;
    std::vector<std::variant<nb::int_, nb::slice>> cpp_tuple;

    for (std::size_t i = 0; i < key.size(); i++) {
        const auto& tuple_element = key[i];
        if (nb::try_cast<nb::slice>(tuple_element, slice)) {
            cpp_tuple.push_back(slice);
        } else if (nb::try_cast<nb::int_>(tuple_element, integer)) {
            cpp_tuple.push_back(integer);
        } else if (nb::try_cast<nb::ellipsis>(tuple_element, ellipsis)) {
            if (ellipsis_found) {
                // An ellipsis object has already been procecced. Only a single ellipsis
                // per tuple key is allowed
                throw nb::value_error(
                    "APyFixedArray.__getitem__: only one single ellipsis object allowed"
                );
            } else {
                // Found first ellipsis, fill missing dimesnsions with full slices
                ellipsis_found = true;
                std::size_t n_fill = _ndim - key.size() + 1;
                for (std::size_t j = 0; j < n_fill; j++) {
                    cpp_tuple.push_back(nb::slice(_shape[i + j]));
                }
            }
        } else {
            throw nb::value_error(
                "APyFixedArray.__getitem__: supported keys are `int`, `slice`, "
                "`ellipsis`, or a single tuple thereof"
            );
        }
    }
    return cpp_tuple;
}

std::variant<
    nb::list,
    nb::ndarray<nb::numpy, uint64_t>,
    nb::ndarray<nb::numpy, uint32_t>,
    nb::ndarray<nb::numpy, uint16_t>,
    nb::ndarray<nb::numpy, uint8_t>>
APyFixedArray::to_bits(bool numpy) const
{
    if (numpy) {
        if (bits() <= 8) {
            return to_bits_ndarray<nb::numpy, uint8_t>();
        } else if (bits() <= 16) {
            return to_bits_ndarray<nb::numpy, uint16_t>();
        } else if (bits() <= 32) {
            return to_bits_ndarray<nb::numpy, uint32_t>();
        } else {
            return to_bits_ndarray<nb::numpy, uint64_t>();
        }
    } else {
        auto it = std::cbegin(_data);
        return to_bits_python_recursive_descent(0, it);
    }
}

nb::list APyFixedArray::to_bits_python_recursive_descent(
    std::size_t dim, APyBuffer<mp_limb_t>::vector_type::const_iterator& it
) const
{
    nb::list result;
    if (dim == _ndim - 1) {
        // Most inner dimension: append data
        for (std::size_t i = 0; i < _shape[dim]; i++) {
            result.append(python_limb_vec_to_long(
                it,                      // start
                it + _itemsize,          // stop
                false,                   // vec_is_signed
                bits() % _LIMB_SIZE_BITS // bits_last_limb
            ));
            it += _itemsize;
        }
    } else {
        // We need to go deeper...
        for (std::size_t i = 0; i < _shape[dim]; i++) {
            result.append(to_bits_python_recursive_descent(dim + 1, it));
        }
    }
    return result;
}

template <typename NB_ARRAY_TYPE, typename INT_TYPE>
nb::ndarray<NB_ARRAY_TYPE, INT_TYPE> APyFixedArray::to_bits_ndarray() const
{
    constexpr std::size_t INT_TYPE_SIZE_BITS = 8 * sizeof(INT_TYPE);
    if (bits() > int(INT_TYPE_SIZE_BITS)) {
        throw nb::value_error(
            fmt::format(
                "APyFixedArray::to_bits_ndarray(): only supports <= {}-bit elements",
                INT_TYPE_SIZE_BITS
            )
                .c_str()
        );
    }

    INT_TYPE* result_data = new INT_TYPE[_nitems];
    for (std::size_t i = 0; i < _nitems; i++) {
        result_data[i] = INT_TYPE(_data[i]);
        if (bits() % (INT_TYPE_SIZE_BITS)) {
            result_data[i] &= (INT_TYPE(1) << (bits() % INT_TYPE_SIZE_BITS)) - 1;
        }
    }

    // Delete `result_data` when the `owner` capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (INT_TYPE*)p; });

    return nb::ndarray<NB_ARRAY_TYPE, INT_TYPE>(result_data, _ndim, &_shape[0], owner);
}

nb::ndarray<nb::numpy, double> APyFixedArray::to_numpy() const
{
    // Dynamically allocate data to be passed to python
    double* result_data = new double[_nitems];

    APyFixed type_caster(bits(), int_bits());
    for (std::size_t i = 0; i < _nitems; i++) {
        std::copy_n(
            std::begin(_data) + i * _itemsize, _itemsize, std::begin(type_caster._data)
        );
        result_data[i] = double(type_caster);
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (double*)p; });

    return nb::ndarray<nb::numpy, double>(result_data, _ndim, &_shape[0], owner);
}

bool APyFixedArray::is_identical(const APyFixedArray& other) const
{
    bool same_shape = _shape == other._shape;
    bool same_bit_spec = bits() == other.bits() && int_bits() == other.int_bits();
    return same_shape && same_bit_spec && (_data == other._data);
}

APyFixedArray APyFixedArray::cast(
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<int> bits
) const
{
    // Sanitize the input (bit-specifier validity tested in `bits_from_optional()`)
    const auto [new_bits, new_int_bits]
        = bits_from_optional_cast(bits, int_bits, frac_bits, _bits, _int_bits);

    const APyFixedCastOption cast_option = get_fixed_cast_mode();
    const auto quantization_mode = quantization.value_or(cast_option.quantization);
    const auto overflow_mode = overflow.value_or(cast_option.overflow);

    // The new result array (`bit_specifier_sanitize()` called in constructor)
    std::size_t result_limbs = bits_to_limbs(new_bits);
    std::size_t pad_limbs = bits_to_limbs(std::max(new_bits, _bits)) - result_limbs;
    APyFixedArray result(
        _shape,
        new_bits + _LIMB_SIZE_BITS * pad_limbs,
        new_int_bits + _LIMB_SIZE_BITS * pad_limbs
    );

    // `APyFixed` with the same word length as `*this` for reusing quantization methods
    APyFixed caster(_bits, _int_bits);

    // Do the casting
    _cast(
        result._data.begin(),
        result._data.end(),
        caster,
        new_bits,
        new_int_bits,
        quantization_mode,
        overflow_mode
    );

    result._bits = new_bits;
    result._int_bits = new_int_bits;
    result.buffer_resize(_shape, result_limbs);
    return result;
}

/* ********************************************************************************** *
 * *                      Static conversion from other types                          *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::from_double(
    const nb::sequence& python_seq,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    if (nb::isinstance<nb::ndarray<>>(python_seq)) {
        // Sequence is NDArray. Initialize using `from_array`
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(python_seq);
        return from_array(ndarray, int_bits, frac_bits, bits);
    }

    APyFixedArray result(
        python_sequence_extract_shape(python_seq), int_bits, frac_bits, bits
    );

    // Extract all Python doubles and integers
    auto py_obj = python_sequence_walk<nb::float_, nb::int_>(python_seq);

    // Set data from doubles (reuse `APyFixed::from_double` conversion)
    APyFixed fix(result.bits(), result.int_bits());
    for (std::size_t i = 0; i < result._data.size() / result._itemsize; i++) {
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            // Python double object
            double d = static_cast<double>(nb::cast<nb::float_>(py_obj[i]));
            fix.set_from_double(d);
            std::copy_n(
                fix._data.begin(),                          // src
                result._itemsize,                           // limbs to copy
                result._data.begin() + i * result._itemsize // dst
            );
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            // Python integer object
            auto max_bits = result.frac_bits() < 0
                ? result.bits() - result.frac_bits() // Negative fractional bits
                : result.bits();                     // Non-negative fractional bits
            auto limb_vec = python_long_to_limb_vec(
                nb::cast<nb::int_>(py_obj[i]), bits_to_limbs(max_bits)
            );
            fix.set_from_double(double(APyFixed(max_bits, max_bits, limb_vec)));
            std::copy_n(
                fix._data.begin(),                          // src
                result._itemsize,                           // limbs to copy
                result._data.begin() + i * result._itemsize // dst
            );
        }
    }

    return result;
}

APyFixedArray APyFixedArray::from_array(
    const nb::ndarray<nb::c_contig>& ndarray,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    std::size_t ndim = ndarray.ndim();
    if (ndim == 0) {
        throw nb::type_error(
            "APyFixedArray.from_float(): NDArray with ndim == 0 not supported"
        );
    }
    std::vector<std::size_t> shape(ndim, 0);
    for (std::size_t i = 0; i < ndim; i++) {
        shape[i] = ndarray.shape(i);
    }

    APyFixedArray result(shape, int_bits, frac_bits, bits);
    result._set_values_from_ndarray(ndarray);
    return result;
}

/* ****************************************************************************** *
 *                    Static methods for creating arrays                          *
 * ****************************************************************************** */

APyFixedArray APyFixedArray::zeros(
    const nb::tuple& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    std::vector<std::size_t> new_shape = cpp_shape_from_python_shape_like(shape);
    APyFixedArray result(new_shape, int_bits, frac_bits, bits);
    return result;
}

APyFixedArray APyFixedArray::ones(
    const nb::tuple& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    return full(shape, one(res_bits, res_int_bits));
}

APyFixedArray APyFixedArray::eye(
    const nb::int_& N,
    std::optional<nb::int_> M,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    // Use N for both dimensions if M is not provided
    nb::tuple shape
        = M.has_value() ? nb::make_tuple(N, M.value()) : nb::make_tuple(N, N);

    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    return diagonal(shape, one(res_bits, res_int_bits));
}

APyFixedArray APyFixedArray::identity(
    const nb::int_& N,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    return eye(N, std::nullopt, int_bits, frac_bits, bits);
}

APyFixedArray APyFixedArray::full(const nb::tuple& shape, const APyFixed& fill_value)
{
    std::vector<std::size_t> cpp_shape = cpp_shape_from_python_shape_like(shape);
    APyFixedArray result(cpp_shape, fill_value.bits(), fill_value.int_bits());

    for (std::size_t i = 0; i < result._nitems; ++i) {
        std::copy_n(
            std::begin(fill_value._data),
            result._itemsize,
            std::begin(result._data) + i * result._itemsize
        );
    }
    return result;
}

APyFixedArray
APyFixedArray::diagonal(const nb::tuple& shape, const APyFixed& fill_value)
{
    std::vector<std::size_t> new_shape = cpp_shape_from_python_shape_like(shape);
    if (new_shape.size() > 2) {
        throw nb::value_error(
            "Creating higher dimensional diagonal arrays are not yet defined"
        );
    }
    APyFixedArray result(new_shape, fill_value.bits(), fill_value.int_bits());
    std::size_t itemsize = result._itemsize;

    std::size_t min_dim = *std::min_element(new_shape.begin(), new_shape.end());
    std::vector<std::size_t> strides = ::strides_from_shape(new_shape);

    for (std::size_t i = 0; i < min_dim; ++i) {
        std::size_t index = i * std::accumulate(strides.begin(), strides.end(), 0);
        std::copy_n(
            fill_value._data.begin(), itemsize, result._data.begin() + index * itemsize
        );
    }
    return result;
}

APyFixedArray APyFixedArray::arange(
    const nb::object& start,
    const nb::object& stop,
    const nb::object& step,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    const int _bits = bits_from_optional(bits, int_bits, frac_bits);
    const int _int_bits = int_bits.has_value() ? *int_bits : _bits - *frac_bits;

    const std::vector<APyFixed> apy_vals = ::arange(start, stop, step);
    APyFixedArray result({ apy_vals.size() }, _bits, _int_bits);

    for (size_t i = 0; i < apy_vals.size(); i++) {
        apy_vals[i]._cast(
            std::begin(result._data) + i * result._itemsize,       // output start
            std::begin(result._data) + (i + 1) * result._itemsize, // output sentinel
            _bits,
            _int_bits,
            QuantizationMode::RND_INF,
            OverflowMode::WRAP
        );
    }

    return result;
}

/* ********************************************************************************** *
 * *                            Private member functions                            * *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::_checked_inner_product(
    const APyFixedArray& rhs,                     // rhs
    std::optional<APyFixedAccumulatorOption> mode // optional accumulation mode
) const
{
    int res_bits = bits() + rhs.bits() + bit_width(_shape[0] - 1);
    int res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[0] - 1);
    APyFixedArray result({ 1 }, res_bits, res_int_bits);

    if (!mode.has_value()) {
        if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
            // Fastest path, no accumulation mode specified and result fit in a single
            // limb.
            result._data[0] = simd::vector_multiply_accumulate(
                _data.begin(), rhs._data.begin(), _data.size()
            );
            return result;
        } else { /* unsigned(res_bits) > _LIMB_SIZE_BITS */
            fixed_point_inner_product(
                std::begin(_data),        // src1
                std::begin(rhs._data),    // src2
                std::begin(result._data), // dst
                _itemsize,                // src1_limbs
                rhs._itemsize,            // src2_limbs
                result._itemsize,         // dst_limbs
                _nitems                   // n_items
            );
            return result;
        }
    } else { /* mode.has_value() */
        result._bits = mode->bits;
        result._int_bits = mode->int_bits;
        result.buffer_resize(result._shape, bits_to_limbs(mode->bits));

        fixed_point_inner_product_accumulator(
            std::begin(_data),           // src1
            std::begin(rhs._data),       // src2
            std::begin(result._data),    // dst
            _itemsize,                   // src1_limbs
            rhs._itemsize,               // src2_limbs
            result._itemsize,            // dst_limbs
            _nitems,                     // n_items
            bits() + rhs.bits(),         // product_bits
            int_bits() + rhs.int_bits(), // product_int_bits
            *mode
        );
        return result;
    }
}

APyFixedArray APyFixedArray::_checked_2d_matmul(
    const APyFixedArray& rhs, std::optional<APyFixedAccumulatorOption> mode
) const
{
    // Resulting shape
    const std::size_t res_cols = rhs._shape.size() > 1 ? rhs._shape[1] : 1;
    std::vector<std::size_t> res_shape = rhs._shape.size() > 1
        ? std::vector<std::size_t> { _shape[0], rhs._shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { _shape[0] };               // rhs is 1-D

    // Resulting number of bits
    std::size_t res_bits = bits() + rhs.bits() + bit_width(_shape[1] - 1);
    std::size_t res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[1] - 1);

    // Resulting tensor and a working column from `rhs`
    APyFixedArray result(res_shape, res_bits, res_int_bits);
    APyFixedArray current_col({ rhs._shape[0] }, rhs.bits(), rhs.int_bits());

    /*
     * Special case #1: No accumulator mode specified and resulting matrix elements fit
     * into a single limb. This is the fastest `_checked_2d_matmul` path.
     */
    if (!mode.has_value()) {
        if (res_bits <= _LIMB_SIZE_BITS) {
            for (std::size_t x = 0; x < res_cols; x++) {
                // Copy column from `rhs` and use as the current working column. As
                // reading columns from `rhs` is cache-inefficient, we like to do this
                // only once for each element in the resulting matrix.
                for (std::size_t row = 0; row < rhs._shape[0]; row++) {
                    current_col._data[row] = rhs._data[x + row * res_cols];
                }
                for (std::size_t y = 0; y < res_shape[0]; y++) {
                    result._data[y * res_cols + x] = simd::vector_multiply_accumulate(
                        _data.begin() + (y * _shape[1]), // src1
                        current_col._data.begin(),       // src2
                        _shape[1]                        // multiply-accumulate length
                    );
                }
            }
            return result; // early exit
        }
    }

    /*
     * General case: This always works but is slower than the special cases.
     */
    // Scratch memories for avoiding memory re-allocation
    APyFixedArray current_row({ _shape[1] }, bits(), int_bits());
    APyFixedArray hadamard_tmp(
        { rhs._shape[0] }, bits() + rhs.bits(), int_bits() + rhs.int_bits()
    );
    if (!mode.has_value()) {

        for (std::size_t x = 0; x < res_cols; x++) {
            // Copy column from `rhs` and use as the current working column. As
            // reading columns from `rhs` is cache-inefficient, we like to do this
            // only once for each element in the resulting matrix.
            for (std::size_t row = 0; row < rhs._shape[0]; row++) {
                std::copy_n(
                    rhs._data.begin() + (x + row * res_cols) * rhs._itemsize,
                    rhs._itemsize,
                    current_col._data.begin() + row * rhs._itemsize
                );
            }
            for (std::size_t y = 0; y < res_shape[0]; y++) {
                // Copy current row from lhs (`*this`)
                std::copy_n(
                    _data.begin() + (y * _shape[1] * _itemsize), // src
                    _itemsize * _shape[1],                       // limbs to copy
                    current_row._data.begin()                    // dst
                );

                fixed_point_inner_product(
                    std::begin(current_col._data), // src1
                    std::begin(current_row._data), // src2
                    std::begin(result._data)       // dst
                        + (y * res_cols + x) * result._itemsize,
                    current_col._itemsize, // src1_limbs
                    current_row._itemsize, // src2_limbs
                    result._itemsize,      // dst_limbs
                    current_col._nitems    // n_items
                );
            }
        }
        return result;

    } else { /* mode.has_value() */
        result._bits = mode->bits;
        result._int_bits = mode->int_bits;
        result.buffer_resize(result._shape, bits_to_limbs(mode->bits));

        for (std::size_t x = 0; x < res_cols; x++) {
            // Copy column from `rhs` and use as the current working column. As
            // reading columns from `rhs` is cache-inefficient, we like to do this
            // only once for each element in the resulting matrix.
            for (std::size_t row = 0; row < rhs._shape[0]; row++) {
                std::copy_n(
                    rhs._data.begin() + (x + row * res_cols) * rhs._itemsize,
                    rhs._itemsize,
                    current_col._data.begin() + row * rhs._itemsize
                );
            }
            for (std::size_t y = 0; y < res_shape[0]; y++) {
                // Copy current row from lhs (`*this`)
                std::copy_n(
                    _data.begin() + (y * _shape[1] * _itemsize), // src
                    _itemsize * _shape[1],                       // limbs to copy
                    current_row._data.begin()                    // dst
                );

                fixed_point_inner_product_accumulator(
                    std::begin(current_col._data),               // src1
                    std::begin(current_row._data),               // src2
                    std::begin(result._data)                     // dst
                        + (y * res_cols + x) * result._itemsize, //
                    current_col._itemsize,                       // src1_limbs
                    current_row._itemsize,                       // src2_limbs
                    result._itemsize,                            // dst_limbs
                    current_col._nitems,                         // n_items
                    bits() + rhs.bits(),                         // product_bits
                    int_bits() + rhs.int_bits(),                 // product_int_bits
                    *mode
                );
            }
        }

        return result;
    }
}

template <typename RANDOM_ACCESS_ITERATOR>
void APyFixedArray::_cast_correct_wl(
    RANDOM_ACCESS_ITERATOR output_it, int new_bits, int new_int_bits
) const
{
    auto result_itemsize = bits_to_limbs(new_bits);
    auto shift_amount = new_bits - new_int_bits - frac_bits();
    auto it_begin = output_it;

    // If item sizes are the same, copy the whole block
    if (_itemsize == result_itemsize) {
        // Copy data into the result
        std::copy_n(
            _data.begin(),       // src
            _itemsize * _nitems, // limbs to copy
            it_begin             // dst
        );
        // If shift if required
        if (shift_amount > 0) {
            // Short data optimization, single limb
            if (_itemsize == 1) {
                // For each scalar in the tensor...
                // Note that it is not possible to shift out any data here
                for (std::size_t i = 0; i < _nitems; i++) {
                    output_it[i] = _data[i] << shift_amount;
                }
                return; // early exit
            }
            unsigned limb_skip = shift_amount / _LIMB_SIZE_BITS;
            unsigned limb_shift = shift_amount % _LIMB_SIZE_BITS;
            // For each scalar in the tensor...
            // Note that it is not possible to shift out any data here
            for (std::size_t i = 0; i < _nitems; i++) {
                auto it_end = it_begin + result_itemsize; // output sentinel
                limb_vector_lsl_inner(
                    it_begin, it_end, limb_skip, limb_shift, result_itemsize
                );
                it_begin = it_end;
            }
        }
        return; // early return
    }

    auto data_begin = _data.begin();
    // Shift if required
    if (shift_amount > 0) {
        // Compute limb skip and shift once
        unsigned limb_skip = shift_amount / _LIMB_SIZE_BITS;
        unsigned limb_shift = shift_amount % _LIMB_SIZE_BITS;
        // For each scalar in the tensor...
        for (std::size_t i = 0; i < _nitems; i++) {
            auto it_end = it_begin + result_itemsize; // output sentinel
            auto data_end = data_begin + _itemsize;
            // Copy data into the result
            std::copy_n(
                data_begin, // src
                _itemsize,  // limbs to copy
                it_begin    // dst
            );

            bool result_is_negative = limb_vector_is_negative(data_begin, data_end);
            std::fill(
                it_begin + _itemsize, it_end, result_is_negative ? mp_limb_t(-1) : 0
            );
            // Shift if required
            // Note that it is not possible to shift out any data here
            limb_vector_lsl_inner(
                it_begin, it_end, limb_skip, limb_shift, result_itemsize
            );
            it_begin = it_end;
            data_begin = data_end;
        }
    } else {
        // For each scalar in the tensor...
        // Same as above, but no shift so only copy and sign-extend
        for (std::size_t i = 0; i < _nitems; i++) {
            auto it_end = it_begin + result_itemsize; // output sentinel
            auto data_end = data_begin + _itemsize;
            // Copy data into the result
            std::copy_n(
                data_begin, // src
                _itemsize,  // limbs to copy
                it_begin    // dst
            );

            bool result_is_negative = limb_vector_is_negative(data_begin, data_end);
            std::fill(
                it_begin + _itemsize, it_end, result_is_negative ? mp_limb_t(-1) : 0
            );
            it_begin = it_end;
            data_begin = data_end;
        }
    }
}

template <typename RANDOM_ACCESS_ITERATOR>
void APyFixedArray::_cast(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    APyFixed& caster,
    int new_bits,
    int new_int_bits,
    QuantizationMode quantization,
    OverflowMode overflow
) const
{
    (void)it_end;

    // For each scalar in the tensor...
    std::size_t result_limbs = bits_to_limbs(new_bits);
    std::size_t pad_limbs = bits_to_limbs(std::max(new_bits, _bits)) - result_limbs;
    auto data_begin = _data.begin();
    auto it_start = it_begin;
    for (std::size_t i = 0; i < _nitems; i++) {
        // Copy data into caster `APyFixed`. No sign-extension.
        std::copy_n(
            data_begin,          // src
            _itemsize,           // limbs to copy
            caster._data.begin() // dst
        );

        auto it_end_ = it_start + result_limbs;

        // Perform the resizing
        caster._cast(
            it_start,            // output start
            it_end_ + pad_limbs, // output sentinel
            new_bits,
            new_int_bits,
            quantization,
            overflow
        );
        data_begin += _itemsize;
        it_start = it_end_;
    }
}

void APyFixedArray::_set_bits_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
#define CHECK_AND_SET_BITS_FROM_NPTYPE(__TYPE__)                                       \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                mp_limb_t data;                                                        \
                if (std::is_signed<__TYPE__>::value) {                                 \
                    data = static_cast<mp_limb_signed_t>(ndarray_view.data()[i]);      \
                } else {                                                               \
                    data = static_cast<mp_limb_t>(ndarray_view.data()[i]);             \
                }                                                                      \
                _data[i * _itemsize] = data;                                           \
                if (_itemsize > 0) {                                                   \
                    std::fill_n(                                                       \
                        _data.begin() + i * _itemsize + 1,                             \
                        _itemsize - 1,                                                 \
                        mp_limb_signed_t(data) < 0 ? -1 : 0                            \
                    );                                                                 \
                }                                                                      \
            }                                                                          \
            return; /* Conversion completed, exit `_set_bits_from_ndarray()` */        \
        }                                                                              \
    } while (0)

    // Each `CHECK_AND_SET_BITS_FROM_NPTYPE` checks the dtype of `ndarray` and
    // converts all the data if it matches. If successful,
    // `CHECK_AND_SET_BITS_FROM_NPTYPES` returns. Otherwise, the next attempted
    // conversion will take place
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int64_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int32_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int16_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int8_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint64_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint32_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint16_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint8_t);

    // None of the `CHECK_AND_SET_BITS_FROM_NPTYPE` succeeded. Unsupported type,
    // throw an error. If possible, it would be nice to show a string representation
    // of the `dtype`. Seems hard to achieve with nanobind, but please fix this if
    // you find out how this can be achieved.
    throw nb::type_error(
        "APyFixedArray::_set_bits_from_ndarray(): "
        "expecting integer `dtype`"
    );
}

void APyFixedArray::_set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
#define CHECK_AND_SET_VALUES_FROM_NPTYPE(__TYPE__)                                     \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            if (unsigned(_bits) <= _LIMB_SIZE_BITS) {                                  \
                int fr_bits = frac_bits();                                             \
                unsigned limb_shift_val = bits() & (_LIMB_SIZE_BITS - 1);              \
                auto shft_amnt = _LIMB_SIZE_BITS - limb_shift_val;                     \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    double data = static_cast<double>(ndarray_view.data()[i]);         \
                    _data[i] = get_data_from_double(data, bits(), fr_bits, shft_amnt); \
                }                                                                      \
            } else {                                                                   \
                APyFixed caster(bits(), int_bits());                                   \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    double data = static_cast<double>(ndarray_view.data()[i]);         \
                    caster.set_from_double(data);                                      \
                    std::copy_n(                                                       \
                        caster._data.begin(), _itemsize, _data.begin() + i * _itemsize \
                    );                                                                 \
                }                                                                      \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

    // Each `CHECK_AND_SET_VALUES_FROM_NPTYPE` checks the dtype of `ndarray` and
    // converts all the data if it matches. If successful,
    // `CHECK_AND_SET_VALUES_FROM_NPTYPES` returns. Otherwise, the next attempted
    // conversion will take place
    CHECK_AND_SET_VALUES_FROM_NPTYPE(double);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(float);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int64_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int32_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int16_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int8_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint64_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint32_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint16_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint8_t);

    // None of the `CHECK_AND_VALUES_FROM_NPTYPE` succeeded. Unsupported type, throw
    // an error. If possible, it would be nice to show a string representation of
    // the `dtype`. Seems hard to achieve with nanobind, but please fix this if you
    // find out how this can be achieved.
    throw nb::type_error(
        "APyFixedArray::_set_values_from_ndarray(): "
        "unsupported `dtype` expecting integer/float"
    );
}
