#pragma once

#include "ecc/gf28.h"
#include <iostream>

namespace hitdsp {
namespace ecc {

class Poly {
public:
    Poly() = default;
    Poly(const uint8_t *poly, int order, Gf28 gf = Gf28()) {
        gf_ = gf;
        poly_.resize(order + 1);
        for (int idx = 0; idx < order + 1; ++idx) {
            poly_[idx] = poly[idx];
        }
    }

    uint8_t GetCoeff(int idx) const {
        if (idx < poly_.size()) {
            return poly_[idx];
        } else {
            return 0;
        }
    }

    void MultScalar(uint8_t x);
    uint8_t EvaluateAt(uint8_t x);

    void Add(const Poly &x);
    void Mul(const Poly &x);
    Poly Div(const Poly &x);

    void Print() const {
        ::std::cout << "[";
        for (int idx = 0; idx < poly_.size(); ++idx) {
            if (0 != idx) {
                ::std::cout << " ";
            }
            ::std::cout << ::std::hex << (int)poly_[idx];
        }
        ::std::cout << "]" << ::std::endl;
    }

protected:
    ::std::vector<uint8_t> poly_; // Descending order
    Gf28 gf_;
};

} // namespace ecc
} // namespace hitdsp