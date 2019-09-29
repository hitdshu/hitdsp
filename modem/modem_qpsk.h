#pragma once

#include "modem/modem_base.h"
#include "modem/pam.h"
#include <cassert>
#include <memory>

namespace hitdsp {
namespace modem {

class ModemQpsk : public ModemBase {
public:
    virtual void InitModem(const ModemParam &param) override {
        assert(2 == param.nbits);
        assert(4 == param.csize);
        ModemBase::InitModem(param);
        pam2_.reset(new Pam<1>(0.5));
    }

    virtual ::std::complex<float> Modulate(const ::std::vector<uint8_t> &bits) override {
        return ::std::complex<float>(pam2_->Encode(&bits[0]), pam2_->Encode(&bits[nbits_ / 2]));
    }
    virtual ::std::vector<uint8_t> Demodulate(const ::std::complex<float> &sym) override {
        ::std::vector<uint8_t> result;
        ::std::vector<uint8_t> result1 = pam2_->DecodeHard(sym.real());
        ::std::vector<uint8_t> result2 = pam2_->DecodeHard(sym.imag());
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
        ::std::vector<float> result1 = pam2_->DecodeSoftLlr(sym.real());
        ::std::vector<float> result2 = pam2_->DecodeSoftLlr(sym.imag());
        for (size_t idx = 0; idx < result1.size(); ++idx) {
            result.push_back(result1[idx]);
        }
        for (size_t idx = 0; idx < result2.size(); ++idx) {
            result.push_back(result2[idx]);
        }
        return result;
    }
    virtual float GetLlrNp() override {
        return pam2_->GetNoisePower();
    }
    virtual void SetLlrNp(float np) {
        pam2_->SetNoisePower(np);
    }
    virtual ::std::string Name() const override {
        return "QPSK";
    }

protected:
    ::std::shared_ptr<Pam<1>> pam2_;
};

} // namespace modem
} // namespace hitdsp