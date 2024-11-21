#ifndef _ARRAY_UTILS_H
#define _ARRAY_UTILS_H

#include "apyfixed.h"
#include "apyfloat.h"
#include "apytypes_util.h"
#include "broadcast.h"
#include "nanobind/ndarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h>
namespace nb = nanobind;

#include <cstddef>
#include <unordered_set>
#include <variant>
#include <vector>

/**
 * @brief Converts the provided axes into a vector of positive indices.
 *
 * @throws nb::value_error if an axis value is out of bounds, if there are duplicate
 * axes in the tuple or the axis don't match.
 */
static APY_INLINE std::vector<std::size_t> get_normalized_axes(
    const std::variant<nb::tuple, nb::int_>& axes, const std::size_t n_dim
)
{
    std::vector<std::size_t> result {};
    std::vector<int> cpp_axes = cpp_shape_from_python_shape_like<int, true>(axes);
    std::unordered_set<std::size_t> unique_axes;

    for (std::size_t i = 0; i < cpp_axes.size(); i++) {
        int axis = cpp_axes[i];
        if (axis < 0) {
            axis = n_dim + axis;
        }
        std::size_t pos_axis = static_cast<std::size_t>(axis);
        if (pos_axis >= n_dim) {
            throw nb::value_error("Axis don't match array");
        }
        if (!unique_axes.insert(pos_axis).second) {
            throw nb::value_error("Repeated axis in provided axes");
        }
        result.push_back(pos_axis);
    }

    return result;
}

/**
 * @brief Permutes the dimensions of the input data and copies it to the destination.
 *
 * This function permutes the dimensions of the source data according to the provided
 * permutation array and copies the permuted data to the destination.
 * Assumunes that the sizes specified in src_shape and perm match the dimensionality of
 * the data.
 */
template <typename RANDOM_ACCESS_CONST_ITERATOR, typename RANDOM_ACCESS_ITERATOR>
static APY_INLINE void transpose_axes_and_copy_data(
    RANDOM_ACCESS_CONST_ITERATOR src,
    RANDOM_ACCESS_ITERATOR dst,
    const std::vector<std::size_t>& src_shape,
    const std::vector<std::size_t>& new_axis,
    std::size_t itemsize = 1
)
{
    // Calculate the destination shape based on the permutation
    std::vector<std::size_t> dst_shape(src_shape.size());
    for (std::size_t i = 0; i < new_axis.size(); ++i) {
        dst_shape[i] = src_shape[new_axis[i]];
    }

    std::vector<std::size_t> dst_stride = strides_from_shape(dst_shape);

    // Start at indice [0,0, ..., 0]
    std::vector<std::size_t> indices(src_shape.size(), 0);
    std::vector<std::size_t> dst_indices(indices.size());

    std::size_t num_elem = fold_shape(src_shape);

    for (std::size_t src_index = 0; src_index < num_elem; ++src_index) {

        // For each dimension in the destination, assign the corresponding index from
        // the source based on the new axis. For example, if new_axis = {1, 2, 0}, and
        // current_indices = {i, j, k}, the resulting dst_indices will be {j, k, i}
        std::size_t dst_index = 0;
        for (std::size_t i = 0; i < indices.size(); ++i) {
            std::size_t tmp_index = indices[new_axis[i]];

            // Calculate the flat index for destination
            dst_index += tmp_index * dst_stride[i];
        }

        // Copy the data
        std::copy_n(src + src_index * itemsize, itemsize, dst + dst_index * itemsize);

        // Increment the indices one by one
        for (std::size_t j = indices.size(); j-- > 0;) {
            if (++indices[j] < src_shape[j])
                break;
            indices[j] = 0;
        }
    }
}

//! Convert a value to APyFixed. The format of the result will be big enough to
//! accommodate the result.
static APY_INLINE APyFixed to_apyfixed(const nb::object& val)
{
    if (nb::isinstance<APyFloat>(val)) {
        const auto v = static_cast<APyFloat>(nb::cast<APyFloat>(val));
        return v.to_fixed();
    } else if (nb::isinstance<APyFixed>(val)) {
        return static_cast<APyFixed>(nb::cast<APyFixed>(val));
    } else if (nb::isinstance<nb::int_>(val)) {
        return APyFixed::from_unspecified_integer(nb::cast<nb::int_>(val));
    } else if (nb::isinstance<nb::float_>(val)) {
        const auto v = static_cast<double>(nb::cast<nb::float_>(val));
        return APyFixed::from_unspecified_double(v);
    } else {
        const nb::type_object type = nb::cast<nb::type_object>(val.type());
        const nb::str type_string = nb::str(type);
        throw std::domain_error(
            std::string("Non supported type: ") + type_string.c_str()
        );
    }
}

//! Generate a vector of APyFixed object with evenly spaced values within a given range.
static APY_INLINE std::vector<APyFixed>
arange(const nb::object& start, const nb::object& stop, const nb::object& step)
{
    const APyFixed apy_step = to_apyfixed(step);
    const APyFixed apy_start = to_apyfixed(start);
    const APyFixed apy_stop = to_apyfixed(stop);

    if (apy_step.is_zero()) {
        throw nb::value_error("Step size cannot be zero");
    }

    if (!(apy_start < apy_stop && apy_step.is_positive())
        && !(apy_start > apy_stop && apy_step.is_negative())) {
        throw nb::value_error("Undefined inputs");
    }

    std::vector<APyFixed> result;

    auto curr_val = apy_start;

    if (apy_step.is_negative()) {
        while (curr_val > apy_stop) {
            result.push_back(curr_val);
            curr_val = curr_val + apy_step;
        }
    } else {
        while (curr_val < apy_stop) {
            result.push_back(curr_val);
            curr_val = curr_val + apy_step;
        }
    }

    return result;
}

#endif // _ARRAY_UTILS_H
