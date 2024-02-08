// Python object access through Pybind
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <algorithm>
#include <iostream>
#include "apyfloatarray.h"
#include "apyfloat.h"
#include "python_util.h"
#include <stdexcept>
#include <string>

APyFloatArray::APyFloatArray(
    const pybind11::sequence& sign_seq,
    const pybind11::sequence& exp_seq,
    const pybind11::sequence& man_seq,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias) :
    exp_bits(exp_bits), man_bits(man_bits)
    {
    if (bias.has_value()) {
        this->bias = bias.value();
    } else {
        // Not very pretty way to get the IEEE-like bias
        this->bias = APyFloat::ieee_bias(exp_bits);
    }

    const auto signs_shape = python_sequence_extract_shape(sign_seq);
    const auto exps_shape = python_sequence_extract_shape(exp_seq);
    const auto mans_shape = python_sequence_extract_shape(man_seq);

    if (!((signs_shape == exps_shape) && (signs_shape == mans_shape))) {
        throw std::domain_error("Shape mismatch during construction");
    }

    shape = signs_shape;

    auto signs = python_sequence_walk<py::int_, py::bool_>(sign_seq);
    auto exps = python_sequence_walk<py::int_>(exp_seq);
    auto mans = python_sequence_walk<py::int_>(man_seq);

    for (std::size_t i = 0; i < signs.size(); ++i) {
        bool sign;
        if (py::isinstance<py::bool_>(signs[i])) {
            sign = static_cast<bool>(py::cast<py::bool_>(signs[i]));
        } else if (py::isinstance<py::int_>(signs[i])) {
            sign = static_cast<int>(py::cast<py::int_>(signs[i])); // Must cast to int here
        } else {
            throw std::domain_error("Invalid objects in sign");
        }
        exp_t exp = static_cast<exp_t>(py::cast<py::int_>(exps[i]));
        man_t man = static_cast<man_t>(py::cast<py::int_>(mans[i]));

        data.push_back({sign, exp, man});
    }
}

APyFloatArray::APyFloatArray(const std::vector<std::size_t> &shape, exp_t exp_bits, std::uint8_t man_bits, std::optional<exp_t> bias) 
: shape(shape), exp_bits(exp_bits), man_bits(man_bits), bias(bias.value_or(APyFloat::ieee_bias(exp_bits))) {
}

std::string APyFloatArray::repr() const
{
    std::stringstream ss {};
    ss << "APyFloatArray(";
    if (shape[0]) {
        std::stringstream sign_str{}, exp_str{}, man_str{};
        sign_str << "[";
        exp_str << "[";
        man_str << "[";
        for (std::size_t i = 0; i < data.size(); ++i) {
            const APyFloatData d = data[i];
            const bool is_last = i == (data.size() - 1);
            sign_str << (d.sign ? "1" : "0") << (is_last ? "" : ", ");
            exp_str << d.exp << (is_last ? "" : ", ");
            man_str << d.man << (is_last ? "" : ", ");
        }
        ss << sign_str.str() << "], " << exp_str.str() << "], " << man_str.str() << "], ";
    } else {
        ss << "[], [], [], ";
    }
    ss << "shape=(";
    ss << string_from_vec(shape);
    ss << "), "
       << "exp_bits=" << static_cast<unsigned>(exp_bits) << ", "
       << "man_bits=" << static_cast<unsigned>(man_bits) << ", "
       << "bias=" << bias << ")";
    return ss.str();
}

// The shape of the array
pybind11::tuple APyFloatArray::get_shape() const
{
    py::tuple result(get_ndim());
    for (std::size_t i = 0; i < get_ndim(); i++) {
        result[i] = shape[i];
    }
    return result;
}

size_t APyFloatArray::get_ndim() const { return shape.size(); }

size_t APyFloatArray::get_size() const { return shape[0]; }

bool APyFloatArray::is_identical(const APyFloatArray& other) const {
    const bool same_spec = (shape == other.shape) && (exp_bits == other.exp_bits)
    && (man_bits == other.man_bits) && (bias == other.bias) && (data.size() == other.data.size());

    return same_spec && std::equal(data.begin(), data.end(), other.data.begin(), other.data.end()); 
}

APyFloatArray APyFloatArray::from_double(
        const pybind11::sequence& double_seq,
        std::uint8_t exp_bits,
        std::uint8_t man_bits,
        std::optional<exp_t> bias,
        std::optional<RoundingMode> rounding_mode) {

    APyFloatArray arr(python_sequence_extract_shape(double_seq), exp_bits, man_bits, bias);

    auto py_obj = python_sequence_walk<py::float_, py::int_>(double_seq);

    for (const auto &obj : py_obj) {
        double d;
        if (py::isinstance<py::float_>(obj)) {
            d = static_cast<double>(py::cast<py::float_>(obj));
        } else if (py::isinstance<py::int_>(obj)) {
            d = static_cast<int>(py::cast<py::int_>(obj));
        } else {
            throw std::domain_error("Invalid Python objects in sequence");
        }
        const auto fp = APyFloat::from_double(d, exp_bits, man_bits, arr.bias);
        arr.data.push_back({fp.get_sign(), fp.get_exp(), fp.get_man()});
    }
    return arr;
}