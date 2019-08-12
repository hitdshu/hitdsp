#pragma once

#include "filter/filter_fir_exth.h"

namespace hitdsp {
namespace filter {

class FilterFirHammingLp : public FilterFirExth {
public:
    virtual void InitFilterFir(const FilterFirBaseParam &param) override;
};

} // namespace filter
} // namespace hitdsp