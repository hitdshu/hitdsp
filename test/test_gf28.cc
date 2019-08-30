#include "catch/catch.hpp"
#include "ecc/gf28.h"
#include "ecc/poly.h"
#include "ecc/reed_solomon.h"

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
    REQUIRE(poly_x[0] == 0x37);
    REQUIRE(poly_x[1] == 0xe6);
    REQUIRE(poly_x[2] == 0x78);
    REQUIRE(poly_x[3] == 0xd9);
    REQUIRE(quot[0] == 0x12);
    REQUIRE(quot[1] == 0xda);
    REQUIRE(quot[2] == 0xdf);
}

TEST_CASE("Test base reed solomon encoder implementation", "[gf28]") {
    ::std::vector<uint8_t> msg_in = {0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06, 0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec};
    ::hitdsp::ecc::ReedSolomon rs(16, 10);
    ::std::vector<uint8_t> msg_out(26);
    rs.Encode(&msg_in[0], &msg_out[0]);
    REQUIRE(msg_out[0] == 0x40);
    REQUIRE(msg_out[1] == 0xd2);
    REQUIRE(msg_out[24] == 0x4b);
    REQUIRE(msg_out[25] == 0xe0);
}