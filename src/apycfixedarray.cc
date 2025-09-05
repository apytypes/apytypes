// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

#include "apybuffer.h"
#include "apycfixed.h"
#include "apycfixed_util.h"
#include "apycfixedarray.h"
#include "apyfixed_util.h"
#include "apyfixedarray.h"
#include "apytypes_common.h"
#include "apytypes_simd.h"
#include "apytypes_util.h"
#include "python_util.h"
#include "src/apytypes_intrinsics.h"
#include "src/apytypes_mp.h"
#include "src/array_utils.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

// Standard header includes
#include <algorithm>   // std::copy, std::max, std::transform, etc...
#include <cstddef>     // std::size_t
#include <cstdint>     // std::int16, std::int32, std::int64, etc...
#include <optional>    // std::optional
#include <set>         // std::set
#include <stdexcept>   // std::length_error
#include <string>      // std::string
#include <type_traits> // std::remove_cv_t
#include <variant>     // std::variant
#include <vector>      // std::vector, std::swap

#include <fmt/format.h>

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

APyCFixedArray::APyCFixedArray(
    const nb::typed<nb::iterable, nb::any>& seq,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : APyCFixedArray(
          python_iterable_extract_shape</* IS_COMPLEX_COLLAPSE = */ true>(
              seq, "APyCFixedArray.__init__"
          ),
          int_bits,
          frac_bits,
          bits
      )
{
    // Specialized initialization for NDArray
    if (nb::isinstance<nb::ndarray<>>(seq)) {
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(seq);
        _set_bits_from_ndarray(ndarray);
        return; // initialization completed
    }

    // 1D vector of Python int objects (`nb::int_` objects)
    auto python_objs = python_iterable_walk<nb::int_>(seq, "APyCFixedArray.__init__");

    // If the walked sequence of Python integers is
    assert(python_objs.size() == _nitems || python_objs.size() == 2 * _nitems);
    bool is_inner_dim_complex = (python_objs.size() == 2 * _nitems);

    if (is_inner_dim_complex) {
        for (std::size_t i = 0; i < 2 * _nitems; i++) {
            nb::int_ python_int = nb::cast<nb::int_>(python_objs[i]);
            auto limb_vec = python_long_to_limb_vec(python_int, _itemsize / 2);
            _overflow_twos_complement(
                std::begin(limb_vec), std::end(limb_vec), _bits, _int_bits
            );
            std::copy_n(
                std::begin(limb_vec), _itemsize / 2, real_begin() + i * _itemsize / 2
            );
        }
    } else {
        for (std::size_t i = 0; i < _nitems; i++) {
            nb::int_ python_int = nb::cast<nb::int_>(python_objs[i]);
            auto limb_vec = python_long_to_limb_vec(python_int, _itemsize / 2);
            _overflow_twos_complement(
                std::begin(limb_vec), std::end(limb_vec), _bits, _int_bits
            );
            std::copy_n(
                std::begin(limb_vec), _itemsize / 2, real_begin() + i * _itemsize
            );
        }
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
inline APyCFixedArray
APyCFixedArray::_apycfixed_base_add_sub(const APyCFixed& rhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting vector
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Special case #1: Operands and result fit in single limb
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
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
        return try_broadcast_and_then<std::plus<>>(rhs, "__add__");
    }

    return _apycfixedarray_base_add_sub<
        apy_add_n_functor<>,
        simd::add_functor<>,
        simd::shift_add_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::operator+(const APyCFixed& rhs) const
{
    return _apycfixed_base_add_sub<std::plus<>, apy_add_n_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::operator-(const APyCFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::minus<>>(rhs, "__sub__");
    }

    return _apycfixedarray_base_add_sub<
        apy_sub_n_functor<>,
        simd::sub_functor<>,
        simd::shift_sub_functor<>>(rhs);
}

APyCFixedArray APyCFixedArray::operator-(const APyCFixed& rhs) const
{
    return _apycfixed_base_add_sub<std::minus<>, apy_sub_n_functor<>>(rhs);
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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
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
        apy_inplace_reversed_subtraction_same_length( // real part
            &result._data[i],                         // dst/src2
            &imm._data[0],                            // src1
            result._itemsize / 2                      // limb vector length
        );
        apy_inplace_reversed_subtraction_same_length( // imaginary part
            &result._data[i + result._itemsize / 2],  // dst(src2)
            &imm._data[0 + result._itemsize / 2],     // src1
            result._itemsize / 2                      // limb vector length
        );
    }

    return result;
}

APyCFixedArray APyCFixedArray::operator*(const APyCFixedArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::multiplies<>>(rhs, "__mul__");
    }

    const int res_int_bits = 1 + int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    const int res_bits = res_int_bits + res_frac_bits;

    // Resulting `APyCFixedArray` fixed-point tensor
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            result._data[i + 0]
                = apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[i + 0])
                - apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[i + 1]);
            result._data[i + 1]
                = apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[i + 0])
                + apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[i + 1]);
        }
        return result; // early exit
    }

    // Double limb result specialization
    if (unsigned(res_bits) <= 2 * APY_LIMB_SIZE_BITS
        && unsigned(bits()) <= APY_LIMB_SIZE_BITS
        && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            complex_multiplication_1_1_2(
                &result._data[2 * i], &_data[i], &rhs._data[i]
            );
        }
        return result; // early exit
    }

    // Scratch data:
    // * op1_abs:       _itemsize / 2
    // * op2_abs:       rhs._itemsize / 2
    // * prod_imm:      2 + _itemsize + rhs._itemsize
    std::size_t scratch_size = 2 + (3 * _itemsize + 3 * rhs._itemsize) / 2;
    ScratchVector<apy_limb_t, 64> scratch(scratch_size);
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
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            result._data[i + 0]
                = apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[0])
                - apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[1]);
            result._data[i + 1]
                = apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[0])
                + apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[1]);
        }
        return result; // early exit
    }

