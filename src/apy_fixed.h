/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APY_FIXED_H
#define _APY_FIXED_H

#include <pybind11/pybind11.h>  // pybind11::object

#include "apy_util.h"

#include <cstddef>   // std::size_t
#include <optional>  // std::optional, std::nullopt
#include <ostream>   // std::ostream
#include <string>    // std::string
#include <vector>    // std::vector


// GMP should be included after all other includes
#include <gmp.h>

class APyFixed {

#ifdef _IS_APY_TYPES_UNIT_TEST
    // Unit tests have public access to underlying data fields
    public:
#else
    private:
#endif  // #ifdef _IS_APY_TYPES_UNIT_TEST

    /*
     * APyFixed data fields
     */
    int _bits;
    int _int_bits;
    std::vector<mp_limb_t> _data;
             // mp_limb_t is the underlying data type used for arithmetic in APyFixed
             // (from the GMP library). It is either a 32-bit or a 64-bit unsigned int,
             // depending on the target architecture.
public:

    // No default constructed APyFixed types
    APyFixed() = delete;


    /* ****************************************************************************** *
     *                            Python constructors                                 *
     * ****************************************************************************** */

    // Constructor: initialize from other APyFixed and optionally set bit specifiers
    APyFixed(
        const APyFixed &other,
        std::optional<int> bits      = std::nullopt,
        std::optional<int> int_bits  = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    // Constructor: construct from a Python arbitrary long integer object and optionally
    // set bit specifiers
    explicit APyFixed(
        pybind11::int_ obj,
        std::optional<int> bits      = std::nullopt,
        std::optional<int> int_bits  = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );


    /* ****************************************************************************** *
     *                       Non-Python accessible constructors                       *
     * ****************************************************************************** */

    // Constructor: specify size and initialize from a `double`
    explicit APyFixed(double value, int bits, int int_bits);

    // Constructor: specify only size, and zero data on construction
    explicit APyFixed(int bits, int int_bits);

    // Constructor: specify only size, and zero data on construction
    explicit APyFixed(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    // Constructor: specify size and initialize from string
    explicit APyFixed(int bits, int int_bits, const char *str, int base=10);


    /* ****************************************************************************** *
     *                         Binary arithmetic operators                            *
     * ****************************************************************************** */

    APyFixed operator+(const APyFixed &rhs) const;
    APyFixed operator-(const APyFixed &rhs) const;
    APyFixed operator*(const APyFixed &rhs) const;
    APyFixed operator/(const APyFixed &rhs) const;
    APyFixed operator<<(int shift_val) const;
    APyFixed operator>>(int shift_val) const;


    /* ****************************************************************************** *
     *                          Binary comparion operators                            *
     * ****************************************************************************** */

    bool operator==(const APyFixed &rhs) const;
    bool operator!=(const APyFixed &rhs) const;
    bool operator< (const APyFixed &rhs) const;
    bool operator<=(const APyFixed &rhs) const;
    bool operator> (const APyFixed &rhs) const;
    bool operator>=(const APyFixed &rhs) const;

    
    /* ****************************************************************************** *
     *                        Other public member functions                           *
     * ****************************************************************************** */

    // Get the number of bits in this APyInt object
    int bits() const noexcept { return _bits; }
    int int_bits() const noexcept { return _int_bits; }
    int frac_bits() const noexcept { return _bits - _int_bits; }

    // Retrieve the number of elements in underlying limb data vector
    std::size_t vector_size() const noexcept { return _data.size(); }

    // Perform two's complement overflowing. This method sign-extends any bits outside
    // of the APyFixed range.
    void twos_complement_overflow() noexcept;

    // Unary negation
    APyFixed operator-() const;

    // Test if fixed-point number is negative
    bool is_negative() const noexcept;

    // Test if fixed-point number is zero
    bool is_zero() const noexcept;

    // Increment the LSB without making the fixed-point number wider. Returns carry out
    mp_limb_t increment_lsb() noexcept;

    // Python verbose string representation
    std::string repr() const;


    /* ****************************************************************************** *
     *                           Conversion to other types                            *
     * ****************************************************************************** */

    std::string to_string(int base = 10) const;
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;
    

    /* ****************************************************************************** *
     *                          Conversion from other types                           *
     * ****************************************************************************** */

    void from_double(double value);
    void from_apyfixed(const APyFixed &fixed);
    void from_string(const std::string &str, int base = 10);
    void from_string_hex(const std::string &str);
    void from_string_oct(const std::string &str);
    void from_string_dec(const std::string &str);


    /* ****************************************************************************** *
     *                           Private helper methods                               *
     * ****************************************************************************** */

#ifdef _IS_APY_TYPES_UNIT_TEST
    // Unit tests have public access to all member function
    public:
#else
    private:
#endif  // #ifdef _IS_APY_TYPES_UNIT_TEST
    
    // Set member fields `bits` and `int_bits` from optional input arguments
    void _bits_set_from_optional(
        std::optional<int> bits,
        std::optional<int> int_bits,
        std::optional<int> frac_bits
    );

    // Sanitize the _bits and _int_bits parameters
    void _constructor_sanitize_bits() const;

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
    //   * the number of fractional bits in `operand1` is greater than that of `operand2`
    void _normalize_binary_points(
        APyFixed &result,
        std::vector<mp_limb_t> &operand_shifted,
        const APyFixed &operand1,
        const APyFixed &operand2
    ) const;

    // Get a copy of the negated underlying limb vector. This methods does not extend
    // the size of the limb vector to make place for an additional bit.
    std::vector<mp_limb_t> _non_extending_negate() const;

    // Get the absolute value of the number in the limb vector. This method does not
    // extend the resulting limb vector to make place for an additional bit. Instead, it
    // relies on the user knowing that the number in the vector is now unsigned.
    std::vector<mp_limb_t> _unsigned_abs() const;


    // Constructor: specify size and initialize underlying vector from start and end
    // iterators
    template <typename _ITER>
    explicit APyFixed(int bits, int int_bits, _ITER begin, _ITER end);

    // Constructor: specify size and initialize underlying bits using vector
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_t> &vec);
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_signed_t> &vec);

    // Set the bit-pattern of the fixed-point number from a vector of limbs
    void from_vector(const std::vector<mp_limb_t> &vector);

};  // end: class APyFixed


/* ********************************************************************************** *
 *                        Output to C++ streaming objects                             *
 * ********************************************************************************** */

static inline std::ostream& operator<< (std::ostream &os, const APyFixed &x) {
    if ( (os.flags() & std::ios::hex) != 0 ) {
        os << x.to_string(16);
    } else if ((os.flags() & std::ios::oct) != 0) {
        os << x.to_string(8);
    } else {
        os << x.to_string(10);
    }
    return os;
}

#endif  // _APY_FIXED_H
