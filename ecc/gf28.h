#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

namespace hitdsp {
namespace ecc {

class Gf28 {
public:
    Gf28(uint16_t prime_poly = 0x011d) {
        assert(9 == BitLength(prime_poly));
        prime_poly_ = prime_poly;
        InitExpLogTable();
    }

    uint8_t Add(uint8_t x, uint8_t y) const {
        return x ^ y;
    }
    uint8_t Sub(uint8_t x, uint8_t y) const {
        return x ^ y;
    }
    uint8_t Mul(uint8_t x, uint8_t y) const {
        if (0 == x || 0 == y) {
            return 0;
        }
        return exp_[log_[x] + log_[y]];
    }
    uint8_t Div(uint8_t x, uint8_t y) const {
        assert(0 != y);
        if (0 == x) {
            return 0;
        }
        return exp_[(log_[x] + 255 - log_[y]) % 255];
    }
    uint8_t Pow(uint8_t x, uint8_t y) const {
        return exp_[(log_[x] * y) % 255];
    }
    uint8_t Inv(uint8_t x) const {
        return exp_[255 - log_[x]];
    }
    uint8_t MulNoLUT(uint8_t x, uint8_t y) const;

protected:
    template <typename ValueType>
    int BitLength(ValueType val) const;
    void InitExpLogTable();

    uint16_t prime_poly_;
    ::std::vector<uint8_t> exp_;
    ::std::vector<int> log_;
};

} // namespace ecc
} // namespace hitdsp