/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APYFIXED_H
#define _APYFIXED_H

#include "apytypes_common.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h> // nanobind::object
namespace nb = nanobind;

#include <algorithm>        // std::copy_n
#include <cstddef>          // std::size_t
#include <initializer_list> // initializer_list
#include <limits>           // std::numeric_limits<>::is_iec559
#include <optional>         // std::optional, std::nullopt
#include <ostream>          // std::ostream
#include <string>           // std::string
#include <vector>           // std::vector

class APyFixed {

    /* ****************************************************************************** *
     *                            APyFixed C++ assumptions                            *
     * ****************************************************************************** */

    static_assert(
        (APY_LIMB_SIZE_BYTES == 8 || APY_LIMB_SIZE_BYTES == 4),
        "The `apy_limb_t` data type is either 64-bit or 32-bit. Any other limb size "
        "is unsupported. The size of limbs is specified during compilation with the C "
        "Macro `COMPILER_LIMB_SIZE`."
    );
    static_assert(
        (-1 >> 1 == -1),
        "Right shift applied to signed integral types performs *arithmetic* right "
        "shift. Arithmetic right shift of signed types is *the only* valid behaviour "
        "since C++20, but before C++20 the right shift of signed integral types is "
        "implementation defined. APyFixed relies heavily on arithmetic right shift."
    );
    static_assert(
        (std::numeric_limits<double>::is_iec559),
        "We assume IEEE-754 double-precision floating-point types."
    );

    /* ****************************************************************************** *
     *                            APyFixed data fields                                *
     * ****************************************************************************** */

private:
    int _bits;
    int _int_bits;
    ScratchVector<apy_limb_t> _data;
    // `apy_limb_t` is the underlying data type used for arithmetic in APyFixed.
    // It is either a 32-bit or a 64-bit unsigned int, depending on the target
    // architecture.

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Copy `n` items from `it` into `*this`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_from(RANDOM_ACCESS_ITERATOR src_it, std::size_t n) noexcept
    {
        std::copy_n(src_it, n, std::begin(_data));
    }

    //! Copy `n` items from `*this` into `it`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_to(RANDOM_ACCESS_ITERATOR dst_it, std::size_t n) const noexcept
    {
        std::copy_n(std::begin(_data), n, dst_it);
    }

    //! Test if two fixed-point numbers have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyFixed& other) const
    {
        return _bits == other._bits && _int_bits == other._int_bits;
    }

    //! Retrieve the bit specification
    APY_INLINE APyFixedSpec spec() const noexcept { return { _bits, _int_bits }; }

    //! Peak at the underlying data vector
    const ScratchVector<apy_limb_t>& read_data() const { return _data; }

    /* ****************************************************************************** *
     *                            Python constructors                                 *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyFixed` objects. At least the bit-specifiers
    //! has to be set during construction.
    APyFixed() = delete;

    //! Copy constructor and copy assignment
    APyFixed(const APyFixed& other) = default;
    APyFixed& operator=(const APyFixed& other) = default;

    //! Main Python-exposed `APyFixed` constructor
    explicit APyFixed(
        const nb::int_& python_long_int_bit_pattern,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                       Non-Python accessible constructors                       *
     * ****************************************************************************** */

public:
    //! Construct a zero-initialized number with `bits` and `int_bits`. Undefined
    //! behaviour if `bits < 1`.
    explicit APyFixed(int bits, int int_bits);

