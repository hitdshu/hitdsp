#include "catch/catch.hpp"
#include "ecc/conv.h"

TEST_CASE("Test base conv implementation", "[conv]") {
    ::hitdsp::ecc::Conv conv;
    int poly = 0b101;
    conv.Init(2, 1, &poly);
    uint8_t data[1] = {0b01011100};
    uint8_t symbols[10];
    uint8_t data_dec[1];
    conv.Encode(data, symbols, 10);
    conv.Decode(symbols, data_dec, 8);
    REQUIRE(data_dec[0] == data[0]);
}

TEST_CASE("Test rate12 conv implementation", "[conv]") {
    ::hitdsp::ecc::Conv conv;
    int poly[2] = {121, 91};
    conv.Init(6, 2, &poly[0]);
    uint8_t data[8] = {0b01011100, 0b00111010, 0b01011100, 0b00111010, 0b11111111, 0b00111010, 0b01011100, 0b00000000};
    uint8_t symbols[140];
    uint8_t data_dec[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    conv.Encode(data, symbols, 64);
    symbols[0] = 1;
    conv.Decode(symbols, data_dec, 64);
    REQUIRE(data_dec[0] == data[0]);
    REQUIRE(data_dec[1] == data[1]);
    REQUIRE(data_dec[2] == data[2]);
    REQUIRE(data_dec[3] == data[3]);
    REQUIRE(data_dec[4] == data[4]);
    REQUIRE(data_dec[5] == data[5]);
    REQUIRE(data_dec[6] == data[6]);
    REQUIRE(data_dec[7] == data[7]);
}