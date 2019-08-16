#pragma once

#include "modem/modem_base.h"
#include "modem/pam.h"
#include <cassert>
#include <memory>

namespace hitdsp {
namespace modem {

class ModemQam64 : public ModemBase {
public:
    virtual void InitModem(const ModemParam &param) override {
        assert(6 == param.nbits);
        assert(64 == param.csize);
        ModemBase::InitModem(param);
        pam8_.reset(new Pam<3>(0.5));
    }

    virtual ::std::complex<float> Modulate(const ::std::vector<uint8_t> &bits) override {
        return ::std::complex<float>(pam8_->Encode(&bits[0]), pam8_->Encode(&bits[nbits_ / 2]));
    }
    virtual ::std::vector<uint8_t> Demodulate(const ::std::complex<float> &sym) override {
        ::std::vector<uint8_t> result;
        ::std::vector<uint8_t> result1 = pam8_->DecodeHard(sym.real());
        ::std::vector<uint8_t> result2 = pam8_->DecodeHard(sym.imag());
        for (size_t idx = 0; idx < result1.size(); ++idx) {
            result.push_back(result1[idx]);
        }
        for (size_t idx = 0; idx < result2.size(); ++idx) {
            result.push_back(result2[idx]);
        }
        return result;
    }
    virtual ::std::string Name() const override {
        return "64QAM";
    }

protected:
    ::std::shared_ptr<Pam<3>> pam8_;
};

} // namespace modem
} // namespace hitdsp