/*
 * Common general broadcasting functionality for APyTypes
 *
 * NumPy fundamental broadcasting:
 * https://numpy.org/doc/stable/user/basics.broadcasting.html
 */

#include "apytypes_util.h"

#include <algorithm> // std::any_of, std::copy_n
#include <tuple>
#include <vector> // std::vector

//! Test if the shape `src_shape` can be broadcast to `dst_shape`
static APY_INLINE bool is_broadcastable(
    const std::vector<std::size_t>& src_shape, const std::vector<std::size_t>& dst_shape
)
{
    // Can not broadcast if either source or destination shape are zero-dimensional
    if (src_shape.size() == 0 || dst_shape.size() == 0) {
        return false;
    }

    // Can not broadcast if either source or destination has any zero-dimension
    auto is_zero = [](auto n) { return n == 0; };
    if (std::any_of(std::begin(src_shape), std::end(src_shape), is_zero)) {
        return false;
    }
    if (std::any_of(std::begin(dst_shape), std::end(dst_shape), is_zero)) {
        return false;
    }

    // Can not broadcast if destination shape has fewer dimensions than source shape
    if (src_shape.size() > dst_shape.size()) {
        return false;
    }

    // Iterate shapes from the trailing (right-most) dimensions
    auto [src_it, dst_it] = std::make_tuple(src_shape.crbegin(), dst_shape.crbegin());
    while (src_it != src_shape.crend() && dst_it != dst_shape.crend()) {
        if (*src_it != 1) {
            if (*src_it != *dst_it) {
                return false;
            }
        }
        ++src_it;
        ++dst_it;
    }

    return true;
}

//! Perform a broadcast by copying data. This function assumes that `src_shape` can be
//! broadcast to `dst_shape`, i.e., `is_broadcastable(src_shape, dst_shape) == true`. It
//! further assumes that the data in `src` is stored in C-style order and that `dst` has
//! enough space to store the broadcast result.
template <typename RANDOM_ACCESS_CONST_ITERATOR, typename RANDOM_ACCESS_ITERATOR>
static APY_INLINE void broadcast_data_copy(
    RANDOM_ACCESS_CONST_ITERATOR src_it,
    RANDOM_ACCESS_ITERATOR dst_it,
    const std::vector<std::size_t>& src_shape,
    const std::vector<std::size_t>& dst_shape,
    std::size_t itemsize = 1 // TODO
)
{
    std::size_t src_elements = fold_shape(src_shape);
    std::size_t dst_elements = fold_shape(dst_shape);
    std::size_t broadcast_elements = dst_elements / src_elements;

    // Compute the destination stride vector
    std::vector<std::size_t> dst_stride = strides_from_shape(dst_shape);

    // Compute the adjusted source shape
    std::size_t index_diff = dst_shape.size() - src_shape.size();
    std::vector<std::size_t> src_shape_adjusted(dst_shape.size());
    for (std::size_t i = 0; i < dst_shape.size(); i++) {
        if (i < index_diff) {
            src_shape_adjusted[i] = 1;
        } else {
            src_shape_adjusted[i] = src_shape[i - index_diff];
        }
    }

    // Compute the broadcasting vector
    std::vector<std::size_t> broadcast_vector(dst_shape.size());
    for (std::size_t i = 0; i < dst_shape.size(); i++) {
        broadcast_vector[i] = dst_shape[i] - src_shape_adjusted[i] + 1;
    }

    // Compute broadcast offsets (stride-weighted permutations of broadcasting vector)
    std::vector<std::size_t> broadcast_offset(broadcast_elements);
    for (std::size_t i = 0; i < broadcast_elements; i++) {
        std::size_t index = i;
        std::vector<std::size_t> broadcast_cord(src_shape_adjusted.size());
        for (std::size_t j = dst_shape.size(); j--;) {
            if (index == 0) {
                break;
            }
            broadcast_cord[j] = index % broadcast_vector[j];
            index /= broadcast_vector[j];
        }

        // Convert broadcast coordinate to weighted offset
        std::size_t offset = 0;
        for (std::size_t j = 0; j < broadcast_cord.size(); j++) {
            offset += dst_stride[j] * broadcast_cord[j];
        }
        broadcast_offset[i] = offset;
    }

    // Loop over elements in the source vector and broadcast
    std::vector<std::size_t> src_cord(src_shape_adjusted.size());
    for (std::size_t i = 0; i < src_elements; i++) {
        // Compute the source coordinate for the i-th source element
        std::size_t index = i;
        for (std::size_t j = src_shape_adjusted.size(); j--;) {
            if (index == 0) {
                break;
            }
            src_cord[j] = index % src_shape_adjusted[j];
            index /= src_shape_adjusted[j];
        }

        // Convert source coordinate for the i-th element to reference destination index
        std::size_t dst_idx = 0;
        for (std::size_t j = 0; j < src_cord.size(); j++) {
            dst_idx += dst_stride[j] * src_cord[j];
        }

        // Broadcast the data
        for (std::size_t j = 0; j < broadcast_elements; j++) {
            std::copy_n(
                src_it + i * itemsize,
                itemsize,
                dst_it + (dst_idx + broadcast_offset[j]) * itemsize
            );
        }
    }
}
