// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

#include "apybuffer.h"
#include "apycfixed.h"
#include "apycfixedarray.h"
#include "apyfixed_util.h"
#include "apyfixedarray.h"
#include "apytypes_common.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "broadcast.h"
#include "python_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/complex.h>
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
#include <variant>   // std::variant
#include <vector>    // std::vector, std::swap

#include <fmt/format.h>

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

APyCFixedArray::APyCFixedArray(
    const nb::sequence& bit_pattern_sequence,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyCFixedArray(
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
        std::copy_n(std::begin(limb_vec), _itemsize / 2, real_begin() + i * _itemsize);
    }
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyCFixedArray::APyCFixedArray(
    const std::vector<std::size_t>& shape, int bits, int int_bits
)
    : APyArray(shape, 2 * bits_to_limbs(bits))
    , _bits { bits }
    , _int_bits { int_bits }
{
}

APyCFixedArray::APyCFixedArray(
    const std::vector<std::size_t>& shape, int bits, int int_bits, vector_type&& v
)
    : APyArray(shape, 2 * bits_to_limbs(bits), std::move(v))
    , _bits { bits }
    , _int_bits { int_bits }
{
}

APyCFixedArray::APyCFixedArray(
    const std::vector<std::size_t>& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyArray(shape, 2 * bits_to_limbs(bits_from_optional(bits, int_bits, frac_bits)))
    , _bits { bits.has_value() ? *bits : *int_bits + *frac_bits }
    , _int_bits { int_bits.has_value() ? *int_bits : *bits - *frac_bits }
{
}

/* *********************************************************************************** *
 * *                          Arithmetic member functions                            * *
 * ***********************************************************************************
 */

template <class ripple_carry_op, class simd_op, class simd_shift_op>
inline APyCFixedArray
APyCFixedArray::_apycfixedarray_base_add_sub(const APyCFixedArray& rhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting vector
    APyCFixedArray result(_shape, res_bits, res_int_bits);

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
            _cast_no_quantize_no_overflow(
                std::begin(_data),               // src
                std::begin(result._data),        // dst
                _itemsize / 2,                   // src_limbs
                result._itemsize / 2,            // dst_limbs
                2 * _nitems,                     // n_items
                result.frac_bits() - frac_bits() // left_shift_amount
            );
            src1_ptr = &result._data[0];
            src2_ptr = &rhs._data[0];
        } else {
            // Left-hand side has more fractional bits. Upsize `rhs`
            _cast_no_quantize_no_overflow(
                std::begin(rhs._data),               // src
                std::begin(result._data),            // dst
                rhs._itemsize / 2,                   // src_limbs
                result._itemsize / 2,                // dst_limbs
                2 * rhs._nitems,                     // n_items
                result.frac_bits() - rhs.frac_bits() // left_shift_amount
            );
            src1_ptr = &_data[0];
            src2_ptr = &result._data[0];
        }
        for (std::size_t i = 0; i < result._data.size(); i += result._itemsize / 2) {
            ripple_carry_op {}(
                &result._data[i],    // dst
                &src1_ptr[i],        // src1
                &src2_ptr[i],        // src2
                result._itemsize / 2 // limb vector length
            );
        }
        return result; // early exit
    }

    // Most general case: Works in any situation, but is slowest
    APyCFixedArray imm(_shape, res_bits, res_int_bits);
    _cast_no_quantize_no_overflow(
        std::begin(_data),               // src
        std::begin(result._data),        // dst
        _itemsize / 2,                   // src_limbs
        result._itemsize / 2,            // dst_limbs
        2 * _nitems,                     // n_items
        result.frac_bits() - frac_bits() // left_shift_amount
    );
    _cast_no_quantize_no_overflow(
        std::begin(rhs._data),            // src
        std::begin(imm._data),            // dst
        rhs._itemsize / 2,                // src_limbs
        imm._itemsize / 2,                // dst_limbs
        2 * rhs._nitems,                  // n_items
        imm.frac_bits() - rhs.frac_bits() // left_shift_amount
    );

    // Perform ripple-carry operation for each element
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize / 2) {
        ripple_carry_op {}(
            &result._data[i],    // dst
            &result._data[i],    // src1
            &imm._data[i],       // src2
            result._itemsize / 2 // limb vector length
        );
    }
    return result;
}

