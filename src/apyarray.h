/*
 * APyTypes array base class, for sharing functionality between array types. All the
 * member functions of the APyTypes array types I don't want to write twice...
 */

#ifndef _APYARRAY_H
#define _APYARRAY_H

#include "apybuffer.h"
#include "apytypes_fwd.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "broadcast.h"
#include "fmt/format.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

#include <algorithm>   // std::min_element
#include <cassert>     // assert
#include <functional>  // std::function
#include <iterator>    // std::begin
#include <set>         // std::set
#include <string_view> // std::string_view
#include <variant>     // std::variant

template <typename T, typename ARRAY_TYPE> class APyArray : public APyBuffer<T> {

public:
    using vector_type = typename APyBuffer<T>::vector_type;
    using vector_iterator = typename vector_type::iterator;
    using vector_const_iterator = typename vector_type::const_iterator;
    using APyBuffer<T>::_shape;
    using APyBuffer<T>::_ndim;
    using APyBuffer<T>::_data;
    using APyBuffer<T>::_itemsize;
    using APyBuffer<T>::_nitems;

public:
    //! Base constructor for creating a new `APyArray`
    APyArray(const std::vector<std::size_t>& shape, std::size_t itemsize = 1)
        : APyBuffer<T>(shape, itemsize)
    {
    }

    //! Constructor for stealing data
    APyArray(
        const std::vector<std::size_t>& shape, std::size_t itemsize, vector_type&& v
    )
        : APyBuffer<T>(shape, itemsize, std::move(v))
    {
    }

    /* ****************************************************************************** *
     * *                    `__getitem__` family of methods                         * *
     * ****************************************************************************** */

    //! Convert a possibly negative integer index `idx` to a positive one if used to
    //! index dimension `dim` of `*this`. Throws `std::out_of_range` if the index could
    //! not be converted or is out of range.
    std::size_t adjust_integer_index(
        std::ptrdiff_t idx, std::size_t dim, std::string_view func_name
    ) const
    {
        // Bounds checking
        if (idx >= std::ptrdiff_t(_shape[dim]) || idx < -std::ptrdiff_t(_shape[dim])) {
            std::string msg = fmt::format(
                "{}.{}: index {} is out of bounds for axis {} with size {}",
                ARRAY_TYPE::ARRAY_NAME,
                func_name,
                idx,
                dim,
                _shape[dim]
            );
            throw std::out_of_range(msg);
        }

        return idx < 0 ? idx + _shape[dim] : idx;
    }

    //! Compute the resulting shape when slicing `*this` with non-empty C++ tuple `key`.
    //! Calling this function requires that `key.size() <= _shape.size()`
    std::vector<std::size_t>
    compute_slice_shape(const std::vector<std::variant<nb::int_, nb::slice>>& key) const
    {
        assert(key.size() <= _shape.size());
        std::vector<std::size_t> shape;

        // Elements in the key tuple
        for (std::size_t i = 0; i < key.size(); i++) {
            if (std::holds_alternative<nb::int_>(key[i])) {
                continue;
            } else { /* std::hold_alternative<nb::slice>(key[i]) */
                auto&& slice = std::get<nb::slice>(key[i]);
                auto [_, __, ___, len] = slice.compute(_shape[i]);
                shape.push_back(len);
            }
        }

        // Any dimension in `*this` not referred to in `key` simply "come along"
        for (std::size_t i = key.size(); i < _shape.size(); i++) {
            shape.push_back(_shape[i]);
        }

        return shape;
    }

    //! Convert a `nb::tuple` of `nb::int_`, `nb::slice`, and `nb::ellipsis` to
    //! a `std::vector<std::variant<nb::int_, nb::slice>>`, resolving any ellipsis
    //! slicing.
    std::vector<std::variant<nb::int_, nb::slice>>
    resolve_python_tuple_slice(const nb::tuple& key, std::string_view func_name) const
    {
        if (key.size() > _ndim) {
            // The key tuple must have fewer elements than this array has number of
            // dimensions
            std::string msg = fmt::format(
                "{}.{}: key tuple size (={}) > ndim (={})",
                ARRAY_TYPE::ARRAY_NAME,
                func_name,
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
                        "{}.{}: only one single ellipsis object allowed",
                        ARRAY_TYPE::ARRAY_NAME,
                        func_name
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
                    "{}.{}: supported keys are `int`, `slice`, `ellipsis`, or a single "
                    "tuple thereof",
                    ARRAY_TYPE::ARRAY_NAME,
                    func_name
                );
                throw nb::value_error(msg.c_str());
            }
        }
        return cpp_tuple;
    }

    //! Retrieve item from integer index
    std::variant<ARRAY_TYPE, scalar_variant_t<ARRAY_TYPE>>
    get_item_integer(std::ptrdiff_t idx) const
    {
        using SCALAR_TYPE = scalar_variant_t<ARRAY_TYPE>;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        // Adjust a possibly negative index
        idx = adjust_integer_index(idx, 0, "__getitem__");

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

    //! Working part of `get_item_tuple`, using recursive descent to copy items from
    //! `input_it` to `output_it`. Return the number of elements copied. Assumes that
    //! `dim < tuple.size()`.
    std::size_t get_item_tuple_recursive_descent(
        const std::vector<std::variant<nb::int_, nb::slice>>& tuple,
        const std::vector<std::size_t>& strides,
        typename vector_type::const_iterator input_it,
        typename vector_type::iterator output_it,
        std::size_t dim = 0
    ) const
    {
        auto&& element = tuple[dim];
        if (dim == tuple.size() - 1) {
            /*
             * Final dimension in the tuple slice of the recursive descent. Do the
             * actual copying of data.
             */
            if (std::holds_alternative<nb::int_>(element)) {
                auto idx = static_cast<std::ptrdiff_t>(std::get<nb::int_>(element));
                idx = adjust_integer_index(idx, dim, "__getitem__");
                std::copy_n(
                    input_it + _itemsize * idx * strides[dim],
                    _itemsize * strides[dim],
                    output_it
                );
                return strides[dim];
            } else { /* std::holds_alternative<nb::slice>(element) */
                auto&& slice = std::get<nb::slice>(element);
                auto [start, stop, step, len] = slice.compute(_shape[dim]);
                std::size_t elements_copied = 0;
                if (step < 0) {
                    for (std::ptrdiff_t src_i = start; src_i > stop; src_i += step) {
                        std::copy_n(
                            input_it + _itemsize * src_i * strides[dim],
                            _itemsize * strides[dim],
                            output_it + _itemsize * elements_copied
                        );
                        elements_copied += strides[dim];
                    }
                } else { /* step >= 0 */
                    for (std::ptrdiff_t src_i = start; src_i < stop; src_i += step) {
                        std::copy_n(
                            input_it + _itemsize * src_i * strides[dim],
                            _itemsize * strides[dim],
                            output_it + _itemsize * elements_copied
                        );
                        elements_copied += strides[dim];
                    }
                }
                return elements_copied;
            }
        } else {
            /*
             * This is not yet the final dimesnsion of the tuple slice in this recursive
             * descent. We need to go deeper...
             */
            if (std::holds_alternative<nb::int_>(element)) {
                auto idx = static_cast<std::ptrdiff_t>(std::get<nb::int_>(element));
                idx = adjust_integer_index(idx, dim, "__getitem__");
                return get_item_tuple_recursive_descent(
                    tuple,
                    strides,
                    input_it + _itemsize * idx * strides[dim],
                    output_it,
                    dim + 1
                );
            } else { /* std::holds_alternative<nb::slice>(tuple_element) */
                auto&& slice = std::get<nb::slice>(element);
                auto [start, stop, step, len] = slice.compute(_shape[dim]);
                std::size_t elements_copied = 0;
                if (step < 0) {
                    for (std::ptrdiff_t src_i = start; src_i > stop; src_i += step) {
                        elements_copied += get_item_tuple_recursive_descent(
                            tuple,
                            strides,
                            input_it + _itemsize * src_i * strides[dim],
                            output_it + _itemsize * elements_copied,
                            dim + 1
                        );
                    }
                } else { /* step >= 0 */
                    for (std::ptrdiff_t src_i = start; src_i < stop; src_i += step) {
                        elements_copied += get_item_tuple_recursive_descent(
                            tuple,
                            strides,
                            input_it + _itemsize * src_i * strides[dim],
                            output_it + _itemsize * elements_copied,
                            dim + 1
                        );
                    }
                }

                return elements_copied;
            }
        }
    }

    //! Retrieve item(s) from a `std::vector` of `nb::int_` and `nb::slice`. Assumes
    //! that `tuple.size() <= _shape.size()`.
    std::variant<ARRAY_TYPE, scalar_variant_t<ARRAY_TYPE>>
    get_item_tuple(const std::vector<std::variant<nb::int_, nb::slice>>& tuple) const
    {
        assert(tuple.size() <= _shape.size());
        using SCALAR_TYPE = scalar_variant_t<ARRAY_TYPE>;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        // Return everything on an empty tuple
        if (tuple.size() == 0) {
            ARRAY_TYPE result = *static_cast<const ARRAY_TYPE*>(this);
            return RESULT_TYPE(result);
        }

        // Compute the resulting shape
        std::vector<std::size_t> result_shape = compute_slice_shape(tuple);

        // Compute the stride of `*this`
        std::vector<std::size_t> strides = strides_from_shape(_shape);

        if (result_shape.size() == 0) {
            /*
             * The result is scalar
             */
            SCALAR_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_scalar();
            std::size_t item_idx = 0;
            for (std::size_t i = 0; i < tuple.size(); i++) {
                assert(std::holds_alternative<nb::int_>(tuple[i]));
                auto axis = static_cast<std::ptrdiff_t>(std::get<nb::int_>(tuple[i]));
                axis = adjust_integer_index(axis, i, "__getitem__");
                item_idx += strides[i] * axis;
            }
            result.copy_n_from(std::begin(_data) + item_idx * _itemsize, _itemsize);
            return RESULT_TYPE(result);
        } else {
            /*
             * The result is an array
             */
            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(result_shape);

            // Perform the recursive descent of data-copying along each dimension
            get_item_tuple_recursive_descent(
                tuple, strides, std::cbegin(_data), std::begin(result._data)
            );

            return RESULT_TYPE(result);
        }
    }

    //! Top-level Python exported `__getitem__` function
    std::variant<ARRAY_TYPE, scalar_variant_t<ARRAY_TYPE>> get_item(
        const std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple>& key
    ) const
    {
        using SCALAR_TYPE = scalar_variant_t<ARRAY_TYPE>;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        if (std::holds_alternative<nb::int_>(key)) {

            // Key is of integer type
            return RESULT_TYPE(
                get_item_integer(static_cast<std::ptrdiff_t>(std::get<nb::int_>(key)))
            );

        } else if (std::holds_alternative<nb::slice>(key)) {

            // Key is of slice type
            nb::tuple tuple = nb::make_tuple(std::get<nb::slice>(key));
            return RESULT_TYPE(
                get_item_tuple(resolve_python_tuple_slice(tuple, "__getitem__"))
            );

        } else if (std::holds_alternative<nb::ellipsis>(key)) {

            // Key is a single ellipsis, return a copy of `*this`
            return RESULT_TYPE(*static_cast<const ARRAY_TYPE*>(this));

        } else { /* std::holds_alternative<nb::tuple>(key) */

            // Key is a tuple of slicing instructions
            return RESULT_TYPE(get_item_tuple(
                resolve_python_tuple_slice(std::get<nb::tuple>(key), "__getitem__")
            ));
        }
    }

    /* ****************************************************************************** *
     * *                    `__setitem__` family of methods                         * *
     * ****************************************************************************** */

    //! Working part of `set_item`, using recursive descent to copy items from
    //! `input_it` to `output_it`. Return the number of elements copied. Assumes that
    //! `dim < key.size()`.
    std::size_t set_item_recursive_descent(
        const std::vector<std::variant<nb::int_, nb::slice>>& key,
        const std::vector<std::size_t>& strides,
        typename vector_type::const_iterator input_it,
        typename vector_type::iterator output_it,
        std::size_t dim = 0
    )
    {
        auto&& element = key[dim];
        if (dim == key.size() - 1) {
            /*
             * Final dimension in the tuple key of the recursive descent. Do the
             * actual copying of data.
             */
            if (std::holds_alternative<nb::int_>(element)) {
                auto idx = static_cast<std::ptrdiff_t>(std::get<nb::int_>(element));
                idx = adjust_integer_index(idx, dim, "__setitem__");
                std::copy_n(
                    input_it,
                    _itemsize * strides[dim],
                    output_it + _itemsize * strides[dim] * idx
                );
                return strides[dim];
            } else { /* std::holds_alternative<nb::slice>(element) */
                auto&& slice = std::get<nb::slice>(element);
                auto [start, stop, step, len] = slice.compute(_shape[dim]);
                std::size_t elements_copied = 0;
                if (step < 0) {
                    for (std::ptrdiff_t dst_i = start; dst_i > stop; dst_i += step) {
                        std::copy_n(
                            input_it + _itemsize * elements_copied,
                            _itemsize * strides[dim],
                            output_it + _itemsize * dst_i * strides[dim]
                        );
                        elements_copied += strides[dim];
                    }
                } else { /* step >= 0 */
                    for (std::ptrdiff_t dst_i = start; dst_i < stop; dst_i += step) {
                        std::copy_n(
                            input_it + _itemsize * elements_copied,
                            _itemsize * strides[dim],
                            output_it + _itemsize * dst_i * strides[dim]
                        );
                        elements_copied += strides[dim];
                    }
                }
                return elements_copied;
            }
        } else {
            /*
             * This is not yet the final dimesnsion of the tuple slice in this recursive
             * descent. We need to go deeper...
             */
            if (std::holds_alternative<nb::int_>(element)) {
                auto idx = static_cast<std::ptrdiff_t>(std::get<nb::int_>(element));
                idx = adjust_integer_index(idx, dim, "__setitem__");
                return set_item_recursive_descent(
                    key,
                    strides,
                    input_it,
                    output_it + _itemsize * idx * strides[dim],
                    dim + 1
                );
            } else { /* std::holds_alternative<nb::slice>(tuple_element) */
                auto&& slice = std::get<nb::slice>(element);
                auto [start, stop, step, len] = slice.compute(_shape[dim]);
                std::size_t elements_copied = 0;
                if (step < 0) {
                    for (std::ptrdiff_t dst_i = start; dst_i > stop; dst_i += step) {
                        elements_copied += set_item_recursive_descent(
                            key,
                            strides,
                            input_it + _itemsize * elements_copied,
                            output_it + _itemsize * dst_i * strides[dim],
                            dim + 1
                        );
                    }
                } else { /* step >= 0 */
                    for (std::ptrdiff_t dst_i = start; dst_i < stop; dst_i += step) {
                        elements_copied += set_item_recursive_descent(
                            key,
                            strides,
                            input_it + _itemsize * elements_copied,
                            output_it + _itemsize * dst_i * strides[dim],
                            dim + 1
                        );
                    }
                }

                return elements_copied;
            }
        }
    }

    void set_item_from_array(
        const std::vector<std::variant<nb::int_, nb::slice>>& key, const ARRAY_TYPE& val
    )
    {
        assert(key.size() <= _shape.size());

        // Make sure that all bit specifiers in `*this` and `val` are equal.
        if (!static_cast<const ARRAY_TYPE*>(this)->same_type_as(val)) {
            std::string error_msg = fmt::format(
                "{}.__setitem__: `val` has different bit specifiers than `self`",
                ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }

        // Compute the slice shape
        std::vector<std::size_t> slice_shape = compute_slice_shape(key);
        if (!slice_shape.size()) {
            // `slice_shape` is empty *iff* integers are used to slice all dimensions
            slice_shape.push_back(1);
        }

        // Compute the stride of `*this`
        std::vector<std::size_t> strides = strides_from_shape(_shape);

        if (val._shape == slice_shape) {
            /*
             * Shape of value array and `*this` slice matches up exactly, simply copy
             * the data using recursive descent.
             */
            set_item_recursive_descent(
                key, strides, std::begin(val._data), std::begin(_data)
            );
        } else if (is_broadcastable(val._shape, slice_shape)) {
            /*
             * The value shape can be broadcast to the slice shape. Broadcast first, and
             * then copy the data using recursive descent.
             */
            ARRAY_TYPE val_broadcasted
                = static_cast<const ARRAY_TYPE*>(this)->create_array(slice_shape);
            broadcast_data_copy(
                std::begin(val._data),             // src
                std::begin(val_broadcasted._data), // dst
                val._shape,                        // src_shape
                slice_shape,                       // dst_shape
                _itemsize                          // itemsize
            );
            set_item_recursive_descent(
                key, strides, std::begin(val_broadcasted._data), std::begin(_data)
            );
        } else {
            std::string error_msg = fmt::format(
                "{}.__setitem__: `val` shape not broadcastable to `key` slice",
                ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }
    }

    void set_item_from_scalar(
        const std::vector<std::variant<nb::int_, nb::slice>>& key,
        const scalar_variant_t<ARRAY_TYPE>& val
    )
    {
        // Make sure that all bit specifiers in `*this` and `val` are equal.
        if (!static_cast<const ARRAY_TYPE*>(this)->same_type_as(val)) {
            std::string error_msg = fmt::format(
                "{}.__setitem__: `val` has different bit specifiers than `self`",
                ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }

        // Make an array out of the scalar and set item from there
        ARRAY_TYPE array_val
            = static_cast<const ARRAY_TYPE*>(this)->create_array({ 1 });
        val.copy_n_to(std::begin(array_val._data), _itemsize);
        set_item_from_array(key, array_val);
    }

    //! Python exported `__setitem__` method for APyArrays
    void set_item(
        const std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple>& key,
        const std::variant<std::monostate, ARRAY_TYPE, scalar_variant_t<ARRAY_TYPE>>&
            val
    )
    {
        // Convert the variant of Python objects into a tuple of keys
        nb::tuple python_tuple_key;
        if (std::holds_alternative<nb::int_>(key)) {
            python_tuple_key = nb::make_tuple(std::get<nb::int_>(key));
        } else if (std::holds_alternative<nb::slice>(key)) {
            python_tuple_key = nb::make_tuple(std::get<nb::slice>(key));
        } else if (std::holds_alternative<nb::ellipsis>(key)) {
            python_tuple_key = nb::make_tuple(std::get<nb::ellipsis>(key));
        } else { /* std::holds_alternative<nb::ellipsis>(key) */
            python_tuple_key = std::get<nb::tuple>(key);
        }

        // Resolve the tuple of ellipsis objects and flatten to `std::vector` key
        std::vector<std::variant<nb::int_, nb::slice>> cpp_key
            = resolve_python_tuple_slice(python_tuple_key, "__setitem__");

        if (std::holds_alternative<scalar_variant_t<ARRAY_TYPE>>(val)) {
            set_item_from_scalar(cpp_key, std::get<scalar_variant_t<ARRAY_TYPE>>(val));
        } else if (std::holds_alternative<ARRAY_TYPE>(val)) {
            set_item_from_array(cpp_key, std::get<ARRAY_TYPE>(val));
        } else {
            std::string msg = fmt::format(
                "{}.__setitem__: unsupported value type (internal error: `monostate`?)",
                ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(msg.c_str());
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

    /* ****************************************************************************** *
     * *                     `reshape` family of methods                            * *
     * ****************************************************************************** */

    //! Attempt to reshape `*this` into `new_shape`, resolving any -1 dimension. Returns
    //! the resolved new shape on success.
    std::vector<std::size_t> try_reshape(nb::tuple new_shape) const
    {
        std::vector<std::size_t> new_shape_vec;
        std::size_t index = 0;
        std::size_t reshape_size = 1;
        std::size_t neg_one_count = 0;
        std::ptrdiff_t neg_one_pos = -1;
        for (auto it = new_shape.begin(); it != new_shape.end(); ++it, ++index) {
            std::ptrdiff_t current_value = nb::cast<std::ptrdiff_t>(*it);
            if (current_value == -1) {
                neg_one_count++;
                neg_one_pos = index;        // store the position of -1
                new_shape_vec.push_back(1); // temporarily push 1, replaced later
            } else if (current_value < -1) {
                std::string error_msg = fmt::format(
                    "{}.reshape: array dimensions must be greater than or equal to -1",
                    ARRAY_TYPE::ARRAY_NAME
                );
                throw nb::value_error(error_msg.c_str());
            } else {
                std::size_t c_size_t = nb::cast<std::size_t>(*it);
                new_shape_vec.push_back(c_size_t);
                reshape_size *= c_size_t;
            }
        }

        if (neg_one_count > 1) {
            std::string error_msg = fmt::format(
                "{}.reshape: only one dimension can be -1", ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }

        if (neg_one_count == 1) {
            if (_nitems % reshape_size != 0) {
                std::string error_msg = fmt::format(
                    "{}.reshape: the size of target array must be unchanged and "
                    "divisible by the known dimensions",
                    ARRAY_TYPE::ARRAY_NAME
                );
                throw nb::value_error(error_msg.c_str());
            }
            new_shape_vec[neg_one_pos] = _nitems / reshape_size;
        }

        std::size_t reshape_elem_count = fold_shape(new_shape_vec);

        // Validate if total elements match the original array
        if (_nitems != reshape_elem_count) {
            std::string error_msg = fmt::format(
                "{}.reshape: target array number of elements does not match the "
                "original array",
                ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }

        return new_shape_vec;
    }

    //! Python exported `reshape` method
    ARRAY_TYPE reshape(nb::tuple shape) const
    {
        std::vector<std::size_t> cpp_shape = try_reshape(shape);
        ARRAY_TYPE result
            = static_cast<const ARRAY_TYPE*>(this)->create_array(cpp_shape);
        std::copy_n(_data.begin(), _data.size(), result._data.begin());

        return result;
    }

    //! Python exported `flatten` method
    ARRAY_TYPE flatten() const
    {
        // Reuse the reshape function to flatten the array
        return reshape(nb::make_tuple(-1));
    }

    //! Python exported `ravel` method
    ARRAY_TYPE ravel() const { return flatten(); }

    //! Python exported `transpose` method.
    ARRAY_TYPE transpose(std::optional<nb::tuple> axes = std::nullopt) const
    {
        switch (_ndim) {
        case 0:
        case 1:
            // Behave like NumPy, simply return `*this` if single dimensional
            return *static_cast<const ARRAY_TYPE*>(this);
        case 2: {
            // Optimized code for `_ndim` == 2
            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(_shape);
            std::reverse(result._shape.begin(), result._shape.end());
            for (std::size_t y = 0; y < _shape[0]; y++) {
                for (std::size_t x = 0; x < _shape[1]; x++) {
                    std::copy_n(
                        _data.begin() + (y * _shape[1] + x) * _itemsize,       // src
                        _itemsize,                                             // limbs
                        result._data.begin() + (x * _shape[0] + y) * _itemsize // dst
                    );
                }
            }
            return result;
        }

        default: {
            std::vector<std::size_t> new_axis(_ndim);

            if (axes.has_value()) {
                std::variant<nb::tuple, nb::int_> axis_variant = axes.value();
                new_axis = get_normalized_axes(axis_variant, _ndim);
            } else {
                // reverse the order of axes by default
                std::iota(new_axis.begin(), new_axis.end(), 0);
                std::reverse(new_axis.begin(), new_axis.end());
            }

            std::vector<std::size_t> new_shape(_ndim);
            for (std::size_t i = 0; i < _ndim; ++i) {
                new_shape[i] = _shape[new_axis[i]];
            }

            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);
            transpose_axes_and_copy_data(
                _data.begin(), result._data.begin(), _shape, new_axis, _itemsize
            );
            return result;
        }
        }
    }

    //! Python exported `squeeze` method
    ARRAY_TYPE squeeze(std::optional<std::variant<nb::int_, nb::tuple>> axis) const
    {
        std::vector<std::size_t> shape = _shape;
        std::set<std::ptrdiff_t> axis_set;
        if (axis.has_value()) {
            // Given an int or tuple of ints, remove the specified dimensions if their
            // size is one. Else throw an error.
            auto ax = *axis;
            nb::tuple axis_tuple;

            if (std::holds_alternative<nb::tuple>(ax)) {
                axis_tuple = std::get<nb::tuple>(ax);
            } else if (std::holds_alternative<nb::int_>(ax)) {
                std::ptrdiff_t axis = std::ptrdiff_t(std::get<nb::int_>(ax));
                axis_tuple = nb::make_tuple(axis);
            }
            for (auto ptr = axis_tuple.begin(); ptr != axis_tuple.end(); ptr++) {
                std::ptrdiff_t axis_n = std::ptrdiff_t(nanobind::cast<nb::int_>(*ptr));
                if (axis_n >= std::ptrdiff_t(shape.size())) {
                    std::string error_msg = fmt::format(
                        "{}.squeeze: specified axis larger than number of array "
                        "dimensions",
                        ARRAY_TYPE::ARRAY_NAME
                    );
                    throw nb::index_error(error_msg.c_str());
                }
                axis_set.insert(axis_n);
            }
            std::ptrdiff_t cnt = 0;
            auto predicate = [&](std::size_t dim) {
                if (axis_set.find(cnt) != axis_set.end() && dim != 1) {
                    std::string error_msg = fmt::format(
                        "{}.squeeze: cannot squeeze non-one dimensions",
                        ARRAY_TYPE::ARRAY_NAME
                    );
                    throw nb::value_error(error_msg.c_str());
                }
                return axis_set.find(cnt++) != axis_set.end() && dim == 1;
            };
            shape.erase(
                std::remove_if(shape.begin(), shape.end(), predicate), shape.end()
            );
        } else {
            // Given no specified axis, remove all dimensions of size one.
            auto predicate = [](std::size_t dim) { return dim == 1; };
            shape.erase(
                std::remove_if(shape.begin(), shape.end(), predicate), shape.end()
            );
        }

        // Don't squeeze all dimensions...
        if (shape.size() == 0) {
            shape = { 1 };
        }

        // Create resulting array and copy the data
        ARRAY_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
        std::copy_n(_data.begin(), _data.size(), result._data.begin());
        return result;
    }

    //! Python exported `swapaxes` method
    ARRAY_TYPE swapaxes(nb::int_ axis1, nb::int_ axis2) const
    {
        std::size_t _axis1 = get_normalized_axes(axis1, _ndim).front();
        std::size_t _axis2 = get_normalized_axes(axis2, _ndim).front();

        std::vector<std::size_t> new_axis(_ndim);
        std::iota(new_axis.begin(), new_axis.end(), 0);

        // Swap the specified axes
        std::swap(new_axis[_axis1], new_axis[_axis2]);

        std::vector<std::size_t> shape(_ndim);
        for (std::size_t i = 0; i < _ndim; ++i) {
            shape[i] = _shape[new_axis[i]];
        }

        ARRAY_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
        transpose_axes_and_copy_data(
            std::begin(_data), std::begin(result._data), _shape, new_axis, _itemsize
        );
        return result;
    }

    /* ****************************************************************************** *
     * *                 Static creation methods for common tensors                 * *
     * ****************************************************************************** */

    //! Create a tensor of `shape` and fill all elements using `fill_value`
    static ARRAY_TYPE
    full(const nb::tuple& shape, const scalar_variant_t<ARRAY_TYPE>& fill_value)
    {
        auto cpp_shape = cpp_shape_from_python_shape_like(shape);
        ARRAY_TYPE result = ARRAY_TYPE::create_array_static(cpp_shape, fill_value);
        for (std::size_t i = 0; i < result._nitems; ++i) {
            auto dst_it = std::begin(result._data) + i * result._itemsize;
            fill_value.copy_n_to(dst_it, result._itemsize);
        }
        return result;
    }

    //! Create a tensor of `shape` with diagonal elements set to `diag_value`
    static ARRAY_TYPE
    diagonal(const nb::tuple& shape, const scalar_variant_t<ARRAY_TYPE>& diag_value)
    {
        auto cpp_shape = cpp_shape_from_python_shape_like(shape);
        if (cpp_shape.size() > 2) {
            throw nb::value_error("Diagonal tensors with `ndim > 2` not defined");
        }
        ARRAY_TYPE result = ARRAY_TYPE::create_array_static(cpp_shape, diag_value);

        auto min_dim = *std::min_element(cpp_shape.begin(), cpp_shape.end());
        auto strides = strides_from_shape(cpp_shape);
        auto stride_sum = std::accumulate(std::begin(strides), std::end(strides), 0);

        for (std::size_t i = 0; i < min_dim; ++i) {
            auto dst_it = std::begin(result._data) + i * stride_sum * result._itemsize;
            diag_value.copy_n_to(dst_it, result._itemsize);
        }
        return result;
    }

    /* ****************************************************************************** *
     * *                        Array folding methods                               * *
     * ****************************************************************************** */

private:
    //! Work horse of `array_fold` using recursive descent.
    template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_INOUT>
    std::size_t array_fold_recursive_descent(
        RANDOM_ACCESS_ITERATOR_IN src_it,
        RANDOM_ACCESS_ITERATOR_INOUT dst_it,
        const std::vector<std::size_t>& axes,
        const std::vector<std::size_t>& strides,
        std::size_t dst_itemsize,
        std::function<
            void(typename vector_type::iterator, typename vector_type::const_iterator)>
            bin_op,
        std::size_t dim = 0
    ) const
    {
        auto dim_it = std::find(std::begin(axes), std::end(axes), dim);
        if (dim == _ndim - 1) {
            /*
             * Final dimension, apply the binary operator.
             */
            if (dim_it != std::end(axes)) {
                // Fold (collapse) the final dimension
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    bin_op(dst_it, src_it + i * _itemsize);
                }
                return 1;
            } else {
                // Leave the final dimension be
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    bin_op(dst_it + i * dst_itemsize, src_it + i * _itemsize);
                }
                return _shape[dim];
            }

        } else {
            /*
             * More dimensions do discover. We need to go deeper...
             */
            if (dim_it != std::end(axes)) {
                // Fold (collapse) this dimension
                std::size_t items = 0;
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    auto src = src_it + i * strides[dim] * _itemsize;
                    items = array_fold_recursive_descent(
                        src, dst_it, axes, strides, dst_itemsize, bin_op, dim + 1
                    );
                }
                return items;
            } else {
                // Leave this dimension be
                std::size_t items = 0;
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    auto src = src_it + i * strides[dim] * _itemsize;
                    auto dst = dst_it + items * dst_itemsize;
                    items += array_fold_recursive_descent(
                        src, dst, axes, strides, dst_itemsize, bin_op, dim + 1
                    );
                }
                return items;
            }
        }
    }

    //! Work horse of `array_fold_cumulative` using recursive descent.
    template <
        typename RANDOM_ACCESS_ITERATOR_IN,
        typename RANDOM_ACCESS_ITERATOR_INOUT,
        typename FOLD_OP,
        typename POST_PROC_OP>
    void array_fold_cumulative_recursive_descent(
        RANDOM_ACCESS_ITERATOR_IN src_it,
        RANDOM_ACCESS_ITERATOR_INOUT dst_it,
        std::size_t axis,
        const std::vector<std::size_t>& strides,
        std::size_t dst_itemsize,
        FOLD_OP fold,
        POST_PROC_OP post_proc,
        std::size_t dim = 0
    ) const
    {
        if (dim == _ndim - 1) {
            /*
             * Final dimension, apply the binary operator.
             */
            if (dim == axis) {
                // Cumulative fold the final dimension
                fold(dst_it, src_it);
                for (std::size_t i = 1; i < _shape[dim]; i++) {
                    // Apply the fold
                    auto prev_dst = dst_it + (i - 1) * dst_itemsize;
                    std::copy_n(prev_dst, dst_itemsize, dst_it + i * dst_itemsize);
                    fold(dst_it + i * dst_itemsize, src_it + i * _itemsize);
                }
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    // Apply post-processing
                    post_proc(dst_it + i * dst_itemsize, i);
                }
            } else {
                // Leave the final dimension be
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    fold(dst_it + i * dst_itemsize, src_it + i * _itemsize);
                }
            }
        } else {
            /*
             * More dimensions do discover. We need to go deeper...
             */
            std::size_t src_size = strides[dim] * _itemsize;
            std::size_t dst_size = strides[dim] * dst_itemsize;
            auto recursive_descent = [&](auto src, auto dst) {
                array_fold_cumulative_recursive_descent(
                    src, dst, axis, strides, dst_itemsize, fold, post_proc, dim + 1
                );
            };
            if (dim == axis) {
                // Cumulative fold this dimension
                recursive_descent(src_it, dst_it);
                for (std::size_t i = 1; i < _shape[dim]; i++) {
                    auto prev_dst = dst_it + (i - 1) * dst_size;
                    std::copy_n(prev_dst, dst_size, dst_it + i * dst_size);
                    recursive_descent(src_it + i * src_size, dst_it + i * dst_size);
                }
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    // Apply post-processing
                    for (std::size_t j = 0; j < strides[dim]; j++) {
                        post_proc(dst_it + i * dst_size + j * dst_itemsize, i);
                    }
                }
            } else {
                // Leave this dimension be
                for (std::size_t i = 0; i < _shape[dim]; i++) {
                    recursive_descent(src_it + i * src_size, dst_it + i * dst_size);
                }
            }
        }
    }

public:
    //! Fold an array over `axes` using some binary folding operation `fold`. The fold
    //! operation is applied from first element towards last, along each axis. An
    //! initial element `init` can conditionally be used as the first element in the
    //! fold. Any `args` will simply be passed along when constructing the
    //! result.
    template <typename FOLD_OP, typename... ARGS>
    std::variant<ARRAY_TYPE, scalar_variant_t<ARRAY_TYPE>> array_fold(
        const std::vector<std::size_t>& axes,
        FOLD_OP fold,
        std::optional<const scalar_variant_t<ARRAY_TYPE>> init,
        ARGS... args
    ) const
    {
        // Compute the resulting shape
        std::vector<std::size_t> result_shape = _shape;
        for (auto rev_it = std::crbegin(axes); rev_it != std::crend(axes); ++rev_it) {
            result_shape.erase(std::begin(result_shape) + *rev_it);
        }

        ARRAY_TYPE result(result_shape, args...);

        // Copy initialization element, if present
        if (init.has_value()) {
            for (std::size_t i = 0; i < result._nitems; i++) {
                auto dst = std::begin(result._data) + i * result._itemsize;
                init->copy_n_to(dst, _itemsize);
            }
        }

        // Perform the folding
        std::vector<std::size_t> strides = strides_from_shape(_shape);
        auto src = std::cbegin(_data);
        auto dst = std::begin(result._data);
        array_fold_recursive_descent(src, dst, axes, strides, result._itemsize, fold);

        if (result_shape.size()) {
            return result;
        } else {
            // Result is scalar
            scalar_variant_t<ARRAY_TYPE> scalar(args...);
            scalar.copy_n_from(std::begin(result._data), result._itemsize);
            return scalar;
        }
    }

    //! Fold an array cumulatively along a single `axis`. If `axis` is `std::nullopt`,
    //! the result is flattened before performing the cumulative fold. The initial
    //! element in the fold can be set with `init`, or left unset if none. Each element
    //! in the fold is post-processed using the unary operator `post_proc`.
    template <typename FOLD_OP, typename POST_PROC_OP, typename... ARGS>
    ARRAY_TYPE array_fold_cumulative(
        std::optional<std::size_t> axis,
        FOLD_OP fold,
        POST_PROC_OP post_proc,
        std::optional<const scalar_variant_t<ARRAY_TYPE>> init,
        ARGS... args
    ) const
    {
        // Flatten if no axis is provided
        if (!axis.has_value()) {
            return flatten().array_fold_cumulative(0, fold, post_proc, init, args...);
        }

        ARRAY_TYPE result(_shape, args...);

        // Copy initialization element, if present
        if (init.has_value()) {
            for (std::size_t i = 0; i < result._nitems; i++) {
                auto dst = std::begin(result._data) + i * result._itemsize;
                init->copy_n_to(dst, _itemsize);
            }
        }

        // Recursively perform the fold
        std::vector<std::size_t> strides = strides_from_shape(_shape);
        auto src = std::cbegin(_data);
        auto dst = std::begin(result._data);
        array_fold_cumulative_recursive_descent(
            src, dst, *axis, strides, result._itemsize, fold, post_proc
        );

        return result;
    }

    // Retrieve the number of elements folded when folding `*this` using `axes`
    std::size_t array_fold_get_elements(const std::vector<std::size_t>& axes) const
    {
        auto acc = [&](std::size_t p, std::size_t i) { return p * _shape[i]; };
        return std::accumulate(std::begin(axes), std::end(axes), 1, acc);
    }

    /* ****************************************************************************** *
     * *                        Other APyArray methods                              * *
     * ****************************************************************************** */

    /*!
     * Test if `*this` is identical to another `APyArray` of the same type. Two array
     * objects are considered identical if, and only if:
     *   * They represent exactly the same tensor shape
     *   * They store the exact same values in their `_data` vector
     *   * They have the exact same specifiers (as decided by `same_type_as`
     */
    bool is_identical(const ARRAY_TYPE& other) const
    {
        return _shape == other._shape
            && static_cast<const ARRAY_TYPE*>(this)->same_type_as(other)
            && _data == other._data;
    }

}; // end class: `APyArray`

#endif
