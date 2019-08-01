#pragma once

#include "fft/fft_base.h"
#include <vector>

namespace hitdsp {
namespace fft {

class FftBaseImpl : public FftBase {
public:
    virtual void Init(int fft_size, bool forward_flag) override;
    virtual void Transform(const ::std::complex<float> input[], ::std::complex<float> output[]) override;
    virtual void Transform(const ::std::complex<double> input[], ::std::complex<double> output[]) override;

protected:
    int fft_size_;
    bool forward_flag_;
    ::std::vector<::std::complex<float>> twiddlef_;
    ::std::vector<::std::complex<double>> twiddled_;
};

} // namespace fft
} // namespace hitdsp