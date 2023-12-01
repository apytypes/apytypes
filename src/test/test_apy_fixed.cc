#include "../apy_fixed.h"
#include "catch.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using ulimb_vec = std::vector<mp_limb_t>;
using  limb_vec = std::vector<mp_limb_signed_t>;


TEST_CASE("APyFixed must have a positive non-zero size")
{
    // Zero bit fixed-point types does not exist
    REQUIRE_THROWS_AS(APyFixed(0,  1), std::domain_error);

    // One bit fixed-point types does not throw on creation
    APyFixed(1, 0);
}

TEST_CASE("Vector initialization must be consistent with APyFixed word-length")
{
    // Vector-initialization fails on size missmatch
    REQUIRE_THROWS_AS(APyFixed(_LIMB_SIZE_BITS+1, 0, ulimb_vec{0}), std::domain_error);

    // Vector-initalization succeeds on correct sizes
    for (unsigned bits=1; bits<_LIMB_SIZE_BITS; bits++) {
        APyFixed(bits, 0, ulimb_vec{0});
    }

}

TEST_CASE("APyFixed::bits(), APyFixed::int_bits() and APyFixed::vector_size()")
{
    REQUIRE(APyFixed(12345, 12).bits()                       == 12345);
    REQUIRE(APyFixed(12345, 12).int_bits()                   == 12);
    REQUIRE(APyFixed(123*_LIMB_SIZE_BITS,   0).vector_size() == 123);
    REQUIRE(APyFixed(123*_LIMB_SIZE_BITS+1, 0).vector_size() == 124);
    REQUIRE(APyFixed(1, 0).vector_size()                     == 1);
}

TEST_CASE("APyFixed::twos_complement_overflow()")
{
    REQUIRE(
        APyFixed(128, 1, 
        ulimb_vec{0x0, 0x8000000000000000}).to_string_dec() 
        == "-1"
    );
    REQUIRE(
        APyFixed(128, 1, 
        ulimb_vec{0x0, 0x4000000000000000}).to_string_dec() 
        == "0.5"
    );
    REQUIRE(
        APyFixed(96, 1, 
        ulimb_vec{0x0, 0xFFFFFFFF00000000}).to_string_dec() 
        == "0"
    );
    REQUIRE(
        APyFixed(96, 1, 
        ulimb_vec{0x0, 0xFFFFFFFF80000000}).to_string_dec() 
        == "-1"
    );
    REQUIRE(
        APyFixed(96, 1, 
        ulimb_vec{0x0, 0xFFFFFFFF40000000}).to_string_dec() 
        == "0.5"
    );

}


TEST_CASE("APyFixed::_data_asl()")
{
    {  /* Test #1 */
        APyFixed fix(32, 32, ulimb_vec{ uint64_t(-1) });
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(fix._data_asl(32) == ulimb_vec{ 0xFFFFFFFF00000000              });
        REQUIRE(fix._data_asl(33) == ulimb_vec{ 0xFFFFFFFE00000000, uint64_t(-1)});
    }

    {  /* Test #2 */
        APyFixed fix(
            128, 128,
            ulimb_vec{ 0xDEADBEEFDEADBEEF, 0x7FFFFFFBADBADBAD }
        );
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4*10) ==
            ulimb_vec{
                0, 0, 0, 0,
                0xADBEEF0000000000,
                0xBADBADDEADBEEFDE,
                0x7FFFFFFBAD
            }
        );
    }

    {  /* Test #3 */
        APyFixed fix(
            128, 128,
            ulimb_vec{ 0xDEADBEEFDEADBEEF, 0x8FFFFFFBADBADBAD }
        );
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4*10) ==
            ulimb_vec{
                0, 0, 0, 0,
                0xADBEEF0000000000,
                0xBADBADDEADBEEFDE,
                0xFFFFFF8FFFFFFBAD
            }
        );
    }
}

TEST_CASE("from_floating_point")
{
    /*
     * Zero floating-point
     */
    REQUIRE( APyFixed(   1,     0,  0.0).to_string_dec() == "0" );
    REQUIRE( APyFixed(1234, -1000, -0.0).to_string_dec() == "0" );
    REQUIRE( APyFixed(1234,  1000,  0.0).to_string_dec() == "0" );
    REQUIRE( APyFixed(1234,  4000, -0.0).to_string_dec() == "0" );

    /*
     * Integer tests
     */
    REQUIRE( APyFixed(64, 64,  123).to_string_dec() ==  "123" );
    REQUIRE( APyFixed(64, 64, -123).to_string_dec() == "-123" );
    REQUIRE(
        APyFixed(64, 64, std::pow(2.0, 52.0) + 0).to_string_dec()
        == "4503599627370496"
    );
    REQUIRE(
        APyFixed(64, 64, std::pow(2.0, 52.0) + 1).to_string_dec()
        == "4503599627370497"
    );
    REQUIRE(
        APyFixed(64, 64, std::pow(2.0, 53.0) + 0).to_string_dec()
        == "9007199254740992"
    );
    REQUIRE(
        APyFixed(64, 64, std::pow(2.0, 53.0) + 1).to_string_dec()
        == "9007199254740992"  // Precision was lost when adding 1 onto 2^53
    );
    REQUIRE(
        APyFixed(30, 153, std::pow(2.0, 123.0) - std::pow(2.0, 152)).to_string_dec()
        == "-5708990760190015557953816894567524063288229888"
    );
    REQUIRE(
        APyFixed(29, 153, std::pow(2.0, 123.0) - std::pow(2.0, 152)).to_string_dec()
        == "-5708990749556191591674489911337067581045473280"
    );

    std::cout << APyFixed(64,  32,   0.0  ).to_string_dec() << std::endl;
    std::cout << APyFixed(64,  32,   1.0  ).to_string_dec() << std::endl;
    std::cout << APyFixed(64,  32, 123.125).to_string_dec() << std::endl;
    std::cout << APyFixed(64,  32,-123.125).to_string_dec() << std::endl;
    //std::cout << APyFixed(64,  0,    2.0).to_string_dec() << std::endl;
    //std::cout << APyFixed(64, 32, 9123.0).to_string_dec() << std::endl;
}

TEST_CASE("APyFixed::from_vector()")
{
    APyFixed a(64, 64, limb_vec{ -1 });
    REQUIRE(a.to_string() == "-1");
    REQUIRE_THROWS_AS(a.from_vector( ulimb_vec{ 0, 0 }), std::domain_error);
    a.from_vector( ulimb_vec{ 5 } );
    REQUIRE(a.to_string() == "5");
}

TEST_CASE("Non-implemented function")
{
    REQUIRE_THROWS_AS(APyFixed(1,0).to_string_hex(), NotImplementedException);
    REQUIRE_THROWS_AS(APyFixed(1,0).to_string_oct(), NotImplementedException);
}
