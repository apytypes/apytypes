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

#include <algorithm>        // std::copy, std::copy_n
#include <array>            // std::array
#include <cassert>          // assert
#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <initializer_list> // std::initializer_list
#include <iterator>         // std::begin, std::end, std::make_reverse_iterator
#include <memory>           // std::allocator, std::allocator_traits
#include <stdexcept>        // std::out_of_range
#include <vector>           // std::vector

/* ********************************************************************************** *
 * *                         Scratch vector iterator base                           * *
 * ********************************************************************************** */

template <typename T, typename PTR_T> class SVIteratorBase {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = PTR_T;

    /* Iterator category */
    using iterator_category = std::random_access_iterator_tag;

    SVIteratorBase(pointer ptr) noexcept { _ptr = ptr; }

    SVIteratorBase& operator++() noexcept { return ++_ptr, *this; }
    SVIteratorBase& operator--() noexcept { return --_ptr, *this; }
    SVIteratorBase operator++(int) noexcept { return _ptr++; }
    SVIteratorBase operator--(int) noexcept { return _ptr--; }
    SVIteratorBase operator+(difference_type n) const noexcept { return _ptr + n; }
    SVIteratorBase operator-(difference_type n) const noexcept { return _ptr - n; }
    SVIteratorBase& operator+=(difference_type n) noexcept { return _ptr += n, *this; }
    SVIteratorBase& operator-=(difference_type n) noexcept { return _ptr -= n, *this; }
    reference operator[](std::size_t n) noexcept { return _ptr[n]; }
    const_reference operator[](std::size_t n) const noexcept { return _ptr[n]; }
    reference operator*() noexcept { return *_ptr; }
    const_reference operator*() const noexcept { return *_ptr; }
    auto operator-(SVIteratorBase other) const noexcept { return _ptr - other._ptr; }

    bool operator==(SVIteratorBase other) const noexcept { return _ptr == other._ptr; }
    bool operator!=(SVIteratorBase other) const noexcept { return _ptr != other._ptr; }
    bool operator<(SVIteratorBase other) const noexcept { return _ptr < other._ptr; }
    bool operator>(SVIteratorBase other) const noexcept { return _ptr > other._ptr; }
    bool operator<=(SVIteratorBase other) const noexcept { return _ptr <= other._ptr; }
    bool operator>=(SVIteratorBase other) const noexcept { return _ptr >= other._ptr; }

private:
    pointer _ptr {};
};

/* ********************************************************************************** *
 * *                            ScratchVector                                       * *
 * ********************************************************************************** */

template <
    typename T,                             // Item type stored in `ScratchVector`
    std::size_t _N_SCRATCH_ELEMENTS = 2,    // Number of pre-allocated items
    typename Allocator = std::allocator<T>> // Allocator used for allocating new items
class ScratchVector {

    // Scratch vector needs at-least one element of scratch data
    static_assert(_N_SCRATCH_ELEMENTS >= 1);

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
    using iterator = SVIteratorBase<T, T*>;
    using const_iterator = SVIteratorBase<const T, const T*>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* ****************************************************************************** *
     * *                             Constructors                                   * *
     * ****************************************************************************** */

    ScratchVector() noexcept
        : _size { 0 }
        , _capacity { _N_SCRATCH_ELEMENTS }
        , _scratch_data() // `_scratch_data` uninitialized
        , _ptr()          // `_ptr` uninitialized
    {
        /* Default constructor */
    }

    ~ScratchVector()
    {
        if (_capacity > _N_SCRATCH_ELEMENTS) {
            Allocator().deallocate(_ptr, _capacity);
        }
    }

    ScratchVector(size_type count, const T& value)
        : ScratchVector()
    {
        if (count <= _N_SCRATCH_ELEMENTS) {
            _size = count;
            _capacity = _N_SCRATCH_ELEMENTS;
            _ptr = _scratch_data.data();
        } else {
            _size = count;
            _capacity = count;
            _ptr = Allocator().allocate(_capacity);
        }
        std::fill_n(begin(), count, value);
    }

    explicit ScratchVector(size_type count)
        : ScratchVector(count, T())
    {
    }

    template <class InputIt>
    ScratchVector(InputIt first, InputIt last)
        : ScratchVector()
    {
        auto distance = std::distance(first, last);
        if (distance <= std::ptrdiff_t(_N_SCRATCH_ELEMENTS)) {
            _size = distance;
            _capacity = _N_SCRATCH_ELEMENTS;
            _ptr = _scratch_data.data();
        } else {
            _size = distance;
            _capacity = distance;
            _ptr = Allocator().allocate(_capacity);
        }
        std::copy(first, last, iterator(_ptr));
    }

