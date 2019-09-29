#include "catch/catch.hpp"
#include "modem/pam.h"
#include <vector>

TEST_CASE("Test base 2pam implementation", "[pam]") {
    ::hitdsp::modem::Pam<1> pam;
    ::std::vector<uint8_t> bits0;
    bits0.push_back(0);
    float s0 = pam.Encode(&bits0[0]);
    ::std::vector<uint8_t> bits_dec0 = pam.DecodeHard(s0);
    REQUIRE(fabs(s0 + 1) < 1e-5);
    REQUIRE(bits_dec0[0] == 0);
    ::std::vector<float> bits_dec0_s = pam.DecodeSoftLlr(s0);
    ::std::vector<uint8_t> bits_dec0_h(bits_dec0_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec0_s.size(); ++idx) {
        bits_dec0_h[idx] = bits_dec0_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec0_h[0] == 0);
    ::std::vector<uint8_t> bits1;
    bits1.push_back(1);
    float s1 = pam.Encode(&bits1[0]);
    ::std::vector<uint8_t> bits_dec1 = pam.DecodeHard(s1);
    REQUIRE(fabs(s1 - 1) < 1e-5);
    REQUIRE(bits_dec1[0] == 1);
    ::std::vector<float> bits_dec1_s = pam.DecodeSoftLlr(s1);
    ::std::vector<uint8_t> bits_dec1_h(bits_dec1_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec1_s.size(); ++idx) {
        bits_dec1_h[idx] = bits_dec1_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec1_h[0] == 1);
}

TEST_CASE("Test base 4pam implementation", "[pam]") {
    ::hitdsp::modem::Pam<2> pam(0.5);
    ::std::vector<uint8_t> bits0;
    bits0.push_back(0);
    bits0.push_back(0);
    float s0 = pam.Encode(&bits0[0]);
    ::std::vector<uint8_t> bits_dec0 = pam.DecodeHard(s0);
    REQUIRE(fabs(s0 + 0.948683) < 1e-5);
    REQUIRE(bits_dec0[0] == 0);
    REQUIRE(bits_dec0[1] == 0);
    ::std::vector<float> bits_dec0_s = pam.DecodeSoftLlr(s0);
    ::std::vector<uint8_t> bits_dec0_h(bits_dec0_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec0_s.size(); ++idx) {
        bits_dec0_h[idx] = bits_dec0_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec0_h[0] == 0);
    REQUIRE(bits_dec0_h[1] == 0);
    ::std::vector<uint8_t> bits1;
    bits1.push_back(0);
    bits1.push_back(1);
    float s1 = pam.Encode(&bits1[0]);
    ::std::vector<uint8_t> bits_dec1 = pam.DecodeHard(s1);
    REQUIRE(fabs(s1 + 0.316228) < 1e-5);
    REQUIRE(bits_dec1[0] == 0);
    REQUIRE(bits_dec1[1] == 1);
    ::std::vector<float> bits_dec1_s = pam.DecodeSoftLlr(s1);
    ::std::vector<uint8_t> bits_dec1_h(bits_dec1_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec1_s.size(); ++idx) {
        bits_dec1_h[idx] = bits_dec1_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec1_h[0] == 0);
    REQUIRE(bits_dec1_h[1] == 1);
    ::std::vector<uint8_t> bits2;
    bits2.push_back(1);
    bits2.push_back(1);
    float s2 = pam.Encode(&bits2[0]);
    ::std::vector<uint8_t> bits_dec2 = pam.DecodeHard(s2);
    REQUIRE(fabs(s2 - 0.316228) < 1e-5);
    REQUIRE(bits_dec2[0] == 1);
    REQUIRE(bits_dec2[1] == 1);
    ::std::vector<float> bits_dec2_s = pam.DecodeSoftLlr(s2);
    ::std::vector<uint8_t> bits_dec2_h(bits_dec2_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec2_s.size(); ++idx) {
        bits_dec2_h[idx] = bits_dec2_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec2_h[0] == 1);
    REQUIRE(bits_dec2_h[1] == 1);
    ::std::vector<uint8_t> bits3;
    bits3.push_back(1);
    bits3.push_back(0);
    float s3 = pam.Encode(&bits3[0]);
    ::std::vector<uint8_t> bits_dec3 = pam.DecodeHard(s3);
    REQUIRE(fabs(s3 - 0.948683) < 1e-5);
    REQUIRE(bits_dec3[0] == 1);
    REQUIRE(bits_dec3[1] == 0);
    ::std::vector<float> bits_dec3_s = pam.DecodeSoftLlr(s3);
    ::std::vector<uint8_t> bits_dec3_h(bits_dec3_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec3_s.size(); ++idx) {
        bits_dec3_h[idx] = bits_dec3_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec3_h[0] == 1);
    REQUIRE(bits_dec3_h[1] == 0);
}

