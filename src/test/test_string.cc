/*
 * String utility functions for the APyFixed data type.
 */

#include "../apy_fixed.h"
#include "catch.hpp"
#include <iostream>

TEST_CASE("APyFixed::to_str_dec()")
{
    // Single to_str_dec() point test
    const auto POINT_TEST = [](unsigned bits,
                               int int_bits,
                               const std::vector<mp_limb_t>& vec_in,
                               const char* expected_result) {
        APyFixed fixed(bits, int_bits, vec_in.begin(), vec_in.end());
        REQUIRE(fixed.to_string_dec() == expected_result);
    };

    /*
     * Integer tests
     */
    POINT_TEST(400, 0, to_limb_vec({ 0, 0, 0, 0, 0, 0, 0 }), "0");
    POINT_TEST(400, 400, to_limb_vec({ 0, 0, 0, 0, 0, 0, 0 }), "0");
    POINT_TEST(400, -1400, to_limb_vec({ 0, 0, 0, 0, 0, 0, 0 }), "0");
    POINT_TEST(400, 12000, to_limb_vec({ 0, 0, 0, 0, 0, 0, 0 }), "0");
    POINT_TEST(8, 8, to_limb_vec({ 0 }), "0");
    POINT_TEST(64, 64, to_limb_vec({ 1234000876300021324 }), "1234000876300021324");
    POINT_TEST(
        64,
        64,
        to_limb_vec({ static_cast<unsigned long>(-4321000867300021394) }),
        "-4321000867300021394"
    );
    POINT_TEST(
        32,
        200,
        to_limb_vec({ 0x976541 }),
        "3712207424220948591436712392519649713293914084621881966592"
    );
    POINT_TEST(128, 128, to_limb_vec({ 0x1234, 0xABCD }), "811306251105819789627956");
    POINT_TEST(
        128,
        128,
        to_limb_vec({ 0x1234000067ABC, uint64_t(0xfff123ull << (64 - 4 * 6)) }),
        "-77174568541894605962799058381276484"
    );

    /*
     * Fractional testing
     */
    POINT_TEST(32, 28, to_limb_vec({ 0x976541 }), "620116.0625");
    POINT_TEST(32, 23, to_limb_vec({ 0x976541 }), "19378.626953125");
    POINT_TEST(32, 1, to_limb_vec({ 0x976541 }), "0.0046202247031033039093017578125");
    POINT_TEST(32, 0, to_limb_vec({ 0x976541 }), "0.00231011235155165195465087890625");
    POINT_TEST(
        32, -1, to_limb_vec({ 0x976541 }), "0.001155056175775825977325439453125"
    );
    POINT_TEST(26, -1, to_limb_vec({ 0x976541 }), "0.073923595249652862548828125");
    POINT_TEST(
        26, -9, to_limb_vec({ 0x976541 }), "0.00028876404394395649433135986328125"
    );

    // Special case:
    // The aboslute value during negation in to_str_dec() increases the underlying
    // vector size by one.
    POINT_TEST(64, 64, to_limb_vec({ 0x0000000000000013 }), "19");
    POINT_TEST(64, 64, to_limb_vec({ 0xFFFFFFFFFFFFFFFF }), "-1");
    POINT_TEST(64, 64, to_limb_vec({ 0x8000000000000000 }), "-9223372036854775808");
    POINT_TEST(
        128,
        128,
        to_limb_vec({ 0, 0x8000000000000000 }),
        "-170141183460469231731687303715884105728"
    );
}

TEST_CASE("APyFixed::from_string_dec()")
{
    auto TEST_VALID_STRINGS = [](const std::vector<std::string>& string_vec) {
        for (auto str : string_vec) {
            APyFixed fixed(64, 0);
            fixed.from_string_dec(str);
        }
    };
    auto TEST_INVALID_STRING = [](const std::vector<std::string>& string_vec) {
        for (auto str : string_vec) {
            APyFixed fixed(64, 0);
            REQUIRE_THROWS_AS(fixed.from_string_dec(str), std::domain_error);
        }
    };

    /*
     * String validity testing. Should be consistent with Python decimal numeric
     * construction from strings
     */
    TEST_INVALID_STRING({
        "",      ".",     "-.",       "..",   "0..",   ".0.",  "..0",
        "1.2.3", "1-.2",  "-",        "-0..", "-.0.",  "-..0", "-1.2.3",
        "-1-.2", "-1.-2", "123.456-", "--",   "--1.2", "--0",  "--123",
    });
    TEST_VALID_STRINGS({
        "1.2",    "1.",     ".1",     "00",   "000", "00.00", "00.12",
        "010.",   "010.0",  "-1.2",   "-1.",  "-.1", "-00",   "-000",
        "-00.00", "-00.12", "-010.0", "-010", "0.0", "-0.0",
    });

    /*
     * Zero-string initialization
     */
    REQUIRE(APyFixed(1, 0, "0").to_string() == "0");
    REQUIRE(APyFixed(1234, 12345, "0").to_string() == "0");
    REQUIRE(APyFixed(1234, -12345, "0").to_string() == "0");

    /*
     * Integer string
     */
    auto large_integer_str = "-1234567890987654321234567899876543234568";
    REQUIRE(APyFixed(150, 150, large_integer_str).to_string() == large_integer_str);
    REQUIRE(APyFixed(150, 153, large_integer_str).to_string() == large_integer_str);
    REQUIRE(
        APyFixed(150, 154, large_integer_str).to_string()
        == "-1234567890987654321234567899876543234560"
    );
    REQUIRE(
        APyFixed(150, 230, large_integer_str).to_string()
        == "-1234567890987653842370520939723795464192"
    );
    REQUIRE(APyFixed(150, 500, large_integer_str).to_string() == "0");

    /*
     * Fractional strings
     */

    // Fractional strings are rounded (away from infinity on ties) when precision is
    // lost
    REQUIRE(APyFixed(5, 4, "0.24").to_string() == "0");
    REQUIRE(APyFixed(5, 4, "0.25").to_string() == "0.5");
    REQUIRE(APyFixed(5, 4, "-0.25").to_string() == "-0.5");
    REQUIRE(APyFixed(5, 4, "-0.24").to_string() == "0");

    REQUIRE(APyFixed(128, 64, "1234.5").to_string() == "1234.5");
    REQUIRE(APyFixed(10, 5, "0.1328125").to_string() == "0.125");
    REQUIRE(APyFixed(10, 8, "12.5").to_string() == "12.5");
    REQUIRE(APyFixed(10, 8, "-12.5").to_string() == "-12.5");

    {
        auto str = "0.00028876404394395649433135986328125";
        REQUIRE(APyFixed(26, -9, str).to_string() == str);
    }
}

TEST_CASE("APyFixed::repr()")
{
    {
        auto str = "0.00028876404394395649433135986328125";
        REQUIRE(
            APyFixed(26, -9, str).repr()
            == "APyFixed<26, -9>(0.00028876404394395649433135986328125)"
        );
    }
}

TEST_CASE("Not implemented to/from string function")
{
    REQUIRE_THROWS_AS(APyFixed(1, 0).to_string_hex(), NotImplementedException);
    REQUIRE_THROWS_AS(APyFixed(1, 0).to_string_oct(), NotImplementedException);
}