    ScratchVector(const ScratchVector& other)
        : ScratchVector(std::begin(other), std::end(other))
    {
    }

    ScratchVector(std::initializer_list<T> init)
        : ScratchVector(std::begin(init), std::end(init))
    {
    }

    template <typename RANDOM_ACCESS_VECTOR_TYPE, bool SPECIALIZED = false>
    ScratchVector& operator=(const RANDOM_ACCESS_VECTOR_TYPE& other)
    {
        // If allocation exists, remove it first
        if (_capacity > _N_SCRATCH_ELEMENTS) {
            Allocator().deallocate(_ptr, _capacity);
        }

        if (other.size() <= _N_SCRATCH_ELEMENTS) {
            // Copy data from other `ScratchVector` into this scratch-array. Set the
            // pointer to the scratch-array.
            _size = other.size();
            _capacity = _N_SCRATCH_ELEMENTS;
            _ptr = _scratch_data.data();
        } else {
            // Copy heap-vector from other `ScratchVector` and set pointer to this
            // heap-vector.
            _size = other.size();
            _capacity = other.capacity();
            _ptr = Allocator().allocate(_capacity);
        }
        std::copy_n(std::begin(other), other.size(), iterator(_ptr));

        return *this;
    }

    ScratchVector& operator=(const ScratchVector& other)
    {
        return operator= <ScratchVector, true>(other);
    }

    /* ****************************************************************************** *
     * *                                Iterators                                   * *
     * ****************************************************************************** */

    iterator begin() noexcept { return iterator(_ptr); }
    const_iterator begin() const noexcept { return const_iterator(_ptr); };
    const_iterator cbegin() const noexcept { return const_iterator(_ptr); }
    iterator end() noexcept { return iterator(_ptr + size()); }
    const_iterator end() const noexcept { return const_iterator(_ptr + size()); }
    const_iterator cend() const noexcept { return const_iterator(_ptr + size()); }
    reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(cend());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(cend());
    }

    reverse_iterator rend() noexcept { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(cbegin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(cbegin());
    }

    /* ****************************************************************************** *
     * *                                Methods                                     * *
     * ****************************************************************************** */

    size_type size() const noexcept { return _size; }
    size_type capacity() const noexcept { return _capacity; }
    reference back() { return *std::prev(end()); }
    const_reference back() const { return *std::prev(cend()); }
    reference operator[](size_type pos)
    {
        assert(pos < _size);
        return _ptr[pos];
    }
    const_reference operator[](size_type pos) const
    {
        assert(pos < _size);
        return _ptr[pos];
    }

    reference at(size_type pos)
    {
        if (pos < _size) {
            return this[pos];
        } else {
            throw std::out_of_range(
                fmt::format(
                    "ScratchVector::at(size_type pos): out of range, "
                    "_size=={}, pos=={}",
                    _size,
                    pos
                )
            );
        }
    }

    const_reference at(size_type pos) const
    {
        if (pos < _size) {
            return this[pos];
        } else {
            throw std::out_of_range(
                fmt::format(
                    "ScratchVector::at(size_type pos): out of range, "
                    "_size=={}, pos=={}",
                    _size,
                    pos
                )
            );
        }
    }

    void resize(size_type count)
    {
        // Resize within capacity
        if (count <= _capacity) {
            _size = count;
            return; // early exit
        }

        // Allocate new data and copy it
        std::size_t new_capacity = count;
        T* tmp = Allocator().allocate(new_capacity);
        std::copy_n(begin(), _size, iterator(tmp));

        // Possibly free the old data
        if (_capacity > _N_SCRATCH_ELEMENTS) {
            Allocator().deallocate(_ptr, _capacity);
        }

        // Apply the new settings
        _size = count;
        _capacity = new_capacity;
        _ptr = tmp;
    }

    void push_back(const T& value)
    {
        std::size_t old_size = _size;
        resize(old_size + 1);
        _ptr[old_size] = value;
    }

    void pop_back() { resize(_size - 1); }

    //! Convert a `ScratchVector` into a `std::vector`
    explicit operator std::vector<T>() const { return std::vector<T>(begin(), end()); }

private:
    std::size_t _size;                                // Elements in this ScratchVector
    std::size_t _capacity;                            // Capacity
    std::array<T, _N_SCRATCH_ELEMENTS> _scratch_data; // Underlying scratch
    T* _ptr;                                          // Pointer to data
};

#endif // _APYTYPES_SCRATCH_VECTOR_H
