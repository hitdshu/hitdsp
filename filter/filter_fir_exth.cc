#include "filter/filter_fir_exth.h"

namespace hitdsp {
namespace filter {

void FilterFirExth::InitFirCoeff(const ::std::complex<float> coeff[], int coeff_size) {
    coeffsf_.reset(new ::std::complex<float>[coeff_size]);
    coeffsd_.reset(new ::std::complex<double>[coeff_size]);
    coeff_len_ = coeff_size;
    for (int idx = 0; idx < coeff_size; ++idx) {
        coeffsf_[idx] = coeff[idx];
        coeffsd_[idx] = coeff[idx];
        data_listf_.push_back(::std::complex<float>(0, 0));
        data_listd_.push_back(::std::complex<double>(0, 0));
    }
    int block_size = 1;
    while (block_size < coeff_size) {
        block_size <<= 1;
    }
    InitBlockParam(block_size);
}

HITDSP_REGISTER_FILTER(FilterFirExth);

} // namespace filter
} // namespace hitdsp