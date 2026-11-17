/* Dynamic arbitrary precision unsigned fixed-point data type. */

#ifndef _APYUFIXED_H
#define _APYUFIXED_H

#include "apyfixed.h"

class APyUFixed {
private:
    APyFixed _value;

    explicit APyUFixed(APyFixed value);
    void clear_sign_bit();
    static std::optional<int> with_sign_bit(std::optional<int> value);
    static APyUFixed from_backing(APyFixed value);

public:
    APyUFixed() = delete;
    APyUFixed(const APyUFixed&) = default;
    APyUFixed& operator=(const APyUFixed&) = default;

    explicit APyUFixed(
        const nb::int_& python_long_int_bit_pattern,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    APyUFixed python_copy() const { return *this; }
    APyUFixed python_deepcopy(const nb::typed<nb::dict, nb::int_, nb::any>&) const
    {
        return *this;
    }
    std::tuple<int, int, std::vector<std::uint64_t>> python_pickle() const;
    static void python_unpickle(
        APyUFixed* apyufixed,
        const std::tuple<int, int, std::vector<std::uint64_t>>& state
    );

    APyUFixed operator+(const APyUFixed& rhs) const;
    APyFixed operator-(const APyUFixed& rhs) const;
    APyUFixed operator*(const APyUFixed& rhs) const;
    APyUFixed operator/(const APyUFixed& rhs) const;

    APyFixed operator+(const nb::int_& rhs) const;
    APyFixed operator-(const nb::int_& rhs) const;
    APyFixed operator*(const nb::int_& rhs) const;
    APyFixed operator/(const nb::int_& rhs) const;

    APyFixed operator+(double rhs) const;
    APyFixed operator-(double rhs) const;
    APyFixed operator*(double rhs) const;
    APyFixed operator/(double rhs) const;

    bool operator==(const APyUFixed& rhs) const;
    bool operator!=(const APyUFixed& rhs) const;
    bool operator<(const APyUFixed& rhs) const;
    bool operator<=(const APyUFixed& rhs) const;
    bool operator>(const APyUFixed& rhs) const;
    bool operator>=(const APyUFixed& rhs) const;

    bool operator==(const nb::int_& rhs) const;
    bool operator!=(const nb::int_& rhs) const;
    bool operator<(const nb::int_& rhs) const;
    bool operator<=(const nb::int_& rhs) const;
    bool operator>(const nb::int_& rhs) const;
    bool operator>=(const nb::int_& rhs) const;

    bool operator==(double rhs) const;
    bool operator!=(double rhs) const;
    bool operator<(double rhs) const;
    bool operator<=(double rhs) const;
    bool operator>(double rhs) const;
    bool operator>=(double rhs) const;

    APyUFixed operator<<(int shift_val) const;
    APyUFixed operator>>(int shift_val) const;
    APyUFixed& operator<<=(int shift_val);
    APyUFixed& operator>>=(int shift_val);
    APyFixed operator-() const;
    APyUFixed operator+() const { return *this; }
    APyUFixed operator~() const;
    APyUFixed abs() const { return *this; }
    APyUFixed pown(int n) const;

    int bits() const noexcept { return _value.bits() - 1; }
    int int_bits() const noexcept { return _value.int_bits() - 1; }
    int frac_bits() const noexcept { return _value.frac_bits(); }
    std::size_t vector_size() const noexcept { return _value.vector_size(); }

    bool is_negative() const noexcept { return false; }
    bool is_positive() const noexcept { return !is_zero(); }
    bool is_zero() const noexcept { return _value.is_zero(); }
    nb::int_ to_bits() const { return _value.to_bits(); }
    int get_bit_value(int index) const;
    nb::int_ get_hdl_bit_range(const nb::slice& index) const;
    nb::object to_fraction() const { return _value.to_fraction(); }
    nb::object as_integer_ratio() const { return _value.as_integer_ratio(); }
    std::string bit_pattern_to_string_dec() const
    {
        return _value.bit_pattern_to_string_dec();
    }
    std::string repr() const;
    std::string latex() const { return _value.latex(); }
    bool is_identical(const APyUFixed& other) const;
    std::size_t leading_zeros() const;
    std::size_t leading_ones() const;
    std::size_t leading_fractional_zeros() const
    {
        return _value.leading_fractional_zeros();
    }
    std::size_t leading_signs() const { return leading_zeros(); }
    std::size_t trailing_zeros() const { return _value.trailing_zeros(); }
    bool positive_greater_than_equal_pow2(int n) const
    {
        return _value.positive_greater_than_equal_pow2(n);
    }
    void set_bit_pow2(int n, bool bit);
    std::uint64_t get_lsbs() const { return _value.get_lsbs(); }
    APyUFixed get_zero() const;
    APyFixed to_apyfixed() const { return _value; }

    double to_double() const { return _value.to_double(); }
    std::string to_string(int base = 10) const { return _value.to_string(base); }
    std::string to_string_hex() const { return _value.to_string_hex(); }
    std::string to_string_oct() const { return _value.to_string_oct(); }
    std::string to_string_dec() const { return _value.to_string_dec(); }
    operator double() const { return to_double(); }

    APyUFixed cast(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    ) const;
    APyUFixed cast_no_overflow(
        int bits, int int_bits, QuantizationMode quantization = QuantizationMode::TRN
    ) const;

    static APyUFixed from_number(
        const nb::object& py_obj,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );
    static APyUFixed from_double(
        double value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );
    static APyUFixed from_integer(
        const nb::int_& value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );
    static APyUFixed from_string(
        std::string string_value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        int base = 10,
        std::optional<int> bits = std::nullopt
    );
    static APyUFixed get_max(int bits, int int_bits);
    static APyUFixed get_min(int bits, int int_bits);
    static APyUFixed one(int bits, int int_bits);
};

#endif // _APYUFIXED_H
