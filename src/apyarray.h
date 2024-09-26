/*
 * APyTypes array base class, for sharing functionality between array types.
 */

#ifndef _APYARRAY_H
#define _APYARRAY_H

#include "apybuffer.h"
#include "apytypes_util.h"
#include "broadcast.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

#include <iterator> // std::begin
#include <variant>  // std::variant

template <typename T, typename ARRAY_TYPE> class APyArray : public APyBuffer<T> {

public:
    //! Base constructor for creating a new `APyArray`
    APyArray(const std::vector<std::size_t>& shape, std::size_t itemsize = 1)
        : APyBuffer<T>(shape, itemsize)
    {
    }

    using APyBuffer<T>::_shape;
    using APyBuffer<T>::_ndim;
    using APyBuffer<T>::_data;
    using APyBuffer<T>::_itemsize;

    /* ****************************************************************************** *
     * *                    `__getitem__` family of methods                         * *
     * ****************************************************************************** */

    //! Retrieve item from integer index
    auto get_item_integer(std::ptrdiff_t idx) const
    {
        using SCALAR_TYPE = typename ARRAY_TYPE::SCALAR_VARIANT;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        // Bounds checking
        if (idx >= std::ptrdiff_t(_shape[0]) || idx < -std::ptrdiff_t(_shape[0])) {
            std::string msg = fmt::format(
                "{}.__getitem__: index {} is out of bounds for axis 0 with size {}",
                ARRAY_TYPE::ARRAY_NAME,
                idx,
                _shape[0]
            );
            throw std::out_of_range(msg);
        }

        // Adjust for negative index
        idx = idx < 0 ? idx + _shape[0] : idx;

        if (_ndim == 1) {
            SCALAR_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_scalar();
            result.copy_n_from(std::begin(_data) + idx * _itemsize, _itemsize);
            return RESULT_TYPE(result);
        } else {
            // New shape contains all dimensions except the very first one
            auto new_shape
                = std::vector<std::size_t>(std::begin(_shape) + 1, std::end(_shape));

            // Element stride is the new shape folded over multiplication
            std::size_t element_stride = fold_shape(new_shape);

            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);
            std::copy_n(
                std::begin(_data) + idx * _itemsize * element_stride,
                _itemsize * element_stride,
                std::begin(result._data)
            );
            return RESULT_TYPE(result);
        }
    }

    //! Retrieve item from a slice index
    auto get_item_slice(nb::slice slice) const
    {
        auto [start, stop, step, len] = slice.compute(_shape[0]);

        // New resulting shape
        std::vector<std::size_t> new_shape = _shape;
        new_shape[0] = len;

        // Result array
        ARRAY_TYPE result
            = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);

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

    //! Convert a `nb::tuple` of `nb::int_`, `nb::slice`, and `nb::ellipsis` to
    //! a `std::vector<std::variant<nb::int_, nb::slice>>`, resolving any ellipsis
    //! slicing.
    auto get_item_to_cpp_tuple(const nb::tuple& key) const
    {
        if (key.size() > _ndim) {
            // The key tuple must have fewer elements than this array has number of
            // dimensions
            std::string msg = fmt::format(
                "{}.__getitem__: key tuple size (={}) > ndim (={})",
                ARRAY_TYPE::ARRAY_NAME,
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
                    // An ellipsis object has already been procecced. Only a single
                    // ellipsis per tuple key is allowed
                    std::string msg = fmt::format(
                        "{}.__getitem__: only one single ellipsis object allowed",
                        ARRAY_TYPE::ARRAY_NAME
                    );
                    throw nb::value_error(msg.c_str());
                } else {
                    // Found first ellipsis, fill missing dimesnsions with full slices
                    ellipsis_found = true;
                    std::size_t n_fill = _ndim - key.size() + 1;
                    for (std::size_t j = 0; j < n_fill; j++) {
                        cpp_tuple.push_back(nb::slice(_shape[i + j]));
                    }
                }
            } else {
                std::string msg = fmt::format(
                    "{}.__getitem__: supported keys are `int`, `slice`, `ellipsis`, or "
                    "a single tuple thereof",
                    ARRAY_TYPE::ARRAY_NAME
                );
                throw nb::value_error(msg.c_str());
            }
        }
        return cpp_tuple;
    }

    //! Helper method for `get_item_tuple`, returns the shape when slicing using `tuple`
    //! and `remaining`
    std::vector<std::size_t> get_item_tuple_shape(
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
                auto [_, __, ___, len]
                    = std::get<nb::slice>(element).compute(_shape[i]);
                result_shape.push_back(len);
            }
        }
        for (std::size_t i = remaining.size() + 1; i < _shape.size(); i++) {
            result_shape.push_back(_shape[i]);
        }
        return result_shape;
    }

    //! Helper method for `get_item_tuple`, returns sliced array in `std::vector`
    auto get_item_slice_nested(nb::slice slice) const
    {
        auto [start, stop, step, len] = slice.compute(_shape[0]);

        // The shape of the sliced vectors
        std::vector<std::size_t> new_shape(std::begin(_shape) + 1, std::end(_shape));
        if (!new_shape.size()) {
            new_shape = { 1 };
        }

        // Result array
        std::vector<ARRAY_TYPE> result;

        auto size = fold_shape(new_shape) * _itemsize;
        ARRAY_TYPE tmp = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);
        if (step < 0) {
            // Copy data into result and return (negative src step size)
            for (std::ptrdiff_t src_i = start; src_i > stop; src_i += step) {
                std::copy_n(
                    std::begin(_data) + src_i * size, size, std::begin(tmp._data)
                );
                result.push_back(tmp);
            }
        } else {
            // Copy data into result and return (positive src step size)
            for (std::ptrdiff_t src_i = start; src_i < stop; src_i += step) {
                std::copy_n(
                    std::begin(_data) + src_i * size, size, std::begin(tmp._data)
                );
                result.push_back(tmp);
            }
        }
        return result;
    }

    //! Retrieve item(s) from a `std::vector` of `nb::int_` and `nb::slice`
    auto get_item_tuple(std::vector<std::variant<nb::int_, nb::slice>> tuple) const
    {
        using SCALAR_TYPE = typename ARRAY_TYPE::SCALAR_VARIANT;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        // Return everything on an empty tuple
        if (tuple.size() == 0) {
            ARRAY_TYPE result = *static_cast<const ARRAY_TYPE*>(this);
            return RESULT_TYPE(result);
        }

        std::variant<nb::int_, nb::slice> current = tuple[0];
        std::vector<std::variant<nb::int_, nb::slice>> remaining(
            std::begin(tuple) + 1, std::end(tuple)
        );

        if (std::holds_alternative<nb::int_>(current)) {

            /*
             * Current tuple element is an integer
             */
            std::ptrdiff_t key
                = static_cast<std::ptrdiff_t>(std::get<nb::int_>(current));
            RESULT_TYPE result = get_item_integer(key);
            if (!remaining.size()) {
                return result; // is always SCALAR_TYPE
            } else {
                return RESULT_TYPE(std::get<ARRAY_TYPE>(result).get_item_tuple(remaining
                ));
            }

        } else { /* std::holds_alternative<nb::slice>(current) */

            /*
             * Current tuple element is a slice
             */
            std::vector<std::size_t> shape = get_item_tuple_shape(tuple, remaining);
            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
            nb::slice slice = std::get<nb::slice>(current);
            auto arrays = get_item_slice_nested(slice);

            for (std::size_t i = 0; i < arrays.size(); i++) {
                auto v = arrays[i].get_item_tuple(remaining);
                if (std::holds_alternative<SCALAR_TYPE>(v)) {
                    auto scalar = std::get<SCALAR_TYPE>(v);
                    scalar.copy_n_to(
                        std::begin(result._data) + i * _itemsize, _itemsize
                    );
                } else {
                    auto array = std::get<ARRAY_TYPE>(v);
                    std::copy_n(
                        std::begin(array._data),
                        array._nitems * _itemsize,
                        std::begin(result._data) + i * array._nitems * _itemsize
                    );
                }
            }

            return RESULT_TYPE(result);
        }
    }

    //! Top-level Python-exported `__getitem__` function
    auto get_item(std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple> key) const
    {
        using SCALAR_TYPE = typename ARRAY_TYPE::SCALAR_VARIANT;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        if (std::holds_alternative<nb::int_>(key)) {

            // Key is of integer type
            return RESULT_TYPE(
                get_item_integer(static_cast<std::ptrdiff_t>(std::get<nb::int_>(key)))
            );

        } else if (std::holds_alternative<nb::slice>(key)) {

            // Key is of slice type
            return RESULT_TYPE(get_item_slice(std::get<nb::slice>(key)));

        } else if (std::holds_alternative<nb::ellipsis>(key)) {

            // Key is a single ellipsis, return a copy of `*this`
            return RESULT_TYPE(*static_cast<const ARRAY_TYPE*>(this));

        } else { /* std::holds_alternative<nb::tuple>(key) */

            // Key is a tuple of slicing instructions
            return RESULT_TYPE(
                get_item_tuple(get_item_to_cpp_tuple(std::get<nb::tuple>(key)))
            );
        }
    }

    /* ****************************************************************************** *
     * *                     `broadcast` family of methods                          * *
     * ****************************************************************************** */

    //! Broadcast `*this` array to `shape`. Throws `nb::value_error` if `*this` shape is
    //! incompatible with `shape`.
    ARRAY_TYPE broadcast_to(const std::vector<std::size_t> shape) const
    {
        if (!is_broadcastable(_shape, shape)) {
            std::string error_msg = fmt::format(
                "Operands could not be broadcast together with shapes: {}, {}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(shape)
            );
            throw nb::value_error(error_msg.c_str());
        }

        ARRAY_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
        broadcast_data_copy(
            _data.begin(), result._data.begin(), _shape, shape, _itemsize
        );
        return result;
    }

    //! Python exported `broadcast_to` method
    ARRAY_TYPE broadcast_to_python(const std::variant<nb::tuple, nb::int_> shape) const
    {
        return broadcast_to(cpp_shape_from_python_shape_like(shape));
    }

}; // class APyArray

#endif
