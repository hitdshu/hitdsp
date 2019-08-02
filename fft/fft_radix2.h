#pragma once

#include "fft/fft_base_impl.h"
#include <vector>

namespace hitdsp {
namespace fft {

class FftRadix2 : public FftBaseImpl {
public:
    virtual void Init(int fft_size, bool forward_flag) override;
    virtual void Transform(const ::std::complex<float> input[], ::std::complex<float> output[]) override;
    virtual void Transform(const ::std::complex<double> input[], ::std::complex<double> output[]) override;

protected:
    uint32_t ReverseBit(uint32_t idx, int bn) const;
    int bn_;
    ::std::vector<int> reverse_order_;
};

} // namespace fft
} // namespace hitdsp