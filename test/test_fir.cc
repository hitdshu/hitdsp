#include "catch/catch.hpp"
#include "filter/filter_fir_exth.h"
#include <cstdlib>
#include <random>

TEST_CASE("Test base fir implementation", "[fir]") {
    ::hitdsp::filter::FilterFirExth fir;
    const int tap_len = 256;
    const int sap_len = tap_len * 3;
    ::std::complex<float> h[tap_len];
    const float sqrt2inv = 1 / ::std::sqrt(2);
    for (int idx = 0; idx < tap_len; ++idx) {
        h[idx] = ::std::complex<float>((rand() % 2) ? sqrt2inv : -sqrt2inv, (rand() % 2) ? sqrt2inv : -sqrt2inv) / (float)tap_len;
    }
    fir.InitFirCoeff(h, tap_len);
    ::std::complex<float> x[sap_len];
    ::std::complex<float> rxy[sap_len];
    for (int idx = 0; idx < sap_len; ++idx) {
        x[idx] = 0;
    }
    for (int idx = 0; idx < tap_len; ++idx) {
        x[tap_len + tap_len - idx - 1] = ::std::conj(h[idx]) * (float)tap_len;
    }
    ::std::complex<float> peak_val = 0;
    int peak_idx = 0;
    for (int idx = 0; idx < sap_len; ++idx) {
        rxy[idx] = fir.PushAndExecute(x[idx]);
        if (::std::abs(rxy[idx]) > ::std::abs(peak_val)) {
            peak_val = rxy[idx];
            peak_idx = idx;
        }
    }
    REQUIRE(std::abs((float)1 - peak_val) < 1e-5);
    REQUIRE(511 == peak_idx);
}

TEST_CASE("Test base fft block fir implementation", "[fir]") {
    ::hitdsp::filter::FilterFirExth fir;
    const int tap_len = 57;
    const int block_size = 64;
    const int sap_len = block_size * 3;
    ::std::complex<float> h[tap_len];
    const float sqrt2inv = 1 / ::std::sqrt(2);
    for (int idx = 0; idx < tap_len; ++idx) {
        h[idx] = ::std::complex<float>((rand() % 2) ? sqrt2inv : -sqrt2inv, (rand() % 2) ? sqrt2inv : -sqrt2inv) / (float)tap_len;
    }
    fir.InitFirCoeff(h, tap_len);
    ::std::complex<float> x[sap_len];
    ::std::default_random_engine generator;
    ::std::normal_distribution<float> normal_dist(0, 1);
    for (int idx = 0; idx < sap_len; ++idx) {
        x[idx] = ::std::complex<float>(normal_dist(generator), normal_dist(generator));
    }
    ::std::complex<float> y1[sap_len];
    for (int idx = 0; idx < sap_len; ++idx) {
        y1[idx] = fir.PushAndExecute(x[idx]);
    }
    ::std::complex<float> y2[sap_len];
    for (int idx = 0; idx < 3; ++idx) {
        fir.BlockExecute(x + idx * block_size, y2 + idx * block_size);
    }
    for (int idx = 0; idx < sap_len; ++idx) {
        REQUIRE(::std::abs(y1[idx] - y2[idx]) < 1e-5);
    }
}