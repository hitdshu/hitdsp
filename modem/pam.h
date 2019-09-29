#pragma once

#include <vector>
#include <cstdint>
#include <cmath>
#include <map>

namespace hitdsp {
namespace modem {

template <int nbits>
class Pam {
public:
    Pam(float avg_eg = 1.0) {
        biase_ = (1 << nbits) - 1;
        scale_ = ::std::sqrt(3 * avg_eg / ((1 << (nbits * 2)) - 1));
        avg_eg_ = avg_eg;
        for (uint64_t idx = 0; idx < (1 << nbits); ++idx) {
            ::std::vector<uint8_t> tmp_bits = Symbol2Bits(idx);
            float cons_pt = Encode(&tmp_bits[0]);
            sym2cons_[idx] = cons_pt;
        }
        has_np_ = false;
    }

    void SetNoisePower(float np) {
        has_np_ = true;
        np_ = np;
    }
    float GetNoisePower() const {
        if (has_np_) {
            return np_;
        } else {
            return avg_eg_ / 4;
        }
    }

    float Encode(const uint8_t *bits) const {
        int pt = 0;
        uint8_t pre_bit = 0;
        for (int idx = 0; idx < nbits; ++idx) {
            uint8_t cur_bit = bits[idx];
            uint8_t ori_bit = cur_bit ^ pre_bit;
            pre_bit = ori_bit;
            pt = (pt << 1) + ori_bit;
        }
        pt = (pt << 1) - biase_;
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
    ::std::vector<float> DecodeSoftLlr(float sym) {
        ::std::vector<float> result(nbits, 0);
        ::std::vector<float> p0(nbits, 0);
        ::std::vector<float> p1(nbits, 0);
        float var = avg_eg_ / 4;
        if (has_np_) {
            var = np_;
        }
        for (uint64_t idx = 0; idx < (1 << nbits); ++idx) {
            float p_sym = ::std::exp(-::std::pow(::std::abs(sym - sym2cons_[idx]), 2) / 2 / var);
            ::std::vector<uint8_t> tmp_bits = Symbol2Bits(idx);
            for (int bidx = 0; bidx < nbits; ++bidx) {
                if (tmp_bits[bidx]) {
                    p1[bidx] += p_sym;
                } else {
                    p0[bidx] += p_sym;
                }
            }
        }
        for (int idx = 0; idx < nbits; ++idx) {
            result[idx] = ::std::log(p0[idx] / p1[idx]);
        }
        return result;
    }

protected:
    uint64_t Bits2Symbol(const uint8_t *bits) const;
    ::std::vector<uint8_t> Symbol2Bits(uint64_t symb) const;

    bool has_np_;
    float np_;
    int biase_;
    float scale_;
    float avg_eg_;
    ::std::map<uint64_t, float> sym2cons_;
};

template <int nbits>
uint64_t Pam<nbits>::Bits2Symbol(const uint8_t *bits) const {
    uint64_t result = 0;
    for (int idx = 0; idx < nbits; ++idx) {
        result = result << 1 + bits[idx];
    }
    return result;
}

template <int nbits>
::std::vector<uint8_t> Pam<nbits>::Symbol2Bits(uint64_t symb) const {
    ::std::vector<uint8_t> result;
    result.reserve(nbits);
    for (int idx = 0; idx < nbits; ++idx) {
        result[nbits - 1 - idx] = symb % 2;
        symb = symb >> 1;
    }
    return result;
}

} // namespace modem
} // namespace hitdsp