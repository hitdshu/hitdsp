#pragma once

#include "common/register.h"
#include <complex>
#include <string>
#include <vector>

namespace hitdsp {
namespace modem {

struct ModemParam {
    int nbits;
    int csize;
    ModemParam(int bits_num = 1) {
        nbits = bits_num;
        csize = 1 << nbits;
    }
};

class ModemBase {
public:
    ModemBase() = default;
    virtual ~ModemBase() = default;

    virtual void InitModem(const ModemParam &param) {
        nbits_ = param.nbits;
        csize_ = param.csize;
    }

    virtual ::std::complex<float> Modulate(const ::std::vector<uint8_t> &bits) = 0;
    virtual ::std::vector<uint8_t> Demodulate(const ::std::complex<float> &sym) = 0;
    virtual ::std::vector<float> DemodulateLlr(const ::std::complex<float> &sym) = 0;
    virtual float GetLlrNp() = 0;
    virtual void SetLlrNp(float np) = 0;
    virtual ::std::string Name() const = 0;

    ModemBase(const ModemBase &) = delete;
    ModemBase &operator=(const ModemBase &) = delete;

protected:
    int nbits_;
    int csize_;
};

HITDSP_REGISTER_REGISTER(ModemBase);
#define HITDSP_REGISTER_MODEM(name) \
    HITDSP_REGISTER_CLASS(ModemBase, name)

} // namespace modem
} // namespace hitdsp