#include "filter/filter_fir_kaiser_lp.h"
#include "common/macro.h"
#include "common/maths.h"
#include <cassert>
#include <algorithm>

namespace hitdsp {
namespace filter {

void FilterFirKaiserLp::InitFilterFir(const FilterFirBaseParam &param) {
    assert(param.type == kLowPass);
    float delta_p = 1 - ::std::pow(10, - param.rp_db / 20);
    float delta_s = ::std::pow(10, - param.as_db / 20);
    float delta = ::std::min(delta_p, delta_s);
    float delta_db = 20 * ::std::log10(1 / delta);
    float beta = EstimateBeta(delta_db);
    float wc = (param.bands[0] + param.bands[1]) / 2;
    float delta_w = param.bands[1] - param.bands[0];
    int len = (delta_db - 8) / (2.285 * delta_w) + 1;
    len += (len % 2) ? 1 : 0;
    coeff_len_ = len;
    coeffsf_.reset(new ::std::complex<float>[coeff_len_]);
    coeffsd_.reset(new ::std::complex<double>[coeff_len_]);
    ::std::vector<::std::complex<float>> ideal_coeff = ::hitdsp::common::ComputeIdealLowPassPulse(wc, coeff_len_);
    ::std::vector<float> kaiser_weight = ::hitdsp::common::Kaiser(coeff_len_, beta);
    for (int idx = 0; idx < coeff_len_; ++idx) {
        coeffsf_[idx] = ideal_coeff[idx] * kaiser_weight[idx];
        coeffsd_[idx] = ideal_coeff[idx] * kaiser_weight[idx];
        data_listf_.push_back(::std::complex<float>(0, 0));
        data_listd_.push_back(::std::complex<double>(0, 0));
    }
    int block_size = 1;
    while (block_size < coeff_len_) {
        block_size <<= 1;
    }
    InitBlockParam(block_size);
}

float FilterFirKaiserLp::EstimateBeta(float delta_db) {
    if (delta_db < 21) {
        return 0;
    } else if (delta_db < 50) {
        return 0.5842 * ::std::pow(delta_db - 21, 0.4) + 0.07886 * (delta_db - 21);
    } else {
        0.1102 * (delta_db - 8.7);
    }
}

} // namespace filter
} // namespace hitdsp