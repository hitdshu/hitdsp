#pragma once

#include "modem/modem_base.h"
#include "modem/pam.h"
#include <cassert>
#include <memory>

namespace hitdsp {
namespace modem {

class ModemBpsk : public ModemBase {
public:
    virtual void InitModem(const ModemParam &param) override {
        assert(1 == param.nbits);
        assert(2 == param.csize);
        ModemBase::InitModem(param);
        pam2_.reset(new Pam<1>);
    }

    virtual ::std::complex<float> Modulate(const ::std::vector<uint8_t> &bits) override {
        return ::std::complex<float>(pam2_->Encode(&bits[0]), 0);
    }
    virtual ::std::vector<uint8_t> Demodulate(const ::std::complex<float> &sym) override {
        return pam2_->DecodeHard(sym.real());
    }
    virtual ::std::string Name() const override {
        return "BPSK";
    }

protected:
    ::std::shared_ptr<Pam<1>> pam2_;
};

} // namespace modem
} // namespace hitdsp