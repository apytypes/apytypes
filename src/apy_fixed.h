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

    // Default constructor deleted
    APyFixed() = delete;

    // Specify only size (number of bits). Data will be zeroed on construction.
    explicit APyFixed(unsigned bits, int int_bits);

    // Specify size and initialize using c++ integer
    explicit APyFixed(unsigned bits, int int_bits, int value);

    /*
     * Helpers
     */

    // Get the number of bits in this APyInt
    unsigned bits() const { return _bits; }
    int int_bits() const { return _int_bits; }

    // Get the number of elements in underlying vector
    std::size_t vector_size() const { return _data.size(); }

    // to string function
    inline std::string to_string(STRING_TYPE type = STRING_TYPE::DEC) const {
        switch (type) {
            case STRING_TYPE::HEX:
                return to_string_hex();
            case STRING_TYPE::OCT:
                return to_string_oct();
            case STRING_TYPE::DEC:
                return to_string_dec();
        }
    };
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;

    /*
     * Binary operations
     */
    bool operator==(const APyFixed &rhs) const;
    APyFixed operator+(const APyFixed &rhs) const;
    APyFixed operator-(const APyFixed &rhs) const;

    /*
     * Friend functions
     */
    friend inline std::ostream& operator << (std::ostream &os, const APyFixed &x);


private:

    // Number of bits in fixed-point data type
    unsigned _bits;
    int _int_bits;

    // Underlying data vector
    std::vector<int64_t> _data;
};


/*
 * Print APy_Fixed type to stream
 */
inline std::ostream& operator << (std::ostream &os, const APyFixed &x) {
    if ( (os.flags() & std::ios::hex) != 0 ) {
        os << x.to_string(STRING_TYPE::HEX);
    } else if ((os.flags() & std::ios::oct) != 0) {
        os << x.to_string(STRING_TYPE::OCT);
    } else {
        os << x.to_string(STRING_TYPE::DEC);
    }
    return os;
}


#endif