template <class op, class ripple_carry_op>
inline APyCFixedArray APyCFixedArray::_apycfixed_base_add_sub(const APyCFixed& rhs
) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting vector
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: Operands and result fit in single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        if (frac_bits() == rhs.frac_bits()) {
            // Operands have equally many fractional bits.
            for (std::size_t i = 0; i < _nitems * 2; i += 2) {
                result._data[i + 0] = op {}(_data[i + 0], rhs._data[0]);
                result._data[i + 1] = op {}(_data[i + 1], rhs._data[1]);
            }
        } else {
            auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());
            auto lhs_shift_amount = unsigned(res_frac_bits - frac_bits());
            for (std::size_t i = 0; i < _nitems * 2; i += 2) {
                result._data[i + 0] = op {}(
                    (_data[i + 0] << lhs_shift_amount),
                    (rhs._data[0] << rhs_shift_amount)
                );
                result._data[i + 1] = op {}(
                    (_data[i + 1] << lhs_shift_amount),
                    (rhs._data[1] << rhs_shift_amount)
                );
            }
        }
        return result; // early exit
    }

    // Most general case: works in any situation
    APyCFixed imm(res_bits, res_int_bits);
    auto rhs_shift_amount = unsigned(res_frac_bits - rhs.frac_bits());
    auto lhs_shift_amount = unsigned(res_frac_bits - frac_bits());
    _cast_no_quantize_no_overflow(
        std::begin(_data),        // src
        std::begin(result._data), // dst
        _itemsize / 2,            // src_limbs
        result._itemsize / 2,     // dst_limbs
        2 * _nitems,              // n_items
        lhs_shift_amount          // left_shift_amount
    );
    _cast_no_quantize_no_overflow(
        std::begin(rhs._data), // src_begin
        std::end(rhs._data),   // src_end
        std::begin(imm._data), // dst_begin
        std::end(imm._data),   // dst_end
        rhs_shift_amount       // left_shift_amount
    );
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        // Perform ripple-carry operation
        ripple_carry_op {}(      // real part
            &result._data[i],    // dst
            &result._data[i],    // src1
            &imm._data[0],       // src2
            result._itemsize / 2 // limb vector length
        );
        ripple_carry_op {}(                          // imaginary part
            &result._data[i + result._itemsize / 2], // dst
            &result._data[i + result._itemsize / 2], // src1
            &imm._data[0 + result._itemsize / 2],    // src2
            result._itemsize / 2                     // limb vector length
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator+(const APyCFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyCFixedArray.__add__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) + rhs.broadcast_to(broadcast_shape);
    }

    return _apycfixedarray_base_add_sub<
        mpn_add_n_functor<>,
        simd::add_functor<>,
        simd::shift_add_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::operator+(const APyCFixed& rhs) const
{
    return _apycfixed_base_add_sub<std::plus<>, mpn_add_n_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::operator-(const APyCFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyCFixedArray.__sub__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) - rhs.broadcast_to(broadcast_shape);
    }

    return _apycfixedarray_base_add_sub<
        mpn_sub_n_functor<>,
        simd::sub_functor<>,
        simd::shift_sub_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::operator-(const APyCFixed& rhs) const
{
    return _apycfixed_base_add_sub<std::minus<>, mpn_sub_n_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::rsub(const APyCFixed& lhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(lhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(lhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting vector
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: Operands and result fit in single limb
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        if (frac_bits() == lhs.frac_bits()) {
            // Operands have equally many fractional bits.
            for (std::size_t i = 0; i < _nitems * 2; i += 2) {
                result._data[i + 0] = lhs._data[0] - _data[i + 0];
                result._data[i + 1] = lhs._data[1] - _data[i + 1];
            }
        } else {
            auto rhs_shift_amount = unsigned(res_frac_bits - frac_bits());
            auto lhs_shift_amount = unsigned(res_frac_bits - lhs.frac_bits());
            for (std::size_t i = 0; i < _nitems * 2; i += 2) {
                result._data[i + 0] = (lhs._data[0] << lhs_shift_amount)
                    - (_data[i + 0] << rhs_shift_amount);
                result._data[i + 1] = (lhs._data[1] << lhs_shift_amount)
                    - (_data[i + 1] << rhs_shift_amount);
            }
        }
        return result; // early exit
    }

    // Most general case: works in any situation
    APyCFixed imm(res_bits, res_int_bits);
    auto rhs_shift_amount = unsigned(res_frac_bits - frac_bits());
    auto lhs_shift_amount = unsigned(res_frac_bits - lhs.frac_bits());
    _cast_no_quantize_no_overflow(
        std::begin(_data),        // src
        std::begin(result._data), // dst
        _itemsize / 2,            // src_limbs
        result._itemsize / 2,     // dst_limbs
        2 * _nitems,              // n_items
        rhs_shift_amount          // left_shift_amount
    );
    _cast_no_quantize_no_overflow(
        std::begin(lhs._data), // src_begin
        std::end(lhs._data),   // src_end
        std::begin(imm._data), // dst_begin
        std::end(imm._data),   // dst_end
        lhs_shift_amount       // left_shift_amount
    );
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        // Perform ripple-carry operation
        mpn_sub_n(               // real part
            &result._data[i],    // dst
            &imm._data[0],       // src1
            &result._data[i],    // src2
            result._itemsize / 2 // limb vector length
        );
        mpn_sub_n(                                   // imaginary part
            &result._data[i + result._itemsize / 2], // dst
            &imm._data[0 + result._itemsize / 2],    // src1
            &result._data[i + result._itemsize / 2], // src2
            result._itemsize / 2                     // limb vector length
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator*(const APyCFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyCFixedArray.__mul__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) * rhs.broadcast_to(broadcast_shape);
    }

    const int res_int_bits = 1 + int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting `APyCFixedArray` fixed-point tensor
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            result._data[i + 0]
                = mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[i + 0])
                - mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[i + 1]);
            result._data[i + 1]
                = mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[i + 0])
                + mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[i + 1]);
        }
        return result; // early exit
    }

    // Scratch data:
    // * op1_abs:       _itemsize / 2
    // * op2_abs:       rhs._itemsize / 2
    // * prod_imm:      2 + _itemsize + rhs._itemsize
    std::size_t scratch_size = 2 + (3 * _itemsize + 3 * rhs._itemsize) / 2;
    ScratchVector<mp_limb_t, 64> scratch(scratch_size);
    auto op1_abs_begin = std::begin(scratch);
    auto op2_abs_begin = op1_abs_begin + _itemsize / 2;
    auto prod_imm_begin = op2_abs_begin + rhs._itemsize / 2;

    for (std::size_t i = 0; i < result._nitems; i++) {
        complex_fixed_point_product(
            std::begin(_data) + i * _itemsize,               // src1
            std::begin(rhs._data) + i * rhs._itemsize,       // src2
            std::begin(result._data) + i * result._itemsize, // dst
            _itemsize / 2,                                   // src1_limbs
            rhs._itemsize / 2,                               // src2_limbs
            result._itemsize / 2,                            // dst_limbs
            op1_abs_begin,                                   // op1_abs
            op2_abs_begin,                                   // op2_abs
            prod_imm_begin                                   // prod_imm
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator*(const APyCFixed& rhs) const
{
    const int res_int_bits = 1 + int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting `APyCFixedArray` fixed-point tensor
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            result._data[i + 0]
                = mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[0])
                - mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[1]);
            result._data[i + 1]
                = mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[0])
                + mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[1]);
        }
        return result; // early exit
    }

    // Scratch data:
    // * op1_abs:       _itemsize / 2
    // * op2_abs:       rhs._data.size() / 2
    // * prod_imm:      2 + _itemsize + rhs.rhs._data.size()
    std::size_t scratch_size = 2 + (3 * _itemsize + 3 * rhs._data.size()) / 2;
    ScratchVector<mp_limb_t, 64> scratch(scratch_size);
    auto op1_abs_begin = std::begin(scratch);
    auto op2_abs_begin = op1_abs_begin + _itemsize / 2;
    auto prod_imm_begin = op2_abs_begin + rhs._data.size() / 2;

    for (std::size_t i = 0; i < result._nitems; i++) {
        complex_fixed_point_product(
            std::begin(_data) + i * _itemsize,               // src1
            std::begin(rhs._data),                           // src2
            std::begin(result._data) + i * result._itemsize, // dst
            _itemsize / 2,                                   // src1_limbs
            rhs._data.size() / 2,                            // src2_limbs
            result._itemsize / 2,                            // dst_limbs
            op1_abs_begin,                                   // op1_abs
            op2_abs_begin,                                   // op2_abs
            prod_imm_begin                                   // prod_imm
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator/(const APyCFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyCFixedArray.__div__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) / rhs.broadcast_to(broadcast_shape);
    }

    // Divider bits (denominator known to be positive)
    const int num_int_bits = 1 + int_bits() + rhs.int_bits();
    const int num_frac_bits = frac_bits() + rhs.frac_bits();
    const int div_bits = num_int_bits + num_frac_bits + rhs.bits();

    const int res_int_bits = int_bits() + rhs.frac_bits() + 1;
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(div_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            mp_limb_signed_t den = mp_limb_signed_t(rhs._data[i + 0])
                    * mp_limb_signed_t(rhs._data[i + 0])
                + mp_limb_signed_t(rhs._data[i + 1])
                    * mp_limb_signed_t(rhs._data[i + 1]);
            mp_limb_signed_t real
                = mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[i + 0])
                + mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[i + 1]);
            mp_limb_signed_t imag
                = mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[i + 0])
                - mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[i + 1]);

            result._data[i + 0] = (mp_limb_signed_t(real << (rhs.bits())) / den);
            result._data[i + 1] = (mp_limb_signed_t(imag << (rhs.bits())) / den);
        }
        return result; // early exit
    }

    // General case: This always works but is slower than the special cases.
    std::size_t src1_limbs = _itemsize / 2;
    std::size_t src2_limbs = rhs._itemsize / 2;
    std::size_t dst_limbs = result._itemsize / 2;
    std::size_t div_limbs = bits_to_limbs(div_bits);

    // Scratch data (size):
    // * op1_abs:   src1_limbs
    // * op2_abs:   src2_limbs
    // * prod_imm:  2 + 2 * src1_limbs + 2 * src2_limbs
    // * num_imm:   div_limbs
    // * den_imm:   2 * src2_limbs
    // * qte_imm:   div_limbs
    std::size_t scratch_limbs = 2 + 3 * src1_limbs + 5 * src2_limbs + 2 * div_limbs;
    ScratchVector<mp_limb_t, 64> scratch(scratch_limbs);

    auto op1_abs = std::begin(scratch);
    auto op2_abs = op1_abs + src1_limbs;
    auto prod_imm = op2_abs + src2_limbs;
    auto num_imm = prod_imm + 2 + 2 * src1_limbs + 2 * src2_limbs;
    auto den_imm = num_imm + div_limbs;
    auto qte_imm = den_imm + 2 * src2_limbs;

    for (std::size_t i = 0; i < result._nitems; i++) {
        bool den_zero = limb_vector_is_zero(
            std::begin(rhs._data) + (i + 0) * rhs._itemsize,
            std::begin(rhs._data) + (i + 1) * rhs._itemsize
        );
        if (den_zero) {
            continue;
        }

        complex_fixed_point_division(
            std::begin(_data) + i * _itemsize,               // src1
            std::begin(rhs._data) + i * rhs._itemsize,       // src2
            std::begin(result._data) + i * result._itemsize, // dst
            src1_limbs,                                      // src1_limbs
            src2_limbs,                                      // src2_limbs
            dst_limbs,                                       // dst_limbs
            rhs.bits(),                                      // src2_bits
            div_limbs,                                       // div_limbs
            op1_abs,                                         // op1_abs
            op2_abs,                                         // op2_abs
            prod_imm,                                        // prod_imm
            den_imm,                                         // den_imm
            num_imm,                                         // num_imm
            qte_imm                                          // qte_imm
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator/(const APyCFixed& rhs) const
{
    // Divider bits (denominator known to be positive)
    const int num_int_bits = 1 + int_bits() + rhs.int_bits();
    const int num_frac_bits = frac_bits() + rhs.frac_bits();
    const int div_bits = num_int_bits + num_frac_bits + rhs.bits();

    const int res_int_bits = int_bits() + rhs.frac_bits() + 1;
    const int res_frac_bits = frac_bits() + rhs.int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(div_bits) <= _LIMB_SIZE_BITS) {
        mp_limb_signed_t den
            = mp_limb_signed_t(rhs._data[0]) * mp_limb_signed_t(rhs._data[0])
            + mp_limb_signed_t(rhs._data[1]) * mp_limb_signed_t(rhs._data[1]);
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            mp_limb_signed_t real
                = mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[0])
                + mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[1]);
            mp_limb_signed_t imag
                = mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(rhs._data[0])
                - mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(rhs._data[1]);

            result._data[i + 0] = (mp_limb_signed_t(real << (rhs.bits())) / den);
            result._data[i + 1] = (mp_limb_signed_t(imag << (rhs.bits())) / den);
        }
        return result; // early exit
    }

    // General case: This always works but is slower than the special cases.
    std::size_t src1_limbs = _itemsize / 2;
    std::size_t src2_limbs = rhs._data.size() / 2;
    std::size_t dst_limbs = result._itemsize / 2;
    std::size_t div_limbs = bits_to_limbs(div_bits);

    // Scratch data (size):
    // * op1_abs:   src1_limbs
    // * op2_abs:   src2_limbs
    // * prod_imm:  2 + 2 * src1_limbs + 2 * src2_limbs
    // * num_imm:   div_limbs
    // * den_imm:   2 * src2_limbs
    // * qte_imm:   div_limbs
    std::size_t scratch_limbs = 2 + 3 * src1_limbs + 5 * src2_limbs + 2 * div_limbs;
    ScratchVector<mp_limb_t, 64> scratch(scratch_limbs);

    auto op1_abs = std::begin(scratch);
    auto op2_abs = op1_abs + src1_limbs;
    auto prod_imm = op2_abs + src2_limbs;
    auto num_imm = prod_imm + 2 + 2 * src1_limbs + 2 * src2_limbs;
    auto den_imm = num_imm + div_limbs;
    auto qte_imm = den_imm + 2 * src2_limbs;

    bool den_zero = limb_vector_is_zero(
        std::begin(rhs._data), std::begin(rhs._data) + rhs._data.size()
    );
    if (den_zero) {
        // Right-hand side is zero, simply return...
        return result;
    }

    for (std::size_t i = 0; i < result._nitems; i++) {
        complex_fixed_point_division(
            std::begin(_data) + i * _itemsize,               // src1
            std::begin(rhs._data),                           // src2
            std::begin(result._data) + i * result._itemsize, // dst
            src1_limbs,                                      // src1_limbs
            src2_limbs,                                      // src2_limbs
            dst_limbs,                                       // dst_limbs
            rhs.bits(),                                      // src2_bits
            div_limbs,                                       // div_limbs
            op1_abs,                                         // op1_abs
            op2_abs,                                         // op2_abs
            prod_imm,                                        // prod_imm
            den_imm,                                         // den_imm
            num_imm,                                         // num_imm
            qte_imm                                          // qte_imm
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator<<(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyCFixedArray result = *this;
    result._int_bits += shift_val;
    return result;
}

APyCFixedArray APyCFixedArray::operator>>(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyCFixedArray result = *this;
    result._int_bits -= shift_val;
    return result;
}

APyCFixedArray& APyCFixedArray::operator<<=(const int shift_val)
{
    // Left shift in place
    _int_bits += shift_val;
    return *this;
}

APyCFixedArray& APyCFixedArray::operator>>=(const int shift_val)
{
    // Right shift in place
    _int_bits -= shift_val;
    return *this;
}

APyCFixedArray APyCFixedArray::rdiv(const APyCFixed& lhs) const
{
    // Divider bits (denominator known to be positive)
    const int num_int_bits = 1 + int_bits() + lhs.int_bits();
    const int num_frac_bits = frac_bits() + lhs.frac_bits();
    const int div_bits = num_int_bits + num_frac_bits + lhs.bits();

    const int res_int_bits = lhs.int_bits() + frac_bits() + 1;
    const int res_frac_bits = lhs.frac_bits() + int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(div_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            mp_limb_signed_t den
                = mp_limb_signed_t(_data[i + 0]) * mp_limb_signed_t(_data[i + 0])
                + mp_limb_signed_t(_data[i + 1]) * mp_limb_signed_t(_data[i + 1]);
            mp_limb_signed_t real
                = mp_limb_signed_t(lhs._data[0]) * mp_limb_signed_t(_data[i + 0])
                + mp_limb_signed_t(lhs._data[1]) * mp_limb_signed_t(_data[i + 1]);
            mp_limb_signed_t imag
                = mp_limb_signed_t(lhs._data[1]) * mp_limb_signed_t(_data[i + 0])
                - mp_limb_signed_t(lhs._data[0]) * mp_limb_signed_t(_data[i + 1]);

            result._data[i + 0] = (mp_limb_signed_t(real << (bits())) / den);
            result._data[i + 1] = (mp_limb_signed_t(imag << (bits())) / den);
        }
        return result; // early exit
    }

    // General case: This always works but is slower than the special cases.
    std::size_t src1_limbs = lhs._data.size() / 2;
    std::size_t src2_limbs = _itemsize / 2;
    std::size_t dst_limbs = result._itemsize / 2;
    std::size_t div_limbs = bits_to_limbs(div_bits);

    // Scratch data (size):
    // * op1_abs:   src1_limbs
    // * op2_abs:   src2_limbs
    // * prod_imm:  2 + 2 * src1_limbs + 2 * src2_limbs
    // * num_imm:   div_limbs
    // * den_imm:   2 * src2_limbs
    // * qte_imm:   div_limbs
    std::size_t scratch_limbs = 2 + 3 * src1_limbs + 5 * src2_limbs + 2 * div_limbs;
    ScratchVector<mp_limb_t, 64> scratch(scratch_limbs);

    auto op1_abs = std::begin(scratch);
    auto op2_abs = op1_abs + src1_limbs;
    auto prod_imm = op2_abs + src2_limbs;
    auto num_imm = prod_imm + 2 + 2 * src1_limbs + 2 * src2_limbs;
    auto den_imm = num_imm + div_limbs;
    auto qte_imm = den_imm + 2 * src2_limbs;

    for (std::size_t i = 0; i < result._nitems; i++) {
        bool den_zero = limb_vector_is_zero(
            std::begin(_data) + (i + 0) * _itemsize,
            std::begin(_data) + (i + 1) * _itemsize
        );
        if (den_zero) {
            // Right-hand side is zero, simply continue
            continue;
        }

        complex_fixed_point_division(
            std::begin(lhs._data),                           // src1
            std::begin(_data) + i * _itemsize,               // src2
            std::begin(result._data) + i * result._itemsize, // dst
            src1_limbs,                                      // src1_limbs
            src2_limbs,                                      // src2_limbs
            dst_limbs,                                       // dst_limbs
            bits(),                                          // src2_bits
            div_limbs,                                       // div_limbs
            op1_abs,                                         // op1_abs
            op2_abs,                                         // op2_abs
            prod_imm,                                        // prod_imm
            den_imm,                                         // den_imm
            num_imm,                                         // num_imm
            qte_imm                                          // qte_imm
        );
    }

    return result;
}

//! Elementwise unary negation
APyCFixedArray APyCFixedArray::operator-() const
{
    // Increase word length of result by one
    const int res_int_bits = _int_bits + 1;
    const int res_bits = _bits + 1;

    // Single limb specialization
    APyCFixedArray result(_shape, res_bits, res_int_bits);
    if (unsigned(res_bits) <= _LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = -_data[i];
        }
        return result;
    }

    // General case
    _cast_no_quantize_no_overflow(
        std::begin(_data),               // src
        std::begin(result._data),        // dst
        _itemsize / 2,                   // src_limbs
        result._itemsize / 2,            // dst_limbs
        2 * _nitems,                     // n_items
        result.frac_bits() - frac_bits() // left_shift_amount
    );
    auto it_begin = result._data.begin();
    for (std::size_t i = 0; i < 2 * _nitems; i++) {
        auto it_end = it_begin + result._itemsize / 2;
        limb_vector_negate(it_begin, it_end, it_begin);
        it_begin = it_end;
    }
    return result;
}

