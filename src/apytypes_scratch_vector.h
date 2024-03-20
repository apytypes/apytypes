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

#include <cassert>
#include <fmt/format.h>

#include <algorithm>        // std::copy, std::copy_n
#include <array>            // std::array
#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <initializer_list> // std::initializer_list
#include <iterator>         // std::begin, std::end, std::make_reverse_iterator
#include <memory>           // std::allocator_traits
#include <stdexcept>        // std::out_of_range
#include <vector>           // std::vector

template <typename T, typename PTR_T> class ScratchVectorIteratorBase {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = PTR_T;

    /* Iterator category */
    using iterator_category = std::random_access_iterator_tag;

    /* Random access iterator */
    ScratchVectorIteratorBase(pointer ptr) noexcept
        : _ptr { ptr }
    {
    }
    ScratchVectorIteratorBase& operator++() noexcept { return ++_ptr, *this; }
    ScratchVectorIteratorBase& operator--() noexcept { return --_ptr, *this; }
    ScratchVectorIteratorBase operator++(int) noexcept
    {
        auto res = *this;
        ++_ptr;
        return res;
    }
    ScratchVectorIteratorBase operator--(int) noexcept
    {
        auto res = *this;
        --_ptr;
        return res;
    }
    ScratchVectorIteratorBase operator+(difference_type n) const noexcept
    {
        return ScratchVectorIteratorBase(_ptr + n);
    }
    ScratchVectorIteratorBase& operator+=(difference_type n) noexcept
    {
        _ptr += n;
        return *this;
    }
    difference_type operator-(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr - other._ptr;
    }
    ScratchVectorIteratorBase operator-(difference_type n) const noexcept
    {
        return ScratchVectorIteratorBase(_ptr - n);
    }
    ScratchVectorIteratorBase& operator-=(difference_type n) noexcept
    {
        _ptr -= n;
        return *this;
    }
    reference operator[](std::size_t n) noexcept { return _ptr[n]; }
    const_reference operator[](std::size_t n) const noexcept { return _ptr[n]; }
    reference operator*() noexcept { return *_ptr; }
    const_reference operator*() const noexcept { return *_ptr; }

    bool operator==(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr == other._ptr;
    }
    bool operator!=(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr != other._ptr;
    }
    bool operator<(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr < other._ptr;
    }
    bool operator>(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr > other._ptr;
    }
    bool operator<=(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr <= other._ptr;
    }
    bool operator>=(ScratchVectorIteratorBase other) const noexcept
    {
        return _ptr >= other._ptr;
    }

private:
    pointer _ptr;
};

