#pragma once

#include "filter/filter_fir_exth.h"

namespace hitdsp {
namespace filter {

class FilterFirKaiser : public FilterFirExth {
public:
    virtual void InitFilterFir(const FilterFirBaseParam &param) override;
protected:
    float EstimateBeta(float delta_db);
};

} // namespace filter
} // namespace hitdsp