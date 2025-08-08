/*
 * Dynamic arbitrary precision complex-valued fixed-point data type.
 */

#ifndef _APYCFIXED_H
#define _APYCFIXED_H

#include "apyfixed.h"
#include "apytypes_scratch_vector.h"
#include "apytypes_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h> // nanobind::object
#include <variant>
namespace nb = nanobind;

#include <algorithm>        // std::copy_n
#include <complex>          // std::complex
#include <cstddef>          // std::size_t
#include <initializer_list> // std::initializer_list
#include <limits>           // std::numeric_limits<>::is_iec559
#include <optional>         // std::optional, std::nullopt
#include <string>           // std::string
#include <vector>           // std::vector

class APyCFixed {

    /* ****************************************************************************** *
     *                            APyCFixed C++ assumptions                           *
     * ****************************************************************************** */

    static_assert(
        (APY_LIMB_SIZE_BYTES == 8 || APY_LIMB_SIZE_BYTES == 4),
        "The `apy_limb_t` data type is either 64-bit or 32-bit. Any other limb size "
        "is unsupported. The size of limbs is specified during compilation with the C "
        "Macro `COMPILER_LIMB_SIZE`."
    );
    static_assert(
        (-1 >> 1 == -1),
        "Right shift applied to signed integral types performs *arithmetic* right "
        "shift. Arithmetic right shift of signed types is *the only* valid behaviour "
        "since C++20, but before C++20 the right shift of signed integral types is "
        "implementation defined. APyCFixed relies heavily on arithmetic right shift."
    );
    static_assert(
        (std::numeric_limits<double>::is_iec559),
        "We assume IEEE-754 double-precision floating-point types."
    );

    /* ****************************************************************************** *
     *                            APyCFixed data fields                               *
     * ****************************************************************************** */

private:
    int _bits;
    int _int_bits;

    using vector_t = ScratchVector<apy_limb_t>;
    vector_t _data;
    // `apy_limb_t` is the underlying data type used for arithmetic in APyCFixed.
    // It is either a 32-bit or a 64-bit unsigned int, depending on the target
    // architecture.

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Copy `n` items from `it` into `*this`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_from(RANDOM_ACCESS_ITERATOR src_it, std::size_t n) noexcept
    {
        std::copy_n(src_it, n, std::begin(_data));
    }

    //! Copy `n` items from `*this` into `it`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_to(RANDOM_ACCESS_ITERATOR dst_it, std::size_t n) const noexcept
    {
        std::copy_n(std::begin(_data), n, dst_it);
    }

    //! Test if two fixed-point numbers have the same bit specifiers
    APY_INLINE bool is_same_spec(const APyCFixed& other) const
    {
        return _bits == other._bits && _int_bits == other._int_bits;
    }

    //! Retrieve the bit specification
    APY_INLINE APyFixedSpec spec() const noexcept { return { _bits, _int_bits }; }

    /* ****************************************************************************** *
     *                            Python constructors                                 *
     * ****************************************************************************** */

public:
    //! No default (empty) constructed `APyCFixed` objects. At least the bit-specifiers
    //! has to be set during construction.
    APyCFixed() = delete;

    //! Construct a copy from `other`.
    APyCFixed(const APyCFixed& other) = default;

