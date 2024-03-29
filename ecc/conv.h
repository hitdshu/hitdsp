#pragma once

#include <vector>
#include "ecc/bed_base.h"

namespace hitdsp {
namespace ecc {

class Conv : public BedBase {
public:
    void Init(int nmem, int rate, const int *polys);

    virtual void Encode(const uint8_t *data, uint8_t *symbols, int data_bits) override;
    virtual void Decode(const uint8_t *symbols, uint8_t *data, int data_bits) override;
    virtual void Decode(const float *symbols, uint8_t *data, int data_bits) override;

protected:
    int nmem_;
    int rate_;
    ::std::vector<int> polys_;
};

} // namespace ecc
} // namespace hitdsp