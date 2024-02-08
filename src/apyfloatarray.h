
#ifndef _APYFLOAT_ARRAY_H
#define _APYFLOAT_ARRAY_H

#include "apytypes_common.h"
#include <cstdint>
#include <optional>
#include <pybind11/numpy.h>    // pybind11::array_t
#include <pybind11/pybind11.h> // pybind11::object
#include <pybind11/pytypes.h>  // pybind11::sequence
#include <tuple>
#include <vector>

using exp_t = std::uint32_t;
using man_t = std::uint64_t;

class APyFloatArray {
public:
    explicit APyFloatArray(
        const pybind11::sequence& sign_seq,
        const pybind11::sequence& exp_seq,
        const pybind11::sequence& man_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Python `__repr__()` function
    std::string repr() const;

    //! Shape of the array
    pybind11::tuple get_shape() const;

    //! Number of dimensions
    size_t get_ndim() const;

    //! Length of the array
    size_t get_size() const;

    /*!
     * Test if two `APyFloatArray` objects are identical. Two `APyFloatArray` objects
     * are considered identical if, and only if:
     *   * They represent exatly the same tensor shape
     *   * They store the exact same floating-point values in all tensor elements
     *   * They have the exact same sized fields
     */
    bool is_identical(const APyFloatArray& other) const;

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

    //! Create an `APyFloatArray` tensor object initialized with values from a sequence
    //! of `doubles`
    static APyFloatArray from_double(
        const pybind11::sequence& double_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt,
        std::optional<RoundingMode> rounding_mode = std::nullopt
    );

    //! Transposition function. For a 1-D array, returns an exact copy of `*this`. For
    //! a 2-D array, returns the matrix transposition of `*this`.
    APyFloatArray transpose() const;

    inline exp_t get_bias() const { return bias; }
    inline std::uint8_t get_man_bits() const { return man_bits; }
    inline std::uint8_t get_exp_bits() const { return exp_bits; }

private:
    struct APyFloatData {
        bool sign;
        exp_t exp; // Biased exponent
        man_t man; // Hidden one
        bool operator==(const APyFloatData& other) const
        {
            return std::make_tuple(sign, exp, man)
                == std::make_tuple(other.sign, other.exp, other.man);
        }
    };

    APyFloatArray(
        const std::vector<std::size_t>& shape,
        exp_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    std::uint8_t exp_bits, man_bits;
    exp_t bias;

    std::vector<std::size_t> shape;
    std::vector<APyFloatData> data;
};

#endif