    //! Main Python-exposed `APyCFixed` constructor
    explicit APyCFixed(
        const nb::int_& python_long_int_bit_pattern,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Main Python-exposed `APyCFixed` constructor
    explicit APyCFixed(
        const nb::tuple& python_long_int_bit_pattern,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    /* ****************************************************************************** *
     *                       Non-Python accessible constructors                       *
     * ****************************************************************************** */

public:
    //! Construct a zero-initialized number with `bits` and `int_bits`. Undefined
    //! behaviour if `bits < 1`.
    explicit APyCFixed(int bits, int int_bits);

    //! Construct a zero-initialized number from bit specification. Throws
    //! `nanobind::value_error` if not exactly two of three bit specifiers (`bits`,
    //! `int_bits`, `frac_bits`) are set, or if the resulting number of `bits` is less
    //! than one.
    explicit APyCFixed(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Construct a number with `bits` and `int_bits`, and initialize underlying
    //! bit-pattern from iterator pair [ `begin`, `end` ).
    template <typename _IT>
    explicit APyCFixed(int bits, int int_bits, _IT begin, _IT end);

    //! Construct a number with `bits` and `int_bits`, and initialize underlying
    //! bit-pattern from initializer list of limbs
    explicit APyCFixed(int bits, int int_bits, std::initializer_list<apy_limb_t> list);

    /* ****************************************************************************** *
     * *                                     Copy                                   * *
     * ****************************************************************************** */

    //! Copy scalar
    APyCFixed python_copy() const { return *this; }
    //! Deepcopy scalar (same as copy here)
    APyCFixed python_deepcopy(const nb::typed<nb::dict, nb::int_, nb::any>&) const
    {
        return *this;
    }

    /* ****************************************************************************** *
     *                         Binary arithmetic operators                            *
     * ****************************************************************************** */

private:
    //! Base addition/subtraction routine for two `APyCFixed`
    template <class base_op, class ripple_carry_op>
    inline APyCFixed _apycfixed_base_add_sub(const APyCFixed& rhs) const;

public:
    APyCFixed operator+(const APyCFixed& rhs) const;
    APyCFixed operator-(const APyCFixed& rhs) const;
    APyCFixed operator*(const APyCFixed& rhs) const;
    APyCFixed operator/(const APyCFixed& rhs) const;
    APyCFixed operator<<(const int shift_val) const;
    APyCFixed operator>>(const int shift_val) const;
    APyCFixed& operator<<=(const int shift_val);
    APyCFixed& operator>>=(const int shift_val);

    APyCFixed operator+(const nb::int_& rhs) const;
    APyCFixed operator-(const nb::int_& rhs) const;
    APyCFixed operator*(const nb::int_& rhs) const;
    APyCFixed operator/(const nb::int_& rhs) const;

    bool operator==(const APyCFixed& rhs) const;
    bool operator!=(const APyCFixed& rhs) const;

    bool operator==(const APyFixed& rhs) const;
    bool operator!=(const APyFixed& rhs) const;

    bool operator==(const nb::int_& rhs) const;
    bool operator!=(const nb::int_& rhs) const;

    bool operator==(double rhs) const;
    bool operator!=(double rhs) const;

    bool operator==(const std::complex<double>& rhs) const;
    bool operator!=(const std::complex<double>& rhs) const;

    /* ****************************************************************************** *
     *                        Other public member functions                           *
     * ****************************************************************************** */

public:
    //! Return the `bits` specifier
    APY_INLINE int bits() const noexcept { return _bits; }

    //! Return the `int_bits` specifier
    APY_INLINE int int_bits() const noexcept { return _int_bits; }

    //! Return the `frac_bits` specifier
    APY_INLINE int frac_bits() const noexcept { return _bits - _int_bits; }

    //! Python verbose string representation
    std::string repr() const;

    //! Convert the underlying bit pattern to decimal and return in a string
    std::string bit_pattern_to_string_dec() const;

    //! Conversion to string
    std::string to_string(int base = 10) const;
    std::string to_string_hex() const;
    std::string to_string_oct() const;
    std::string to_string_dec() const;

    //! Test if two fixed-point numbers are identical, i.e., has the same values, the
    //! same number of integer bits, and the same number of fractional bits
    bool is_identical(
        const std::variant<const APyCFixed*, const APyCFixedArray*>& other
    ) const;

    //! Test if fixed-point number is zero
    bool is_zero() const noexcept;

    //! Convert to a tuple of bits
    nb::tuple to_bits() const;

    //! Unary negation
    APyCFixed operator-() const;

    //! Unary positive
    APY_INLINE APyCFixed operator+() const { return *this; };

    //! Logic not
    APyCFixed operator~() const;

    //! Retrieve real part
    APyFixed get_real() const;

    //! Retrieve imaginary part
    APyFixed get_imag() const;

    //! Convert to built-in complex type over double-precision floating-point
    std::complex<double> to_complex() const;

    /* ****************************************************************************** *
     *                     Resize and quantization method (cast)                    * *
     * ****************************************************************************** */

public:
    //! Publicly exposed cast method
    APyCFixed cast(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    ) const;

    /* ****************************************************************************** *
     *                       Static conversion from other types                       *
     * ****************************************************************************** */

public:
    //! Create APyCFixed from Python float or int.
    static APyCFixed from_number(
        const nb::object& py_obj,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyCFixed from complex double.
    static APyCFixed from_complex(
        std::complex<double> value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyCFixed from double.
    static APyCFixed from_double(
        double value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyCFixed from Python integer.
    static APyCFixed from_integer(
        const nb::int_& value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Create APyCFixed from an APyFixed
    static APyCFixed from_apyfixed(
        const APyFixed& value,
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<int> bits = std::nullopt
    );

    //! Get bit pattern for the complex-valued fixed-point one
    static APyCFixed one(int bits, int int_bits);

    /* ****************************************************************************** *
     *                        Private helper methods                                * *
     * ****************************************************************************** */
private:
    auto real_begin() noexcept { return std::begin(_data); }
    auto real_begin() const noexcept { return std::cbegin(_data); }
    auto real_cbegin() const noexcept { return real_begin(); }
    auto real_end() noexcept { return std::begin(_data) + _data.size() / 2; }
    auto real_end() const noexcept { return std::cbegin(_data) + _data.size() / 2; }
    auto real_cend() const noexcept { return real_end(); }

    auto imag_begin() noexcept { return std::begin(_data) + _data.size() / 2; }
    auto imag_begin() const noexcept { return std::cbegin(_data) + _data.size() / 2; }
    auto imag_cbegin() const noexcept { return imag_begin(); }
    auto imag_end() noexcept { return std::end(_data); }
    auto imag_end() const noexcept { return std::end(_data); }
    auto imag_cend() const noexcept { return std::end(_data); }

    /* ****************************************************************************** *
     * *                          Friends of APyCFixed :)                            * *
     * ****************************************************************************** */
public:
    friend class APyCFixedArray;
    friend class APyCFloat;
};

#endif
