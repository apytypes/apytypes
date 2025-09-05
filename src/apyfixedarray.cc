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
#include "python_util.h"

// Python object access through Nanobind
#include <iterator>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

// Standard header includes
#include <algorithm> // std::copy, std::max, std::transform, etc...
#include <cstddef>   // std::size_t
#include <cstdint>   // std::int16, std::int32, std::int64, etc...
#include <optional>  // std::optional
#include <set>       // std::set
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
    const nb::typed<nb::iterable, nb::any>& bit_pattern_sequence,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyFixedArray(
          python_iterable_extract_shape(bit_pattern_sequence, "APyFixedArray.__init__"),
          int_bits,
          frac_bits,
          bits
      )
{
    // Specialized initialization for NDArray
    if (nb::isinstance<nb::ndarray<>>(bit_pattern_sequence)) {
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(bit_pattern_sequence);
        _set_bits_from_ndarray(ndarray);
        return; // initialization completed
    }

    // 1D vector of Python int object (`nb::int_` objects)
    auto python_ints = python_iterable_walk<nb::int_>(
        bit_pattern_sequence, "APyFixedArray.__init__"
    );

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
inline APyFixedArray
APyFixedArray::_apyfixedarray_base_add_sub(const APyFixedArray& rhs) const
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
        return try_broadcast_and_then<std::plus<>>(rhs, "__add__");
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
        return try_broadcast_and_then<std::minus<>>(rhs, "__sub__");
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
        return try_broadcast_and_then<std::multiplies<>>(rhs, "__mul__");
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
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::divides<>>(rhs, "__truediv__");
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

    assert(den_significant_limbs > 0);
    assert(result._itemsize >= den_significant_limbs);

    // Absolute value left-shifted numerator
    ScratchVector<apy_limb_t> abs_num(result._itemsize);

    // Compute inverse
    auto inv = APyDivInverse(&abs_den[0], den_significant_limbs);

    // Normalize denominator
    apy_limb_t carry
        = apy_inplace_left_shift(&abs_den[0], den_significant_limbs, inv.norm_shift);
    assert(carry == 0);
    (void)carry; // Avoid unused-warning
    for (std::size_t i = 0; i < _nitems; i++) {
        std::fill(std::begin(abs_num), std::end(abs_num), 0);
        bool num_sign = limb_vector_abs(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize,
            std::begin(abs_num)
        );
        limb_vector_lsl(abs_num.begin(), abs_num.end(), rhs.bits());
        auto quotient = &result._data[i * result._itemsize];

        apy_unsigned_division_preinverted(
            quotient,              // Quotient
            &abs_num[0],           // Numerator
            result._itemsize,      // Numerator limbs
            &abs_den[0],           // Denominator
            den_significant_limbs, // Denominator limbs
            &inv                   // Inverse
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

std::variant<APyFixedArray, APyFixed>
APyFixedArray::matmul(const APyFixedArray& rhs) const
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
    throw std::length_error(
        fmt::format(
            "APyFixedArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
            tuple_string_from_vec(_shape),
            tuple_string_from_vec(rhs._shape)
        )
    );
}

/* ********************************************************************************** *
 * *                               Other methods                                    * *
 * ********************************************************************************** */

std::variant<APyFixedArray, APyFixed>
APyFixedArray::sum(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    int pad_bits = n_elems ? bit_width(n_elems - 1) : 0;
    int bits = _bits + pad_bits;
    int int_bits = _int_bits + pad_bits;
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
    int pad_bits = n_elems ? bit_width(n_elems - 1) : 0;
    int bits = _bits + pad_bits;
    int int_bits = _int_bits + pad_bits;
    std::size_t res_limbs = bits_to_limbs(bits);

    // Accumulation function
    auto fold = fold_accumulate<vector_type>(_itemsize, res_limbs);

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(axis, fold, post_proc, std::nullopt, bits, int_bits);
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::prod(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    if (n_elems == 0) {
        // Empty array, return scalar one (NumPy semantics)
        return APyFixed::one(_bits, _int_bits);
    } else {
        // Non-empty array
        int int_bits = n_elems * _int_bits;
        int frac_bits = n_elems * (_bits - _int_bits);
        int bits = int_bits + frac_bits;
        std::size_t res_limbs = bits_to_limbs(bits);

        // Multiplicative fold function function
        ScratchVector<apy_limb_t, 32> scratch(2 * res_limbs + 2 * _itemsize);
        auto fold_func = fold_multiply<vector_type>(_itemsize, res_limbs, scratch);

        APyFixed init_one(_bits, _int_bits, { apy_limb_t(1) });
        return array_fold(axes, fold_func, init_one, bits, int_bits);
    }
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
APyFixedArray::convolve(const APyFixedArray& other, const std::string& conv_mode) const
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

    // Get convolution properties from the convolution mode
    auto [len, n_left, n_right] = get_conv_lengths(conv_mode, a, b);

    int pad_bits = b->_shape[0] ? bit_width(b->_shape[0] - 1) : 0;
    int res_bits = a->bits() + b->bits() + pad_bits;
    int res_int_bits = a->int_bits() + b->int_bits() + pad_bits;
    std::optional<APyFixedAccumulatorOption> acc = get_accumulator_mode_fixed();
    if (acc.has_value()) {
        res_bits = acc->bits;
        res_int_bits = acc->int_bits;
    }

    APyFixedArray res({ len }, res_bits, res_int_bits);

    auto inner_product = FixedPointInnerProduct(a->spec(), b->spec(), res.spec(), acc);

    // Loop working variables
    std::size_t n = b->_shape[0] - n_left;
    auto dst = std::begin(res._data);
    auto src1 = std::cbegin(a->_data);
    auto src2 = std::cbegin(b->_data) + n_left * b->_itemsize;

    // `b` limits length of the inner product length
    for (std::size_t i = 0; i < n_left; i++) {
        inner_product(src1, src2, dst, n);
        src2 -= b->_itemsize;
        dst += res._itemsize;
        n++;
    }

    // full inner product length
    for (std::size_t i = 0; i < a->_shape[0] - b->_shape[0] + 1; i++) {
        inner_product(src1, src2, dst, n);
        src1 += a->_itemsize;
        dst += res._itemsize;
    }

    // `a` limits length of the inner product length
    for (std::size_t i = 0; i < n_right; i++) {
        n--;
        inner_product(src1, src2, dst, n);
        src1 += a->_itemsize;
        dst += res._itemsize;
    }

    return res;
}

std::variant<APyFixedArray, APyFixed>
APyFixedArray::max(const std::optional<PyShapeParam_t>& py_axis) const
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
APyFixedArray::min(const std::optional<PyShapeParam_t>& py_axis) const
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
    const auto formatter = [bits = _bits](auto cbegin_it, auto cend_it) -> std::string {
        std::vector<apy_limb_t> data(std::distance(cbegin_it, cend_it));
        std::copy(cbegin_it, cend_it, std::begin(data));

        // Zero sign bits outside of bit-range
        if (bits % APY_LIMB_SIZE_BITS) {
            apy_limb_t and_mask = (apy_limb_t(1) << (bits % APY_LIMB_SIZE_BITS)) - 1;
            data.back() &= and_mask;
        }
        return bcds_to_string(double_dabble(data));
    };
    return array_repr(
        { formatter },
        { fmt::format("int_bits={}", int_bits()),
          fmt::format("frac_bits={}", frac_bits()) }
    );
}

std::string APyFixedArray::to_string_dec() const
{
    const auto formatter
        = [frac_bits = _bits - _int_bits](auto cbegin_it, auto cend_it) -> std::string {
        double fixed_as_double = fixed_point_to_double(cbegin_it, cend_it, frac_bits);
        return fmt::format("{}", fixed_as_double);
    };

    return array_format(formatter, 88, false);
}

std::string APyFixedArray::to_string(int base) const
{
    switch (base) {
    case 10:
        return to_string_dec();
    default:
        auto msg = fmt::format("APyFixedArray.__str__: base={} is not supported", base);
        throw nb::value_error(msg.c_str());
    }
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
    nb::ndarray<nb::numpy, std::uint64_t>,
    nb::ndarray<nb::numpy, std::uint32_t>,
    nb::ndarray<nb::numpy, std::uint16_t>,
    nb::ndarray<nb::numpy, std::uint8_t>>
APyFixedArray::to_bits(bool numpy) const
{
    if (numpy) {
        if (bits() <= 8) {
            return to_bits_ndarray<nb::numpy, std::uint8_t>();
        } else if (bits() <= 16) {
            return to_bits_ndarray<nb::numpy, std::uint16_t>();
        } else if (bits() <= 32) {
            return to_bits_ndarray<nb::numpy, std::uint32_t>();
        } else {
            return to_bits_ndarray<nb::numpy, std::uint64_t>();
        }
    } else {
        auto it = std::cbegin(_data);
        return to_bits_python_recursive_descent(0, it);
    }
}

nb::list APyFixedArray::to_bits_python_recursive_descent(
    std::size_t dim,
    APyBuffer<apy_limb_t>::vector_type::const_iterator& it,
    bool vec_is_signed
) const
{
    nb::list result;
    if (dim == _ndim - 1) {
        // Most inner dimension: append data
        for (std::size_t i = 0; i < _shape[dim]; i++) {
            result.append(python_limb_vec_to_long(
                it,                         // start
                it + _itemsize,             // stop
                vec_is_signed,              // vec_is_signed
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

nb::list APyFixedArray::to_signed_bits() const
{
    auto it = std::cbegin(_data);
    return to_bits_python_recursive_descent(0, it, true);
}

nb::ndarray<nb::numpy, double>
APyFixedArray::to_numpy(std::optional<nb::object> dtype, std::optional<bool> copy) const
{
    (void)dtype;

    if (!copy.value_or(true)) {
        throw nb::value_error("APyFixedArray.to_numpy: copy must be True");
    }

    // Dynamically allocate data to be passed to python
    double* result_data = new double[_nitems];

    auto _frac_bits = frac_bits();
    for (std::size_t i = 0; i < _nitems; i++) {
        auto it = std::begin(_data) + i * _itemsize;
        result_data[i] = fixed_point_to_double(it, it + _itemsize, _frac_bits);
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

    // Do the casting: `fixed_point_cast_unsafe` is safe to use because of `pad_limbs`
    for (std::size_t i = 0; i < _nitems; i++) {
        fixed_point_cast_unsafe(
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
    const nb::typed<nb::iterable, nb::any>& number_seq,
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
        python_iterable_extract_shape(number_seq, "APyFixedArray.from_float"),
        int_bits,
        frac_bits,
        bits
    );

    // Extract all Python doubles and integers
    auto py_objs = python_iterable_walk<nb::float_, nb::int_, APyFixed, APyFloat>(
        number_seq, "APyFixedArray.from_float"
    );

    // Function used to set from a floating-point
    unsigned float_shift {};
    std::function<void(APyFixedArray&, std::size_t, double, unsigned)> from_fp;
    if (result._itemsize == 1) {
        float_shift = 64 - (result._bits & (64 - 1));
        from_fp = [](APyFixedArray& res, std::size_t i, double val, unsigned shift) {
            res._data[i]
                = fixed_point_from_double_single_limb(val, res.frac_bits(), shift);
        };
    } else { /* result._itemsize > 1 */
        assert(result._itemsize > 1);
        from_fp = [](APyFixedArray& res, std::size_t i, double val, unsigned) {
            fixed_point_from_double(
                val,
                std::begin(res._data) + (i + 0) * res._itemsize,
                std::begin(res._data) + (i + 1) * res._itemsize,
                res._bits,
                res._int_bits
            );
        };
    }

    // Function used to set from a fixed-point
    auto from_fx = [](APyFixedArray& res, std::size_t i, const APyFixed& fx) {
        fixed_point_cast(
            std::begin(fx._data),
            std::end(fx._data),
            std::begin(res._data) + (i + 0) * res._itemsize,
            std::begin(res._data) + (i + 1) * res._itemsize,
            fx._bits,
            fx._int_bits,
            res._bits,
            res._int_bits,
            QuantizationMode::RND_INF,
            OverflowMode::WRAP
        );
    };

    // Iterate over objects and set in fixed-point array
    for (std::size_t i = 0; i < result._nitems; i++) {
        if (nb::isinstance<nb::float_>(py_objs[i])) {
            // Python double object
            double value = static_cast<double>(nb::cast<nb::float_>(py_objs[i]));
            from_fp(result, i, value, float_shift);
        } else if (nb::isinstance<nb::int_>(py_objs[i])) {
            // Python integer object
            fixed_point_from_py_integer(
                nb::cast<nb::int_>(py_objs[i]),
                std::begin(result._data) + (i + 0) * result._itemsize,
                std::begin(result._data) + (i + 1) * result._itemsize,
                result._bits,
                result._int_bits
            );
        } else if (nb::isinstance<APyFixed>(py_objs[i])) {
            const APyFixed& fx = nb::cast<const APyFixed&>(py_objs[i]);
            from_fx(result, i, fx);
        } else if (nb::isinstance<APyFloat>(py_objs[i])) {
            const APyFixed& fx = nb::cast<const APyFloat&>(py_objs[i]).to_fixed();
            from_fx(result, i, fx);
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
    const PyShapeParam_t& shape,
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
    const PyShapeParam_t& shape,
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
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;

    // Use N for both dimensions if M is not provided
    PyShapeTuple_t shape = PyShapeTuple_t(nb::make_tuple(N, M.value_or(N)));
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
        fixed_point_cast(
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

APyFixedArray APyFixedArray::fullrange(
    const nb::object& start,
    const nb::object& stop,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : res_bits - *frac_bits;

    const APyFixed apy_start = APyFixed::from_number(start, int_bits, frac_bits, bits);
    const APyFixed apy_stop = APyFixed::from_number(stop, int_bits, frac_bits, bits);

    std::vector<APyFixed> apy_vals;

    APyFixed curr_val = apy_start;
    while (curr_val < apy_stop) {
        apy_vals.push_back(curr_val);
        curr_val.increment_lsb();
    }

    APyFixedArray result({ apy_vals.size() }, res_bits, res_int_bits);

    for (std::size_t i = 0; i < apy_vals.size(); i++) {
        std::copy_n(
            std::begin(apy_vals[i]._data),
            result._itemsize,
            std::begin(result._data) + i * result._itemsize
        );
    }

    return result;
}

/* ********************************************************************************** *
 * *                            Private member functions                            * *
 * ********************************************************************************** */

APyFixed APyFixedArray::_checked_inner_product(
    const APyFixedArray& rhs,                     // rhs
    std::optional<APyFixedAccumulatorOption> mode // optional accumulation mode
) const
{
    int pad_bits = _shape[0] ? bit_width(_shape[0] - 1) : 0;
    int res_bits = bits() + rhs.bits() + pad_bits;
    int res_int_bits = int_bits() + rhs.int_bits() + pad_bits;
    if (mode.has_value()) {
        res_bits = mode->bits;
        res_int_bits = mode->int_bits;
    }

    APyFixedArray res_arr({ 1 }, res_bits, res_int_bits);

    auto inner_product
        = FixedPointInnerProduct(spec(), rhs.spec(), res_arr.spec(), mode);

    inner_product(
        std::begin(_data),         // src1
        std::begin(rhs._data),     // src2
        std::begin(res_arr._data), // dst
        _nitems
    );

    APyFixed res(res_bits, res_int_bits);
    for (std::size_t i = 0; i < res_arr._data.size(); i++) {
        res._data[i] = res_arr._data[i];
    }
    return res;
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
    std::size_t pad_bits = _shape[1] ? bit_width(_shape[1] - 1) : 0;
    std::size_t res_bits = bits() + rhs.bits() + pad_bits;
    std::size_t res_int_bits = int_bits() + rhs.int_bits() + pad_bits;
    if (mode.has_value()) {
        res_bits = mode->bits;
        res_int_bits = mode->int_bits;
    }

    // Resulting tensor and a working column from `rhs`
    APyFixedArray res(res_shape, res_bits, res_int_bits);
    APyFixedArray current_col({ rhs._shape[0] }, rhs.bits(), rhs.int_bits());

    auto inner_product = FixedPointInnerProduct(spec(), rhs.spec(), res.spec(), mode);
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

        // dst = A x b
        inner_product(
            std::begin(_data),                         // src1, A: [M x N]
            std::begin(current_col._data),             // src2, b: [N x 1]
            std::begin(res._data) + res._itemsize * x, // dst
            _shape[1],                                 // N
            res_shape[0],                              // M
            res_cols                                   // DST_STEP
        );
    }

    return res;
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
        "APyFixedArray.__init__: unsupported `dtype`, expecting integer"
    );
}

void APyFixedArray::_set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
#define CHECK_AND_SET_VALUES_FROM_FLOAT_NPTYPE(__TYPE__)                               \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            if (_itemsize == 1) {                                                      \
                unsigned limb_shift_val = bits() & (64 - 1);                           \
                unsigned twos_complement_shift = 64 - limb_shift_val;                  \
                int _frac_bits = frac_bits();                                          \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    _data[i] = fixed_point_from_double_single_limb(                    \
                        view.data()[i], _frac_bits, twos_complement_shift              \
                    );                                                                 \
                }                                                                      \
            } else { /* _itemsize > 1 */                                               \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    fixed_point_from_double(                                           \
                        static_cast<double>(view.data()[i]),                           \
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

#define CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(__TYPE__)                                 \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                fixed_point_from_integer(                                              \
                    static_cast<std::uint64_t>(ndarray_view.data()[i]),                \
                    std::begin(_data) + (i + 0) * _itemsize,                           \
                    std::begin(_data) + (i + 1) * _itemsize,                           \
                    _bits,                                                             \
                    _int_bits                                                          \
                );                                                                     \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

    // Each `CHECK_AND_SET_VALUES_FROM_NPTYPE` checks the dtype of `ndarray` and
    // converts all the data if it matches. If successful, the macro returns. Otherwise,
    // the next attempted conversion will take place.
    CHECK_AND_SET_VALUES_FROM_FLOAT_NPTYPE(double);
    CHECK_AND_SET_VALUES_FROM_FLOAT_NPTYPE(float);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int64_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int32_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int16_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::int8_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint64_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint32_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint16_t);
    CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(std::uint8_t);

#undef CHECK_AND_SET_VALUES_FROM_FLOAT_NPTYPE
#undef CHECK_AND_SET_VALUES_FROM_INT_NPTYPE

    // None of the `CHECK_AND_SET_VALUES_FROM_*_NPTYPE` succeeded. Unsupported type,
    // throw an error. If possible, it would be nice to show a string representation of
    // the `dtype`. Seems hard to achieve with nanobind, but please fix this if you
    // find out how this can be achieved.
    throw nb::type_error(
        "APyFixedArray.from_array: unsupported `dtype`, expecting float or integer"
    );
}
