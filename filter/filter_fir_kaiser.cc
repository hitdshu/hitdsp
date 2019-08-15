#include "filter/filter_fir_kaiser.h"
#include "common/macro.h"
#include "common/maths.h"
#include <cassert>
#include <algorithm>

namespace hitdsp {
namespace filter {

void FilterFirKaiser::InitFilterFir(const FilterFirBaseParam &param) {
    float delta_p = 1 - ::std::pow(10, - param.rp_db / 20);
    float delta_s = ::std::pow(10, - param.as_db / 20);
    float delta = ::std::min(delta_p, delta_s);
    float delta_db = 20 * ::std::log10(1 / delta);
    float beta = EstimateBeta(delta_db);
    std::vector<float> wcs;
    std::vector<BandIndicator> ids;
    float delta_w = 1e5;
    for (int idx = 0; idx < param.bands.size(); ++idx) {
        if (param.indicators[idx] == kPass || param.indicators[idx] == kStop) {
            float wc_tmp = (param.bands[idx] + param.bands[::std::min(idx + 1, (int)param.bands.size() - 1)]) / 2.0;
            wcs.push_back(wc_tmp);
            ids.push_back(param.indicators[idx]);
        } else {
            float prev_wc = (idx == 0) ? 0 : param.bands[idx - 1];
            delta_w = ::std::min(delta_w, param.bands[idx] - prev_wc);
        }
    }
    int len = (delta_db - 8) / (2.285 * delta_w) + 1;
    len += (len % 2) ? 0 : 1;
    coeff_len_ = len;
    coeffsf_.reset(new ::std::complex<float>[coeff_len_]);
    coeffsd_.reset(new ::std::complex<double>[coeff_len_]);
    float gain_init = (int)ids[0] - (int)ids[1];
    ::std::vector<::std::complex<float>> ideal_coeff = ::hitdsp::common::ComputeIdealLowPassPulse(wcs[0], coeff_len_);
    for (int idx = 0; idx < coeff_len_; ++idx) {
        ideal_coeff[idx] *= gain_init;
    }
    for (int bidx = 1; bidx < wcs.size(); ++bidx) {
        float gain_tmp = (int)ids[bidx] - (int)(bidx == wcs.size() - 1 ? 0 : ids[bidx + 1]);
        ::std::vector<::std::complex<float>> ideal_coeff_tmp = ::hitdsp::common::ComputeIdealLowPassPulse(wcs[bidx], coeff_len_);
        for (int idx = 0; idx < coeff_len_; ++idx) {
            ideal_coeff[idx] += gain_tmp * ideal_coeff_tmp[idx];
        }
    }
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

float FilterFirKaiser::EstimateBeta(float delta_db) {
    if (delta_db < 21) {
        return 0;
    } else if (delta_db < 50) {
        return 0.5842 * ::std::pow(delta_db - 21, 0.4) + 0.07886 * (delta_db - 21);
    } else {
        return 0.1102 * (delta_db - 8.7);
    }
}

HITDSP_REGISTER_FILTER(FilterFirKaiser);

} // namespace filter
} // namespace hitdsp