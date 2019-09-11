#include "filter/filter_halfband.h"
#include "common/maths.h"
#include "common/macro.h"

namespace hitdsp {
namespace filter {

void FilterHalfband::InitFilterFir(const FilterFirBaseParam &param) {
    int tap_len = param.max_tap_len;
    tap_len = tap_len - (tap_len % 4) + 3;
    coeff_len_ = tap_len;
    coeffsf_.reset(new ::std::complex<float>[coeff_len_]);
    coeffsd_.reset(new ::std::complex<double>[coeff_len_]);
    ::std::vector<::std::complex<float>> ideal_coeff = ::hitdsp::common::ComputeIdealLowPassPulse(PI / 2.0, coeff_len_);
    ::std::vector<float> kaiser_weight = ::hitdsp::common::Kaiser(coeff_len_, 6);
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

::std::complex<float> FilterHalfband::PushAndExecute(const ::std::complex<float> &input) {
    data_listf_.pop_front();
    data_listf_.push_back(input);
    ::std::complex<float> result(0, 0);
    int fidx = coeff_len_ - 1;
    for (auto iter = data_listf_.begin(); iter != data_listf_.end(); ) {
        result += (*iter) * coeffsf_[fidx];
        ++iter;
        ++iter;
        --fidx;
        --fidx;
    }
    return result;
}

::std::complex<double> FilterHalfband::PushAndExecute(const ::std::complex<double> &input) {
    data_listd_.pop_front();
    data_listd_.push_back(input);
    ::std::complex<double> result(0, 0);
    int fidx = coeff_len_ - 1;
    for (auto iter = data_listd_.begin(); iter != data_listd_.end(); ) {
        result += (*iter) * coeffsd_[fidx];
        ++iter;
        ++iter;
        --fidx;
        --fidx;
    }
    return result;
}

HITDSP_REGISTER_FILTER(FilterHalfband);

} // namespace filter
} // namespace hitdsp