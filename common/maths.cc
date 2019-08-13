#include "common/maths.h"
#include "common/macro.h"

namespace hitdsp {
namespace common {

::std::vector<::std::complex<float>> ComputeIdealLowPassPulse(float w, int tap_len) {
    ::std::vector<::std::complex<float>> result;
    for (int idx = 0; idx < tap_len; ++idx) {
        int shift_idx = idx - (tap_len - 1) / 2;
        ::std::complex<float> tmp;
        if (fabs(shift_idx * w) < 0.001) {
            tmp = w / PI;
        } else {
            tmp = ::std::sin(w * shift_idx) / (PI * shift_idx);
        }
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

float LnGamma(float x) {
    float result;
    if (x < 10) {
        return LnGamma(x + 1) - ::std::log(x);
    } else {
        result = 0.5 * (::std::log(2 * PI) - ::std::log(x));
        result += x * (::std::log(x + (1.0 / (12.0 * x - 0.1 / x))) - 1);
    }
    return result;
}

float Izero(float x, int order) {
    float y = 1;
    for (int idx = 1; idx < order + 1; ++idx) {
        float tmp = idx * ::std::log(x / 2.0) - LnGamma(idx + 1);
        y += ::std::exp(2 * tmp);
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

float ComplexAbs(const ::std::complex<float> &val) {
    float result = 0;
    result += val.real() * val.real();
    result += val.imag() * val.imag();
    return ::std::sqrt(result);
}

double ComplexAbs(const ::std::complex<double> &val) {
    double result = 0;
    result += val.real() * val.real();
    result += val.imag() * val.imag();
    return ::std::sqrt(result);
}

} // namespace commmon
} // namespace hitdsp