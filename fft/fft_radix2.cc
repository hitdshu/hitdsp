#include "fft/fft_radix2.h"

namespace hitdsp {
namespace fft {

void FftRadix2::Init(int fft_size, bool forward_flag) {
    FftBaseImpl::Init(fft_size, forward_flag);
    bn_ = -1;
    while (fft_size) {
        fft_size = fft_size >> 1;
        ++bn_;
    }
    for (int idx = 0; idx < fft_size_; ++idx) {
        reverse_order_.push_back(ReverseBit(idx, bn_));
    }
}


void FftRadix2::Transform(const ::std::complex<float> input[], ::std::complex<float> output[]) const {
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] = input[reverse_order_[idx]];
    }
    for (int bidx = 0; bidx < bn_; ++bidx) {
        int bstride = 1 << bidx;
        int gn = 1 << (bn_ - 1 - bidx);
        for (int gidx = 0; gidx < gn; ++gidx) {
            int gstart_idx = gidx * bstride * 2;
            for (int iidx = 0; iidx < bstride; ++iidx) {
                ::std::complex<float> tmp = output[gstart_idx + bstride + iidx] * twiddlef_[gn * iidx];
                output[gstart_idx + bstride + iidx] = output[gstart_idx + iidx] - tmp;
                output[gstart_idx + iidx] += tmp;
            }
        }
    }
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] /= ::std::sqrt(fft_size_);
    }
}

void FftRadix2::Transform(const ::std::complex<double> input[], ::std::complex<double> output[]) const {
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] = input[reverse_order_[idx]];
    }
    for (int bidx = 0; bidx < bn_; ++bidx) {
        int bstride = 1 << bidx;
        int gn = 1 << (bn_ - 1 - bidx);
        for (int gidx = 0; gidx < gn; ++gidx) {
            int gstart_idx = gidx * bstride * 2;
            for (int iidx = 0; iidx < bstride; ++iidx) {
                ::std::complex<double> tmp = output[gstart_idx + bstride + iidx] * twiddled_[gn * iidx];
                output[gstart_idx + bstride + iidx] = output[gstart_idx + iidx] - tmp;
                output[gstart_idx + iidx] += tmp;
            }
        }
    }
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] /= ::std::sqrt(fft_size_);
    }
}

uint32_t FftRadix2::ReverseBit(uint32_t idx, int bn) const {
    uint32_t result = idx;
    result = ((result & 0x55555555) << 1) | ((result & 0xaaaaaaaa) >> 1);
    result = ((result & 0x33333333) << 2) | ((result & 0xcccccccc) >> 2);
    result = ((result & 0x0f0f0f0f) << 4) | ((result & 0xf0f0f0f0) >> 4);
    result = ((result & 0x00ff00ff) << 8) | ((result & 0xff00ff00) >> 8);
    result = ((result & 0x0000ffff) << 16) | ((result & 0xffff0000) >> 16);
    result = result >> (32 - bn);
    return result;
}

HITDSP_REGISTER_FFT(FftRadix2);

} // namespace fft
} // namespace hitdsp