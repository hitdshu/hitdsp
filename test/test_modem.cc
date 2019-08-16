#include "catch/catch.hpp"
#include "modem/modem_bpsk.h"
#include "modem/modem_qpsk.h"
#include "modem/modem_qam16.h"
#include "modem/modem_qam64.h"

TEST_CASE("Test modem bpsk implementation", "[modem]") {
    ::hitdsp::modem::ModemBpsk bpsk;
    ::hitdsp::modem::ModemParam param(1);
    bpsk.InitModem(param);
    ::std::vector<uint8_t> bits0;
    bits0.push_back(0);
    ::std::complex<float> s0 = bpsk.Modulate(bits0);
    ::std::vector<uint8_t> bits_dec0 = bpsk.Demodulate(s0);
    REQUIRE(::std::abs(s0 + ::std::complex<float>(1, 0)) < 1e-5);
    REQUIRE(bits_dec0[0] == 0);
    ::std::vector<uint8_t> bits1;
    bits1.push_back(1);
    ::std::complex<float> s1 = bpsk.Modulate(bits1);
    ::std::vector<uint8_t> bits_dec1 = bpsk.Demodulate(s1);
    REQUIRE(::std::abs(s1 + ::std::complex<float>(-1, 0)) < 1e-5);
    REQUIRE(bits_dec1[0] == 1);
}