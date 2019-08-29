#include "catch/catch.hpp"
#include "ecc/gf28.h"

TEST_CASE("Test base gf28 implementation", "[gf28]") {
    ::hitdsp::ecc::Gf28 gf28;
    uint8_t x = 0b10001001;
    uint8_t y = 0b00101010;
    uint8_t result1 = gf28.MulNoLUT(x, y);
    uint8_t result2 = gf28.MulNoLUT(y, x);
    uint8_t result3 = gf28.Mul(x, y);
    uint8_t result4 = gf28.Mul(y, x);
    uint8_t result = 0b11000011;
    REQUIRE(result1 == result);
    REQUIRE(result2 == result);
    REQUIRE(result3 == result);
    REQUIRE(result4 == result);
    uint8_t x_result = gf28.Div(result, y);
    uint8_t y_result = gf28.Div(result, x);
    REQUIRE(x_result == x);
    REQUIRE(y_result == y);
    uint8_t x_inv = gf28.Inv(x);
    REQUIRE(gf28.Mul(x, x_inv) == 1);
}