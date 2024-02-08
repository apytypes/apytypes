
#ifndef _APYFLOAT_ARRAY_H
#define _APYFLOAT_ARRAY_H

#include <cstdint>
#include <pybind11/numpy.h>    // pybind11::array_t
#include <pybind11/pybind11.h> // pybind11::object
#include <pybind11/pytypes.h>  // pybind11::sequence
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

    inline exp_t get_bias() const { return bias; }
    inline std::uint8_t get_man_bits() const { return man_bits; }
    inline std::uint8_t get_exp_bits() const { return exp_bits; } 

    private:
    struct APyFloatData {
        bool sign;
        exp_t exp; // Biased exponent
        man_t man; // Hidden one
    };

    std::uint8_t exp_bits, man_bits;
    exp_t bias;
    
    std::vector<std::size_t> shape;
    std::vector<APyFloatData> data;
};

#endif