template <typename T, std::size_t _N_SCRATCH_ELEMENTS = 2> class ScratchVector {
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
    using iterator = ScratchVectorIteratorBase<T, T*>;
    using const_iterator = ScratchVectorIteratorBase<const T, const T*>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* ****************************************************************************** *
     * *                             Constructors                                   * *
     * ****************************************************************************** */

    ScratchVector() noexcept
        : _size {}
        , _scratch_data {}
        , _vector_data {}
        , _ptr {}
    {
        /* Default constructor */
    }

    ScratchVector(size_type count, const T& value)
        : ScratchVector()
    {
        _size = count;
        if (count <= _N_SCRATCH_ELEMENTS) {
            std::fill_n(std::begin(_scratch_data), count, value);
            _ptr = _scratch_data.data();
        } else {
            _vector_data = std::vector<T>(count, value);
            _ptr = _vector_data.data();
        }
    }

    explicit ScratchVector(size_type count)
        : ScratchVector(count, T())
    {
        /* Zero-initialization */
    }

    template <class InputIt>
    ScratchVector(InputIt first, InputIt last)
        : ScratchVector()
    {
        auto distance = std::distance(first, last);
        if (distance < 0) {
            throw std::out_of_range(
                "ScratchVector::ScratchVector(_IT first, _IT last): "
                "iterator last before iterator first"
            );
        } else if (std::distance(first, last) < std::ptrdiff_t(_N_SCRATCH_ELEMENTS)) {
            std::copy(first, last, std::begin(_scratch_data));
            _ptr = _scratch_data.data();
        } else {
            _vector_data = std::vector<T>(first, last);
            _ptr = _vector_data.data();
        }
        _size = distance;
    }

    ScratchVector(const ScratchVector& other)
        : ScratchVector()
    {
        if (other.size() <= _N_SCRATCH_ELEMENTS) {
            std::copy_n(std::begin(other), other.size(), std::begin(_scratch_data));
            _ptr = _scratch_data.data();
        } else {
            _vector_data = other._vector_data;
            _ptr = _vector_data.data();
        }
        _size = other._size;
    }

    ScratchVector(std::initializer_list<T> init)
        : ScratchVector()
    {
        _size = init.size();
        if (init.size() <= _N_SCRATCH_ELEMENTS) {
            std::copy(init.begin(), init.end(), std::begin(_scratch_data));
            _ptr = _scratch_data.data();
        } else {
            _vector_data = std::vector<T>(init);
            _ptr = _vector_data.data();
        }
    }

    /* ****************************************************************************** *
     * *                                Iterators                                   * *
     * ****************************************************************************** */

    iterator begin() noexcept { return iterator(_ptr); }

    const_iterator begin() const noexcept { return const_iterator(_ptr); };

    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return iterator(_ptr + _size); }

    const_iterator end() const noexcept { return const_iterator(_ptr + _size); }

    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    /* ****************************************************************************** *
     * *                                Methods                                     * *
     * ****************************************************************************** */

    size_type size() const noexcept { return _size; }

    reference back() { return *std::prev(std::end(*this)); }

    const_reference back() const { return *std::prev(std::end(*this)); }

    reference operator[](size_type pos) { return _ptr[pos]; }

    const_reference operator[](size_type pos) const { return _ptr[pos]; }

    reference at(size_type pos)
    {
        if (pos < _size) {
            return this[pos];
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
            return this[pos];
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
            _size = count;
            _ptr = _vector_data.data();
        } else if (count > _N_SCRATCH_ELEMENTS) {
            // Upsizing to the heap vector. Initialize heap vector and copy data
            _vector_data = std::vector<T>(count);
            std::copy(
                std::begin(_scratch_data),
                std::end(_scratch_data),
                std::begin(_vector_data)
            );
            _size = count;
            _ptr = _vector_data.data();
        } else {
            // Resizing within the scratch array
            _size = count;
            _ptr = _scratch_data.data();
        }
    }

    ScratchVector& operator=(const ScratchVector& other)
    {
        if (other._size <= _N_SCRATCH_ELEMENTS) {
            // Copy data from other `ScratchVector` into this scratch-array. Zero this
            // heap-vector. Set the pointer to the scratch-array.
            std::copy_n(std::begin(other), other._size, std::begin(_scratch_data));
            _vector_data = std::vector<T>();
            _ptr = _scratch_data.data();
        } else {
            // Copy heap-vector from other `ScratchVector` and set pointer to this
            // heap-vector.
            _vector_data = other._vector_data;
            _ptr = _vector_data.data();
        }
        _size = other._size;
        return *this;
    }

    ScratchVector& operator=(const std::vector<T>& other)
    {
        if (other.size() <= _N_SCRATCH_ELEMENTS) {
            // Copy data from `other` `std::vector` into this scratch-array. Zero this
            // heap-vector. Set the pointer to the scratch-array.
            std::copy_n(std::begin(other), other.size(), std::begin(_scratch_data));
            _vector_data = std::vector<T>();
            _ptr = _scratch_data.data();
        } else {
            // Copy `other` `std::vector` into this heap-vector and set pointer to this
            // heap-vector.
            _vector_data = other;
            _ptr = _vector_data.data();
        }
        _size = other.size();
        return *this;
    }

    explicit operator std::vector<T>() const
    {
        if (_vector_data.size() > 0) {
            return _vector_data;
        } else {
            std::vector<T> result(_size);
            std::copy_n(std::begin(_scratch_data), _size, std::begin(result));
            return result;
        }
    }

private:
    std::size_t _size;                                // Elements in this ScratchVector
    std::array<T, _N_SCRATCH_ELEMENTS> _scratch_data; // Underlying scratch
    std::vector<T> _vector_data;                      // Underlying vector
    T* _ptr;
};

#endif // _APYTYPES_SCRATCH_VECTOR_H
