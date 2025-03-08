// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

#include "apybuffer.h"
#include "apyfixed.h"
#include "apyfixed_util.h"
#include "apyfixedarray.h"
#include "apytypes_common.h"
#include "apytypes_mp.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "broadcast.h"
#include "python_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

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
#include <utility>   // std::move
#include <variant>   // std::variant
#include <vector>    // std::vector, std::swap

#include <fmt/format.h>

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

    // 1D vector of Python int object (`nb::int_` objects)
    auto python_ints = python_sequence_walk<nb::int_>(bit_pattern_sequence);

    for (std::size_t i = 0; i < _data.size() / _itemsize; i++) {
        nb::int_ python_int = nb::cast<nb::int_>(python_ints[i]);
        auto limb_vec = python_long_to_limb_vec(python_int, _itemsize);
        _overflow_twos_complement(
            std::begin(limb_vec), std::end(limb_vec), _bits, _int_bits
        );
        std::copy_n(std::begin(limb_vec), _itemsize, std::begin(_data) + i * _itemsize);
    }
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyFixedArray::APyFixedArray(
    const std::vector<std::size_t>& shape, int bits, int int_bits
)
    : APyArray(shape, bits_to_limbs(bits))
    , _bits { bits }
    , _int_bits { int_bits }
{
}

APyFixedArray::APyFixedArray(
    const std::vector<std::size_t>& shape, int bits, int int_bits, vector_type&& v
)
    : APyArray(shape, bits_to_limbs(bits), std::move(v))
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
    : APyArray(shape, bits_to_limbs(bits_from_optional(bits, int_bits, frac_bits)))
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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
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
        const apy_limb_t* src1_ptr;
        const apy_limb_t* src2_ptr;
        if (frac_bits() == rhs.frac_bits()) {
            // Right-hand side and left-hand side have equally many fractional bits
            src1_ptr = &_data[0];
            src2_ptr = &rhs._data[0];
        } else if (frac_bits() <= rhs.frac_bits()) {
            // Right-hand side has more fractional bits. Upsize `*this`
            _cast_no_quantize_no_overflow(
                std::begin(_data),               // src
                std::begin(result._data),        // dst
                _itemsize,                       // src_limbs
                result._itemsize,                // dst_limbs
                _nitems,                         // n_items
                result.frac_bits() - frac_bits() // left_shift_amount
            );
            src1_ptr = &result._data[0];
            src2_ptr = &rhs._data[0];
        } else {
            // Left-hand side has more fractional bits. Upsize `rhs`
            _cast_no_quantize_no_overflow(
                std::begin(rhs._data),               // src
                std::begin(result._data),            // dst
                rhs._itemsize,                       // src_limbs
                result._itemsize,                    // dst_limbs
                rhs._nitems,                         // n_items
                result.frac_bits() - rhs.frac_bits() // left_shift_amount
            );
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
    _cast_no_quantize_no_overflow(
        std::begin(_data),               // src
        std::begin(result._data),        // dst
        _itemsize,                       // src_limbs
        result._itemsize,                // dst_limbs
        _nitems,                         // n_items
        result.frac_bits() - frac_bits() // left_shift_amount
    );
    _cast_no_quantize_no_overflow(
        std::begin(rhs._data),            // src
        std::begin(imm._data),            // dst
        rhs._itemsize,                    // src_limbs
        imm._itemsize,                    // dst_limbs
        rhs._nitems,                      // n_items
        imm.frac_bits() - rhs.frac_bits() // left_shift_amount
    );

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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
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
    auto lhs_shift_amount = unsigned(res_frac_bits - frac_bits());
    _cast_no_quantize_no_overflow(
        std::begin(_data),        // src
        std::begin(result._data), // dst
        _itemsize,                // src_limbs
        result._itemsize,         // dst_limbs
        _nitems,                  // n_items
        lhs_shift_amount          // left_shift_amount
    );
    _cast_no_quantize_no_overflow(
        std::begin(rhs._data),
        std::end(rhs._data),
        std::begin(imm._data),
        std::end(imm._data),
        rhs_shift_amount
    );
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        // Perform ripple-carry operation
        ripple_carry_op {}(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[0],    // src2
            result._itemsize  // limb vector length
        );
    }

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
        apy_add_n_functor<>,
        simd::add_functor<>,
        simd::shift_add_functor<>>(rhs);
}

