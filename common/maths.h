#pragma once

#include <vector>
#include <complex>

namespace hitdsp {
namespace common {

::std::vector<::std::complex<float>> ComputeIdealLowPassPulse(float w, int tap_len);

::std::vector<float> Hamming(int tap_len);

int Factorial(int n);
float Izero(float x, int order);
::std::vector<float> Kaiser(int tap_len, float beta);

} // namespace common
} // namespace hitdsp