TEST_CASE("Test base 8pam implementation", "[pam]") {
    ::hitdsp::modem::Pam<3> pam(0.5);
    ::std::vector<uint8_t> bits0;
    bits0.push_back(0);
    bits0.push_back(0);
    bits0.push_back(0);
    float s0 = pam.Encode(&bits0[0]);
    ::std::vector<uint8_t> bits_dec0 = pam.DecodeHard(s0);
    REQUIRE(fabs(s0 + 1.08012) < 1e-5);
    REQUIRE(bits_dec0[0] == 0);
    REQUIRE(bits_dec0[1] == 0);
    REQUIRE(bits_dec0[2] == 0);
    ::std::vector<float> bits_dec0_s = pam.DecodeSoftLlr(s0);
    ::std::vector<uint8_t> bits_dec0_h(bits_dec0_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec0_s.size(); ++idx) {
        bits_dec0_h[idx] = bits_dec0_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec0_h[0] == 0);
    REQUIRE(bits_dec0_h[1] == 0);
    REQUIRE(bits_dec0_h[2] == 0);
    ::std::vector<uint8_t> bits1;
    bits1.push_back(0);
    bits1.push_back(0);
    bits1.push_back(1);
    float s1 = pam.Encode(&bits1[0]);
    ::std::vector<uint8_t> bits_dec1 = pam.DecodeHard(s1);
    REQUIRE(fabs(s1 + 0.771516) < 1e-5);
    REQUIRE(bits_dec1[0] == 0);
    REQUIRE(bits_dec1[1] == 0);
    REQUIRE(bits_dec1[2] == 1);
    ::std::vector<float> bits_dec1_s = pam.DecodeSoftLlr(s1);
    ::std::vector<uint8_t> bits_dec1_h(bits_dec1_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec1_s.size(); ++idx) {
        bits_dec1_h[idx] = bits_dec1_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec1_h[0] == 0);
    REQUIRE(bits_dec1_h[1] == 0);
    REQUIRE(bits_dec1_h[2] == 1);
    ::std::vector<uint8_t> bits2;
    bits2.push_back(0);
    bits2.push_back(1);
    bits2.push_back(1);
    float s2 = pam.Encode(&bits2[0]);
    ::std::vector<uint8_t> bits_dec2 = pam.DecodeHard(s2);
    REQUIRE(fabs(s2 + 0.462910) < 1e-5);
    REQUIRE(bits_dec2[0] == 0);
    REQUIRE(bits_dec2[1] == 1);
    REQUIRE(bits_dec2[2] == 1);
    ::std::vector<float> bits_dec2_s = pam.DecodeSoftLlr(s2);
    ::std::vector<uint8_t> bits_dec2_h(bits_dec2_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec2_s.size(); ++idx) {
        bits_dec2_h[idx] = bits_dec2_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec2_h[0] == 0);
    REQUIRE(bits_dec2_h[1] == 1);
    REQUIRE(bits_dec2_h[2] == 1);
    ::std::vector<uint8_t> bits3;
    bits3.push_back(0);
    bits3.push_back(1);
    bits3.push_back(0);
    float s3 = pam.Encode(&bits3[0]);
    ::std::vector<uint8_t> bits_dec3 = pam.DecodeHard(s3);
    REQUIRE(fabs(s3 + 0.154303) < 1e-5);
    REQUIRE(bits_dec3[0] == 0);
    REQUIRE(bits_dec3[1] == 1);
    REQUIRE(bits_dec3[2] == 0);
    ::std::vector<float> bits_dec3_s = pam.DecodeSoftLlr(s3);
    ::std::vector<uint8_t> bits_dec3_h(bits_dec3_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec3_s.size(); ++idx) {
        bits_dec3_h[idx] = bits_dec3_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec3_h[0] == 0);
    REQUIRE(bits_dec3_h[1] == 1);
    REQUIRE(bits_dec3_h[2] == 0);
    ::std::vector<uint8_t> bits4;
    bits4.push_back(1);
    bits4.push_back(1);
    bits4.push_back(0);
    float s4 = pam.Encode(&bits4[0]);
    ::std::vector<uint8_t> bits_dec4 = pam.DecodeHard(s4);
    REQUIRE(fabs(s4 - 0.154303) < 1e-5);
    REQUIRE(bits_dec4[0] == 1);
    REQUIRE(bits_dec4[1] == 1);
    REQUIRE(bits_dec4[2] == 0);
    ::std::vector<float> bits_dec4_s = pam.DecodeSoftLlr(s4);
    ::std::vector<uint8_t> bits_dec4_h(bits_dec4_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec4_s.size(); ++idx) {
        bits_dec4_h[idx] = bits_dec4_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec4_h[0] == 1);
    REQUIRE(bits_dec4_h[1] == 1);
    REQUIRE(bits_dec4_h[2] == 0);
    ::std::vector<uint8_t> bits5;
    bits5.push_back(1);
    bits5.push_back(1);
    bits5.push_back(1);
    float s5 = pam.Encode(&bits5[0]);
    ::std::vector<uint8_t> bits_dec5 = pam.DecodeHard(s5);
    REQUIRE(fabs(s5 - 0.462910) < 1e-5);
    REQUIRE(bits_dec5[0] == 1);
    REQUIRE(bits_dec5[1] == 1);
    REQUIRE(bits_dec5[2] == 1);
    ::std::vector<float> bits_dec5_s = pam.DecodeSoftLlr(s5);
    ::std::vector<uint8_t> bits_dec5_h(bits_dec5_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec5_s.size(); ++idx) {
        bits_dec5_h[idx] = bits_dec5_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec5_h[0] == 1);
    REQUIRE(bits_dec5_h[1] == 1);
    REQUIRE(bits_dec5_h[2] == 1);
    ::std::vector<uint8_t> bits6;
    bits6.push_back(1);
    bits6.push_back(0);
    bits6.push_back(1);
    float s6 = pam.Encode(&bits6[0]);
    ::std::vector<uint8_t> bits_dec6 = pam.DecodeHard(s6);
    REQUIRE(fabs(s6 - 0.771516) < 1e-5);
    REQUIRE(bits_dec6[0] == 1);
    REQUIRE(bits_dec6[1] == 0);
    REQUIRE(bits_dec6[2] == 1);
    ::std::vector<float> bits_dec6_s = pam.DecodeSoftLlr(s6);
    ::std::vector<uint8_t> bits_dec6_h(bits_dec6_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec6_s.size(); ++idx) {
        bits_dec6_h[idx] = bits_dec6_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec6_h[0] == 1);
    REQUIRE(bits_dec6_h[1] == 0);
    REQUIRE(bits_dec6_h[2] == 1);
    ::std::vector<uint8_t> bits7;
    bits7.push_back(1);
    bits7.push_back(0);
    bits7.push_back(0);
    float s7 = pam.Encode(&bits7[0]);
    ::std::vector<uint8_t> bits_dec7 = pam.DecodeHard(s7);
    REQUIRE(fabs(s7 - 1.08012) < 1e-5);
    REQUIRE(bits_dec7[0] == 1);
    REQUIRE(bits_dec7[1] == 0);
    REQUIRE(bits_dec7[2] == 0);
    ::std::vector<float> bits_dec7_s = pam.DecodeSoftLlr(s7);
    ::std::vector<uint8_t> bits_dec7_h(bits_dec7_s.size(), 0);
    for (size_t idx = 0; idx < bits_dec7_s.size(); ++idx) {
        bits_dec7_h[idx] = bits_dec7_s[idx] > 0 ? 0 : 1;
    }
    REQUIRE(bits_dec7_h[0] == 1);
    REQUIRE(bits_dec7_h[1] == 0);
    REQUIRE(bits_dec7_h[2] == 0);
}