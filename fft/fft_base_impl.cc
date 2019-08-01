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
        twiddlef_[idx] = ::std::exp(sign * 2 * ::hitdsp::common::PI * idx / fft_size_ * ::std::complex<float>(0, 1));
        twiddled_[idx] = ::std::exp(sign * 2 * ::hitdsp::common::PI * idx / fft_size_ * ::std::complex<double>(0, 1));
    }
}

HITDSP_REGISTER_FFT(FftBaseImpl);
} // namespace fft
} // namespace hitdsp