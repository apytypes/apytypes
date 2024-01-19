/*
 * General tests for the APyFixed data type
 */

#include "../apyfixed.h"
#include "../apytypes_util.h"
#include "catch.hpp"

#include <cmath>
#include <cstdlib>
#include <gmp.h>
#include <iostream>
#include <limits>
#include <stdexcept>

TEST_CASE("APyFixed assumptions")
{
    // The GMP `mp_limb_t` data type is either 64 bit or 32 bit. Any other limb size is
    // unsupported. This assumption should hold true always, according to the GMP
    // documentation, but check it just to be safe.
    REQUIRE((sizeof(mp_limb_t) == 8 || sizeof(mp_limb_t) == 4));

    // Right shift applied to signed integral types performs *arithmetic* right shift.
    // Arithmetic right shift of signed types is *the only* valid behaviour since C++20,
    // but before C++20 the right shift of signed integral types is implementation
    // defined. APyFixed relies heavily on arithmetic right shift.
    REQUIRE(-1 >> 1 == -1);

    // We assume IEEE-754 double-precision floating-point types
    REQUIRE(std::numeric_limits<double>::is_iec559);
}

TEST_CASE("APyFixed must have a positive non-zero size")
{
    // Zero bit fixed-point types does not exist
    REQUIRE_THROWS_AS(APyFixed(0, 1), std::domain_error);

    // One bit fixed-point types does not throw on creation
    APyFixed(1, 0);
}

TEST_CASE("APyFixed::bits(), APyFixed::int_bits() and APyFixed::vector_size()")
{
    REQUIRE(APyFixed(12345, 12).bits() == 12345);
    REQUIRE(APyFixed(12345, 12).int_bits() == 12);
    REQUIRE(APyFixed(123 * _LIMB_SIZE_BITS, 0).vector_size() == 123);
    REQUIRE(APyFixed(123 * _LIMB_SIZE_BITS + 1, 0).vector_size() == 124);
    REQUIRE(APyFixed(1, 0).vector_size() == 1);
}

TEST_CASE("APyFixed::twos_complement_overflow()")
{
    REQUIRE(
        APyFixed(128, 1, to_limb_vec({ 0x0, 0x8000000000000000 })).to_string_dec()
        == "-1"
    );
    REQUIRE(
        APyFixed(128, 1, to_limb_vec({ 0x0, 0x4000000000000000 })).to_string_dec()
        == "0.5"
    );
    REQUIRE(
        APyFixed(96, 1, to_limb_vec({ 0x0, 0xFFFFFFFF00000000 })).to_string_dec() == "0"
    );
    REQUIRE(
        APyFixed(96, 1, to_limb_vec({ 0x0, 0xFFFFFFFF80000000 })).to_string_dec()
        == "-1"
    );
    REQUIRE(
        APyFixed(96, 1, to_limb_vec({ 0x0, 0xFFFFFFFF40000000 })).to_string_dec()
        == "0.5"
    );
}

TEST_CASE("APyFixed::_data_asl()")
{
    { /* Test #1 */
        APyFixed fix(32, 32, to_limb_vec({ uint64_t(-1) }));
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(fix._data_asl(32) == to_limb_vec({ 0xFFFFFFFF00000000 }));
        REQUIRE(fix._data_asl(33) == to_limb_vec({ 0xFFFFFFFE00000000, uint64_t(-1) }));
    }

    { /* Test #2 */
        APyFixed fix(128, 128, to_limb_vec({ 0xDEADBEEFDEADBEEF, 0x7FFFFFFBADBADBAD }));
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4 * 10)
            == to_limb_vec(
                { 0, 0, 0, 0, 0xADBEEF0000000000, 0xBADBADDEADBEEFDE, 0x7FFFFFFBAD }
            )
        );
    }

    { /* Test #3 */
        APyFixed fix(128, 128, to_limb_vec({ 0xDEADBEEFDEADBEEF, 0x8FFFFFFBADBADBAD }));
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4 * 10)
            == to_limb_vec({ 0,
                             0,
                             0,
                             0,
                             0xADBEEF0000000000,
                             0xBADBADDEADBEEFDE,
                             0xFFFFFF8FFFFFFBAD })
        );
    }
}

