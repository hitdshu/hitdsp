#pragma once

#include <cstdint>
#include <vector>

namespace hitdsp {
namespace modem {

class Interleaver {
public:
    Interleaver(int nbits, int ncarriers) {
        bpsc_ = nbits;
        bpsy_ = nbits * ncarriers;
        inter_idx_.resize(bpsy_);
        invit_idx_.resize(bpsy_);
        for (int idx = 0; idx < bpsy_; ++idx) {
            inter_idx_[idx] = Index(idx);
            invit_idx_[Index(idx)] = idx;
        }
    }
    virtual ~Interleaver() = default;

    ::std::vector<uint8_t> InterLeave(const ::std::vector<uint8_t> &data);
    ::std::vector<uint8_t> DeInterLeave(const ::std::vector<uint8_t> &data);

    Interleaver(const Interleaver &) = delete;
    Interleaver &operator=(const Interleaver &) = delete;

protected:
    uint32_t Index(uint32_t idx) const;

    uint32_t bpsc_;
    uint32_t bpsy_;
    ::std::vector<uint32_t> inter_idx_;
    ::std::vector<uint32_t> invit_idx_;
};

} // namespace modem
} // namespace hitdsp