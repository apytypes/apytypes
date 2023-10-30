/*
 * Dynamic arbitrary precision integer data type.
 */

#ifndef _APY_INT_H
#define _APY_INT_H

#include <vector>
#include <cstdint>

class APyInt {

public:

    /*
     * Constructors
     */

    // Default constructor deleted
    APyInt() = delete;

    // Specify only size (number of bits). Data will be zeroed on construction.
    APyInt(std::size_t bits);

    // Specify size and initialize using c++ integer
    APyInt(std::size_t bits, int value);

    /*
     * Helpers
     */

    // Get the number of bits in this APyInt
    std::size_t bits() const { return _bits; }

    // Get the number of elements in underlying vector
    std::size_t vector_size() const { return _data.size(); }

    /*
     * Binary operations
     */
    bool operator==(const APyInt &rhs) const;
    APyInt operator+(const APyInt &rhs) const;
    APyInt operator-(const APyInt &rhs) const;



private:

    // Number of bits in integer type
    std::size_t _bits;

    // Underlying data vector
    std::vector<int64_t> _data;
};


#endif
