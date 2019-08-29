#include "ecc/poly.h"
#include <algorithm>

namespace hitdsp {
namespace ecc {

void Poly::MultScalar(uint8_t x) {
    for (int idx = 0; idx < poly_.size(); ++idx) {
        poly_[idx] = gf_.Mul(poly_[idx], x);
    }
}

void Poly::Add(const Poly &x) {
    ::std::vector<uint8_t> poly_tmp(::std::max(poly_.size(), x.poly_.size()), 0);
    for (int idx = 0; idx < poly_.size(); ++idx) {
        poly_tmp[idx + poly_tmp.size() - poly_.size()] = poly_[idx];
    }
    for (int idx = 0; idx < x.poly_.size(); ++idx) {
        poly_tmp[idx + poly_tmp.size() - x.poly_.size()] ^= x.poly_[idx];
    }
    poly_ = poly_tmp;
}

void Poly::Mul(const Poly &x) {
    ::std::vector<uint8_t> poly_tmp(poly_.size() + x.poly_.size() - 1, 0);
    for (int idx1 = 0; idx1 < poly_.size(); ++idx1) {
        for (int idx2 = 0; idx2 < x.poly_.size(); ++idx2) {
            poly_tmp[idx1 + idx2] ^= gf_.Mul(poly_[idx1], x.poly_[idx2]);
        }
    }
    poly_ = poly_tmp;
}

Poly Poly::Div(const Poly &x) {
    ::std::vector<uint8_t> poly_tmp;
    poly_tmp = poly_;
    if (x.poly_.size() > poly_.size()) {
        return Poly();
    }
    for (int idx1 = 0; idx1 < poly_.size() - x.poly_.size() + 1; ++idx1) {
        uint8_t coeff = poly_tmp[idx1];
        if (0 != coeff) {
            for (int idx2 = 1; idx2 < x.poly_.size(); ++idx2) {
                if (0 != x.poly_[idx2]) {
                    poly_tmp[idx1 + idx2] ^= gf_.Mul(x.poly_[idx2], coeff);
                }
            }
        }
    }
    Poly quot(&poly_tmp[0], poly_.size() - x.poly_.size());
    poly_ = ::std::vector<uint8_t>(poly_tmp.begin() + poly_.size() - x.poly_.size() + 1, poly_tmp.end());
    return quot;
}

uint8_t Poly::EvaluateAt(uint8_t x) {
    uint8_t result = poly_[0];
    for (int idx = 1; idx < poly_.size(); ++idx) {
        result = gf_.Mul(result, x) ^ poly_[idx];
    }
    return result;
}

} // namesapce ecc
} // namespace hitdsp