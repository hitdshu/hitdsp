#include "common/maths.h"
#include "common/macro.h"

namespace hitdsp {
namespace common {

::std::vector<::std::complex<float>> ComputeIdealLowPassPulse(float w, int tap_len) {
    ::std::vector<::std::complex<float>> result;
    for (int idx = 0; idx < tap_len; ++idx) {
        int shift_idx = idx - (tap_len - 1) / 2;
        ::std::complex<float> tmp = ::std::sin(w * shift_idx) / (PI * shift_idx);
        result.push_back(tmp);
    }
    return result;
}

::std::vector<float> Hamming(int tap_len) {
    ::std::vector<float> result;
    int m = tap_len - 1;
    for (int idx = 0; idx < tap_len; ++idx) {
        float tmp = 0.54 - 0.46 * ::std::cos(2 * PI * idx / m);
        result.push_back(tmp);
    }
    return result;
}

} // namespace commmon
} // namespace hitdsp