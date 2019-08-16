#include "modem/interleaver.h"
#include <cassert>
#include <algorithm>

namespace hitdsp {
namespace modem {

::std::vector<uint8_t> Interleaver::InterLeave(const ::std::vector<uint8_t> &data) {
    ::std::vector<uint8_t> result(data.size());
    for (int idx1 = 0; idx1 < data.size(); idx1 += inter_idx_.size()) {
        for (int idx2 = 0; idx2 < inter_idx_.size(); ++idx2) {
            result[idx1 + inter_idx_[idx2]] = data[idx1 + idx2];
        }
    }
    return result;
}

::std::vector<uint8_t> Interleaver::DeInterLeave(const ::std::vector<uint8_t> &data) {
    ::std::vector<uint8_t> result(data.size());
    for (int idx1 = 0; idx1 < data.size(); idx1 += invit_idx_.size()) {
        for (int idx2 = 0; idx2 < invit_idx_.size(); ++idx2) {
            result[idx1 + invit_idx_[idx2]] = data[idx1 + idx2];
        }
    }
    return result;
}

uint32_t Interleaver::Index(uint32_t idx) const {
    assert(idx < bpsy_);
    uint32_t s = ::std::max(bpsc_ / 2, (uint32_t)1);
    uint32_t i = (bpsy_ / 16) * (idx % 16) + (idx / 16);
    uint32_t j = s * (i / s) + ((i + bpsy_ - (16 * i / bpsy_)) % s);
    assert(j < bpsy_);
    return j;
}

} // namespace modem
} // namespace hitdsp