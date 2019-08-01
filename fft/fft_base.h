#pragma once

#include <complex>
#include "common/register.h"

namespace hitdsp {
namespace fft {

class FftBase {
public:
    FftBase() = default;
    virtual ~FftBase() = default;

    virtual void Init(int fft_size, bool forward_flag);
    template <typename ValueType>
    virtual void Transform(const ::std::complex<ValueType> input[], ::std::complex<ValueType> output[]);

    FftBase(const FftBase &) = delete;
    FftBase &operator=(const FftBase &) = delete;
};

HITDSP_REGISTER_REGISTER(FftBase);
#define HITDSP_REGISTER_FFT(name) \
    HITDSP_REGISTER_CLASS(FftBase, name)

} // namespace fft
} // namespace hitdsp