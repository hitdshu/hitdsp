#pragma once

#include "filter/filter_base.h"
#include "fft/fft_radix2.h"
#include <memory>
#include <list>
#include <vector>
#include <utility>

namespace hitdsp {
namespace filter {

enum FilterType {
    kLowPass = 0,
    kBandPass,
    kHighPass,
    kBandStop,
    kArbitrary,
    kFilterTypeNum
};

enum BandIndicator {
    kStop = 0,
    kPass,
    kTransition,
    kIndicatorNum
};

struct FilterFirBaseParam {
    FilterType type;
    int max_tap_len;
    int block_size;
    ::std::vector<float> bands;
    ::std::vector<BandIndicator> indicators;
    float rp_db;
    float as_db;

    FilterFirBaseParam() {
        type = kLowPass;
        max_tap_len = 100;
        block_size = 64;
        rp_db = 0.25;
        as_db = 50;
    }
};

class FilterFirBase : FilterBase {
public:
    FilterFirBase() = default;
    virtual ~FilterFirBase() = default;

    virtual void InitFilterFir(const FilterFirBaseParam &param) = 0;
    virtual void InitFirCoeff(const ::std::complex<float> coeff[], int coeff_size) = 0;

    virtual ::std::complex<float> PushAndExecute(const ::std::complex<float> &input) override;
    virtual ::std::complex<double> PushAndExecute(const ::std::complex<double> &input) override;
    virtual void BlockExecute(const ::std::complex<float> inputs[], ::std::complex<float> outputs[]) final;
    virtual void BlockExecute(const ::std::complex<double> inputs[], ::std::complex<double> outputs[]) final;

    ::std::vector<::std::complex<float>> GetCoefff() const {
        ::std::vector<::std::complex<float>> result;
        for (int idx = 0; idx < coeff_len_; ++idx) {
            result.push_back(coeffsf_[idx]);
        }
        return result;
    }
    ::std::vector<::std::complex<double>> GetCoeffd() const {
        ::std::vector<::std::complex<double>> result;
        for (int idx = 0; idx < coeff_len_; ++idx) {
            result.push_back(coeffsd_[idx]);
        }
        return result;
    }

protected:
    void InitBlockParam(const int &block_size);

    int coeff_len_;
    ::std::list<::std::complex<float>> data_listf_;
    ::std::list<::std::complex<double>> data_listd_;
    ::std::unique_ptr<::std::complex<float>[]> coeffsf_;
    ::std::unique_ptr<::std::complex<double>[]> coeffsd_;
    int block_size_;
    ::std::unique_ptr<::hitdsp::fft::FftRadix2> fft_exe_;
    ::std::unique_ptr<::hitdsp::fft::FftRadix2> ifft_exe_;
    ::std::unique_ptr<::std::complex<float>[]> coeffsf_freq_;
    ::std::unique_ptr<::std::complex<double>[]> coeffsd_freq_;
    ::std::unique_ptr<::std::complex<float>[]> overlapf_;
    ::std::unique_ptr<::std::complex<double>[]> overlapd_;
    ::std::unique_ptr<::std::complex<float>[]> time_buff_;
    ::std::unique_ptr<::std::complex<double>[]> time_bufd_;
    ::std::unique_ptr<::std::complex<float>[]> freq_buff_;
    ::std::unique_ptr<::std::complex<double>[]> freq_bufd_;
};

} // namespace hitdsp
} // namespace hitdsp