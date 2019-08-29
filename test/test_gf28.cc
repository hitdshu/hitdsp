#include "catch/catch.hpp"
#include "ecc/gf28.h"
#include "ecc/poly.h"

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

TEST_CASE("Test base poly implementation", "[gf28]") {
    ::std::vector<uint8_t> coeffs_x = {0x12, 0x34, 0x56, 0x00, 0x00, 0x00, 0x00};
    ::std::vector<uint8_t> coeffs_y = {0x01, 0x0f, 0x36, 0x78, 0x40};
    ::hitdsp::ecc::Poly poly_x(&coeffs_x[0], 6);
    ::hitdsp::ecc::Poly poly_y(&coeffs_y[0], 4);
    ::hitdsp::ecc::Poly quot = poly_x.Div(poly_y);
    REQUIRE(poly_x.GetCoeff(0) == 0x37);
    REQUIRE(poly_x.GetCoeff(1) == 0xe6);
    REQUIRE(poly_x.GetCoeff(2) == 0x78);
    REQUIRE(poly_x.GetCoeff(3) == 0xd9);
    REQUIRE(quot.GetCoeff(0) == 0x12);
    REQUIRE(quot.GetCoeff(1) == 0xda);
    REQUIRE(quot.GetCoeff(2) == 0xdf);
}