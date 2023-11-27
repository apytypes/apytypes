/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APY_FIXED_H
#define _APY_FIXED_H

#include "apy_util.h"

#include <cstddef>  // std::size_t
#include <ostream>  // std::ostream
#include <string>   // std::string
#include <vector>   // std::vector

// GMP should be included after all other includes
#include <gmp.h>


class APyFixed {

    #ifdef _IS_APY_TYPES_UNIT_TEST
        // Unit tests have public access to underlying data files
        public:
    #else
        private:
    #endif  // #ifdef _IS_APY_TYPES_UNIT_TEST

    /*
     * APyFixed data fields
     */
    int _bits;
    int _int_bits;
    std::vector<mp_limb_t> _data;  // mp_limb_t is the underlying data type used for 
                                   // arithmetic in APyFixed. It is either a 32-bit or
                                   // a 64-bit unsigned int, depending on the target
                                   // architecture.
public:

    /*
     * Constructors
     */

    // No default constructed APyFixed types
    APyFixed() = delete;

    // Constructor: specify only size (number of bits). Data will be zeroed on
    // construction
    explicit APyFixed(int bits, int int_bits);

    // Constructor: specify size and initialize from string
    explicit APyFixed(int bits, int int_bits, const char *str, int base=10);

    // Constructor: specify size and initialize underlying bits from two iterators
    template <typename _ITER>
    explicit APyFixed(int bits, int int_bits, _ITER begin, _ITER end);

    // Constructor: specify size and initialize underlying bits using vector
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_t> &vec);
    explicit APyFixed(int bits, int int_bits, const std::vector<mp_limb_signed_t> &vec);


    /*
     * Binary arithmetic operators
     */

    APyFixed operator+(const APyFixed &rhs) const;
    APyFixed operator-(const APyFixed &rhs) const;
    APyFixed operator*(const APyFixed &rhs) const;
    APyFixed operator/(const APyFixed &rhs) const;
    APyFixed operator<<(int shift_val) const;
    APyFixed operator>>(int shift_val) const;

    /*
     * Binary comparison operators
     */
    bool operator==(const APyFixed &rhs) const;
    bool operator<(const APyFixed &rhs) const;


    /*
     * Methods
     */

    // Get the number of bits in this APyInt object
    int bits() const noexcept { return _bits; }
    int int_bits() const noexcept { return _int_bits; }
    int frac_bits() const noexcept { return _bits - _int_bits; }

    // Retrieve the number of elements in underlying limb data vector
    std::size_t vector_size() const noexcept { return _data.size(); }

    // Perform two's complement overflowing. This method sign-extends any bits outside
    // of the APyFixed range.
    void twos_complement_overflow() noexcept;

    // Set the bit-pattern of the fixed-point number from a vector of limbs
    void from_vector(const std::vector<mp_limb_t> &vector);

    // Unary negation
    APyFixed operator-() const;

    // Test if fixed-point number is negative
    bool is_negative() const noexcept { return mp_limb_signed_t(_data.back()) < 0; }

    // Increment the LSB without making the fixed-point number wider. Returns carry out
    mp_limb_t increment_lsb() noexcept;


    /*
     * Assignment operators
     */


    /*
     * Conversion to/from stirng
     */
    inline std::string to_string(STRING_TYPE type = STRING_TYPE::DEC) const
    {
        switch (type) {
            case STRING_TYPE::HEX: return to_string_hex(); break;
            case STRING_TYPE::OCT: return to_string_oct(); break;
            case STRING_TYPE::DEC: return to_string_dec(); break;
            default: throw NotImplementedException(); break;
        }
    };
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;
    std::string repr() const;  // Python verbose string conversion
    
    inline void from_string(const std::string &str, STRING_TYPE type = STRING_TYPE::DEC)
    {
        switch (type) {
            case STRING_TYPE::HEX: from_string_hex(str); break;
            case STRING_TYPE::OCT: from_string_oct(str); break;
            case STRING_TYPE::DEC: from_string_dec(str); break;
            default: throw NotImplementedException(); break;
        }
    }
    void from_string_hex(const std::string &str);
    void from_string_oct(const std::string &str);
    void from_string_dec(const std::string &str);


    /*
     * Private helper methods
     */
    #ifdef _IS_APY_TYPES_UNIT_TEST
        // Unit tests have public access to all member function
        public:
    #else
        private:
    #endif  // #ifdef _IS_APY_TYPES_UNIT_TEST

    // Sanitize the _bits and _int_bits parameters
    void _constructor_sanitize_bits() const;

    // Sign preserving automatic size extending arithmetic left shift
    std::vector<uint64_t> _data_asl(unsigned shift_val) const;

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

};


/*
 * Print APyFixed object to ostream objects (e.g., std::cout, std::cerr)
 */
static inline std::ostream& operator<< (std::ostream &os, const APyFixed &x) {
    if ( (os.flags() & std::ios::hex) != 0 ) {
        os << x.to_string(STRING_TYPE::HEX);
    } else if ((os.flags() & std::ios::oct) != 0) {
        os << x.to_string(STRING_TYPE::OCT);
    } else {
        os << x.to_string(STRING_TYPE::DEC);
    }
    return os;
}

#endif  // _APY_FIXED_H
