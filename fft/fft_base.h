#pragma once

#include "common/register.h"

namespace hitdsp {
namespace fft {

class FftBase {
public:
    FftBase() = default;
    virtual ~FftBase() = default;

    FftBase(const FftBase &) = delete;
    FftBase &operator=(const FftBase &) = delete;

protected:
    
};

HITDSP_REGISTER_REGISTER(FftBase);
#define HITDSP_REGISTER_FFT(name) \
    HITDSP_REGISTER_CLASS(FftBase, name)

} // namespace fft
} // namespace hitdsp