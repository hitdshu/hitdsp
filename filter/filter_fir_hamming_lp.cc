#include "filter/filter_fir_hamming_lp.h"
#include "common/macro.h"
#include "common/maths.h"
#include <cassert>
#include <algorithm>

namespace hitdsp {
namespace filter {

void FilterFirHammingLp::InitFilterFir(const FilterFirBaseParam &param) {
    assert(param.type == kLowPass);
    assert(param.as_db <= 53);
    assert(param.rp_db >= 0.019);
    float delta_p = 1 - ::std::pow(10, - param.rp_db / 20);
    float delta_s = ::std::pow(10, - param.as_db / 20);
    float delta = ::std::min(delta_p, delta_s);
    float wc = (param.bands[0] + param.bands[1]) / 2;
    float delta_w = param.bands[1] - param.bands[0];
    int len = 6.6 * PI / delta_w;
    len += (len % 2) ? 0 : 1;
    coeff_len_ = len;
    coeffsf_.reset(new ::std::complex<float>[coeff_len_]);
    coeffsd_.reset(new ::std::complex<double>[coeff_len_]);
    ::std::vector<::std::complex<float>> ideal_coeff = ::hitdsp::common::ComputeIdealLowPassPulse(wc, coeff_len_);
    ::std::vector<float> hamming_weight = ::hitdsp::common::Hamming(coeff_len_);
    for (int idx = 0; idx < coeff_len_; ++idx) {
        coeffsf_[idx] = ideal_coeff[idx] * hamming_weight[idx];
        coeffsd_[idx] = ideal_coeff[idx] * hamming_weight[idx];
        data_listf_.push_back(::std::complex<float>(0, 0));
        data_listd_.push_back(::std::complex<double>(0, 0));
    }
    int block_size = 1;
    while (block_size < coeff_len_) {
        block_size <<= 1;
    }
    InitBlockParam(block_size);
}

HITDSP_REGISTER_FILTER(FilterFirHammingLp);

} // namespace filter
} // namespace hitdsp