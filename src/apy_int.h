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

    // Specify only size (number of bits). Data will be zerod on construction.
    APyInt(std::size_t size);

    // Specify size and initialize using integer
    APyInt(std::size_t size, int value);

    /*
     * Helpers
     */

    // Get the number of bits in this APyInt
    std::size_t bits_size() { return _size; }

    /*
     * Binary operations
     */
    bool operator==(const APyInt &rhs) const;
    APyInt operator+(const APyInt &rhs) const;
    APyInt operator-(const APyInt &rhs) const;



private:

    // Number of bits in integer type
    std::size_t _size;

    // Underlying data vector
    std::vector<int64_t> _data;
};


#endif