    //! Construct a zero-initialized number from bit specification. Throws
    //! `nanobind::value_error` if not exactly two of three bit specifiers (`bits`,
    //! `int_bits`, `frac_bits`) are set, or if the resulting number of `bits` is less
    //! than one.
    explicit APyFixed(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Construct a number with `bits` and `int_bits`, and initialize underlying
    //! bit-pattern from iterator pair [ `begin`, `end` ).
    template <typename _IT>
    explicit APyFixed(int bits, int int_bits, _IT begin, _IT end);

    //! Construct a number with `bits` and `int_bits`, and initialize underlying
    //! bit-pattern from limb-vector `vec`.
    explicit APyFixed(int bits, int int_bits, const std::vector<apy_limb_t>& vec);

    //! Construct a number with `bits` and `int_bits`, and initialize underlying
    //! bit-pattern from initializer list.
    explicit APyFixed(int bits, int int_bits, std::initializer_list<apy_limb_t> list);

    //! Construct a number with `bits` and `int_bits`, and initialize underlying
    //! bit-pattern bit stealing it from a `ScratchVector`.
    explicit APyFixed(int bits, int int_bits, const ScratchVector<apy_limb_t>& data);

    /* ************************************************************************* *
     * *                                        Copy                           * *
     * ************************************************************************* */

    //! Copy scalar
    APyFixed python_copy() const { return *this; }
    //! Deepcopy scalar
    APyFixed python_deepcopy(const nb::typed<nb::dict, nb::int_, nb::any>&) const
    {
        return *this;
    }

    /* ****************************************************************************** *
     *                         Binary arithmetic operators                            *
     * ****************************************************************************** */

private:
    //! Base addition/subtraction routine for two `APyFixed`
    template <class base_op, class ripple_carry_op>
    inline APyFixed _apyfixed_base_add_sub(const APyFixed& rhs) const;

public:
    /*
     * Arithmetic operators
     */
    APyFixed operator+(const APyFixed& rhs) const;
    APyFixed operator-(const APyFixed& rhs) const;
    APyFixed operator*(const APyFixed& rhs) const;
    APyFixed operator/(const APyFixed& rhs) const;
    APyFixed operator<<(const int shift_val) const;
    APyFixed operator>>(const int shift_val) const;
    APyFixed& operator<<=(const int shift_val);
    APyFixed& operator>>=(const int shift_val);

    APyFixed operator+(const nb::int_& rhs) const;
    APyFixed operator-(const nb::int_& rhs) const;
    APyFixed operator*(const nb::int_& rhs) const;
    APyFixed operator/(const nb::int_& rhs) const;

    /* ****************************************************************************** *
     *                          Binary comparison operators                            *
     * ****************************************************************************** */

public:
    bool operator==(const APyFixed& rhs) const;
    bool operator!=(const APyFixed& rhs) const;
    bool operator<(const APyFixed& rhs) const;
    bool operator<=(const APyFixed& rhs) const;
    bool operator>(const APyFixed& rhs) const;
    bool operator>=(const APyFixed& rhs) const;

    bool operator==(const nb::int_& rhs) const;
    bool operator!=(const nb::int_& rhs) const;
    bool operator<(const nb::int_& rhs) const;
    bool operator<=(const nb::int_& rhs) const;
    bool operator>(const nb::int_& rhs) const;
    bool operator>=(const nb::int_& rhs) const;

    bool operator==(const double rhs) const;
    bool operator!=(const double rhs) const;
    bool operator<=(const double rhs) const;
    bool operator<(const double rhs) const;
    bool operator>=(const double rhs) const;
    bool operator>(const double rhs) const;

    /* ****************************************************************************** *
     *                        Other public member functions                           *
     * ****************************************************************************** */

public:
    //! Return the `bits` specifier
    APY_INLINE int bits() const noexcept { return _bits; }

    //! Return the `int_bits` specifier
    APY_INLINE int int_bits() const noexcept { return _int_bits; }

    //! Return the `frac_bits` specifier
    APY_INLINE int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Return the number of elements in the underlying limb data vector
    APY_INLINE std::size_t vector_size() const noexcept { return _data.size(); }

    //! Unary negation
    APyFixed operator-() const;

    //! Unary positive
    APY_INLINE APyFixed operator+() const { return *this; };

    //! Logic not
    APyFixed operator~() const;

    //! Absolute value
    APyFixed abs() const;

    //! Integer power
    APyFixed pown(int n) const;

    //! Test if fixed-point number is negative
    bool is_negative() const noexcept;

    //! Test if fixed-point number is positive
    bool is_positive() const noexcept;

    //! Test if fixed-point number is zero
    bool is_zero() const noexcept;

    //! Increment the LSB without making the fixed-point number wider. Return carry out
    apy_limb_t increment_lsb() noexcept;

    //! Convert the underlying bit pattern to a Python long integer
    nb::int_ to_bits() const;

    //! Convert the underlying bit pattern to decimal and return in a string
    std::string bit_pattern_to_string_dec() const;

    //! Python verbose string representation
    std::string repr() const;

    //! LaTeX representation
    std::string latex() const;

    //! Test if two Fixed-point numbers are identical, i.e., has the same values, the
    //! same number of integer bits, and the same number of fractional bits
    bool is_identical(
        const std::variant<const APyFixed*, const APyFixedArray*>& other
    ) const;

    //! Return leading zeros
    std::size_t leading_zeros() const;

    //! Return leading ones
    std::size_t leading_ones() const;

    //! Return leading fractional zeros
    std::size_t leading_fractional_zeros() const;

    //! Return leading signs
    std::size_t leading_signs() const;

    //! Test if positive value is greater than equal to a integer power-of-two (>= 2 ^
    //! `n`)
    bool positive_greater_than_equal_pow2(int n) const;

    //! Set the `2 ^ n` -th bit (zero indexed) to `bit`. Undefined behaviour if
    //! bit `2 ^ n` is located outside the limb vector.
    void set_bit_pow2(int n, bool bit);

    //! Return the least significant 64-bits from the underlying limb-vector
    //! (convenience function used in `APyFloat`)
    std::uint64_t get_lsbs() const { return limb_vector_to_uint64(_data, 0); }

    APyFixed ipow(unsigned int n) const;

    /* ****************************************************************************** *
     *                           Conversion to other types                            *
     * ****************************************************************************** */

public:
    //! Convert number to double.
    double to_double() const;
    std::string to_string(int base = 10) const;
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;

    operator double() const;

    /* ****************************************************************************** *
     *                          Setters from other types                              *
     * ****************************************************************************** */

public:
    void set_from_string(const std::string& str, int base = 10);
    void set_from_string_hex(const std::string& str);
    void set_from_string_oct(const std::string& str);
    void set_from_string_dec(const std::string& str);

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

public:
    //! Create APyFixed from Python object.
    static APyFixed from_number(
        const nb::object& py_obj,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyFixed from double.
    static APyFixed from_double(
        double value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyFixed from double without specifying the format.
    //! The format will be big enough to accommodate the double.
    static APyFixed from_unspecified_double(double value);

    //! Create APyFixed from Python integer.
    static APyFixed from_integer(
        const nb::int_& value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyFixed from Python integer without specifying the format.
    //! The format will be big enough to accommodate the Python integer.
    static APyFixed from_unspecified_integer(const nb::int_& value);

    //! Create APyFixed from string.
    static APyFixed from_string(
        std::string string_value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        int base = 10,
        std::optional<int> bits = std::nullopt
    );

    //! Retrieve an `APyFixed` of `bits` and `int_bits` with the maximum value
    static APyFixed get_max(int bits, int int_bits)
    {
        APyFixed init_max(bits, int_bits);
        std::size_t limbs = init_max._data.size();
        init_max._data[limbs - 1]
            |= (apy_limb_t(1) << ((bits - 1) % APY_LIMB_SIZE_BITS));
        apy_inplace_subtraction_single_limb(&init_max._data[0], limbs, 1);
        return init_max;
    }

    //! Retrieve an `APyFixed` of `bits` and `int_bits` with the minimum value
    static APyFixed get_min(int bits, int int_bits)
    {
        APyFixed init_min = get_max(bits, int_bits);
        auto begin_it = std::begin(init_min._data);
        auto end_it = std::end(init_min._data);
        std::transform(begin_it, end_it, begin_it, std::bit_not {});
        return init_min;
    }

    //! Get bit pattern for the value one
    static APyFixed one(int bits, int int_bits);

    /* ****************************************************************************** *
     * *                   Resize and quantization method (cast)                    * *
     * ****************************************************************************** */

public:
    //! Publicly exposed cast method
    APyFixed cast(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    ) const;

    //! Simplified cast methods for internal use when it is known that the result will
    //! not overflow
    APyFixed cast_no_overflow(
        int bits, int int_bits, QuantizationMode quantization = QuantizationMode::TRN
    ) const;

    /* ****************************************************************************** *
     * *                          Friends of APyFixed :)                            * *
     * ****************************************************************************** */

public:
    friend class APyCFixed;
    friend class APyFixedArray;
    friend class APyCFixedArray;
    friend class APyFloat;
    friend class APyCFloat;

    template <typename T, typename ARRAY_TYPE> friend class APyArray;

}; // end: class APyFixed

/* ********************************************************************************** *
 *                        Output to C++ streaming objects                             *
 * ********************************************************************************** */

static APY_INLINE std::ostream& operator<<(std::ostream& os, const APyFixed& x)
{
    if ((os.flags() & std::ios::hex) != 0) {
        os << x.to_string(16);
    } else if ((os.flags() & std::ios::oct) != 0) {
        os << x.to_string(8);
    } else {
        os << x.to_string(10);
    }
    return os;
}

#endif // _APYFIXED_H
