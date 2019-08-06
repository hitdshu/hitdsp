#include "fft/fft_base_impl.h"
#include "common/macro.h"

namespace hitdsp {
namespace fft {

void FftBaseImpl::Init(int fft_size, bool forward_flag) {
    fft_size_ = fft_size;
    forward_flag_ = forward_flag;
    twiddlef_ = ::std::vector<::std::complex<float>>(fft_size_, ::std::complex<float>());
    twiddled_ = ::std::vector<::std::complex<double>>(fft_size_, ::std::complex<double>());
    int sign = forward_flag_ ? -1 : 1;
    for (int idx = 0; idx < fft_size_; ++idx) {
        twiddlef_[idx] = ::std::exp((float)(sign * 2 * PI * idx / fft_size_) * ::std::complex<float>(0, 1));
        twiddled_[idx] = ::std::exp((double)(sign * 2 * PI * idx / fft_size_) * ::std::complex<double>(0, 1));
    }
}

void FftBaseImpl::Transform(const ::std::complex<float> input[], ::std::complex<float> output[]) const {
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] = ::std::complex<float>(0, 0);
    }
    for (int idx1 = 0; idx1 < fft_size_; ++idx1) {
        for (int idx2 = 0; idx2 < fft_size_; ++idx2) {
            output[idx1] += input[idx2] * twiddlef_[idx1 * idx2 % fft_size_];
        }
    }
    if (!forward_flag_) {
        for (int idx = 0; idx < fft_size_; ++idx) {
            output[idx] /= fft_size_;
        }
    }
}

void FftBaseImpl::Transform(const ::std::complex<double> input[], ::std::complex<double> output[]) const {
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] = ::std::complex<double>(0, 0);
    }
    for (int idx1 = 0; idx1 < fft_size_; ++idx1) {
        for (int idx2 = 0; idx2 < fft_size_; ++idx2) {
            output[idx1] += input[idx2] * twiddled_[idx1 * idx2 % fft_size_];
        }
    }
    if (!forward_flag_) {
        for (int idx = 0; idx < fft_size_; ++idx) {
            output[idx] /= fft_size_;
        }
    }
}

HITDSP_REGISTER_FFT(FftBaseImpl);
} // namespace fft
} // namespace hitdsp