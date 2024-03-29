#pragma once

#include "filter/filter_fir_base.h"

namespace hitdsp {
namespace filter {

class FilterFirExth : public FilterFirBase {
public:
    virtual void InitFilterFir(const FilterFirBaseParam &param) override {}
    virtual void InitFirCoeff(const ::std::complex<float> coeff[], int coeff_size) override;
};

} // namespace filter
} // namespace hitdsp