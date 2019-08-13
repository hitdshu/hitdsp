#pragma once

#include <vector>
#include <complex>

namespace hitdsp {
namespace common {

::std::vector<::std::complex<float>> ComputeIdealLowPassPulse(float w, int tap_len);

::std::vector<float> Hamming(int tap_len);

int Factorial(int n);

float LnGamma(float x);

float Izero(float x, int order);

::std::vector<float> Kaiser(int tap_len, float beta);

float ComplexAbs(const ::std::complex<float> &val);
double ComplexAbs(const ::std::complex<double> &val);

} // namespace common
} // namespace hitdsp