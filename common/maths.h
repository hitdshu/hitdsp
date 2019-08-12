#pragma once

#include <vector>
#include <complex>

namespace hitdsp {
namespace common {

::std::vector<::std::complex<float>> ComputeIdealLowPassPulse(float w, int tap_len);

::std::vector<float> Hamming(int tap_len);

} // namespace common
} // namespace hitdsp