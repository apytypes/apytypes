/*
 * Utility functions
 */

#ifndef _APY_UTIL_H
#define _APY_UTIL_H

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <vector>

// Not implemented exception
class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Not implemeted yet") {};
};

// Perform fast integer ceil( n/64 )
static inline std::size_t idiv64_ceil_fast(std::size_t n) {
    if (n%64 == 0) {
        return n/64;
    } else {
        return n/64+1;
    }
}

// Avaiable string converstion types
enum class STRING_TYPE {
    HEX,
    OCT,
    DEC,
};

// Convert a positive arbitrary size integer array to a nibble list
static inline std::vector<uint8_t> to_nibble_list(
    const std::vector<int64_t> &data_array
) {
    std::vector<uint8_t> result{};
    for (int64_t data : data_array) {
        for (int i=0; i<16; i++) {
            result.push_back(static_cast<uint8_t>((data >> (4*i)) & 0x0F));
        }
    }

    // Remove zero-elements from the end
    int zero_elements = 0;
    for (auto nibble = result.crbegin(); nibble != result.crend(); ++nibble) {
        if (*nibble == 0) {
            zero_elements++;
        } else {
            break;
        }
    }
    result.resize(result.size() - zero_elements);
    return result.size() > 0 ? result : std::vector<uint8_t>{ 0 };
}

// Shift a nibble list left by one stage. Modifies the content of input nibble_list
static inline bool nibble_list_shift_left_once(std::vector<uint8_t> &nibble_list)
{
    bool output_bit = *(--nibble_list.cend()) >= 8;
    for (int i=nibble_list.size()-1; i>=0; i--) {
        nibble_list[i] <<= 1;
        nibble_list[i] &= 0x0F;
        if (i > 0 && nibble_list[i-1] >= 8) {
            nibble_list[i] += 1;
        }
    }
    return output_bit;
}

// Double-dabble algorithm for binary->bcd conversion
static inline std::vector<uint8_t> double_dabble(const std::vector<int64_t> &data) {

    // Data needs to be positive
    if ( *data.rbegin() < 0) {
        throw std::domain_error("Double-dabble needs positive number");
    }

    std::vector<uint8_t> nibble_list = to_nibble_list(data);
    std::vector<uint8_t> bcd_list{ 0 };
    int iterations = 0;
    while (
        std::any_of(
            nibble_list.cbegin(), nibble_list.cend(), [](uint8_t n){ return n!=0; }
        )
        || iterations%4 != 0
    ) {
        iterations++;
        bool new_bit = nibble_list_shift_left_once(nibble_list);
        for (auto &bcd : bcd_list) {
            if (bcd >= 5) {
                bcd += 3;
            }
            bcd <<= 1;
            bcd += static_cast<uint8_t>(new_bit);
            new_bit = bcd >= 16;
            bcd &= 0x0F;
        }
        if (new_bit) {
            bcd_list.push_back(1);
        }
    }
    return bcd_list;
}

#endif
