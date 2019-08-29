#include "ecc/gf28.h"

namespace hitdsp {
namespace ecc {

uint8_t Gf28::MulNoLUT(uint8_t x, uint8_t y) const {
    uint16_t x_itn = x;
    uint16_t y_itn = y;
    uint16_t z = 0;
    int idx = 0;
    while (y_itn >> idx) {
        if (y_itn & (1 << idx)) {
            z ^= x_itn << idx;
        }
        ++idx;
    }
    int zbn = BitLength(z);
    int pbn = BitLength(prime_poly_);
    if (zbn < pbn) {
        return z;
    }
    for (idx = zbn - pbn; idx >= 0; --idx) {
        if (z & (1 << (pbn + idx - 1))) {
            z ^= prime_poly_ << idx;
        }
    }
    return (uint8_t)z;
}

template <typename ValueType>
int Gf28::BitLength(ValueType val) const {
    int nbits = 0;
    while (val >> nbits) {
        ++nbits;
    }
    return nbits;
}

void Gf28::InitExpLogTable() {
    exp_ = ::std::vector<uint8_t>(512, 0);
    log_ = ::std::vector<int>(256, 0);
    uint8_t tmp = 1;
    for (int idx = 0; idx < 255; ++idx) {
        exp_[idx] = tmp;
        log_[tmp] = idx;
        tmp = MulNoLUT(tmp, 2);
    }
    for (int idx = 255; idx < 512; ++idx) {
        exp_[idx] = exp_[idx - 255];
    }
}


} // namespace ecc
} // namespace hitdsp