APyFixedArray APyFixedArray::operator+(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<
        apy_add_n_functor<>,
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
        apy_sub_n_functor<>,
        simd::sub_functor<>,
        simd::shift_sub_functor<>>(rhs);
}

APyFixedArray APyFixedArray::operator-(const APyFixed& rhs) const
{
    return _apyfixed_base_add_sub<
        apy_sub_n_functor<>,
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
    _cast_no_quantize_no_overflow(
        std::begin(_data),               // src
        std::begin(result._data),        // dst
        _itemsize,                       // src_limbs
        result._itemsize,                // dst_limbs
        _nitems,                         // n_items
        result.frac_bits() - frac_bits() // left_shift_amount
    );
    auto lhs_shift_amount = unsigned(res_frac_bits - lhs.frac_bits());
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        apy_limb_t operand = lhs._data[0] << lhs_shift_amount;
        simd::vector_rsub_const(
            result._data.begin(), operand, result._data.begin(), result._data.size()
        );
    } else {
        APyFixed imm(res_bits, res_int_bits);
        _cast_no_quantize_no_overflow(
            std::begin(lhs._data), // src_begin
            std::end(lhs._data),   // src_end
            std::begin(imm._data), // dst_begin
            std::end(imm._data),   // dst_end
            lhs_shift_amount
        );

        // Perform subtraction
        for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
            apy_inplace_reversed_subtraction_same_length(
                &result._data[i], // dst/src2
                &imm._data[0],    // src1
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
    const int res_bits = bits() + rhs.bits();

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);

    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        // Special case #1: The resulting number of bits fit in a single limb
        simd::vector_mul(
            std::begin(_data),        // src1
            std::begin(rhs._data),    // src2
            std::begin(result._data), // dst
            result._data.size()       // elements
        );
    } else if (unsigned(bits()) <= APY_LIMB_SIZE_BITS
               && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
        // Special case #2: Both arguments are single limb, result two limbs
        for (std::size_t i = 0; i < _nitems; i++) {
            auto [high, low] = long_signed_mult(_data[i], rhs._data[i]);
            result._data[i * 2 + 1] = high;
            result._data[i * 2 + 0] = low;
        }
    } else {
        // General case: This always works but is slower than the special cases.
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
    const int res_bits = bits() + rhs.bits();

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: The resulting number of bits fit in a single limb
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        simd::vector_mul_const(
            std::begin(_data),        // src1
            rhs._data[0],             // src2
            std::begin(result._data), // dst
            result._data.size()       // elements
        );
        return result; // early exit
    }

    // Special case #2: Both arguments are single limb, result two limbs
    if (unsigned(bits()) <= APY_LIMB_SIZE_BITS
        && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < _nitems; i++) {
            auto [high, low] = long_signed_mult(_data[i], rhs._data[0]);
            result._data[i * 2 + 1] = high;
            result._data[i * 2 + 0] = low;
        }
        return result;
    }

    // General case: This always works but is slower than the special cases.
    auto op2_begin = rhs._data.begin();
    auto op2_end = rhs._data.begin() + rhs.vector_size();
    std::vector<apy_limb_t> op2_abs(rhs.vector_size());
    // Compute the absolute value of operand, as required by multiplication algorithm
    bool sign2 = limb_vector_abs(op2_begin, op2_end, op2_abs.begin());

    // Perform multiplication for each element in the tensor.
    // `apy_unsigned_multiplication` requires: "The destination has to have space for
    // `s1n` + `s2n` limbs, even if the product’s most significant limbs are zero."
    std::vector<apy_limb_t> res_tmp_vec(_itemsize + rhs.vector_size(), 0);
    std::vector<apy_limb_t> op1_abs(_itemsize);
    auto op1_begin = _data.begin();
    for (std::size_t i = 0; i < _nitems; i++) {
        // Current working operands
        auto op1_end = op1_begin + _itemsize;

        // Compute the absolute value of operand, as required by multiplication
        // algorithm
        bool sign1 = limb_vector_abs(op1_begin, op1_end, op1_abs.begin());

        // Evaluate resulting sign
        bool result_sign = sign1 ^ sign2;

        // Perform the multiplication
        if (op1_abs.size() < op2_abs.size()) {
            apy_unsigned_multiplication(
                &res_tmp_vec[0], // dst
                &op2_abs[0],     // src1
                op2_abs.size(),  // src1 limb vector length
                &op1_abs[0],     // src2
                op1_abs.size()   // src2 limb vector length
            );
        } else {
            apy_unsigned_multiplication(
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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
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
    ScratchVector<apy_limb_t> abs_den(rhs._itemsize);

    // Absolute value left-shifted numerator
    ScratchVector<apy_limb_t> abs_num(result._itemsize);

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

        // `apy_unsigned_division` requires the number of *significant* limbs in
        // denominator
        std::size_t den_significant_limbs
            = significant_limbs(std::begin(abs_den), std::end(abs_den));
        apy_unsigned_division(
            &result._data[i * result._itemsize], // Quotient
            &abs_num[0],                         // Numerator
            abs_num.size(),                      // Numerator limbs
            &abs_den[0],                         // Denominator
            den_significant_limbs                // Denominator significant limbs
        );

        // Negate result if negative
        if (num_sign ^ den_sign) {
            limb_vector_negate_inplace(
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize
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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
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

    // Absolute value denominator. `apy_unsigned_division` requires the number of
    // *significant* limbs in denominator
    ScratchVector<apy_limb_t> abs_den(rhs.vector_size());
    bool den_sign = limb_vector_abs(
        std::begin(rhs._data), std::end(rhs._data), std::begin(abs_den)
    );
    std::size_t den_significant_limbs
        = significant_limbs(std::begin(abs_den), std::end(abs_den));

    // Absolute value left-shifted numerator
    ScratchVector<apy_limb_t> abs_num(result._itemsize);

    for (std::size_t i = 0; i < _nitems; i++) {
        std::fill(std::begin(abs_num), std::end(abs_num), 0);
        bool num_sign = limb_vector_abs(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize,
            std::begin(abs_num)
        );
        limb_vector_lsl(abs_num.begin(), abs_num.end(), rhs.bits());

        // TODO: Add function so that APyDivInverse object can be reused for
        // denominator.
        apy_unsigned_division(
            &result._data[i * result._itemsize], // Quotient
            &abs_num[0],                         // Numerator
            abs_num.size(),                      // Numerator limbs
            &abs_den[0],                         // Denominator
            den_significant_limbs                // Denominator significant limbs
        );

        // Negate result if negative
        if (num_sign ^ den_sign) {
            limb_vector_negate_inplace(
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize
            );
        }
    }
    return result;
}

APyFixedArray APyFixedArray::rdiv(const APyFixed& lhs) const
{
    const int res_int_bits = lhs.int_bits() + frac_bits() + 1;
    const int res_frac_bits = lhs.frac_bits() + int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: The resulting number of bits fit in a single limb
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        simd::vector_rdiv_const_signed(
            std::begin(_data),        // src2 (denominator)
            lhs._data[0] << bits(),   // src1 (constant numerator)
            std::begin(result._data), // dst
            _data.size()              // vector elements
        );
        return result;
    }

    // General case: This always works but is slower than the special cases.

    // Absolute value denominator
    ScratchVector<apy_limb_t> abs_den(_itemsize);

    // Absolute value left-shifted numerator. As `apy_unsigned_division` alters the
    // numerator on call, we allocate twice its size. The first half [ `0`,
    // `result._itemsize` ) is passed to `apy_unsigned_division` and [
    // `result._item_size`, `2*result._itemsize` ) is used to cache the left-shifted
    // absolute numerator.
    ScratchVector<apy_limb_t> abs_num(2 * result._itemsize);
    bool num_sign = limb_vector_abs(
        std::begin(lhs._data),
        std::end(lhs._data),
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

        // `apy_unsigned_division` requires the number of *significant* limbs in
        // denominator
        std::size_t den_significant_limbs
            = significant_limbs(std::begin(abs_den), std::end(abs_den));
        apy_unsigned_division(
            &result._data[i * result._itemsize], // Quotient
            &abs_num[0],                         // Numerator
            result._itemsize,                    // Numerator limbs
            &abs_den[0],                         // Denominator
            den_significant_limbs                // Denominator significant limbs
        );

        // Negate result if negative
        if (num_sign ^ den_sign) {
            limb_vector_negate_inplace(
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize
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

/* ********************************************************************************** *
 * *                               Other methods                                    * *
 * ********************************************************************************** */

std::variant<APyFixedArray, APyFixed>
APyFixedArray::sum(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    int bits = _bits + bit_width(n_elems - 1);
    int int_bits = _int_bits + bit_width(n_elems - 1);
    std::size_t res_limbs = bits_to_limbs(bits);

    // Accumulation function
    auto fold = fold_accumulate<vector_type>(_itemsize, res_limbs);

    return array_fold(axes, fold, std::nullopt /* no init */, bits, int_bits);
}

APyFixedArray APyFixedArray::cumsum(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyFixedArray.cumsum: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Retrieve how many elements will be summed together
    std::size_t n_elems = axis.has_value() ? _shape[*axis] : _nitems;

    // Compute the result word length
    int bits = _bits + bit_width(n_elems - 1);
    int int_bits = _int_bits + bit_width(n_elems - 1);
    std::size_t res_limbs = bits_to_limbs(bits);

    // Accumulation function
    auto fold = fold_accumulate<vector_type>(_itemsize, res_limbs);

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(axis, fold, post_proc, std::nullopt, bits, int_bits);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::prod(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    int int_bits = _int_bits * n_elems;
    int frac_bits = (_bits - _int_bits) * n_elems;
    int bits = int_bits + frac_bits;
    std::size_t res_limbs = bits_to_limbs(bits);

    // Multiplicative fold function function
    ScratchVector<apy_limb_t, 32> scratch(2 * res_limbs + 2 * _itemsize);
    auto fold_func = fold_multiply<vector_type>(_itemsize, res_limbs, scratch);

    APyFixed init_one(_bits, _int_bits, { apy_limb_t(1) });
    return array_fold(axes, fold_func, init_one, bits, int_bits);
}

APyFixedArray APyFixedArray::cumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyFixedArray.cumprod: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Retrieve how many elements will be folded together
    std::size_t n_elems = axis.has_value() ? _shape[*axis] : _nitems;

    // Compute the result word length
    int int_bits = std::max(int(_int_bits * n_elems), _int_bits);
    int frac_bits = std::max(int((_bits - _int_bits) * n_elems), _bits - _int_bits);
    int bits = int_bits + frac_bits;
    std::size_t res_limbs = bits_to_limbs(bits);

    // Multiplicative fold function
    ScratchVector<apy_limb_t, 32> scratch(2 * res_limbs + 2 * _itemsize);
    auto fold_func = fold_multiply<vector_type>(_itemsize, res_limbs, scratch);

    // Post processing: adjust the binary point of each partial product
    auto post_proc = [&](auto dst_it, std::size_t n) {
        int frac_bits = _bits - _int_bits;
        int shift_amnt = frac_bits < 0 ? -frac_bits * n : frac_bits * (n_elems - 1 - n);
        limb_vector_lsl(dst_it, dst_it + res_limbs, shift_amnt);
    };

    APyFixed init_one(_bits, _int_bits, { apy_limb_t(1) });
    return array_fold_cumulative(axis, fold_func, post_proc, init_one, bits, int_bits);
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
    auto dot = inner_product_func_from_acc_mode<APyBuffer<apy_limb_t>::vector_type>(
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
APyFixedArray::max(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Min function
    std::function<void(vector_type::iterator, vector_type::const_iterator)> max_fold;
    if (unsigned(_bits) <= APY_LIMB_SIZE_BITS) {
        max_fold = [](auto dst_it, auto src_it) {
            *dst_it = apy_limb_signed_t(*src_it) < apy_limb_signed_t(*dst_it) ? *dst_it
                                                                              : *src_it;
        };
    } else {
        max_fold = [&](auto dst_it, auto src_it) {
            if (limb_vector_signed_less_than(dst_it, src_it, _itemsize)) {
                std::copy_n(src_it, _itemsize, dst_it);
            }
        };
    }

    auto init_min = APyFixed::get_min(_bits, _int_bits);
    return array_fold(axes, max_fold, init_min, _bits, _int_bits);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::min(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Min function
    std::function<void(vector_type::iterator, vector_type::const_iterator)> min_fold;
    if (unsigned(_bits) <= APY_LIMB_SIZE_BITS) {
        min_fold = [](auto dst_it, auto src_it) {
            *dst_it = apy_limb_signed_t(*src_it) < apy_limb_signed_t(*dst_it) ? *src_it
                                                                              : *dst_it;
        };
    } else {
        min_fold = [&](auto dst_it, auto src_it) {
            if (limb_vector_signed_less_than(src_it, dst_it, _itemsize)) {
                std::copy_n(src_it, _itemsize, dst_it);
            }
        };
    }

    auto init_max = APyFixed::get_max(_bits, _int_bits);
    return array_fold(axes, min_fold, init_max, _bits, _int_bits);
}

std::string APyFixedArray::repr() const
{
    std::stringstream ss {};
    ss << "APyFixedArray([";
    if (_shape[0]) {
        // Setup hex printing which will properly display the BCD characters
        ss << std::hex;

        std::vector<apy_limb_t> data(_itemsize, 0);
        for (std::size_t offset = 0; offset < _data.size(); offset += _itemsize) {
            std::copy_n(_data.begin() + offset, _itemsize, data.begin());

            // Zero sign bits outside of bit-range
            if (bits() % APY_LIMB_SIZE_BITS) {
                apy_limb_t and_mask
                    = (apy_limb_t(1) << (bits() % APY_LIMB_SIZE_BITS)) - 1;
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
    ss << ", " << "bits=" << std::dec << bits() << ", " << "int_bits=" << std::dec
       << int_bits() << ")";
    return ss.str();
}

APyFixedArray APyFixedArray::abs() const
{
    // Increase word length of result by one
    const int res_int_bits = _int_bits + 1;
    const int res_bits = _bits + 1;

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_bits, res_int_bits);
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        simd::vector_abs(result._data.begin(), _data.begin(), _data.size());
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
    /*
     * Specialization #1: Single limb `src` and `dst`
     */
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        simd::vector_neg(result._data.begin(), _data.begin(), _data.size());
        return result;
    }
    /*
     * Specialization #2: `src` and `dst` have equally many limbs
     */
    if (_itemsize == result._itemsize) {

        // Copy inverted data into the result
        simd::vector_not(result._data.begin(), _data.begin(), _itemsize * _nitems);
        auto it_begin = result._data.begin();
        for (std::size_t i = 0; i < _nitems; i++) {
            auto it_end = it_begin + result._itemsize;
            // Add one LSB to get negated results
            limb_vector_add_one_lsb_inplace(it_begin, it_end);
            it_begin = it_end;
        }
        return result; // early return specialization #2
    }

    /*
     * General case: `dst_limbs = src_limbs + 1`
     * Sign-extend as an additional limb is required
     */
    auto src_begin = _data.begin();
    auto dst_begin = result._data.begin();
    for (std::size_t i = 0; i < _nitems; i++) {
        auto src_end = src_begin + _itemsize;
        auto dst_end = dst_begin + result._itemsize;
        // Copy inverted data into the result
        std::transform(src_begin, src_end, dst_begin, std::bit_not {});
        std::fill(
            dst_begin + _itemsize,
            dst_end,
            // Add inverted sign-extension
            limb_vector_is_negative(src_begin, src_end) ? 0 : -1
        );
        // Add one LSB to get negated results
        limb_vector_add_one_lsb_inplace(dst_begin, dst_end);
        src_begin = src_end;
        dst_begin = dst_end;
    }
    return result;
}

APyFixedArray APyFixedArray::operator~() const
{
    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, _bits, _int_bits);
    simd::vector_not(result._data.begin(), _data.begin(), _itemsize * _nitems);
    return result;
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
    std::size_t dim, APyBuffer<apy_limb_t>::vector_type::const_iterator& it
) const
{
    nb::list result;
    if (dim == _ndim - 1) {
        // Most inner dimension: append data
        for (std::size_t i = 0; i < _shape[dim]; i++) {
            result.append(python_limb_vec_to_long(
                it,                         // start
                it + _itemsize,             // stop
                false,                      // vec_is_signed
                bits() % APY_LIMB_SIZE_BITS // bits_last_limb
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

    for (std::size_t i = 0; i < _nitems; i++) {
        auto it = std::begin(_data) + i * _itemsize;
        result_data[i] = fixed_point_to_double(it, it + _itemsize, _bits, _int_bits);
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (double*)p; });

    return nb::ndarray<nb::numpy, double>(result_data, _ndim, &_shape[0], owner);
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
    APyFixedArray::vector_type result_data(_nitems * result_limbs + pad_limbs);

    // Do the casting
    for (std::size_t i = 0; i < _nitems; i++) {
        _cast(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize,
            std::begin(result_data) + (i + 0) * result_limbs,
            std::begin(result_data) + (i + 1) * result_limbs + pad_limbs,
            _bits,
            _int_bits,
            new_bits,
            new_int_bits,
            quantization_mode,
            overflow_mode
        );
    }

    result_data.resize(_nitems * result_limbs);
    return APyFixedArray(_shape, new_bits, new_int_bits, std::move(result_data));
}

/* ********************************************************************************** *
 * *                      Static conversion from other types                          *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::from_numbers(
    const nb::sequence& number_seq,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    if (nb::isinstance<nb::ndarray<>>(number_seq)) {
        // Sequence is NDArray. Initialize using `from_array`
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(number_seq);
        return from_array(ndarray, int_bits, frac_bits, bits);
    }

    APyFixedArray result(
        python_sequence_extract_shape(number_seq), int_bits, frac_bits, bits
    );

    // Extract all Python doubles and integers
    auto py_obj
        = python_sequence_walk<nb::float_, nb::int_, APyFixed, APyFloat>(number_seq);

    // Set data from doubles (reuse `APyFixed::from_double` conversion)
    for (std::size_t i = 0; i < result._data.size() / result._itemsize; i++) {
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            // Python double object
            double d = static_cast<double>(nb::cast<nb::float_>(py_obj[i]));
            fixed_point_from_double(
                d,
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                result._bits,
                result._int_bits
            );
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            // Python integer object
            fixed_point_from_py_integer(
                nb::cast<nb::int_>(py_obj[i]),
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                result._bits,
                result._int_bits
            );
        } else if (nb::isinstance<APyFixed>(py_obj[i])) {
            const auto d = static_cast<APyFixed>(nb::cast<APyFixed>(py_obj[i]));
            _cast(
                std::begin(d._data),
                std::end(d._data),
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                d._bits,
                d._int_bits,
                result._bits,
                result._int_bits,
                QuantizationMode::RND_INF,
                OverflowMode::WRAP
            );
        } else if (nb::isinstance<APyFloat>(py_obj[i])) {
            const auto d
                = static_cast<APyFloat>(nb::cast<APyFloat>(py_obj[i])).to_fixed();
            _cast(
                std::begin(d._data),
                std::end(d._data),
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                d._bits,
                d._int_bits,
                result._bits,
                result._int_bits,
                QuantizationMode::RND_INF,
                OverflowMode::WRAP
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
    return full(shape, APyFixed::one(res_bits, res_int_bits));
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
    nb::tuple shape = nb::make_tuple(N, M.value_or(N));

    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    return diagonal(shape, APyFixed::one(res_bits, res_int_bits));
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

APyFixedArray APyFixedArray::arange(
    const nb::object& start,
    const nb::object& stop,
    const nb::object& step,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : res_bits - *frac_bits;

    const std::vector<APyFixed> apy_vals = ::arange(start, stop, step);
    APyFixedArray result({ apy_vals.size() }, res_bits, res_int_bits);

    for (std::size_t i = 0; i < apy_vals.size(); i++) {
        _cast(
            std::begin(apy_vals[i]._data),
            std::end(apy_vals[i]._data),
            std::begin(result._data) + i * result._itemsize,       // output start
            std::begin(result._data) + (i + 1) * result._itemsize, // output sentinel
            apy_vals[i]._bits,
            apy_vals[i]._int_bits,
            res_bits,
            res_int_bits,
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
        if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
            // Fastest path, no accumulation mode specified and result fit in a single
            // limb.
            result._data[0] = simd::vector_multiply_accumulate(
                _data.begin(), rhs._data.begin(), _data.size()
            );
            return result;
        } else { /* unsigned(res_bits) > APY_LIMB_SIZE_BITS */
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
        if (res_bits <= APY_LIMB_SIZE_BITS) {
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
        /*
         * Special case #2: No accumulator mode specified, resulting matrix elements fit
         * into two limbs, and each argument element fits into a single limb.
         * So far only supported for 64-bit limbs and compilation with gcc or clang.
         */
#if (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
        // GNU C-compatible compiler (including Clang and MacOS Xcode)
        if ((res_bits <= 2 * APY_LIMB_SIZE_BITS)
            && (std::size_t(bits()) <= APY_LIMB_SIZE_BITS)
            && (std::size_t(rhs.bits()) <= APY_LIMB_SIZE_BITS)) {
            for (std::size_t x = 0; x < res_cols; x++) {
                // Copy column from `rhs` and use as the current working column. As
                // reading columns from `rhs` is cache-inefficient, we like to do this
                // only once for each element in the resulting matrix.
                for (std::size_t row = 0; row < rhs._shape[0]; row++) {
                    current_col._data[row] = rhs._data[x + row * res_cols];
                }
                for (std::size_t y = 0; y < res_shape[0]; y++) {
                    __int128 tmp = 0;
                    for (std::size_t i = 0; i < _shape[1]; i++) {
                        tmp += (__int128)apy_limb_signed_t(_data[y * _shape[1] + i])
                            * (__int128)apy_limb_signed_t(current_col._data[i]);
                    }
                    result._data[2 * (y * res_cols + x) + 0] = apy_limb_t(tmp);
                    result._data[2 * (y * res_cols + x) + 1] = apy_limb_t(tmp >> 64);
                }
            }
            return result; // early exit
        }
#elif defined(_MSC_VER)
        // Microsoft Visual C/C++ compiler#endif
        if ((res_bits <= 2 * APY_LIMB_SIZE_BITS)
            && (std::size_t(bits()) <= APY_LIMB_SIZE_BITS)
            && (std::size_t(rhs.bits()) <= APY_LIMB_SIZE_BITS)) {
            for (std::size_t x = 0; x < res_cols; x++) {
                // Copy column from `rhs` and use as the current working column. As
                // reading columns from `rhs` is cache-inefficient, we like to do this
                // only once for each element in the resulting matrix.
                for (std::size_t row = 0; row < rhs._shape[0]; row++) {
                    current_col._data[row] = rhs._data[x + row * res_cols];
                }
                for (std::size_t y = 0; y < res_shape[0]; y++) {
                    apy_limb_t high_limb = 0, low_limb = 0;
                    for (std::size_t i = 0; i < _shape[1]; i++) {
                        auto [high_prod, low_prod] = long_signed_mult(
                            _data[y * _shape[1] + i], current_col._data[i]
                        );
                        low_limb += low_prod;
                        high_limb += high_prod + (low_limb < low_prod);
                    }
                    result._data[2 * (y * res_cols + x) + 0] = low_limb;
                    result._data[2 * (y * res_cols + x) + 1] = high_limb;
                }
            }
            return result; // early exit
        }
#endif
#elif (COMPILER_LIMB_SIZE == 32)
        if ((res_bits <= 2 * APY_LIMB_SIZE_BITS)
            && (std::size_t(bits()) <= APY_LIMB_SIZE_BITS)
            && (std::size_t(rhs.bits()) <= APY_LIMB_SIZE_BITS)) {
            for (std::size_t x = 0; x < res_cols; x++) {
                // Copy column from `rhs` and use as the current working column. As
                // reading columns from `rhs` is cache-inefficient, we like to do this
                // only once for each element in the resulting matrix.
                for (std::size_t row = 0; row < rhs._shape[0]; row++) {
                    current_col._data[row] = rhs._data[x + row * res_cols];
                }
                for (std::size_t y = 0; y < res_shape[0]; y++) {
                    // TODO: rewrite using SIMD?
                    std::int64_t res = 0;
                    for (std::size_t i = 0; i < _shape[1]; i++) {
                        res += (std::int64_t)apy_limb_signed_t(_data[y * _shape[1] + i])
                            * (std::int64_t)apy_limb_signed_t(current_col._data[i]);
                    }
                    result._data[2 * (y * res_cols + x) + 0] = apy_limb_t(res);
                    result._data[2 * (y * res_cols + x) + 1] = apy_limb_t(res >> 32);
                }
            }
            return result; // early exit
        }
#endif
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

void APyFixedArray::_set_bits_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
#define CHECK_AND_SET_BITS_FROM_NPTYPE(__TYPE__)                                       \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                apy_limb_t data;                                                       \
                if constexpr (std::is_signed<__TYPE__>::value) {                       \
                    data = static_cast<apy_limb_signed_t>(ndarray_view.data()[i]);     \
                } else {                                                               \
                    data = static_cast<apy_limb_t>(ndarray_view.data()[i]);            \
                }                                                                      \
                _data[i * _itemsize] = data;                                           \
                if (_itemsize >= 2) {                                                  \
                    std::fill_n(                                                       \
                        _data.begin() + i * _itemsize + 1,                             \
                        _itemsize - 1,                                                 \
                        apy_limb_signed_t(data) < 0 ? -1 : 0                           \
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
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                if constexpr (std::is_same_v<__TYPE__, float>                          \
                              || std::is_same_v<__TYPE__, double>) {                   \
                    fixed_point_from_double(                                           \
                        static_cast<double>(ndarray_view.data()[i]),                   \
                        std::begin(_data) + (i + 0) * _itemsize,                       \
                        std::begin(_data) + (i + 1) * _itemsize,                       \
                        _bits,                                                         \
                        _int_bits                                                      \
                    );                                                                 \
                } else {                                                               \
                    fixed_point_from_integer(                                          \
                        static_cast<std::uint64_t>(ndarray_view.data()[i]),            \
                        std::begin(_data) + (i + 0) * _itemsize,                       \
                        std::begin(_data) + (i + 1) * _itemsize,                       \
                        _bits,                                                         \
                        _int_bits                                                      \
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
