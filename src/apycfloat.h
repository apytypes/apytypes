#ifndef _APYCFLOAT_H
#define _APYCFLOAT_H

#include "apyfloat.h"
#include "apyfloat_util.h"

class APyCFloat {

    /* ****************************************************************************** *
     * *                          APyCFloat data fields                             * *
     * ****************************************************************************** */

private:
    // Bit specifiers and exponent bias
    std::uint8_t exp_bits;
    std::uint8_t man_bits;
    exp_t bias;

    // Data of the `APyCFloat`
    APyFloatData real;
    APyFloatData imag;

    /* ****************************************************************************** *
     * *                              CRTP methods                                  * *
     * ****************************************************************************** */

public:
    //! Copy `n` items from `it` into `*this`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_from(RANDOM_ACCESS_ITERATOR src_it, std::size_t n) noexcept
    {
        using iterator_value_type = typename RANDOM_ACCESS_ITERATOR::value_type;
        static_assert(std::is_same_v<iterator_value_type, APyFloatData>);
        assert(n == 2);

        real = src_it[0];
        imag = src_it[1];
    }

    //! Copy `n` items from `*this` into `it`
    template <typename RANDOM_ACCESS_ITERATOR>
    void copy_n_to(RANDOM_ACCESS_ITERATOR dst_it, std::size_t n) const noexcept
    {
        using iterator_value_type = typename RANDOM_ACCESS_ITERATOR::value_type;
        static_assert(std::is_same_v<iterator_value_type, APyFloatData>);
        assert(n == 2);

        dst_it[0] = real;
        dst_it[1] = imag;
    }

    //! Test if two floating-point numbers have the same bit specifiers
    APY_INLINE bool same_type_as(const APyCFloat& other) const
    {
        return man_bits == other.man_bits && exp_bits == other.exp_bits
            && bias == other.bias;
    }

    /* ****************************************************************************** *
     * *                      Static conversion from other types                    * *
     * ****************************************************************************** */

    //! Create APyCFloat from Python object
    static APyCFloat from_number(
        const nb::object& py_obj,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create APyCFloat from double
    static APyCFloat from_double(
        double value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    //! Create APyCFloat from Python integer
    static APyCFloat from_integer(
        const nb::int_ value,
        int exp_bits,
        int man_bits,
        std::optional<exp_t> bias = std::nullopt
    );

    /* ****************************************************************************** *
     * *                           Friends of `APyCFloat`                           * *
     * ****************************************************************************** */

    template <typename T, typename ARRAY_TYPE> friend class APyArray;
};

#endif
