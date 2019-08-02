#include "catch/catch.hpp"
#include "fft/fft_base_impl.h"

TEST_CASE( "Test base dft implementation", "[fft]") {
    ::hitdsp::fft::FftBaseImpl fft;
    fft.Init(8, true);
    ::std::complex<float> input[8];
    ::std::complex<float> output[8];
    input[0] = ::std::complex<float>(0.707106781186548, -0.707106781186548);
    input[1] = ::std::complex<float>(0, -1);
    input[2] = ::std::complex<float>(-0.707106781186548, -0.707106781186548);
    input[3] = ::std::complex<float>(-1, 0);
    input[4] = ::std::complex<float>(-0.707106781186548, 0.707106781186548);
    input[5] = ::std::complex<float>(0, 1);
    input[6] = ::std::complex<float>(0.707106781186548, 0.707106781186548);
    input[7] = ::std::complex<float>(1, 0);
    fft.Transform(input, output);
    REQUIRE(output[0].real() == Approx(0));
    REQUIRE(output[0].imag() == Approx(-2.10734e-08));
    REQUIRE(output[1].real() == Approx(-2.10734e-08));
    REQUIRE(output[1].imag() == Approx(0));
    REQUIRE(output[2].real() == Approx(4.21608e-09));
    REQUIRE(output[2].imag() == Approx(0));
    REQUIRE(output[3].real() == Approx(-1.05367e-07));
    REQUIRE(output[3].imag() == Approx(-1.05367e-07));
    REQUIRE(output[4].real() == Approx(-4.21468e-08));
    REQUIRE(output[4].imag() == Approx(9.83519e-09));
    REQUIRE(output[5].real() == Approx(0));
    REQUIRE(output[5].imag() == Approx(-2.10734e-08));
    REQUIRE(output[6].real() == Approx(-1.54543e-08));
    REQUIRE(output[6].imag() == Approx(0));
    REQUIRE(output[7].real() == Approx(2));
    REQUIRE(output[7].imag() == Approx(-2));
    fft.Init(8, false);
    fft.Transform(output, input);
    REQUIRE(input[0].real() == Approx(0.707107));
    REQUIRE(input[0].imag() == Approx(-0.707107));
    REQUIRE(input[1].real() == Approx(-1.11539e-07));
    REQUIRE(input[1].imag() == Approx(-1));
    REQUIRE(input[2].real() == Approx(-0.707107));
    REQUIRE(input[2].imag() == Approx(-0.707107));
    REQUIRE(input[3].real() == Approx(-1));
    REQUIRE(input[3].imag() == Approx(3.19105e-09));
    REQUIRE(input[4].real() == Approx(-0.707107));
    REQUIRE(input[4].imag() == Approx(0.707107));
    REQUIRE(input[5].real() == Approx(-2.72457e-08));
    REQUIRE(input[5].imag() == Approx(1));
    REQUIRE(input[6].real() == Approx(0.707107));
    REQUIRE(input[6].imag() == Approx(0.707107));
    REQUIRE(input[7].real() == Approx(1));
    REQUIRE(input[7].imag() == Approx(4.53379e-08));
}