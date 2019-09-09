#pragma once

#include <vector>
#include "ecc/bed_base.h"

namespace hitdsp {
namespace ecc {

class Conv : public BedBase {
public:
    Conv(int nmem, int rate, const int *polys, bool soft_dec = false);

    virtual void Encode(const uint8_t *data, uint8_t *symbols, int data_bits) override;
    virtual void Decode(const uint8_t *symbols, uint8_t *data, int data_bits) override;

protected:
    int nmem_;
    int rate_;
    ::std::vector<int> polys_;
    bool soft_dec_;
};

} // namespace ecc
} // namespace hitdsp