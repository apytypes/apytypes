/*
 * APyUFixed: Dynamic arbitrary unsigned fixed-point data type.
 */

#include <Python.h>

#include "apyufixed.h"
#include "python_util.h"

#include <new>
#include <utility>

#include <fmt/format.h>

static int hdl_slice_bound_or_default(
    const nb::object& bound, int default_value, const char* bound_name
)
{
    if (bound.is_none()) {
        return default_value;
    }

    try {
        return nb::cast<int>(bound);
    } catch (const nb::cast_error&) {
        PyErr_Format(
            PyExc_TypeError,
            "APyUFixed.__getitem__: slice %s must be int or None",
            bound_name
        );
        throw nb::python_error();
    }
}

APyUFixed::APyUFixed(
    const nb::int_& python_long_int_bit_pattern,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
    : _value(
          python_long_int_bit_pattern,
          with_sign_bit(int_bits),
          frac_bits,
          with_sign_bit(bits)
      )
{
    clear_sign_bit();
}

APyUFixed::APyUFixed(APyFixed value)
    : _value(std::move(value))
{
    clear_sign_bit();
}

void APyUFixed::clear_sign_bit() { _value.set_bit_pow2(_value.int_bits() - 1, false); }

std::optional<int> APyUFixed::with_sign_bit(std::optional<int> value)
{
    if (value.has_value()) {
        return *value + 1;
    }
    return std::nullopt;
}

APyUFixed APyUFixed::from_backing(APyFixed value)
{
    return APyUFixed(std::move(value));
}

APyUFixed APyUFixed::operator+(const APyUFixed& rhs) const
{
    return from_backing(_value + rhs._value);
}

APyFixed APyUFixed::operator-(const APyUFixed& rhs) const
{
    APyFixed result = _value - rhs._value;
    return result.cast_no_overflow(result.bits() - 1, result.int_bits() - 1);
}

APyUFixed APyUFixed::operator*(const APyUFixed& rhs) const
{
    APyFixed result = _value * rhs._value;
    result = result.cast_no_overflow(result.bits() - 1, result.int_bits() - 1);
    return from_backing(std::move(result));
}

APyUFixed APyUFixed::operator/(const APyUFixed& rhs) const
{
    APyFixed result = _value / rhs._value;
    result = result.cast_no_overflow(result.bits() - 2, result.int_bits() - 1);
    return from_backing(std::move(result));
}

APyFixed APyUFixed::operator+(const nb::int_& rhs) const { return _value + rhs; }

APyFixed APyUFixed::operator-(const nb::int_& rhs) const { return _value - rhs; }

APyFixed APyUFixed::operator*(const nb::int_& rhs) const { return _value * rhs; }

APyFixed APyUFixed::operator/(const nb::int_& rhs) const { return _value / rhs; }

static APyFixed fixed_double_with_spec(double value, const APyFixed& spec_source)
{
    return APyFixed::from_double(
        value, spec_source.int_bits(), spec_source.frac_bits()
    );
}

APyFixed APyUFixed::operator+(double rhs) const
{
    return _value + fixed_double_with_spec(rhs, _value);
}

APyFixed APyUFixed::operator-(double rhs) const
{
    return _value - fixed_double_with_spec(rhs, _value);
}

APyFixed APyUFixed::operator*(double rhs) const
{
    return _value * fixed_double_with_spec(rhs, _value);
}

APyFixed APyUFixed::operator/(double rhs) const
{
    return _value / fixed_double_with_spec(rhs, _value);
}

bool APyUFixed::operator==(const APyUFixed& rhs) const { return _value == rhs._value; }

bool APyUFixed::operator!=(const APyUFixed& rhs) const { return _value != rhs._value; }

bool APyUFixed::operator<(const APyUFixed& rhs) const { return _value < rhs._value; }

bool APyUFixed::operator<=(const APyUFixed& rhs) const { return _value <= rhs._value; }

bool APyUFixed::operator>(const APyUFixed& rhs) const { return _value > rhs._value; }

bool APyUFixed::operator>=(const APyUFixed& rhs) const { return _value >= rhs._value; }

bool APyUFixed::operator==(const nb::int_& rhs) const { return _value == rhs; }

bool APyUFixed::operator!=(const nb::int_& rhs) const { return _value != rhs; }

bool APyUFixed::operator<(const nb::int_& rhs) const { return _value < rhs; }

bool APyUFixed::operator<=(const nb::int_& rhs) const { return _value <= rhs; }

bool APyUFixed::operator>(const nb::int_& rhs) const { return _value > rhs; }

bool APyUFixed::operator>=(const nb::int_& rhs) const { return _value >= rhs; }

bool APyUFixed::operator==(double rhs) const { return _value == rhs; }

bool APyUFixed::operator!=(double rhs) const { return _value != rhs; }

bool APyUFixed::operator<(double rhs) const { return _value < rhs; }

bool APyUFixed::operator<=(double rhs) const { return _value <= rhs; }

bool APyUFixed::operator>(double rhs) const { return _value > rhs; }

bool APyUFixed::operator>=(double rhs) const { return _value >= rhs; }

APyUFixed APyUFixed::operator<<(int shift_val) const
{
    return from_backing(_value << shift_val);
}

APyUFixed APyUFixed::operator>>(int shift_val) const
{
    return from_backing(_value >> shift_val);
}

APyUFixed& APyUFixed::operator<<=(int shift_val)
{
    _value <<= shift_val;
    clear_sign_bit();
    return *this;
}

APyUFixed& APyUFixed::operator>>=(int shift_val)
{
    _value >>= shift_val;
    clear_sign_bit();
    return *this;
}

APyFixed APyUFixed::operator-() const
{
    APyFixed result = -_value;
    return result.cast_no_overflow(result.bits() - 1, result.int_bits() - 1);
}

APyUFixed APyUFixed::operator~() const { return from_backing(~_value); }

APyUFixed APyUFixed::pown(int n) const
{
    if (n < 0) {
        throw NotImplementedException("Not implemented: power with negative integers.");
    }

    APyFixed result = _value.pown(n);
    if (n > 1) {
        result
            = result.cast_no_overflow(result.bits() - n + 1, result.int_bits() - n + 1);
    }
    return from_backing(std::move(result));
}

int APyUFixed::get_bit_value(int index) const
{
    if (index < -frac_bits() || index >= int_bits()) {
        PyErr_Format(
            PyExc_IndexError,
            "APyUFixed bit index %d out of range [%d, %d]",
            index,
            -frac_bits(),
            int_bits() - 1
        );
        throw nb::python_error();
    }
    return _value.get_bit_value(index);
}

nb::int_ APyUFixed::get_hdl_bit_range(const nb::slice& index) const
{
    nb::object start_obj = index.attr("start");
    nb::object stop_obj = index.attr("stop");
    nb::object step_obj = index.attr("step");

    if (!step_obj.is_none()) {
        PyErr_SetString(
            PyExc_ValueError,
            "APyUFixed.__getitem__: HDL-style slices do not support an explicit step"
        );
        throw nb::python_error();
    }

    const int min_index = -frac_bits();
    const int max_index = int_bits() - 1;
    const int msb = hdl_slice_bound_or_default(start_obj, max_index, "start");
    const int lsb = hdl_slice_bound_or_default(stop_obj, min_index, "stop");

    if (msb < min_index || msb > max_index) {
        PyErr_Format(
            PyExc_IndexError,
            "APyUFixed slice start %d out of range [%d, %d]",
            msb,
            min_index,
            max_index
        );
        throw nb::python_error();
    }
    if (lsb < min_index || lsb > max_index) {
        PyErr_Format(
            PyExc_IndexError,
            "APyUFixed slice stop %d out of range [%d, %d]",
            lsb,
            min_index,
            max_index
        );
        throw nb::python_error();
    }
    if (msb < lsb) {
        PyErr_Format(
            PyExc_ValueError,
            "APyUFixed.__getitem__: invalid HDL-style slice [%d:%d], expected start >= "
            "stop",
            msb,
            lsb
        );
        throw nb::python_error();
    }

    const int result_bits = msb - lsb + 1;
    std::vector<apy_limb_t> result(bits_to_limbs(result_bits), apy_limb_t(0));
    for (int source_index = lsb; source_index <= msb; ++source_index) {
        if (!get_bit_value(source_index)) {
            continue;
        }
        const unsigned dst_bit_index = source_index - lsb;
        result[dst_bit_index / APY_LIMB_SIZE_BITS] |= apy_limb_t(1)
            << (dst_bit_index % APY_LIMB_SIZE_BITS);
    }

    return python_limb_vec_to_long(
        result.begin(), result.end(), false, static_cast<unsigned>(result_bits)
    );
}

std::string APyUFixed::repr() const
{
    return fmt::format(
        "APyUFixed({}, int_bits={}, frac_bits={})",
        bit_pattern_to_string_dec(),
        int_bits(),
        frac_bits()
    );
}

bool APyUFixed::is_identical(const APyUFixed& other) const
{
    return bits() == other.bits() && int_bits() == other.int_bits() && *this == other;
}

std::size_t APyUFixed::leading_zeros() const
{
    const std::size_t result = _value.leading_zeros();
    return result == 0 ? 0 : result - 1;
}

std::size_t APyUFixed::leading_ones() const
{
    std::size_t result = 0;
    for (int index = int_bits() - 1; index >= -frac_bits(); --index) {
        if (get_bit_value(index) == 0) {
            break;
        }
        ++result;
    }
    return result;
}

void APyUFixed::set_bit_pow2(int n, bool bit)
{
    if (n < -frac_bits() || n >= int_bits()) {
        PyErr_Format(
            PyExc_IndexError,
            "APyUFixed bit index %d out of range [%d, %d]",
            n,
            -frac_bits(),
            int_bits() - 1
        );
        throw nb::python_error();
    }
    _value.set_bit_pow2(n, bit);
}

APyUFixed APyUFixed::get_zero() const
{
    return from_backing(APyFixed(_value.bits(), _value.int_bits()));
}

std::tuple<int, int, std::vector<std::uint64_t>> APyUFixed::python_pickle() const
{
    return _value.python_pickle();
}

void APyUFixed::python_unpickle(
    APyUFixed* apyufixed, const std::tuple<int, int, std::vector<std::uint64_t>>& state
)
{
    const int bits = std::get<0>(state);
    const int int_bits = std::get<1>(state);
    APyFixed value(bits, int_bits);
    APyFixed::python_unpickle(&value, state);
    new (apyufixed) APyUFixed(std::move(value));
}

APyUFixed APyUFixed::from_number(
    const nb::object& py_obj,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    if (nb::isinstance<APyUFixed>(py_obj)) {
        return nb::cast<APyUFixed>(py_obj).cast(int_bits, frac_bits, {}, {}, bits);
    }

    if (nb::isinstance<APyFixed>(py_obj) && !int_bits && !frac_bits && !bits) {
        const APyFixed source = nb::cast<APyFixed>(py_obj);
        APyFixed value = source.cast(
            source.int_bits() + 1,
            source.frac_bits(),
            QuantizationMode::RND_INF,
            OverflowMode::WRAP,
            source.bits() + 1
        );
        return from_backing(std::move(value));
    }

    return from_backing(
        APyFixed::from_number(
            py_obj, with_sign_bit(int_bits), frac_bits, with_sign_bit(bits)
        )
    );
}

APyUFixed APyUFixed::from_double(
    double value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    return from_backing(
        APyFixed::from_double(
            value, with_sign_bit(int_bits), frac_bits, with_sign_bit(bits)
        )
    );
}

APyUFixed APyUFixed::from_integer(
    const nb::int_& value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<int> bits
)
{
    return from_backing(
        APyFixed::from_integer(
            value, with_sign_bit(int_bits), frac_bits, with_sign_bit(bits)
        )
    );
}

APyUFixed APyUFixed::from_string(
    std::string string_value,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    int base,
    std::optional<int> bits
)
{
    return from_backing(
        APyFixed::from_string(
            std::move(string_value),
            with_sign_bit(int_bits),
            frac_bits,
            base,
            with_sign_bit(bits)
        )
    );
}

APyUFixed APyUFixed::get_max(int bits, int int_bits)
{
    return from_backing(APyFixed::get_max(bits + 1, int_bits + 1));
}

APyUFixed APyUFixed::get_min(int bits, int int_bits)
{
    return from_backing(APyFixed(bits + 1, int_bits + 1));
}

APyUFixed APyUFixed::one(int bits, int int_bits)
{
    return from_backing(APyFixed::one(bits + 1, int_bits + 1));
}

APyUFixed APyUFixed::cast(
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    std::optional<QuantizationMode> quantization,
    std::optional<OverflowMode> overflow,
    std::optional<int> bits
) const
{
    return from_backing(_value.cast(
        with_sign_bit(int_bits), frac_bits, quantization, overflow, with_sign_bit(bits)
    ));
}

APyUFixed
APyUFixed::cast_no_overflow(int bits, int int_bits, QuantizationMode quantization) const
{
    return from_backing(_value.cast_no_overflow(bits + 1, int_bits + 1, quantization));
}
