#include "catch/catch.hpp"
#include "ecc/conv.h"
#include <iostream>

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