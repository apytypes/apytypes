/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APYFIXED_H
#define _APYFIXED_H

#include <nanobind/nanobind.h> // nanobind::object
namespace nb = nanobind;

#include "apytypes_common.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_util.h"

#include <cstddef>  // std::size_t
#include <limits>   // std::numeric_limits<>::is_iec559
#include <optional> // std::optional, std::nullopt
#include <ostream>  // std::ostream
#include <string>   // std::string
#include <vector>   // std::vector

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

class APyFixed {

    /* ****************************************************************************** *
     *                            APyFixed C++ assumptions                            *
     * ****************************************************************************** */

    static_assert(
        (sizeof(mp_limb_t) == 8 || sizeof(mp_limb_t) == 4),
        "The GMP `mp_limb_t` data type is either 64-bit or 32-bit. Any other limb size "
        "is unsupported. This assumption should hold true always, according to the GMP "
        "documentation. The size of limbs is specified during compilation with the C "
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
    ScratchVector<mp_limb_t> _data;
    // `mp_limb_t` is the underlying data type used for arithmetic in APyFixed (from the
    // GMP library). It is either a 32-bit or a 64-bit unsigned int, depending on the
    // target architecture.

    /* ****************************************************************************** *
     *                            Python constructors                                 *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyFixed` objects. At lesast the bit-specifiers
    //! has to be set during construction.
    APyFixed() = delete;

    //! Construct a copy from `other`.
    APyFixed(const APyFixed& other);

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
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_t>& vec);

    /* ****************************************************************************** *
     *                         Binary arithmetic operators                            *
     * ****************************************************************************** */

private:
    //! Base addition/subtraction routine for two `APyFixed`
    template <class base_op, class ripple_carry_op>
    inline APyFixed _apyfixed_base_add_sub(const APyFixed& rhs) const;

public:
    //! Addition of another APyFixed
    APyFixed operator+(const APyFixed& rhs) const;
    //! Subtraction of another APyFixed
    APyFixed operator-(const APyFixed& rhs) const;
    //! Multiplication of another APyFixed
    APyFixed operator*(const APyFixed& rhs) const;
    //! Division of another APyFixed
    APyFixed operator/(const APyFixed& rhs) const;
    //! Left-shift (moving the binary point)
    APyFixed operator<<(const int shift_val) const;
    //! Right-shift (moving the binary point)
    APyFixed operator>>(const int shift_val) const;
    //! In-place left-shift (moving the binary point)
    APyFixed& operator<<=(const int shift_val);
    //! In-place right-shift (moving the binary point)
    APyFixed& operator>>=(const int shift_val);

    //! Addition of int
    APyFixed operator+(const nb::int_& rhs) const;
    //! Subtraction of int
    APyFixed operator-(const nb::int_& rhs) const;
    //! Multiplication of int
    APyFixed operator*(const nb::int_& rhs) const;
    //! Division of int
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

    //! Absolute value
    APyFixed abs() const;

    //! Test if fixed-point number is negative
    bool is_negative() const noexcept;

    //! Test if fixed-point number is positive
    bool is_positive() const noexcept;

    //! Test if fixed-point number is zero
    bool is_zero() const noexcept;

    //! Increment the LSB without making the fixed-point number wider. Return carry out
    mp_limb_t increment_lsb() noexcept;

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
    bool is_identical(const APyFixed& other) const;

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

    //! Return the least significant 64-bits from the underlying limb-vector
    //! (convenience function used in `APyFloat`)
    uint64_t get_lsbs() const { return uint64_t_from_limb_vector(_data, 0); }

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
    void set_from_double(double value);
    void set_from_string(const std::string& str, int base = 10);
    void set_from_string_hex(const std::string& str);
    void set_from_string_oct(const std::string& str);
    void set_from_string_dec(const std::string& str);

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

public:
    //! Create APyFixed from Python float or int.
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

    /* ****************************************************************************** *
     *                       Resize and quantization methods                        * *
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

private:
    //! The internal cast method. Uses output iterators to place the result into the
    //! iterator region pointed to by `it_begin` and `it_end`. Requires that:
    //! `std::distance(it_begin, it_end) == bits_to_limbs(std::max(_bits, new_bits))`.
    template <class RANDOM_ACCESS_ITERATOR>
    void _cast(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits,
        QuantizationMode quantization = QuantizationMode::TRN,
        OverflowMode overflow = OverflowMode::WRAP
    ) const;

    //! The internal cast method. Uses output iterators to place result onto its limb
    //! vector. Requires that `std::distance(it_begin, it_end) == bits_to_limbs(bits)`.
    template <class RANDOM_ACCESS_ITERATOR>
    void _cast_no_overflow(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits,
        QuantizationMode quantization = QuantizationMode::TRN
    ) const;

    //! The internal cast method when the word length is known to be correct.
    //! Uses output iterators to place result onto its limb vector.
    //! Requires that `std::distance(it_begin, it_end) == bits_to_limbs(bits)`.
    template <class RANDOM_ACCESS_ITERATOR>
    void _cast_correct_wl(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        unsigned int shift_amount
    ) const;

    /* ****************************************************************************** *
     *                           Private helper methods                               *
     * ****************************************************************************** */

private:
    //! Copy the raw bits from `*this` onto the iterator region pointed to `it_begin`
    //! and `it_end`. If `std::distance(it_begin, it_end) > self->vector_size()`, then
    //! the data is (limb) sign-extended.
    template <typename RANDOM_ACCESS_ITERATOR>
    void _copy_and_sign_extend(
        RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end
    ) const;

    //! `APyFixedArray` is a friend class of APyFixed, and can access all data of an
    //! `APyFixed` object
public:
    friend class APyFixedArray;

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