TEST_CASE("APyFixed::from_double()")
{
    /*
     * Zero floating-point
     */
    REQUIRE(APyFixed(0.0, 1, 0).to_string_dec() == "0");
    REQUIRE(APyFixed(-0.0, 1234, -1000).to_string_dec() == "0");
    REQUIRE(APyFixed(0.0, 1234, 1000).to_string_dec() == "0");
    REQUIRE(APyFixed(-0.0, 1234, 4000).to_string_dec() == "0");

    /*
     * Integer tests
     */
    REQUIRE(APyFixed(123.0, 64, 64).to_string_dec() == "123");
    REQUIRE(APyFixed(-123.0, 64, 64).to_string_dec() == "-123");
    REQUIRE(
        APyFixed(std::pow(2.0, 52.0) + 0, 64, 64).to_string_dec() == "4503599627370496"
    );
    REQUIRE(
        APyFixed(std::pow(2.0, 52.0) + 1, 64, 64).to_string_dec() == "4503599627370497"
    );
    REQUIRE(
        APyFixed(std::pow(2.0, 53.0) + 0, 64, 64).to_string_dec() == "9007199254740992"
    );
    REQUIRE(
        APyFixed(std::pow(2.0, 53.0) + 1, 64, 64).to_string_dec()
        == "9007199254740992" // Precision was lost when adding 1 onto 2^53
    );
    REQUIRE(
        APyFixed(std::pow(2.0, 123.0) - std::pow(2.0, 152), 30, 153).to_string_dec()
        == "-5708990760190015557953816894567524063288229888"
    );
    REQUIRE(
        APyFixed(std::pow(2.0, 123.0) - std::pow(2.0, 152), 29, 153).to_string_dec()
        == "-5708990770823839524233143877797980545530986496" // Precision lost,
    );                                                       // number rounded

    /*
     * Fractional number test
     */
    REQUIRE(APyFixed(123.125, 64, 0).to_string_dec() == "0.125");
    REQUIRE(APyFixed(-std::pow(2, -127), 1, -126).to_string_dec() ==
          "-0.0000000000000000000000000000000000000058774717541114375"
          "3984368268611122838909332778386043760754375853139208629727"
          "36358642578125");
    REQUIRE(
        APyFixed(-1.0 + std::pow(2, -53), 55, 2).to_string_dec()
        == "-0.99999999999999988897769753748434595763683319091796875"
    );

    /*
     * Round away from infinity on tie
     */
    REQUIRE(APyFixed(0.24, 2, 1).to_string_dec() == "0");
    REQUIRE(APyFixed(0.25, 2, 1).to_string_dec() == "0.5");
    REQUIRE(APyFixed(-0.25, 2, 1).to_string_dec() == "-0.5");
    REQUIRE(APyFixed(-0.24, 2, 1).to_string_dec() == "0");
}

TEST_CASE("APyFixed::to_double()")
{
    // Zero-value floats
    REQUIRE(APyFixed(0.0, 1, 0).to_double() == 0.0);
    REQUIRE(APyFixed(0.0, 1234, 123).to_double() == 0.0);
    REQUIRE(APyFixed(0.0, 1234, -123).to_double() == 0.0);

    // More tests
    REQUIRE(APyFixed(0.25, 5, 1).to_double() == 0.25);
    REQUIRE(APyFixed(-0.25, 5, 1).to_double() == -0.25);
    REQUIRE(APyFixed(9, 5, to_limb_vec({ 0x48 })).to_double() == 4.5);
    REQUIRE(
        APyFixed(128 + 12, 8, to_limb_vec({ 0x0, 0x0, 0x558 })).to_double() == 85.5
    );
}

TEST_CASE("Private member function: APyFixed::from_vector()")
{
    APyFixed a(64, 64, to_limb_vec({ uint64_t(-1) }));
    REQUIRE(a.to_string() == "-1");
    REQUIRE_THROWS_AS(a.from_vector(to_limb_vec({ 0, 0 })), std::domain_error);
    a.from_vector(to_limb_vec({ 5 }));
    REQUIRE(a.to_string() == "5");
}

TEST_CASE("Non-implemented functions")
{
    REQUIRE_THROWS_AS(APyFixed(1, 0).to_string_hex(), NotImplementedException);
    REQUIRE_THROWS_AS(APyFixed(1, 0).to_string_oct(), NotImplementedException);
    REQUIRE_THROWS_AS(APyFixed(1, 0, "0", 8), NotImplementedException);
    REQUIRE_THROWS_AS(APyFixed(1, 0, "0", 16), NotImplementedException);
    REQUIRE_THROWS_AS(APyFixed(1, 0, "0", -1), std::domain_error);
}

TEST_CASE("Vector initialization must be consistent with APyFixed word-length")
{
    // Vector-initialization fails on size missmatch
    REQUIRE_THROWS_AS(
        APyFixed(_LIMB_SIZE_BITS + 1, 0, to_limb_vec({ 0 })), std::domain_error
    );

    // Vector-initalization succeeds on correct sizes
    for (unsigned bits = 1; bits < _LIMB_SIZE_BITS; bits++) {
        APyFixed(bits, 0, to_limb_vec({ 0 }));
    }
}

TEST_CASE("Bit specifying copy constructor")
{
    { /* Test #1 */
        APyFixed operand(2.5, 10, 3);
        REQUIRE(operand.to_string() == "2.5");

        APyFixed fix_copy(operand, 10, 5);
        REQUIRE(fix_copy.bits() == 10);
        REQUIRE(fix_copy.int_bits() == 5);
        REQUIRE(fix_copy.to_string() == "2.5");
    }
}

TEST_CASE("APyFixed::is_zero()")
{
    REQUIRE(APyFixed(0.0, 1, 0).is_zero());
    REQUIRE(!APyFixed(1.5, 5, 2).is_zero());
}
