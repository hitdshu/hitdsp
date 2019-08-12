#include "filter/filter_fir_base.h"

namespace hitdsp {
namespace filter {

::std::complex<float> FilterFirBase::PushAndExecute(const ::std::complex<float> &input) {
    data_listf_.pop_front();
    data_listf_.push_back(input);
    ::std::complex<float> result(0, 0);
    int fidx = coeff_len_ - 1;
    for (auto iter = data_listf_.begin(); iter != data_listf_.end(); ++iter) {
        result += (*iter) * coeffsf_[fidx--];
    }
    return result;
}

::std::complex<double> FilterFirBase::PushAndExecute(const ::std::complex<double> &input) {
    data_listd_.pop_front();
    data_listd_.push_back(input);
    ::std::complex<double> result(0, 0);
    int fidx = coeff_len_ - 1;
    for (auto iter = data_listd_.begin(); iter != data_listd_.end(); ++iter) {
        result += (*iter) * coeffsd_[fidx--];
    }
    return result;
}

void FilterFirBase::BlockExecute(const ::std::complex<float> inputs[], ::std::complex<float> outputs[]) {
    for (int idx = 0; idx < 2 * block_size_; ++idx) {
        if (idx < block_size_) {
            time_buff_[idx] = inputs[idx];
        } else {
            time_buff_[idx] = 0;
        }
    }
    fft_exe_->Transform(time_buff_.get(), freq_buff_.get());
    for (int idx = 0; idx < 2 * block_size_; ++idx) {
        freq_buff_.get()[idx] *= coeffsf_freq_.get()[idx];
    }
    ifft_exe_->Transform(freq_buff_.get(), time_buff_.get());
    for (int idx = 0; idx < block_size_; ++idx) {
        outputs[idx] = time_buff_.get()[idx] + overlapf_.get()[idx];
    }
    for (int idx = 0; idx < block_size_; ++idx) {
        overlapf_[idx] = time_buff_.get()[idx + block_size_];
    }
}

void FilterFirBase::BlockExecute(const ::std::complex<double> inputs[], ::std::complex<double> outputs[]) {
    for (int idx = 0; idx < 2 * block_size_; ++idx) {
        if (idx < block_size_) {
            time_bufd_[idx] = inputs[idx];
        } else {
            time_bufd_[idx] = 0;
        }
    }
    fft_exe_->Transform(time_bufd_.get(), freq_bufd_.get());
    for (int idx = 0; idx < 2 * block_size_; ++idx) {
        freq_bufd_.get()[idx] *= coeffsd_freq_.get()[idx];
    }
    ifft_exe_->Transform(freq_bufd_.get(), time_bufd_.get());
    for (int idx = 0; idx < block_size_; ++idx) {
        outputs[idx] = time_bufd_.get()[idx] + overlapd_.get()[idx];
    }
    for (int idx = 0; idx < block_size_; ++idx) {
        overlapf_[idx] = time_bufd_.get()[idx + block_size_];
    }
}

void FilterFirBase::InitBlockParam(const int &block_size) {
    block_size_ = block_size;
    fft_exe_.reset(new ::hitdsp::fft::FftRadix2());
    fft_exe_->Init(2 * block_size_, true);
    ifft_exe_.reset(new ::hitdsp::fft::FftRadix2());
    ifft_exe_->Init(2 * block_size_, false);
    coeffsf_freq_.reset(new ::std::complex<float>[2 * block_size_]);
    coeffsd_freq_.reset(new ::std::complex<double>[2 * block_size_]);
    overlapf_.reset(new ::std::complex<float>[block_size_]);
    overlapd_.reset(new ::std::complex<double>[block_size_]);
    time_buff_.reset(new ::std::complex<float>[2 * block_size_]);
    time_bufd_.reset(new ::std::complex<double>[2 * block_size_]);
    freq_buff_.reset(new ::std::complex<float>[2 * block_size_]);
    freq_bufd_.reset(new ::std::complex<double>[2 * block_size_]);
    for (int idx = 0; idx < 2 * block_size_; ++idx) {
        if (idx < coeff_len_) {
            time_buff_[idx] = coeffsf_[idx];
            time_bufd_[idx] = coeffsd_[idx];
        } else {
            time_buff_[idx] = 0;
            time_bufd_[idx] = 0;
        }
    }
    fft_exe_->Transform(time_buff_.get(), coeffsf_freq_.get());
    fft_exe_->Transform(time_bufd_.get(), coeffsd_freq_.get());
    for (int idx = 0; idx < block_size_; ++idx) {
        overlapf_[idx] = 0;
        overlapd_[idx] = 0;
    }
}

} // namespace filter
} // namespace hitdsp