#if (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
    // Double limb result specialization
    if (unsigned(res_bits) <= 2 * APY_LIMB_SIZE_BITS
        && unsigned(bits()) <= APY_LIMB_SIZE_BITS
        && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
        __int128 re1 = (__int128)apy_limb_signed_t(rhs._data[0]);
        __int128 im1 = (__int128)apy_limb_signed_t(rhs._data[1]);
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            __int128 re0 = (__int128)apy_limb_signed_t(_data[i]);
            __int128 im0 = (__int128)apy_limb_signed_t(_data[i + 1]);
            auto re_res = re0 * re1 - im0 * im1;
            auto im_res = re0 * im1 + im0 * re1;
            result._data[2 * i + 0] = apy_limb_t(re_res);
            result._data[2 * i + 1] = apy_limb_t(re_res >> APY_LIMB_SIZE_BITS);
            result._data[2 * i + 2] = apy_limb_t(im_res);
            result._data[2 * i + 3] = apy_limb_t(im_res >> APY_LIMB_SIZE_BITS);
        }
        return result; // early exit
    }
#endif
#else
    // COMPILER_LIMB_SIZE == 32
    // Double limb result specialization
    if (unsigned(res_bits) <= 2 * APY_LIMB_SIZE_BITS
        && unsigned(bits()) <= APY_LIMB_SIZE_BITS
        && unsigned(rhs.bits()) <= APY_LIMB_SIZE_BITS) {
        std::int64_t re1 = (std::int64_t)apy_limb_signed_t(rhs._data[0]);
        std::int64_t im1 = (std::int64_t)apy_limb_signed_t(rhs._data[1]);
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            std::int64_t re0 = (std::int64_t)apy_limb_signed_t(_data[i]);
            std::int64_t im0 = (std::int64_t)apy_limb_signed_t(_data[i + 1]);
            auto re_res = re0 * re1 - im0 * im1;
            auto im_res = re0 * im1 + im0 * re1;
            result._data[2 * i + 0] = apy_limb_t(re_res);
            result._data[2 * i + 1] = apy_limb_t(re_res >> APY_LIMB_SIZE_BITS);
            result._data[2 * i + 2] = apy_limb_t(im_res);
            result._data[2 * i + 3] = apy_limb_t(im_res >> APY_LIMB_SIZE_BITS);
        }
        return result; // early exit
    }
