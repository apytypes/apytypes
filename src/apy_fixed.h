/*
 * Dynamic arbitrary precision fixed-point data type.
 */

#ifndef _APY_FIXED_H
#define _APY_FIXED_H

#include "apy_util.h"

#include <vector>
#include <cstdint>
#include <iosfwd>
#include <ostream>
#include <string>

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

    // Get the number of elements in underlying limb data vector
    std::size_t vector_size() const noexcept { return _data.size(); }

    // Perform 2's complement overflowing. This method sign-extends all bits outside of
    // the APyFixed range.
    void twos_complement_overflow() noexcept;

    // Set the bit-pattern from a vector of uint64_t
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
    // APyFixed &operator=(const APyFixed &rhs) const;

    /*
     * Conversion to stirng
     */
    inline std::string to_string(STRING_TYPE type = STRING_TYPE::DEC) const
    {
        switch (type) {
            case STRING_TYPE::HEX: return to_string_hex();
            case STRING_TYPE::OCT: return to_string_oct();
            case STRING_TYPE::DEC: return to_string_dec();
            default: throw NotImplementedException();
        }
    };
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;
    std::string repr() const;  // Python verbose string conversion


    /*
     * Private helper methods
     */
    #ifdef _IS_APY_TYPES_UNIT_TEST
        // Unit tests have public access to all member function
        public:
    #else
        private:
    #endif  // #ifdef _IS_APY_TYPES_UNIT_TEST

    // Sign preserving automatic size extending arithmetic left shift
    std::vector<uint64_t> _data_asl(unsigned shift_val) const;

    // Sanitize the _bits and _int_bits parameters
    void _constructor_sanitize_bits() const;
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
