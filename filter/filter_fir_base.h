#pragma once

#include "filter/filter_base.h"
#include "fft/fft_radix2.h"
#include <memory>
#include <list>
#include <vector>
#include <utility>

namespace hitdsp {
namespace filter {

struct FilterFirBaseParam {
    int tap_len_;
    ::std::vector<::std::pair<float, float>> bands_;
    ::std::vector<float> des_response_;
};

class FilterFirBase : FilterBase {
public:
    FilterFirBase() = default;
    virtual ~FilterFirBase() = default;

    virtual void InitFilterFir(const FilterFirBase &param) = 0;

    virtual ::std::complex<float> PushAndExecute(const ::std::complex<float> &input) override;
    virtual ::std::complex<double> PushAndExecute(const ::std::complex<double> &input) override;
    virtual void BlockExecute(const ::std::complex<float> inputs[], ::std::complex<float> outputs[]) final;
    virtual void BlockExecute(const ::std::complex<double> inputs[], ::std::complex<double> outputs[]) final;

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