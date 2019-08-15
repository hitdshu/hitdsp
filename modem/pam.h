#pragma once

#include <vector>
#include <cstdint>
#include <cmath>

namespace hitdsp {
namespace modem {

template <int nbits>
class Pam {
public:
    Pam(float avg_eg = 1.0) {
        biase_ = (1 << nbits) - 1;
        scale_ = ::std::sqrt(3 * avg_eg / ((1 << (nbits + 1)) - 1));
    }

    float Encode(const uint8_t *bits) const {
        int pt = 0;
        uint8_t pre_bit = 0;
        for (int idx = 0; idx < nbits; ++idx) {
            uint8_t cur_bit = bits[idx];
            uint8_t ori_bit = cur_bit ^ pre_bit;
            pre_bit = ori_bit;
            pt = pt << 1 + ori_bit;
        }
        pt = pt << 1 - biase_;
        return (float)pt * scale_;
    }
    ::std::vector<uint8_t> DecodeHard(float sym) const {
        ::std::vector<uint8_t> result;
        result.reserve(nbits);
        sym = (sym / scale_ + biase_) / 2.0;
        int sym_int = std::round(sym);
        if (sym_int < 0) {
            sym_int = 0;
        } else if (sym_int > ((1 << nbits) - 1)) {
            sym_int = (1 << nbits) - 1;
        }
        uint8_t pre_bit = 0;
        for (int idx = 0; idx < nbits; ++idx) {
            uint8_t cur_bit = (sym_int & (1 << (nbits - 1 - idx))) ? 1 : 0;
            uint8_t gray_bit = pre_bit ^ cur_bit;
            pre_bit = cur_bit;
            result.push_back(gray_bit);
        }
        return result;
    }

protected:
    int biase_;
    float scale_;
};

} // namespace modem
} // namespace hitdsp