#ifndef _ARRAY_UTILS_H
#define _ARRAY_UTILS_H

#include "apytypes_util.h"
#include "broadcast.h"
#include "nanobind/ndarray.h"
#include <cstddef>
#include <nanobind/nanobind.h>
#include <ostream>
#include <set>
#include <unordered_set>
#include <variant>
namespace nb = nanobind;

#include <vector>

static APY_INLINE std::vector<std::size_t>
shape_from_tuple(nb::tuple new_shape, size_t elem_count)
{
    std::vector<std::size_t> new_shape_vec;
    std::size_t reshape_size = 1;
    int neg_one_count = 0;
    int neg_one_pos = -1;
    int index = 0;
    for (auto it = new_shape.begin(); it != new_shape.end(); ++it, ++index) {
        int current_value = nb::cast<int>(*it);
        if (current_value == -1) {
            neg_one_count++;
            neg_one_pos = index;        // store the position of -1
            new_shape_vec.push_back(1); // temporarily push 1, will be replaced later
        } else if (current_value < -1) {
            throw nb::value_error("Negative dimensions less than -1 are not allowed.");
        } else {
            std::size_t c_size_t = nb::cast<std::size_t>(*it);
            new_shape_vec.push_back(c_size_t);
            reshape_size *= c_size_t;
        }
    }

    if (neg_one_count > 1) {
        throw nb::value_error("Only one dimension can be -1.");
    }

    if (neg_one_count == 1) {
        if (elem_count % reshape_size != 0) {
            throw nb::value_error(
                "The total size of new array must be unchanged and divisible by the "
                "known dimensions."
            );
        }
        new_shape_vec[neg_one_pos] = elem_count / reshape_size;
    }

    std::size_t reshape_elem_count = ::fold_shape(new_shape_vec);

    // Validate if total elements match the original array
    if (elem_count != reshape_elem_count) {
        throw nb::value_error(
            "Total number of elements does not match the original array."
        );
    }

    return new_shape_vec;
}

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
    bool is_tuple = std::holds_alternative<nanobind::tuple>(axes);
    std::vector<int> cpp_axes {};
    if (is_tuple) {
        for (const auto& element : std::get<nanobind::tuple>(axes)) {
            cpp_axes.push_back(nanobind::cast<int>(element));
        }

    } else {
        cpp_axes.push_back(static_cast<int>(std::get<nanobind::int_>(axes)));
    }

    std::unordered_set<std::size_t> unique_axes;

    std::vector<std::size_t> result {};
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

#endif // _ARRAY_UTILS_H
