#pragma once

#include "filter/filter_fir_exth.h"

namespace hitdsp {
namespace filter {

class FilterHalfband : public FilterFirExth {
public:
    virtual void InitFilterFir(const FilterFirBaseParam &param) override final;
    virtual ::std::complex<float> PushAndExecute(const ::std::complex<float> &input) override final;
    virtual ::std::complex<double> PushAndExecute(const ::std::complex<double> &input) override final;
};

} // namespace filter
} // namespace hitdsp