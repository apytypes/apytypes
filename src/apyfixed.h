/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APYFIXED_H
#define _APYFIXED_H

#include <nanobind/nanobind.h> // nanobind::object

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
        "The GMP `mp_limb_t` data type is either 64 bit or 32 bit. Any other limb size "
        "is unsupported. This assumption should hold true always, according to the GMP "
        "documentation"
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
    //! No default constructed APyFixed types
    APyFixed() = delete;

    //! Constructor: construct from a Python arbitrary long integer object and
    //! optionally set bit specifiers
    explicit APyFixed(
        nanobind::int_ python_long_int_bit_pattern,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                       Non-Python accessible constructors                       *
     * ****************************************************************************** */

public:
    //! Constructor: initialize from other APyFixed
    APyFixed(const APyFixed& other);

    //! Constructor: specify only size, and zero data on construction
    explicit APyFixed(int bits, int int_bits);

    //! Constructor: specify only size, and zero data on construction
    explicit APyFixed(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    //! Constructor: specify size and initialize underlying vector from iterators
    template <typename _IT>
    explicit APyFixed(int bits, int int_bits, _IT begin, _IT end);

    //! Constructor: specify size and initialize underlying bits using vector
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_t>& vec);

    /* ****************************************************************************** *
     *                         Binary arithmetic operators                            *
     * ****************************************************************************** */

public:
    APyFixed operator+(const APyFixed& rhs) const;
    APyFixed operator-(const APyFixed& rhs) const;
    APyFixed operator*(const APyFixed& rhs) const;
    APyFixed operator/(const APyFixed& rhs) const;
    APyFixed operator<<(const int shift_val) const;
    APyFixed operator>>(const int shift_val) const;
    APyFixed& operator<<=(const int shift_val);
    APyFixed& operator>>=(const int shift_val);

    APyFixed operator+(int rhs) const;
    APyFixed operator-(int rhs) const;
    APyFixed operator*(int rhs) const;
    APyFixed operator/(int rhs) const;

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
    bool operator==(const double rhs) const;
    bool operator!=(const double rhs) const;
    bool operator<=(const double rhs) const;
    bool operator<(const double rhs) const;
    bool operator>=(const double rhs) const;
    bool operator>(const double rhs) const;
    bool operator==(const float rhs) const;
    bool operator!=(const float rhs) const;
    bool operator<=(const float rhs) const;
    bool operator<(const float rhs) const;
    bool operator>=(const float rhs) const;
    bool operator>(const float rhs) const;

    /* ****************************************************************************** *
     *                        Other public member functions                           *
     * ****************************************************************************** */

public:
    //! Retrieve the `bits` specifier
    APY_INLINE int bits() const noexcept { return _bits; }

    //! Retrieve the `int_bits` specifier
    APY_INLINE int int_bits() const noexcept { return _int_bits; }

    //! Retrieve the `frac_bits` specifier
    APY_INLINE int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Retrieve the number of elements in the underlying limb data vector
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

    //! Increment the LSB without making the fixed-point number wider. Returns carry out
    mp_limb_t increment_lsb() noexcept;

    //! Convert the underlying bit pattern to a Python long integer
    nanobind::int_ to_bits() const;

    //! Convert the underlying bit pattern to decimal and return in a string
    std::string bit_pattern_to_string_dec() const;

    //! Python verbose string representation
    std::string repr() const;

    //! LaTeX representation
    std::string latex() const;

    //! Test if two Fixed-point numbers are identical, i.e., has the same values, the
    //! same number of integer bits, and the same number of fractional bits
    bool is_identical(const APyFixed& other) const;

    //! Retrieve leading zeros
    std::size_t leading_zeros() const;

    //! Retrieve leading ones
    std::size_t leading_ones() const;

    //! Retrieve leading fractional zeros
    std::size_t leading_fractional_zeros() const;

    //! Retrieve leading signs
    std::size_t leading_signs() const;

    //! Test if value is greater than equal to a integer power-of-two (>= 2 ^ `n`)
    bool greater_than_equal_pow2(int n) const;

    //! Test if positive value is greater than equal to a integer power-of-two (>= 2 ^
    //! `n`)
    bool positive_greater_than_equal_pow2(int n) const;

    /* ****************************************************************************** *
     *                           Conversion to other types                            *
     * ****************************************************************************** */

public:
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
    void set_from_apyfixed(const APyFixed& fixed);
    void set_from_string(const std::string& str, int base = 10);
    void set_from_string_hex(const std::string& str);
    void set_from_string_oct(const std::string& str);
    void set_from_string_dec(const std::string& str);

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

public:
    static APyFixed from_double(
        double value,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    static APyFixed from_string(
        std::string string_value,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        int base = 10,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                       Resize and quantization methods                        * *
     * ****************************************************************************** */

public:
    //! Publicly exposed cast method
    APyFixed cast(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        QuantizationMode quantization = QuantizationMode::TRN,
        OverflowMode overflow = OverflowMode::WRAP,
        std::optional<int> frac_bits = std::nullopt
    ) const;

    APyFixed resize(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        QuantizationMode quantization = QuantizationMode::TRN,
        OverflowMode overflow = OverflowMode::WRAP,
        std::optional<int> frac_bits = std::nullopt
    ) const;

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

    //! Handle quantization of fixed-point numbers
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits,
        QuantizationMode quantization
    ) const;

    //! Quantize towards minus infinity
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_trn(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Quantize towards +/- infinity (away from zero)
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_trn_inf(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Quantize towards zero
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_trn_zero(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Fixed-point magnitude truncation
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_trn_mag(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Round to nearest, ties toward plus infinity
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_rnd(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Round to nearest, ties toward plus infinity
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_rnd_zero(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Round to nearest, ties toward +/- infinity (away from zero)
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_rnd_inf(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Round to nearest, ties toward minus infinity
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_rnd_min_inf(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Round to nearest, ties toward even quantization steps
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_rnd_conv(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Round to nearest, ties toward odd quantization steps
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_rnd_conv_odd(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Quantization through jamming
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_jam(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Quantization through unbiased jamming
    template <class RANDOM_ACCESS_ITERATOR>
    void _quantize_jam_unbiased(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits
    ) const;

    //! Handle overflowing of fixed-point numbers
    template <class RANDOM_ACCESS_ITERATOR>
    void _overflow(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int new_bits,
        int new_int_bits,
        OverflowMode overflow
    ) const;

    //! Perform two's complement overflowing. This method sign-extends any bits outside
    //! of the APyFixed range.
    template <class RANDOM_ACCESS_ITERATOR>
    void _overflow_twos_complement(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int bits,
        int int_bits
    ) const;

    //! Perform saturating overflow.
    template <class RANDOM_ACCESS_ITERATOR>
    void _overflow_saturate(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        int bits,
        int int_bits
    ) const;

    /* ****************************************************************************** *
     *                           Private helper methods                               *
     * ****************************************************************************** */

private:
    //! Sign preserving automatic size extending arithmetic left shift. Result must have
    //! enough space for `bits_to_limbs(bits() + shift_val)` many limbs.
    template <typename RANDOM_ACCESS_ITERATOR>
    void _data_asl(
        RANDOM_ACCESS_ITERATOR it_begin,
        RANDOM_ACCESS_ITERATOR it_end,
        unsigned shift_val
    ) const;

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
