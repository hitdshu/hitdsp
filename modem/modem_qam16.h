#pragma once

#include "modem/modem_base.h"
#include "modem/pam.h"
#include <cassert>
#include <memory>

namespace hitdsp {
namespace modem {

class ModemQam16 : public ModemBase {
public:
    virtual void InitModem(const ModemParam &param) override {
        assert(4 == param.nbits);
        assert(16 == param.csize);
        ModemBase::InitModem(param);
        pam4_.reset(new Pam<2>(0.5));
    }

    virtual ::std::complex<float> Modulate(const ::std::vector<uint8_t> &bits) override {
        return ::std::complex<float>(pam4_->Encode(&bits[0]), pam4_->Encode(&bits[nbits_ / 2]));
    }
    virtual ::std::vector<uint8_t> Demodulate(const ::std::complex<float> &sym) override {
        ::std::vector<uint8_t> result;
        ::std::vector<uint8_t> result1 = pam4_->DecodeHard(sym.real());
        ::std::vector<uint8_t> result2 = pam4_->DecodeHard(sym.imag());
        for (size_t idx = 0; idx < result1.size(); ++idx) {
            result.push_back(result1[idx]);
        }
        for (size_t idx = 0; idx < result2.size(); ++idx) {
            result.push_back(result2[idx]);
        }
        return result;
    }
    virtual ::std::vector<float> DemodulateLlr(const ::std::complex<float> &sym) override {
        ::std::vector<float> result;
        ::std::vector<float> result1 = pam4_->DecodeSoftLlr(sym.real());
        ::std::vector<float> result2 = pam4_->DecodeSoftLlr(sym.imag());
        for (size_t idx = 0; idx < result1.size(); ++idx) {
            result.push_back(result1[idx]);
        }
        for (size_t idx = 0; idx < result2.size(); ++idx) {
            result.push_back(result2[idx]);
        }
        return result;
    }
    virtual float GetLlrNp() override {
        return pam4_->GetNoisePower();
    }
    virtual void SetLlrNp(float np) {
        pam4_->SetNoisePower(np);
    }
    virtual ::std::string Name() const override {
        return "16QAM";
    }

protected:
    ::std::shared_ptr<Pam<2>> pam4_;
};

} // namespace modem
} // namespace hitdsp