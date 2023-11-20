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

class APyFixed {

public:

    /*
     * Constructors
     */

    APyFixed() = delete;

    // Specify only size (number of bits). Data will be zeroed on construction.
    explicit APyFixed(int bits, int int_bits);

    // Specify size and initialize using C++ integer
    explicit APyFixed(int bits, int int_bits, int value);

    // Specify size and initialize the underlying bits using C++ vectory type
    explicit APyFixed(int bits, int int_bits, const std::vector<int64_t> &vec);


    /*
     * Methods
     */

    // Get the number of bits in this APyInt object
    int bits() const noexcept { return _bits; }
    int int_bits() const noexcept { return _int_bits; }

    // Get the number of elements in underlying 64-bit data vector
    std::size_t vector_size() const noexcept { return _data.size(); }

    // Perform 2's complement overflowing. This method sign-extends all bits outside of
    // the APyFixed range.
    void twos_complement_overflow() noexcept;

    // Set the bit-pattern of fixed-point number from string
    void from_bitstring(const std::string &str);

    // Set the bit-pattern from a vector of int64_t
    void from_vector(const std::vector<int64_t> &vector);

    // Unary negation
    APyFixed operator-() const;

    // Others
    bool is_negative() const noexcept { return _data.back() < 0; }
    void increment_lsb() noexcept;


    /*
     * Binary operations
     */
    bool operator==(const APyFixed &rhs) const;
    APyFixed operator+(const APyFixed &rhs) const;
    APyFixed operator-(const APyFixed &rhs) const;
    APyFixed operator*(const APyFixed &rhs) const;
    APyFixed operator/(const APyFixed &rhs) const;

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

private:

    // Number of bits in fixed-point data type
    int _bits;
    int _int_bits;

    // Underlying data vector
    std::vector<int64_t> _data;
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
