#pragma once

#include <cstdint>
#include "common/register.h"

namespace hitdsp {
namespace ecc {

class BedBase {
public:
    BedBase() = default;
    virtual ~BedBase() = default;

    virtual void Encode(const uint8_t *data, uint8_t *symbols, int data_bits) = 0;
    virtual void Decode(const uint8_t *symbols, uint8_t *data, int data_bits) = 0;

    BedBase(const BedBase &) = delete;
    BedBase &operator=(const BedBase &) = delete;
};

HITDSP_REGISTER_REGISTER(BedBase);
#define HITDSP_REGISTER_BED(name) \
    HITDSP_REGISTER_CLASS(BedBase, name)

} // namespace ecc
} // namespace hitdsp