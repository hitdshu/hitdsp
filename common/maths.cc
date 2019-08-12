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

int Factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * Factorial(n - 1);
}

float Izero(float x, int order) {
    float y = 1;
    for (int idx = 1; idx < order + 1; ++idx) {
        y += ::std::pow(::std::pow(x / 2, idx) / Factorial(idx), 2);
    }
    return y;
}

::std::vector<float> Kaiser(int tap_len, float beta) {
    ::std::vector<float> result;
    int m = tap_len - 1;
    float alpha = m / 2;
    float izero_beta = Izero(beta, 32);
    for (int idx = 0; idx < tap_len; ++idx) {
        float tmp = Izero(beta * ::std::sqrt(1 - ::std::pow((idx - alpha) / alpha, 2)), 32) / izero_beta;
        result.push_back(tmp);
    }
    return result;
}

} // namespace commmon
} // namespace hitdsp