#ifndef _ARRAY_UTILS_H
#define _ARRAY_UTILS_H

#include "apytypes_util.h"
#include <nanobind/nanobind.h>
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

#endif // _ARRAY_UTILS_H
