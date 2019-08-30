#include "ecc/reed_solomon.h"

namespace hitdsp {
namespace ecc {

ReedSolomon::ReedSolomon(int nmsg, int ngen, Gf28 gf) {
    gf_ = gf;
    nmsg_ = nmsg;
    ngen_ = ngen;
    uint8_t g = 1;
    uint8_t tmp[2];
    generator_ = Poly(&g, 0, gf_);
    for (int idx = 0; idx < ngen; ++idx) {
        tmp[0] = 1;
        tmp[1] = gf_.Pow(2, idx);
        generator_.Mul(Poly(&tmp[0], 1, gf_));
    }
    ::std::vector<uint8_t> msg(nmsg + ngen, 0);
    message_ = Poly(&msg[0], nmsg + ngen - 1, gf_);
}

void ReedSolomon::Encode(const uint8_t *input, uint8_t *output) {
    Poly tmp(message_);
    for (int idx = 0; idx < nmsg_; ++idx) {
        tmp[idx] = input[idx];
        output[idx] = input[idx];
    }
    tmp.Div(generator_);
    for (int idx = nmsg_; idx < ngen_ + nmsg_; ++idx) {
        output[idx] = tmp[idx - nmsg_];
    }
}

} // namespace ecc
} // namespace hitdsp