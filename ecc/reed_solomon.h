#pragma once

#include "ecc/gf28.h"
#include "ecc/poly.h"

namespace hitdsp {
namespace ecc {

class ReedSolomon {
public:
    ReedSolomon(int nmsg, int ngen, Gf28 gf = Gf28());

    void Encode(const uint8_t *input, uint8_t *output);
    void Decode(const uint8_t *input, uint8_t *output);

    bool Check(const uint8_t *input);

protected:
    ::std::vector<uint8_t> CalcSyndromes(const uint8_t *rcw);
    ::std::vector<uint8_t> FindErrorLocator(const ::std::vector<uint8_t> &synds);
    ::std::vector<uint8_t> FindErrors(const ::std::vector<uint8_t> &err_loc);
    ::std::vector<uint8_t> FindErrataLocator(const ::std::vector<uint8_t> &err_pos);
    ::std::vector<uint8_t> FindErrorEvaluator(const ::std::vector<uint8_t> &synds, const ::std::vector<uint8_t> &err_loc, int nsym);
    void CorrectErrata(const uint8_t *input, const ::std::vector<uint8_t> &synds, const ::std::vector<uint8_t> &err_pos, uint8_t *output);

    Poly generator_;
    Poly message_;
    int nmsg_;
    int ngen_;
    Gf28 gf_;
};

} // namespace ecc
} // namespace hitdsp