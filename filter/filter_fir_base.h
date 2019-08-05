#pragma once

#include "filter/filter_base.h"

namespace hitdsp {
namespace filter {

class FilterFirBase : FilterBase {
public:
    FilterFirBase() = default;
    virtual ~FilterFirBase() = default;

    virtual ::std::complex<float> PushAndExecute(const ::std::complex<float> &input) const override = 0;
    virtual ::std::complex<double> PushAndExecute(const ::std::complex<double> &input) const override = 0;
    virtual void BlockExecute(const ::std::vector<::std::complex<float>> inputs[], ::std::vector<::std::complex<float>> outputs[]) const override = 0;

    virtual void SetBlockParam(const int &block_size) { block_size_ = block_size; }

protected:
    int block_size_;
};

} // namespace hitdsp
} // namespace hitdsp