/* ********************************************************************************** *
 * *                            Public member functions                             * *
 * ********************************************************************************** */

std::string APyCFixedArray::repr() const
{
    std::stringstream ss {};
    ss << "APyCFixedArray([";
    if (_shape[0]) {
        // Setup hex printing which will properly display the BCD characters
        ss << std::hex;

        std::vector<mp_limb_t> data(_itemsize / 2, 0);
        for (std::size_t offset = 0; offset < _data.size(); offset += _itemsize) {

            ss << "(";

            // Real part
            std::copy_n(real_begin() + offset, _itemsize / 2, std::begin(data));
            if (bits() % _LIMB_SIZE_BITS) {
                mp_limb_t and_mask = (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
                data.back() &= and_mask;
            }
            ss << bcds_to_string(double_dabble(data));
            ss << ", ";

            // Imaginary part
            std::copy_n(imag_begin() + offset, _itemsize / 2, std::begin(data));
            if (bits() % _LIMB_SIZE_BITS) {
                mp_limb_t and_mask = (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
                data.back() &= and_mask;
            }
            ss << bcds_to_string(double_dabble(data));
            ss << "), ";
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

nb::ndarray<nb::numpy, std::complex<double>> APyCFixedArray::to_numpy() const
{
    // Dynamically allocate data to be passed to python
    std::complex<double>* result = new std::complex<double>[_nitems];

    for (std::size_t i = 0; i < _nitems; i++) {
        auto rit = real_begin() + i * _itemsize;
        auto it = imag_begin() + i * _itemsize;
        double real = fixed_point_to_double(rit, rit + _itemsize / 2, _bits, _int_bits);
        double imag = fixed_point_to_double(it, it + _itemsize / 2, _bits, _int_bits);
        result[i] = std::complex<double>(real, imag);
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result, [](void* p) noexcept {
        delete[] (std::complex<double>*)p;
    });

    return nb::ndarray<nb::numpy, std::complex<double>>(
        result, _ndim, &_shape[0], owner
    );
}

APyCFixedArray APyCFixedArray::cast(
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
    APyCFixedArray::vector_type result_data(_nitems * 2 * result_limbs + pad_limbs);

    // Do the casting
    for (std::size_t i = 0; i < 2 * _nitems; i++) {
        _cast(
            std::begin(_data) + (i + 0) * _itemsize / 2,
            std::begin(_data) + (i + 1) * _itemsize / 2,
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

    result_data.resize(_nitems * 2 * result_limbs);
    return APyCFixedArray(_shape, new_bits, new_int_bits, std::move(result_data));
}

std::variant<APyCFixedArray, APyCFixed>
APyCFixedArray::sum(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    int bits = _bits + bit_width(n_elems - 1);
    int int_bits = _int_bits + bit_width(n_elems - 1);
    std::size_t res_itemsize = 2 * bits_to_limbs(bits);

    auto fold = fold_complex_accumulate<vector_type>(_itemsize / 2, res_itemsize / 2);

    return array_fold(axes, fold, std::nullopt /* no init */, bits, int_bits);
}

APyCFixedArray APyCFixedArray::cumsum(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyCFixedArray.cumsum: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Retrieve how many elements will be summed together
    std::size_t n_elems = axis.has_value() ? _shape[*axis] : _nitems;

    // Compute the result word length
    int bits = _bits + bit_width(n_elems - 1);
    int int_bits = _int_bits + bit_width(n_elems - 1);
    std::size_t res_itemsize = 2 * bits_to_limbs(bits);

    // Accumulation function
    auto fold = fold_complex_accumulate<vector_type>(_itemsize / 2, res_itemsize / 2);

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(axis, fold, post_proc, std::nullopt, bits, int_bits);
}

std::variant<APyCFixedArray, APyCFixed>
APyCFixedArray::prod(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    int int_bits = _int_bits * n_elems + n_elems - 1;
    int frac_bits = (_bits - _int_bits) * n_elems;
    int bits = int_bits + frac_bits;
    std::size_t res_itemsize = 2 * bits_to_limbs(bits);

    // Multiplicative fold function
    std::size_t scratch_size = 2 + (3 * res_itemsize + 3 * _itemsize) / 2;
    ScratchVector<mp_limb_t, 64> scratch(scratch_size);
    auto fold
        = fold_complex_multiply<vector_type>(_itemsize / 2, res_itemsize / 2, scratch);

    APyCFixed init_one(_bits, _int_bits, { mp_limb_t(1) });
    return array_fold(axes, fold, init_one, bits, int_bits);
}

APyCFixedArray APyCFixedArray::cumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyCFixedArray.cumprod: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Retrieve how many elements will be multiplied together
    std::size_t n_elems = axis.has_value() ? _shape[*axis] : _nitems;

    // Compute the result word length
    int int_bits = std::max(int(_int_bits * n_elems + n_elems - 1), _int_bits);
    int frac_bits = std::max(int((_bits - _int_bits) * n_elems), _bits - _int_bits);
    int bits = int_bits + frac_bits;
    std::size_t res_itemsize = 2 * bits_to_limbs(bits);

    // Multiplicative fold function
    std::size_t scratch_size = 2 + (3 * res_itemsize + 3 * _itemsize) / 2;
    ScratchVector<mp_limb_t, 64> scratch(scratch_size);
    auto fold
        = fold_complex_multiply<vector_type>(_itemsize / 2, res_itemsize / 2, scratch);

    auto post_proc = [&](auto dst_it, std::size_t n) {
        std::size_t res_limbs = res_itemsize / 2;
        int frac_bits = _bits - _int_bits;
        int shift_amnt = frac_bits < 0 ? -frac_bits * n : frac_bits * (n_elems - 1 - n);
        limb_vector_lsl(dst_it, dst_it + res_limbs, shift_amnt);
        limb_vector_lsl(dst_it + res_limbs, dst_it + 2 * res_limbs, shift_amnt);
    };

    APyCFixed init_one(_bits, _int_bits, { mp_limb_t(1) });
    return array_fold_cumulative(axis, fold, post_proc, init_one, bits, int_bits);
}

//! Retrieve real part
APyFixedArray APyCFixedArray::get_real() const
{
    APyFixedArray result(_shape, _bits, _int_bits);
    for (std::size_t i = 0; i < _nitems; i++) {
        std::copy_n(
            real_begin() + i * _itemsize,
            _itemsize / 2,
            std::begin(result._data) + i * result._itemsize
        );
    }
    return result;
}

//! Retrieve imaginary part
APyFixedArray APyCFixedArray::get_imag() const
{
    APyFixedArray result(_shape, _bits, _int_bits);
    for (std::size_t i = 0; i < _nitems; i++) {
        std::copy_n(
            imag_begin() + i * _itemsize,
            _itemsize / 2,
            std::begin(result._data) + i * result._itemsize
        );
    }
    return result;
}

/* ********************************************************************************** *
 * *                     Static conversion from other types                         * *
 * ********************************************************************************** */

APyCFixedArray APyCFixedArray::zeros(
    const nb::tuple& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    std::vector<std::size_t> new_shape = cpp_shape_from_python_shape_like(shape);
    APyCFixedArray result(new_shape, int_bits, frac_bits, bits);
    return result;
}

APyCFixedArray APyCFixedArray::ones(
    const nb::tuple& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    return full(shape, APyCFixed::c_one(res_bits, res_int_bits));
}

APyCFixedArray APyCFixedArray::eye(
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
    return diagonal(shape, APyCFixed::c_one(res_bits, res_int_bits));
}

APyCFixedArray APyCFixedArray::identity(
    const nb::int_& N,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    return eye(N, std::nullopt, int_bits, frac_bits, bits);
}

APyCFixedArray APyCFixedArray::from_complex(
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

    APyCFixedArray result(
        python_sequence_extract_shape(python_seq), int_bits, frac_bits, bits
    );

    // Extract all Python doubles and integers
    auto py_obj = python_sequence_walk<
        nb::float_,
        nb::int_,
        APyFixed,
        APyFloat,
        APyCFixed,
        std::complex<double> // Put last so that APyC-scalars are not casted
        >(python_seq);

    // Set data from doubles (reuse `APyCFixed::from_double` conversion)
    for (std::size_t i = 0; i < result._data.size() / result._itemsize; i++) {
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            // Python double object
            double d = static_cast<double>(nb::cast<nb::float_>(py_obj[i]));
            fixed_point_from_double(
                d,
                result.real_begin() + i * result._itemsize,
                result.real_begin() + i * result._itemsize + result._itemsize / 2,
                result._bits,
                result._int_bits
            );
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            // Python integer object
            auto max_bits = result.frac_bits() < 0
                ? result.bits() - result.frac_bits() // Negative fractional bits
                : result.bits();                     // Non-negative fractional bits
            auto limb_vec = python_long_to_limb_vec(
                nb::cast<nb::int_>(py_obj[i]), bits_to_limbs(max_bits)
            );
            fixed_point_from_double(
                double(APyFixed(max_bits, max_bits, limb_vec)),
                result.real_begin() + i * result._itemsize,
                result.real_begin() + i * result._itemsize + result._itemsize / 2,
                result._bits,
                result._int_bits
            );
        } else if (nb::isinstance<APyFixed>(py_obj[i])) {
            const auto d = static_cast<APyFixed>(nb::cast<APyFixed>(py_obj[i]));
            _cast(
                std::begin(d._data),
                std::end(d._data),
                std::begin(result._data) + i * result._itemsize,
                std::begin(result._data) + i * result._itemsize + result._itemsize / 2,
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
                std::begin(result._data) + i * result._itemsize,
                std::begin(result._data) + i * result._itemsize + result._itemsize / 2,
                d._bits,
                d._int_bits,
                result._bits,
                result._int_bits,
                QuantizationMode::RND_INF,
                OverflowMode::WRAP
            );
        } else if (nb::isinstance<APyCFixed>(py_obj[i])) {
            const auto d = static_cast<APyCFixed>(nb::cast<APyCFixed>(py_obj[i]));
            _cast(
                d.real_begin(),
                d.real_end(),
                result.real_begin() + i * result._itemsize,
                result.real_begin() + i * result._itemsize + result._itemsize / 2,
                d._bits,
                d._int_bits,
                result._bits,
                result._int_bits,
                QuantizationMode::RND_INF,
                OverflowMode::WRAP
            );
            _cast(
                d.imag_begin(),
                d.imag_end(),
                result.imag_begin() + i * result._itemsize,
                result.imag_begin() + i * result._itemsize + result._itemsize / 2,
                d._bits,
                d._int_bits,
                result._bits,
                result._int_bits,
                QuantizationMode::RND_INF,
                OverflowMode::WRAP
            );
        } else if (nb::isinstance<std::complex<double>>(py_obj[i])) {
            // Complex double object
            std::complex<double> c = nb::cast<std::complex<double>>(py_obj[i]);
            fixed_point_from_double(
                c.real(),
                result.real_begin() + i * result._itemsize,
                result.real_begin() + i * result._itemsize + result._itemsize / 2,
                result._bits,
                result._int_bits
            );
            fixed_point_from_double(
                c.imag(),
                result.imag_begin() + i * result._itemsize,
                result.imag_begin() + i * result._itemsize + result._itemsize / 2,
                result._bits,
                result._int_bits
            );
        }
    }

    return result;
}

APyCFixedArray APyCFixedArray::from_numbers(
    const nb::sequence& number_seq,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    return from_complex(number_seq, int_bits, frac_bits, bits);
}

APyCFixedArray APyCFixedArray::from_array(
    const nb::ndarray<nb::c_contig>& ndarray,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    std::size_t ndim = ndarray.ndim();
    if (ndim == 0) {
        throw nb::type_error(
            "APyCFixedArray.from_float(): NDArray with ndim == 0 not supported"
        );
    }
    std::vector<std::size_t> shape(ndim, 0);
    for (std::size_t i = 0; i < ndim; i++) {
        shape[i] = ndarray.shape(i);
    }

    APyCFixedArray result(shape, int_bits, frac_bits, bits);
    result._set_values_from_ndarray(ndarray);
    return result;
}

/* ********************************************************************************** *
 * *                            Private member functions                            * *
 * ********************************************************************************** */

void APyCFixedArray::_set_bits_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
#define CHECK_AND_SET_BITS_FROM_NPTYPE(__TYPE__)                                       \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                mp_limb_t data;                                                        \
                if constexpr (std::is_signed<__TYPE__>::value) {                       \
                    data = static_cast<mp_limb_signed_t>(ndarray_view.data()[i]);      \
                } else {                                                               \
                    data = static_cast<mp_limb_t>(ndarray_view.data()[i]);             \
                }                                                                      \
                _data[i * _itemsize] = data;                                           \
                if (_itemsize / 2 >= 2) {                                              \
                    std::fill_n(/* sign extend real part */                            \
                                real_begin() + i * _itemsize + 1,                      \
                                _itemsize / 2 - 1,                                     \
                                mp_limb_signed_t(data) < 0 ? -1 : 0                    \
                    );                                                                 \
                }                                                                      \
                std::fill_n(/* zero imag part */                                       \
                            imag_begin() + i * _itemsize,                              \
                            _itemsize / 2,                                             \
                            0                                                          \
                );                                                                     \
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
        "APyCFixedArray::_set_bits_from_ndarray(): "
        "expecting integer `dtype`"
    );
}

void APyCFixedArray::_set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
    if (ndarray.dtype() == nb::dtype<std::complex<double>>()) {
        auto ndarray_view = ndarray.view<std::complex<double>, nb::ndim<1>>();
        for (std::size_t i = 0; i < ndarray.size(); i++) {
            fixed_point_from_double(
                static_cast<std::complex<double>>(ndarray_view.data()[i]).real(),
                real_begin() + i * _itemsize,
                real_begin() + i * _itemsize + _itemsize / 2,
                _bits,
                _int_bits
            );
            fixed_point_from_double(
                static_cast<std::complex<double>>(ndarray_view.data()[i]).imag(),
                imag_begin() + i * _itemsize,
                imag_begin() + i * _itemsize + _itemsize / 2,
                _bits,
                _int_bits
            );
        }
        return; /* Conversion completed, exit `_set_values_from_ndarray()` */
    }

#define CHECK_AND_SET_VALUES_FROM_NPTYPE(__TYPE__)                                     \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                fixed_point_from_double(                                               \
                    static_cast<double>(ndarray_view.data()[i]),                       \
                    real_begin() + i * _itemsize,                                      \
                    real_begin() + i * _itemsize + _itemsize / 2,                      \
                    _bits,                                                             \
                    _int_bits                                                          \
                );                                                                     \
                std::fill_n(/* zero imag part */                                       \
                            imag_begin() + i * _itemsize,                              \
                            _itemsize / 2,                                             \
                            0                                                          \
                );                                                                     \
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
        "APyCFixedArray::_set_values_from_ndarray(): "
        "unsupported `dtype` expecting integer/float"
    );
}