#endif

    // Scratch data:
    // * op1_abs:       _itemsize / 2
    // * op2_abs:       rhs._data.size() / 2
    // * prod_imm:      2 + _itemsize + rhs.rhs._data.size()
    std::size_t scratch_size = 2 + (3 * _itemsize + 3 * rhs._data.size()) / 2;
    ScratchVector<apy_limb_t, 64> scratch(scratch_size);
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
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::divides<>>(rhs, "__truediv__");
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
    if (unsigned(div_bits) <= APY_LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            apy_limb_signed_t den = apy_limb_signed_t(rhs._data[i + 0])
                    * apy_limb_signed_t(rhs._data[i + 0])
                + apy_limb_signed_t(rhs._data[i + 1])
                    * apy_limb_signed_t(rhs._data[i + 1]);
            apy_limb_signed_t real
                = apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[i + 0])
                + apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[i + 1]);
            apy_limb_signed_t imag
                = apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[i + 0])
                - apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[i + 1]);

            result._data[i + 0] = (apy_limb_signed_t(real << (rhs.bits())) / den);
            result._data[i + 1] = (apy_limb_signed_t(imag << (rhs.bits())) / den);
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
    ScratchVector<apy_limb_t, 64> scratch(scratch_limbs);

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
    if (unsigned(div_bits) <= APY_LIMB_SIZE_BITS) {
        apy_limb_signed_t den
            = apy_limb_signed_t(rhs._data[0]) * apy_limb_signed_t(rhs._data[0])
            + apy_limb_signed_t(rhs._data[1]) * apy_limb_signed_t(rhs._data[1]);
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            apy_limb_signed_t real
                = apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[0])
                + apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[1]);
            apy_limb_signed_t imag
                = apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(rhs._data[0])
                - apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(rhs._data[1]);

            result._data[i + 0] = (apy_limb_signed_t(real << (rhs.bits())) / den);
            result._data[i + 1] = (apy_limb_signed_t(imag << (rhs.bits())) / den);
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
    ScratchVector<apy_limb_t, 64> scratch(scratch_limbs);

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
    const int div_bits = num_int_bits + num_frac_bits + bits();

    const int res_int_bits = lhs.int_bits() + frac_bits() + 1;
    const int res_frac_bits = lhs.frac_bits() + int_bits();
    const int res_bits = res_int_bits + res_frac_bits;
    APyCFixedArray result(_shape, res_bits, res_int_bits);

    // Single limb specialization
    if (unsigned(div_bits) <= APY_LIMB_SIZE_BITS) {
        for (std::size_t i = 0; i < result._nitems * 2; i += 2) {
            apy_limb_signed_t den
                = apy_limb_signed_t(_data[i + 0]) * apy_limb_signed_t(_data[i + 0])
                + apy_limb_signed_t(_data[i + 1]) * apy_limb_signed_t(_data[i + 1]);
            apy_limb_signed_t real
                = apy_limb_signed_t(lhs._data[0]) * apy_limb_signed_t(_data[i + 0])
                + apy_limb_signed_t(lhs._data[1]) * apy_limb_signed_t(_data[i + 1]);
            apy_limb_signed_t imag
                = apy_limb_signed_t(lhs._data[1]) * apy_limb_signed_t(_data[i + 0])
                - apy_limb_signed_t(lhs._data[0]) * apy_limb_signed_t(_data[i + 1]);

            result._data[i + 0] = (apy_limb_signed_t(real << (bits())) / den);
            result._data[i + 1] = (apy_limb_signed_t(imag << (bits())) / den);
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
    ScratchVector<apy_limb_t, 64> scratch(scratch_limbs);

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

    // Specialization 1: Single limb
    APyCFixedArray result(_shape, res_bits, res_int_bits);
    if (unsigned(res_bits) <= APY_LIMB_SIZE_BITS) {
        simd::vector_neg(result._data.begin(), _data.begin(), _data.size());
        return result;
    }

    // Specialization 2: Same number of limb
    if (_itemsize == result._itemsize) {
        // Copy inverted data into the result
        simd::vector_not(result._data.begin(), _data.begin(), _itemsize * _nitems);
        auto it_begin = result._data.begin();
        for (std::size_t i = 0; i < 2 * _nitems; i++) {
            auto it_end = it_begin + result._itemsize / 2;
            // Add one LSB to get negated results
            limb_vector_add_one_lsb_inplace(it_begin, it_end);
            it_begin = it_end;
        }
        return result; // early return specialization #2
    }

    // General case: one more limb in result
    auto src_begin = _data.begin();
    auto dst_begin = result._data.begin();
    for (std::size_t i = 0; i < 2 * _nitems; i++) {
        auto src_end = src_begin + _itemsize / 2;
        auto dst_end = dst_begin + result._itemsize / 2;
        // Copy inverted data into the result
        std::transform(src_begin, src_end, dst_begin, std::bit_not {});
        std::fill(
            dst_begin + _itemsize / 2,
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

APyCFixedArray APyCFixedArray::operator~() const
{
    // Resulting `APyCFixedArray` fixed-point tensor
    APyCFixedArray result(_shape, _bits, _int_bits);
    simd::vector_not(result._data.begin(), _data.begin(), _itemsize * _nitems);
    return result;
}

/* ********************************************************************************** *
 * *                            Public member functions                             * *
 * ********************************************************************************** */

std::string APyCFixedArray::repr() const
{
    const auto formatter = [bits = _bits](auto cbegin_it, auto cend_it) -> std::string {
        std::size_t itemsize = std::distance(cbegin_it, cend_it);
        auto creal_it = cbegin_it;
        auto cimag_it = cbegin_it + itemsize / 2;

        std::vector<apy_limb_t> real_data(itemsize / 2);
        std::vector<apy_limb_t> imag_data(itemsize / 2);
        std::copy(creal_it, creal_it + itemsize / 2, std::begin(real_data));
        std::copy(cimag_it, cimag_it + itemsize / 2, std::begin(imag_data));

        // Zero sign bits outside of bit-range
        if (bits % APY_LIMB_SIZE_BITS) {
            apy_limb_t and_mask = (apy_limb_t(1) << (bits % APY_LIMB_SIZE_BITS)) - 1;
            real_data.back() &= and_mask;
            imag_data.back() &= and_mask;
        }
        return fmt::format(
            "({}, {})",
            bcds_to_string(double_dabble(real_data)),
            bcds_to_string(double_dabble(imag_data))
        );
    };

    auto&& kw_args = { fmt::format("int_bits={}", int_bits()),
                       fmt::format("frac_bits={}", frac_bits()) };

    return array_repr({ formatter }, kw_args);
}

nb::ndarray<nb::numpy, std::complex<double>> APyCFixedArray::to_numpy(
    std::optional<nb::object> dtype, std::optional<bool> copy
) const
{
    (void)dtype;

    if (!copy.value_or(true)) {
        throw nb::value_error("APyCFixedArray.to_numpy: copy must be True");
    }

    // Dynamically allocate data to be passed to python
    std::complex<double>* result = new std::complex<double>[_nitems];
    auto _frac_bits = frac_bits();
    for (std::size_t i = 0; i < _nitems; i++) {
        auto rit = real_begin() + i * _itemsize;
        auto it = imag_begin() + i * _itemsize;
        double real = fixed_point_to_double(rit, rit + _itemsize / 2, _frac_bits);
        double imag = fixed_point_to_double(it, it + _itemsize / 2, _frac_bits);
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

    // Do the casting: `fixed_point_cast_unsafe` is safe to use because of `pad_limbs`
    for (std::size_t i = 0; i < 2 * _nitems; i++) {
        fixed_point_cast_unsafe(
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
APyCFixedArray::sum(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    // Compute the result word length
    int pad_bits = n_elems ? bit_width(n_elems - 1) : 0;
    int bits = _bits + pad_bits;
    int int_bits = _int_bits + pad_bits;
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
    int pad_bits = n_elems ? bit_width(n_elems - 1) : 0;
    int bits = _bits + pad_bits;
    int int_bits = _int_bits + pad_bits;
    std::size_t res_itemsize = 2 * bits_to_limbs(bits);

    // Accumulation function
    auto fold = fold_complex_accumulate<vector_type>(_itemsize / 2, res_itemsize / 2);

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(axis, fold, post_proc, std::nullopt, bits, int_bits);
}

std::variant<APyCFixedArray, APyCFixed>
APyCFixedArray::prod(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Retrieve how many elements will be summed together
    std::size_t n_elems = array_fold_get_elements(axes);

    if (n_elems == 0) {
        // Empty array, return scalar one (NumPy semantics)
        return APyCFixed::one(_bits, _int_bits);
    } else {
        // Non-empty array
        int int_bits = _int_bits * n_elems + n_elems - 1;
        int frac_bits = (_bits - _int_bits) * n_elems;
        int bits = int_bits + frac_bits;
        std::size_t res_itemsize = 2 * bits_to_limbs(bits);

        // Multiplicative fold function
        std::size_t scratch_size = 2 + (3 * res_itemsize + 3 * _itemsize) / 2;
        ScratchVector<apy_limb_t, 64> scratch(scratch_size);
        auto fold = fold_complex_multiply<vector_type>(
            _itemsize / 2, res_itemsize / 2, scratch
        );

        APyCFixed init_one(_bits, _int_bits, { apy_limb_t(1) });
        return array_fold(axes, fold, init_one, bits, int_bits);
    }
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
    ScratchVector<apy_limb_t, 64> scratch(scratch_size);
    auto fold
        = fold_complex_multiply<vector_type>(_itemsize / 2, res_itemsize / 2, scratch);

    auto post_proc = [&](auto dst_it, std::size_t n) {
        std::size_t res_limbs = res_itemsize / 2;
        int frac_bits = _bits - _int_bits;
        int shift_amnt = frac_bits < 0 ? -frac_bits * n : frac_bits * (n_elems - 1 - n);
        limb_vector_lsl(dst_it, dst_it + res_limbs, shift_amnt);
        limb_vector_lsl(dst_it + res_limbs, dst_it + 2 * res_limbs, shift_amnt);
    };

    APyCFixed init_one(_bits, _int_bits, { apy_limb_t(1) });
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
 * *                            Static array creation                               * *
 * ********************************************************************************** */

APyCFixedArray APyCFixedArray::zeros(
    const PyShapeParam_t& shape,
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
    const PyShapeParam_t& shape,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    const int res_bits = bits_from_optional(bits, int_bits, frac_bits);
    const int res_int_bits = int_bits.has_value() ? *int_bits : *bits - *frac_bits;
    return full(shape, APyCFixed::one(res_bits, res_int_bits));
}

APyCFixedArray APyCFixedArray::eye(
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
    return diagonal(shape, APyCFixed::one(res_bits, res_int_bits));
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
    const nb::typed<nb::iterable, nb::any>& cplx_seq,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    if (nb::isinstance<nb::ndarray<>>(cplx_seq)) {
        // Sequence is NDArray. Initialize using `from_array`
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(cplx_seq);
        return from_array(ndarray, int_bits, frac_bits, bits);
    }

    APyCFixedArray result(
        python_iterable_extract_shape(cplx_seq, "APyCFixedArray.from_complex"),
        int_bits,
        frac_bits,
        bits
    );

    // Extract all Python doubles and integers
    auto py_obj = python_iterable_walk<
        nb::float_,
        nb::int_,
        APyFixed,
        APyFloat,
        APyCFixed,
        std::complex<double>>(cplx_seq, "APyCFixedArray.from_complex");

    // Function used to set a single scalar from a floating-point
    unsigned float_shift {};
    std::function<void(APyCFixedArray&, std::size_t, double, unsigned)> from_fp;
    if (result._itemsize == 2) {
        float_shift = 64 - (result._bits & (64 - 1));
        from_fp = [](APyCFixedArray& res, std::size_t i, double val, unsigned shift) {
            res._data[i]
                = fixed_point_from_double_single_limb(val, res.frac_bits(), shift);
        };
    } else {
        assert(result._itemsize >= 4);
        assert(result._itemsize % 2 == 0);
        from_fp = [](APyCFixedArray& res, std::size_t i, double val, unsigned) {
            fixed_point_from_double(
                val,
                std::begin(res._data) + (i + 0) * (res._itemsize / 2),
                std::begin(res._data) + (i + 1) * (res._itemsize / 2),
                res.bits(),
                res.int_bits()
            );
        };
    }

    // Function used to cast fixed-point data to `*this` format
    auto fx_cast = [](APyCFixedArray& res, std::size_t i, auto&& fx) {
        using CleanFx_t = remove_cvref_t<decltype(fx)>;
        constexpr bool is_cplx = std::is_same_v<CleanFx_t, APyCFixed>;

        std::size_t offst = res._itemsize / 2;
        for (std::size_t j = 0; j < 1 + is_cplx; j++) {
            auto fx_begin = std::begin(fx._data) + j * fx._data.size() / 2;
            auto fx_end = fx_begin + (is_cplx ? fx._data.size() / 2 : fx._data.size());
            fixed_point_cast(
                fx_begin,
                fx_end,
                (j ? res.imag_begin() : res.real_begin()) + i * res._itemsize,
                (j ? res.imag_begin() : res.real_begin()) + i * res._itemsize + offst,
                fx._bits,
                fx._int_bits,
                res._bits,
                res._int_bits,
                QuantizationMode::RND_INF,
                OverflowMode::WRAP
            );
        }
    };

    for (std::size_t i = 0; i < result._nitems; i++) {
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            // Python double object
            double val = static_cast<double>(nb::cast<nb::float_>(py_obj[i]));
            from_fp(result, 2 * i, val, float_shift);
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            // Python integer object
            fixed_point_from_py_integer(
                nb::cast<nb::int_>(py_obj[i]),
                std::begin(result._data) + i * result._itemsize,
                std::begin(result._data) + i * result._itemsize + result._itemsize / 2,
                result._bits,
                result._int_bits
            );
        } else if (nb::isinstance<APyFixed>(py_obj[i])) {
            const APyFixed& fx = static_cast<APyFixed>(nb::cast<APyFixed>(py_obj[i]));
            fx_cast(result, i, fx);
        } else if (nb::isinstance<APyFloat>(py_obj[i])) {
            const APyFixed& fx = nb::cast<APyFloat>(py_obj[i]).to_fixed();
            fx_cast(result, i, fx);
        } else if (nb::isinstance<APyCFixed>(py_obj[i])) {
            const APyCFixed& cfx = nb::cast<APyCFixed>(py_obj[i]);
            fx_cast(result, i, cfx);
        } else if (nb::isinstance<std::complex<double>>(py_obj[i])) {
            // Complex double object
            std::complex<double> cplx = nb::cast<std::complex<double>>(py_obj[i]);
            from_fp(result, 2 * i + 0, cplx.real(), float_shift);
            from_fp(result, 2 * i + 1, cplx.imag(), float_shift);
        } else {
            APYTYPES_UNREACHABLE();
        };
    }

    return result;
}

APyCFixedArray APyCFixedArray::from_numbers(
    const nb::typed<nb::iterable, nb::any>& number_seq,
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

std::string APyCFixedArray::to_string_dec() const
{
    const auto formatter
        = [itemsize = _itemsize,
           frac_bits = _bits - _int_bits](auto cbegin_it, auto cend_it) -> std::string {
        auto imag_begin = cbegin_it + itemsize / 2;
        double real_as_double = fixed_point_to_double(cbegin_it, imag_begin, frac_bits);
        double imag_as_double = fixed_point_to_double(imag_begin, cend_it, frac_bits);
        if (imag_as_double < 0) {
            return fmt::format("{}{}j", real_as_double, imag_as_double);
        } else {
            return fmt::format("{}+{}j", real_as_double, imag_as_double);
        }
    };

    return array_format(formatter, 88, false);
}

std::string APyCFixedArray::to_string(int base) const
{
    switch (base) {
    case 10:
        return to_string_dec();
    default:
        auto msg
            = fmt::format("APyCFixedArray.__str__: base={} is not supported", base);
        throw nb::value_error(msg.c_str());
    }
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
                apy_limb_t data;                                                       \
                if constexpr (std::is_signed<__TYPE__>::value) {                       \
                    data = static_cast<apy_limb_signed_t>(ndarray_view.data()[i]);     \
                } else {                                                               \
                    data = static_cast<apy_limb_t>(ndarray_view.data()[i]);            \
                }                                                                      \
                _data[i * _itemsize] = data;                                           \
                if (_itemsize / 2 >= 2) {                                              \
                    apy_limb_t limb = apy_limb_signed_t(data) < 0 ? -1 : 0;            \
                    std::size_t real_n = _itemsize / 2 - 1;                            \
                    std::fill_n(real_begin() + i * _itemsize + 1, real_n, limb);       \
                }                                                                      \
                std::fill_n(imag_begin() + i * _itemsize, _itemsize / 2, 0);           \
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

#undef CHECK_AND_SET_BITS_FROM_NPTYPE

    // None of the `CHECK_AND_SET_BITS_FROM_NPTYPE` succeeded. Unsupported type,
    // throw an error. If possible, it would be nice to show a string representation
    // of the `dtype`. Seems hard to achieve with nanobind, but please fix this if
    // you find out how this can be achieved.
    throw nb::type_error(
        "APyFixedArray.__init__: unsupported `dtype` in ndarray, expecting integer"
    );
}

void APyCFixedArray::_set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
#define CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE(__TYPE__)                             \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            if (_itemsize == 2) {                                                      \
                unsigned limb_shift_val = bits() & (64 - 1);                           \
                unsigned twos_complement_shift = 64 - limb_shift_val;                  \
                int _frac_bits = frac_bits();                                          \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    std::complex<double> cplx = view.data()[i];                        \
                    _data[2 * i + 0] = fixed_point_from_double_single_limb(            \
                        cplx.real(), _frac_bits, twos_complement_shift                 \
                    );                                                                 \
                    _data[2 * i + 1] = fixed_point_from_double_single_limb(            \
                        cplx.imag(), _frac_bits, twos_complement_shift                 \
                    );                                                                 \
                }                                                                      \
            } else {                                                                   \
                assert(_itemsize >= 4);                                                \
                assert(_itemsize % 2 == 0);                                            \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    std::complex<double> cplx = view.data()[i];                        \
                    fixed_point_from_double(                                           \
                        cplx.real(),                                                   \
                        real_begin() + i * _itemsize,                                  \
                        real_begin() + i * _itemsize + _itemsize / 2,                  \
                        _bits,                                                         \
                        _int_bits                                                      \
                    );                                                                 \
                    fixed_point_from_double(                                           \
                        cplx.imag(),                                                   \
                        imag_begin() + i * _itemsize,                                  \
                        imag_begin() + i * _itemsize + _itemsize / 2,                  \
                        _bits,                                                         \
                        _int_bits                                                      \
                    );                                                                 \
                }                                                                      \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

#define CHECK_AND_SET_VALUES_FROM_FLOAT_NPTYPE(__TYPE__)                               \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            if (_itemsize == 2) {                                                      \
                unsigned limb_shift_val = bits() & (64 - 1);                           \
                unsigned twos_complement_shift = 64 - limb_shift_val;                  \
                int _frac_bits = frac_bits();                                          \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    _data[2 * i + 0] = fixed_point_from_double_single_limb(            \
                        view.data()[i], _frac_bits, twos_complement_shift              \
                    );                                                                 \
                    _data[2 * i + 1] = 0;                                              \
                }                                                                      \
            } else {                                                                   \
                assert(_itemsize >= 4);                                                \
                assert(_itemsize % 2 == 0);                                            \
                for (std::size_t i = 0; i < ndarray.size(); i++) {                     \
                    fixed_point_from_double(                                           \
                        view.data()[i],                                                \
                        real_begin() + i * _itemsize,                                  \
                        real_begin() + i * _itemsize + _itemsize / 2,                  \
                        _bits,                                                         \
                        _int_bits                                                      \
                    );                                                                 \
                    std::fill_n(imag_begin() + i * _itemsize, _itemsize / 2, 0);       \
                }                                                                      \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

#define CHECK_AND_SET_VALUES_FROM_INT_NPTYPE(__TYPE__)                                 \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto view = ndarray.view<__TYPE__, nb::ndim<1>>();                         \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                fixed_point_from_integer(                                              \
                    static_cast<std::uint64_t>(view.data()[i]),                        \
                    real_begin() + i * _itemsize,                                      \
                    real_begin() + i * _itemsize + _itemsize / 2,                      \
                    _bits,                                                             \
                    _int_bits                                                          \
                );                                                                     \
                std::fill_n(imag_begin() + i * _itemsize, _itemsize / 2, 0);           \
            }                                                                          \
            return; /* Conversion completed, exit `_set_values_from_ndarray()` */      \
        }                                                                              \
    } while (0)

    // Each `CHECK_AND_SET_VALUES_FROM_NPTYPE` checks the dtype of `ndarray` and
    // converts all the data if it matches. If successful, the macro returns. Otherwise,
    // the next attempted conversion will take place.
    CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE(std::complex<double>);
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

#undef CHECK_AND_SET_VALUES_FROM_COMPLEX_NPTYPE
#undef CHECK_AND_SET_VALUES_FROM_FLOAT_NPTYPE
#undef CHECK_AND_SET_VALUES_FROM_INT_NPTYPE

    // None of the `CHECK_AND_VALUES_FROM_NPTYPE` succeeded. Unsupported type, throw
    // an error. If possible, it would be nice to show a string representation of
    // the `dtype`. Seems hard to achieve with nanobind, but please fix this if you
    // find out how this can be achieved.
    throw nb::type_error(
        "APyCFixedArray.from_array: unsupported `dtype` in ndarray, expecting "
        "complex or float or integer"
    );
}

std::variant<APyCFixedArray, APyCFixed>
APyCFixedArray::matmul(const APyCFixedArray& rhs) const
{
    if (ndim() == 1 && rhs.ndim() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out.
            // Perform the checked inner product.
            return checked_inner_product(rhs, get_accumulator_mode_fixed());
        }
    }
    if (ndim() == 2 && (rhs.ndim() == 2 || rhs.ndim() == 1)) {
        if (_shape[1] == rhs._shape[0]) {
            // Dimensionality for a standard 2D matrix multiplication checks out.
            // Perform the checked 2D matrix
            return checked_2d_matmul(rhs, get_accumulator_mode_fixed());
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(
        fmt::format(
            "APyCFixedArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
            tuple_string_from_vec(_shape),
            tuple_string_from_vec(rhs._shape)
        )
    );
}

// Evaluate the inner between two vectors. This method assumes that the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
APyCFixed APyCFixedArray::checked_inner_product(
    const APyCFixedArray& rhs, std::optional<APyFixedAccumulatorOption> mode
) const
{
    int pad_bits = _shape[0] ? bit_width(_shape[0] - 1) : 0;
    int res_bits = 1 + bits() + rhs.bits() + pad_bits;
    int res_int_bits = 1 + int_bits() + rhs.int_bits() + pad_bits;
    if (mode.has_value()) {
        res_bits = mode->bits;
        res_int_bits = mode->int_bits;
    }

    APyCFixedArray res_arr({ 1 }, res_bits, res_int_bits);

    auto inner_product
        = ComplexFixedPointInnerProduct(spec(), rhs.spec(), res_arr.spec(), mode);

    inner_product(
        std::begin(_data),         // src1
        std::begin(rhs._data),     // src2
        std::begin(res_arr._data), // dst
        _nitems
    );

    APyCFixed res(res_bits, res_int_bits);
    for (std::size_t i = 0; i < res_arr._data.size(); i++) {
        res._data[i] = res_arr._data[i];
    }
    return res;
}

// Evaluate the matrix product between two 2D matrices. This method assumes that the
// shape of `*this` and `rhs` have been checked to match a 2d matrix multiplication.
APyCFixedArray APyCFixedArray::checked_2d_matmul(
    const APyCFixedArray& rhs, std::optional<APyFixedAccumulatorOption> mode
) const
{
    // Resulting shape
    const std::size_t res_cols = rhs._shape.size() > 1 ? rhs._shape[1] : 1;
    std::vector<std::size_t> res_shape = rhs._shape.size() > 1
        ? std::vector<std::size_t> { _shape[0], rhs._shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { _shape[0] };               // rhs is 1-D

    // Resulting number of bits
    std::size_t pad_bits = _shape[1] ? bit_width(_shape[1] - 1) : 0;
    std::size_t res_bits = 1 + bits() + rhs.bits() + pad_bits;
    std::size_t res_int_bits = 1 + int_bits() + rhs.int_bits() + pad_bits;
    if (mode.has_value()) {
        res_bits = mode->bits;
        res_int_bits = mode->int_bits;
    }

    // Resulting tensor and a working column from `rhs`
    APyCFixedArray res(res_shape, res_bits, res_int_bits);
    APyCFixedArray current_col({ rhs._shape[0] }, rhs.bits(), rhs.int_bits());

    auto inner_product
        = ComplexFixedPointInnerProduct(spec(), rhs.spec(), res.spec(), mode);
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

//! Perform a linear convolution with `other` using `mode`
APyCFixedArray APyCFixedArray::convolve(
    const APyCFixedArray& other, const std::string& conv_mode
) const
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
    APyCFixedArray b_cpy = swap ? *this : other;
    multi_limb_reverse(std::begin(b_cpy._data), std::end(b_cpy._data), b_cpy._itemsize);

    // Let `a` be a pointer to the longer array, and let `b` be a pointer to the reverse
    // copy of the shorter array.
    const APyCFixedArray* a = swap ? &other : this;
    const APyCFixedArray* b = &b_cpy;

    // Get convolution properties from the convolution mode
    auto [len, n_left, n_right] = get_conv_lengths(conv_mode, a, b);

    // Compute resulting bit specification
    int pad_bits = b->_shape[0] ? bit_width(b->_shape[0] - 1) : 0;
    int res_bits = 1 + a->bits() + b->bits() + pad_bits;
    int res_int_bits = 1 + a->int_bits() + b->int_bits() + pad_bits;
    std::optional<APyFixedAccumulatorOption> acc = get_accumulator_mode_fixed();
    if (acc.has_value()) {
        res_bits = acc->bits;
        res_int_bits = acc->int_bits;
    }

    APyCFixedArray res({ len }, res_bits, res_int_bits);

    auto inner_product
        = ComplexFixedPointInnerProduct(a->spec(), b->spec(), res.spec(), acc);

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
