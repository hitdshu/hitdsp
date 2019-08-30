#pragma once

#include "ecc/gf28.h"
#include "ecc/poly.h"

namespace hitdsp {
namespace ecc {

class ReedSolomon {
public:
    ReedSolomon(int nmsg, int ngen, Gf28 gf = Gf28());

    void Encode(const uint8_t *input, uint8_t *output);

protected:
    Poly generator_;
    Poly message_;
    int nmsg_;
    int ngen_;
    Gf28 gf_;
};

} // namespace ecc
} // namespace hitdsp