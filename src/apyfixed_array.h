/*
 * Array/Tensor type for arbitrary precision fixed-point data.
 */

#ifndef _APYFIXED_ARRAY_H
#define _APYFIXED_ARRAY_H

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
#include <gmp.h>

class APyFixedArray {
    int _bits;
    int _int_bits;
    std::vector<unsigned> _shape;
    std::vector<mp_limb_t> _data;

    /* ****************************************************************************** *
     *                            Python constructors                                 *
     * ****************************************************************************** */

public:
    // No default constructed APyFixed types
    APyFixedArray() = delete;

    APyFixedArray(
        pybind11::list bit_pattern_list,
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                       Non-Python accessible constructors                       *
     * ****************************************************************************** */

    //! Constructor: specify only size, and zero data on construction
    explicit APyFixedArray(
        std::optional<int> bits = std::nullopt,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt
    );
};

#endif // _APYFIXED_ARRAY_H
