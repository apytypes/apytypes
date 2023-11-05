/*
 * Dynamic arbitrary precision integer data type.
 */

#include "apy_fixed.h"
#include "apy_util.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

/*
 * Constructors
 */
 APyFixed::APyFixed(unsigned bits, int int_bits) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), int64_t(0))
{
    if (bits <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
}

APyFixed::APyFixed(unsigned bits, int int_bits, int value) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), int64_t(0))
{
    if (bits <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
    _data[0] = int64_t(value);
}

/*
 * Binary operators
 */
bool APyFixed::operator==(const APyFixed &rhs) const
{
    // Test that data is equal in regions of overlapping data
    for (std::size_t i=0; i<std::min(_data.size(), rhs._data.size()); i++) {
        if (_data.at(i) != rhs._data.at(i)) {
            return false;
        }
    }

    // Test that data is zeroed in regions of the larger integer
    if (_data.size() < rhs._data.size()) {
        bool signbit = _data.at(_data.size()-1) & (int64_t(1) << 63);
        for (std::size_t i=_data.size()+1; i<rhs._data.size(); i++) {
            if (signbit && rhs._data.at(i) != int64_t(0)-1) {
                return false;
            }
            else if (!signbit && rhs._data.at(i) != int64_t(0)) {
                return false;
            }
        }
    } else {
        bool signbit = rhs._data.at(rhs._data.size()-1) & (int64_t(1) << 63);
        for (std::size_t i=rhs._data.size()+1; i<_data.size(); i++) {
            if (signbit && _data.at(i) != int64_t(0)-1) {
                return false;
            } else if (!signbit && _data.at(i) != int64_t(0)) {
                return false;
            }
        }
    }
    
    // The integers are equal
    return true;
}

/*
 * Utility functions
 */
std::string APyFixed::to_string_dec() const {

    // Conversion to BCD
    std::vector<uint8_t> data_bcd = double_dabble(_data);
    
}


//  inline std::string to_string(ac_base_mode base_rep, bool sign_mag = false, bool pad_to_width = false) const {
//    char r[(W-AC_MIN(AC_MIN(W-I,I),0)+31)/32*32+5] = {0};
//    int i = 0;
//    if (is_neg())
//      r[i++] = '-';
//    ac_fixed<W+1, I+1, true> t;
//    if( (base_rep == AC_DEC || sign_mag) && is_neg() )
//      t = operator -();
//    else if(pad_to_width)
//      t = ac_fixed<W,I,false>(*this);
//    else
//      t = *this;
//    ac_fixed<AC_MAX(I+1,1),AC_MAX(I+1,1),true> i_part = t;
//    ac_fixed<AC_MAX(W-I,1),0,false> f_part = t;
//    i += ac_private::to_string(i_part.v, AC_MAX(I+!pad_to_width,1), sign_mag, base_rep, false, pad_to_width, r+i);
//    if(W-I > 0) {
//      r[i++] = '.';
//      if(!ac_private::to_string(f_part.v, W-I, false, base_rep, true, pad_to_width, r+i))
//        r[--i] = 0;
//    }
//    if(!i) {
//      r[0] = '0';
//      r[1] = 0;
//    }
//    return std::string(r);
//  }
//  inline static std::string type_name() {
//    const char *tf[] = {"false", "true" };
//    const char *q[] = {"AC_TRN", "AC_RND", "AC_TRN_ZERO", "AC_RND_ZERO", "AC_RND_INF", "AC_RND_MIN_INF", "AC_RND_CONV", "AC_RND_CONV_ODD" };
//    const char *o[] = {"AC_WRAP", "AC_SAT", "AC_SAT_ZERO", "AC_SAT_SYM" };
//    std::string r = "ac_fixed<";
//    r += ac_int<32,true>(W).to_string(AC_DEC) + ',';
//    r += ac_int<32,true>(I).to_string(AC_DEC) + ',';
//    r += tf[S];
//    r += ',';
//    r += q[Q];
//    r += ',';
//    r += o[O];
//    r += '>';
//    return r;
//  }

std::string APyFixed::to_string_hex() const {
    throw NotImplementedException();
}

std::string APyFixed::to_string_oct() const {
    throw NotImplementedException();
}
