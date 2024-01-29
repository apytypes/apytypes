/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APYFIXED_H
#define _APYFIXED_H

#include <pybind11/pybind11.h> // pybind11::object

#include "apytypes_common.h"
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
    int _bits;
    int _int_bits;
    std::vector<mp_limb_t> _data;
    // `mp_limb_t` is the underlying data type used for arithmetic in APyFixed (from the
    // GMP library). It is either a 32-bit or a 64-bit unsigned int, depending on the
    // target architecture.

    /* ****************************************************************************** *
     *                            Python constructors                                 *
     * ****************************************************************************** */

public:
    // No default constructed APyFixed types
    APyFixed() = delete;

    //! Constructor: initialize from other APyFixed and optionally set bit specifiers
    APyFixed(
        const APyFixed& other,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    //! Constructor: construct from a Python arbitrary long integer object and
    //! optionally set bit specifiers
    explicit APyFixed(
        pybind11::int_ python_long_int_bit_pattern,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                       Non-Python accessible constructors                       *
     * ****************************************************************************** */

    //! Constructor: specify size and initialize from a `double`
    explicit APyFixed(double value, int bits, int int_bits);

    //! Constructor: specify only size, and zero data on construction
    explicit APyFixed(int bits, int int_bits);

    //! Constructor: specify only size, and zero data on construction
    explicit APyFixed(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    //! Constructor: specify size and initialize from string
    explicit APyFixed(int bits, int int_bits, const char* str, int base = 10);

    //! Constructor: specify size and initialize underlying vector from iterators
    template <typename _ITER>
    explicit APyFixed(int bits, int int_bits, _ITER begin, _ITER end);

    //! Constructor: specify size and initialize underlying bits using vector
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_t>& vec);
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_signed_t>& vec);

    /* ****************************************************************************** *
     *                         Binary arithmetic operators                            *
     * ****************************************************************************** */

    APyFixed operator+(const APyFixed& rhs) const;
    APyFixed operator-(const APyFixed& rhs) const;
    APyFixed operator*(const APyFixed& rhs) const;
    APyFixed operator/(const APyFixed& rhs) const;
    APyFixed operator<<(int shift_val) const;
    APyFixed operator>>(int shift_val) const;

    /* ****************************************************************************** *
     *                          Binary comparion operators                            *
     * ****************************************************************************** */

    bool operator==(const APyFixed& rhs) const;
    bool operator!=(const APyFixed& rhs) const;
    bool operator<(const APyFixed& rhs) const;
    bool operator<=(const APyFixed& rhs) const;
    bool operator>(const APyFixed& rhs) const;
    bool operator>=(const APyFixed& rhs) const;

    /* ****************************************************************************** *
     *                        Other public member functions                           *
     * ****************************************************************************** */

    //! Retrieve the `bits` specifier for this `APyFixed`
    int bits() const noexcept { return _bits; }

    //! Retrieve the `int_bits` specifier for this `APyFixed`
    int int_bits() const noexcept { return _int_bits; }

    //! Retrieve the `frac_bits` specifier for this `APyFixed`
    int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Retrieve the number of elements in underlying limb data vector
    std::size_t vector_size() const noexcept { return _data.size(); }

    //! Retrieve (read-only) the `_data` field of this `APyFixed`
    const std::vector<mp_limb_t>& read_data() { return _data; }

    // Unary negation
    APyFixed operator-() const;

    // Absolute value
    APyFixed abs() const;

    // Test if fixed-point number is negative
    bool is_negative() const noexcept;

    // Test if fixed-point number is positive
    bool is_positive() const noexcept;

    // Test if fixed-point number is zero
    bool is_zero() const noexcept;

    // Increment the LSB without making the fixed-point number wider. Returns carry out
    mp_limb_t increment_lsb() noexcept;

    // Convert the underlying bit pattern to a Python long integer
    pybind11::int_ bit_pattern_to_int(bool allow_negative_return_value = false) const;

    // Convert the underlying bit pattern to decimal and return in a string
    std::string bit_pattern_to_string_dec() const;

    // Python verbose string representation
    std::string repr() const;

    // Test if two Fixed-point numbers are identical, i.e., has the same values, the
    // same number of integer bits, and the same number of fractional bits
    bool is_identical(const APyFixed& other) const;

    // Create a new, resized version of this fixed-point number. Main method for
    // handling overflow and quantization
    APyFixed resize(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        APyFixedRoundingMode rounding_mode = APyFixedRoundingMode::TRN,
        APyFixedOverflowMode overflow_mode = APyFixedOverflowMode::TWOS_OVERFLOW,
        std::optional<int> frac_bits = std::nullopt
    ) const;

    /* ****************************************************************************** *
     *                           Conversion to other types                            *
     * ****************************************************************************** */

    double to_double() const;
    std::string to_string(int base = 10) const;
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;

    /* ****************************************************************************** *
     *                          Setters from other types                              *
     * ****************************************************************************** */

    void set_from_double(double value);
    void set_from_apyfixed(const APyFixed& fixed);
    void set_from_string(const std::string& str, int base = 10);
    void set_from_string_hex(const std::string& str);
    void set_from_string_oct(const std::string& str);
    void set_from_string_dec(const std::string& str);

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

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
     *                           Private helper methods                               *
     * ****************************************************************************** */

private:
    // Sign preserving automatic size extending arithmetic left shift
    std::vector<mp_limb_t> _data_asl(unsigned shift_val) const;

    // Prepare for binary arithmetic by aligning the binary points of two limb vectors.
    // The limbs of the first operand (`operand1`) are copied into the limb vector of
    // `result` and vector sign extended. The limbs of the second operand (`operand2`)
    // are vector-shifted by `operand1.frac_bits()` - `operand2.frac_bits()` bits to the
    // left and copied into the `operand_shifted` limb vector.
    // Assumptions when calling this method:
    //   * `result` is already initialized with a propriate vector limb size
    //   * `operand_shifted` is *not* initialized, i.e., it's an empty (zero element)
    //      vector
    //   * the number of fractional bits in `operand1` is greater than that of
    //     `operand2`
    void _normalize_binary_points(
        APyFixed& result,
        std::vector<mp_limb_t>& operand_shifted,
        const APyFixed& operand1,
        const APyFixed& operand2
    ) const;

    // Get a copy of the negated underlying limb vector. This methods does not extend
    // the size of the limb vector to make place for an additional bit.
    std::vector<mp_limb_t> _non_extending_negate() const;

    // Get the absolute value of the number in the limb vector. This method does not
    // extend the resulting limb vector to make place for an additional bit. Instead, it
    // relies on the user knowing that the number in the vector is now unsigned.
    std::vector<mp_limb_t> _unsigned_abs() const;

    // Handle rounding of fixed-point numbers
    void _round(APyFixedRoundingMode rounding_mode, int old_bits, int old_int_bits);

    // Truncation rounding
    void _round_trn(int old_bits, int old_int_bits);

    // Round towards plus infinity
    void _round_rnd(int old_bits, int old_int_bits);

    // Handle overflowing of fixed-point numbers
    void _overflow(APyFixedOverflowMode overflow_mode);

    // Perform two's complement overflowing. This method sign-extends any bits outside
    // of the APyFixed range.
    void _twos_complement_overflow() noexcept;

    // Set the bit-pattern of the fixed-point number from a vector of limbs
    void from_vector(const std::vector<mp_limb_t>& vector);

}; // end: class APyFixed

/* ********************************************************************************** *
 *                        Output to C++ streaming objects                             *
 * ********************************************************************************** */

static inline std::ostream& operator<<(std::ostream& os, const APyFixed& x)
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
