/*
 * Scratch vector drop-in replacement for C++17 `std::vector` using small scratch
 * memory. Same functionality as `std::vector`, but a compile-time selected number of
 * elements are stored in the `scratch_vector`, not externally on the heap. If the
 * internal scratch vector cache memory overflows, data is moved and stored on the heap
 * instead.
 *
 * Author: Mikael Henriksson (2024)
 */

#ifndef _APYTYPES_SCRATCH_VECTOR_H
#define _APYTYPES_SCRATCH_VECTOR_H

#include <fmt/format.h>

#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <initializer_list> // std::initializer_list
#include <iterator>         // std::begin, std::end
#include <memory>           // std::allocator_traits
#include <stdexcept>        // std::out_of_range
#include <vector>           // std::vector

template <typename T, std::size_t _N_SCRATCH_ELEMENTS = 1> class ScratchVector {
public:
    /* ****************************************************************************** *
     * *                          Vector name aliasing                              * *
     * ****************************************************************************** */

    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

    /* ****************************************************************************** *
     * *                             Constructors                                   * *
     * ****************************************************************************** */

    ScratchVector() noexcept
        : _size {}
        , _scratch_data {}
        , _vector_data {}
    {
        /* Default constructor */
    }

    ScratchVector(size_type count, const T& value)
        : ScratchVector()
    {
        _size = count;
        if (count <= _N_SCRATCH_ELEMENTS) {
            std::fill_n(std::begin(_scratch_data), count, value);
        } else {
            _vector_data = std::vector<T>(count, value);
        }
    }
    explicit ScratchVector(size_type count)
        : ScratchVector(count, T {})
    {
        /* Zero-initialization */
    }
    template <class InputIt>
    ScratchVector(InputIt first, InputIt last)
        : ScratchVector()
    {
        _size = std::distance(first, last);
        if (std::distance(first, last) < _N_SCRATCH_ELEMENTS) {
            std::copy(first, last, std::begin(_scratch_data));
        } else {
            _vector_data = std::vector<T>(first, last);
        }
    }
    ScratchVector(const ScratchVector& other)
        : ScratchVector()
    {
        _size = other._size;
        std::copy(
            std::begin(other._scratch_data),
            std::end(other._scratch_data),
            std::begin(_scratch_data)
        );
        _vector_data = other._vector_data;
    }
    ScratchVector(std::initializer_list<T> init)
        : ScratchVector()
    {
        if (init.size() < _N_SCRATCH_ELEMENTS) {
            std::copy(init.begin(), init.end(), std::begin(_scratch_data));
        } else {
            _vector_data = std::vector<T>(init);
        }
    }

    /* ****************************************************************************** *
     * *                                Iterators                                   * *
     * ****************************************************************************** */

    iterator begin() noexcept
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return iterator(std::begin(_scratch_data));
        } else {
            return std::begin(_vector_data);
        }
    };
    const_iterator begin() const noexcept
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return iterator(std::begin(_scratch_data));
        } else {
            return std::begin(_vector_data);
        }
    };
    iterator end() noexcept
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return iterator(std::end(_scratch_data));
        } else {
            return std::end(_vector_data);
        }
    }
    const_iterator end() const noexcept
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return iterator(std::end(_scratch_data));
        } else {
            return std::end(_vector_data);
        }
    }
    const_iterator cbegin() noexcept
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return iterator(std::begin(_scratch_data));
        } else {
            return std::begin(_vector_data);
        }
    };
    const_iterator cend() const noexcept
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return const_iterator(std::end(_scratch_data));
        } else {
            return std::end(_vector_data);
        }
    }

    /* ****************************************************************************** *
     * *                                Methods                                     * *
     * ****************************************************************************** */

    size_type size() const noexcept { return _size; }
    reference operator[](size_type pos)
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return _scratch_data[pos];
        } else {
            return _vector_data[pos];
        }
    }
    const_reference operator[](size_type pos) const
    {
        if (_size <= _N_SCRATCH_ELEMENTS) {
            return _scratch_data[pos];
        } else {
            return _vector_data[pos];
        }
    }
    reference at(size_type pos)
    {
        if (pos < _size) {
            return this->operator[](pos);
        } else {
            throw std::out_of_range(fmt::format(
                "ScratchVector::at(size_type pos): out of range, "
                "_size=={}, pos=={}",
                _size,
                pos
            ));
        }
    }
    const_reference at(size_type pos) const
    {
        if (pos < _size) {
            return this->operator[](pos);
        } else {
            throw std::out_of_range(fmt::format(
                "ScratchVector::at(size_type pos): out of range, "
                "_size=={}, pos=={}",
                _size,
                pos
            ));
        }
    }
    void resize(size_type count)
    {
        if (_vector_data.size() > 0) {
            // Heap vector already in use. Just keep on using it, it is free at this
            // point
            _vector_data.resize(count);
        } else if (count > _N_SCRATCH_ELEMENTS) {
            // Upsizing to the heap vector. Initialize heap vector and copy data
            _vector_data = std::vector<T>(count);
            std::copy(
                std::begin(_scratch_data),
                std::end(_scratch_data),
                std::begin(_vector_data)
            );
        }
        _size = count;
    }

    void operator=(const ScratchVector& other)
    {
        _size = other._size;
        std::copy(
            std::begin(other._scratch_data),
            std::end(other._scratch_data),
            std::begin(_scratch_data)
        );
        _vector_data = other._vector_data;
    }

private:
    // Underlying scratch data for small allocations
    std::size_t _size;
    T _scratch_data[_N_SCRATCH_ELEMENTS];
    std::vector<T> _vector_data;
};

#endif // _APYTYPES_SCRATCH_VECTOR_H
