/*
 * Common general broadcasting functionality for APyTypes
 *
 * NumPy fundamental broadcasting:
 * https://numpy.org/doc/stable/user/basics.broadcasting.html
 */

#ifndef _BROADCAST_H
#define _BROADCAST_H

#include "apytypes_util.h"

#include <algorithm> // std::any_of, std::copy_n
#include <tuple>     // std::make_tuple
#include <vector>    // std::vector

//! Test if `src_shape` can be broadcast to `dst_shape`
static APY_INLINE bool is_broadcastable(
    const std::vector<std::size_t>& src_shape, const std::vector<std::size_t>& dst_shape
)
{
    // Cannot broadcast if either shape is zero-dimensional
    if (src_shape.size() == 0 || dst_shape.size() == 0) {
        return false;
    }

    // Cannot broadcast if either shape has a zero dimension
    auto is_zero = [](auto n) { return n == 0; };
    for (const auto& shape : { src_shape, dst_shape }) {
        if (std::any_of(std::begin(shape), std::end(shape), is_zero)) {
            return false;
        }
    }

    // Cannot broadcast if destination shape has fewer dimensions than source shape
    if (src_shape.size() > dst_shape.size()) {
        return false;
    }

    // Iterate shapes from the trailing (right-most) dimensions
    auto [src_it, dst_it] = std::make_tuple(src_shape.crbegin(), dst_shape.crbegin());
    while (src_it != src_shape.crend() && dst_it != dst_shape.crend()) {
        if (*src_it != 1 && *src_it != *dst_it) {
            return false;
        }
        ++src_it;
        ++dst_it;
    }

    return true;
}

//! Get the smallest broadcastable shape from `shape1` and `shape2`. Return an empty
//! vector `_.size() == 0` if the shapes cannot be broadcast together.
static APY_INLINE std::vector<std::size_t> smallest_broadcastable_shape(
    const std::vector<std::size_t>& shape1, const std::vector<std::size_t>& shape2
)
{
    // Cannot broadcast if either shape is zero-dimensional
    if (shape1.size() == 0 || shape2.size() == 0) {
        return {};
    }

    // Cannot broadcast if any shape dimension is zero
    auto is_zero = [](auto n) { return n == 0; };
    for (const auto& shape : { shape1, shape2 }) {
        if (std::any_of(std::begin(shape), std::end(shape), is_zero)) {
            return {};
        }
    }

    // Iterate shapes from the trailing (right-most) dimensions
    std::vector<std::size_t> result {};
    auto [shape1_it, shape2_it] = std::make_tuple(shape1.crbegin(), shape2.crbegin());
    while (shape1_it != shape1.crend() && shape2_it != shape2.crend()) {
        if (*shape1_it != 1 && *shape2_it != 1 && *shape1_it != *shape2_it) {
            // Dimension disallows broadcasting
            return {};
        }
        result.push_back(std::max(*shape1_it, *shape2_it));
        ++shape1_it;
        ++shape2_it;
    }

    // Add remaining shapes from the more high-dimensional shape
    for (; shape1_it < shape1.crend(); ++shape1_it) {
        result.push_back(*shape1_it);
    }
    for (; shape2_it < shape2.crend(); ++shape2_it) {
        result.push_back(*shape2_it);
    }

    // Reverse the dimensions and return
    std::reverse(std::begin(result), std::end(result));
    assert(is_broadcastable(shape1, result));
    assert(is_broadcastable(shape2, result));
    return result;
}

//! Compute the destination index of source index `i` based on a `broadcasting_rule`.
//! The destination index is weighted using `strides`. This function assumes
//! that `strides.size() == broadcast_rule.size()`
template <typename FUNC>
static APY_INLINE std::size_t src_to_dst_idx(
    std::size_t i, FUNC&& broadcast_rule, const std::vector<std::size_t>& strides
)
{
    std::size_t offset = 0;
    for (std::size_t j = strides.size(); j--;) {
        if (i == 0) {
            break;
        }
        offset += strides[j] * (i % broadcast_rule(j));
        i /= broadcast_rule(j);
    }
    return offset;
}

//! Perform a broadcast by copying data. This function assumes that `src_shape` can be
//! broadcast to `dst_shape`, i.e., `is_broadcastable(src_shape, dst_shape) == true`. It
//! further assumes that the data in `src` is stored in C-style order and that `dst` has
//! enough space to store the broadcast result.
template <typename RANDOM_ACCESS_CONST_ITERATOR, typename RANDOM_ACCESS_ITERATOR>
static APY_INLINE void broadcast_data_copy(
    RANDOM_ACCESS_CONST_ITERATOR src,
    RANDOM_ACCESS_ITERATOR dst,
    const std::vector<std::size_t>& src_shape,
    const std::vector<std::size_t>& dst_shape,
    std::size_t itemsize = 1
)
{
    std::size_t src_elements = fold_shape(src_shape);
    std::size_t dst_elements = fold_shape(dst_shape);
    std::size_t broadcast_elements = dst_elements / src_elements;

    if (src_elements == dst_elements) {
        std::copy_n(src, src_elements * itemsize, dst);
        return; // early exit
    }

    // Destination strides and intermediate working coordinate vector
    std::vector<std::size_t> dst_stride = strides_from_shape(dst_shape);

    // Lambda for left-padded source shape
    auto src_shape_adjst = [&](std::size_t i) -> std::size_t {
        std::size_t shape_idx_diff = dst_shape.size() - src_shape.size();
        return i < shape_idx_diff ? 1 : src_shape[i - shape_idx_diff];
    };

    // Compute broadcast offsets (stride-weighted permutations of broadcasting vector)
    auto broadcast_vec = [&](auto j) { return dst_shape[j] - src_shape_adjst(j) + 1; };
    std::vector<std::size_t> broadcast_offsets(broadcast_elements);
    for (std::size_t i = 0; i < broadcast_elements; i++) {
        broadcast_offsets[i] = src_to_dst_idx(i, broadcast_vec, dst_stride);
    }

    // Loop over elements in the source vector and broadcast to the destination
    for (std::size_t i = 0; i < src_elements; i++) {
        auto dst_idx = src_to_dst_idx(i, src_shape_adjst, dst_stride);
        auto broadcast = [&](auto j) {
            std::copy_n(src + i * itemsize, itemsize, dst + (dst_idx + j) * itemsize);
        };
        std::for_each(broadcast_offsets.begin(), broadcast_offsets.end(), broadcast);
    }
}

#endif // _BROADCAST_H
