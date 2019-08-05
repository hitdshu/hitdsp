#include "fft/fft_radix4.h"

namespace hitdsp {
namespace fft {

void FftRadix4::Init(int fft_size, bool forward_flag) {
    FftBaseImpl::Init(fft_size, forward_flag);
    bn_ = -1;
    while (fft_size) {
        fft_size = fft_size >> 1;
        ++bn_;
    }
    r4n_ = bn_ >> 1;
    for (int idx = 0; idx < fft_size_; ++idx) {
        order_.push_back(ReverseRadix4(idx, bn_));
    }
}


void FftRadix4::Transform(const ::std::complex<float> input[], ::std::complex<float> output[]) const {
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] = input[order_[idx]];
    }
    for (int bidx = 0; bidx < r4n_; ++bidx) {
        int bstride = 1 << (bidx * 2);
        int gn = 1 << ((r4n_ - 1 - bidx) * 2);
        for (int gidx = 0; gidx < gn; ++gidx) {
            int gstart_idx = gidx * bstride * 4;
            for (int iidx = 0; iidx < bstride; ++iidx) {
                int r4idx0 = gstart_idx + iidx;
                int r4idx1 = gstart_idx + iidx + bstride;
                int r4idx2 = gstart_idx + iidx + bstride * 2;
                int r4idx3 = gstart_idx + iidx + bstride * 3;
                output[r4idx1] *= twiddlef_[gn * iidx];
                output[r4idx2] *= twiddlef_[gn * iidx * 2];
                output[r4idx3] *= twiddlef_[gn * iidx * 3];
                ::std::complex<float> x0px2 = output[r4idx0] + output[r4idx2];
                ::std::complex<float> x0mx2 = output[r4idx0] - output[r4idx2];
                ::std::complex<float> x1px3 = output[r4idx1] + output[r4idx3];
                ::std::complex<float> x1mx3 = output[r4idx1] - output[r4idx3];
                ::std::complex<float> jx1mx3 = ::std::complex<float>(-x1mx3.imag(), x1mx3.real());
                output[r4idx0] = x0px2 + x1px3;
                output[r4idx1] = x0mx2 - jx1mx3;
                output[r4idx2] = x0px2 - x1px3;
                output[r4idx3] = x0mx2 + jx1mx3;
            }
        }
    }
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] /= ::std::sqrt(fft_size_);
    }
}

void FftRadix4::Transform(const ::std::complex<double> input[], ::std::complex<double> output[]) const {
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] = input[order_[idx]];
    }
    for (int bidx = 0; bidx < r4n_; ++bidx) {
        int bstride = 1 << (bidx * 2);
        int gn = 1 << ((r4n_ - 1 - bidx) * 2);
        for (int gidx = 0; gidx < gn; ++gidx) {
            int gstart_idx = gidx * bstride * 4;
            for (int iidx = 0; iidx < bstride; ++iidx) {
                int r4idx0 = gstart_idx + iidx;
                int r4idx1 = gstart_idx + iidx + bstride;
                int r4idx2 = gstart_idx + iidx + bstride * 2;
                int r4idx3 = gstart_idx + iidx + bstride * 3;
                output[r4idx1] *= twiddled_[gn * iidx];
                output[r4idx2] *= twiddled_[gn * iidx * 2];
                output[r4idx3] *= twiddled_[gn * iidx * 3];
                ::std::complex<double> x0px2 = output[r4idx0] + output[r4idx2];
                ::std::complex<double> x0mx2 = output[r4idx0] - output[r4idx2];
                ::std::complex<double> x1px3 = output[r4idx1] + output[r4idx3];
                ::std::complex<double> x1mx3 = output[r4idx1] - output[r4idx3];
                ::std::complex<double> jx1mx3 = ::std::complex<double>(-x1mx3.imag(), x1mx3.real());
                output[r4idx0] = x0px2 + x1px3;
                output[r4idx1] = x0mx2 - jx1mx3;
                output[r4idx2] = x0px2 - x1px3;
                output[r4idx3] = x0mx2 + jx1mx3;
            }
        }
    }
    for (int idx = 0; idx < fft_size_; ++idx) {
        output[idx] /= ::std::sqrt(fft_size_);
    }
}

uint32_t FftRadix4::ReverseRadix4(uint32_t idx, int bn) const {
    uint32_t result = idx;
    result = ((result & 0x33333333) << 2) | ((result & 0xcccccccc) >> 2);
    result = ((result & 0x0f0f0f0f) << 4) | ((result & 0xf0f0f0f0) >> 4);
    result = ((result & 0x00ff00ff) << 8) | ((result & 0xff00ff00) >> 8);
    result = ((result & 0x0000ffff) << 16) | ((result & 0xffff0000) >> 16);
    result = result >> (32 - bn);
    return result;
}

HITDSP_REGISTER_FFT(FftRadix4);

} // namespace fft
} // namespace hitdsp