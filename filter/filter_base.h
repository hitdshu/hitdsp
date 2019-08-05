#pragma once

#include <complex>
#include <vector>
#include "common/register.h"

namespace hitdsp {
namespace filter {

class FilterBase {
public:
    FilterBase() = default;
    virtual ~FilterBase() = default;

    virtual ::std::complex<float> PushAndExecute(const ::std::complex<float> &input) = 0;
    virtual ::std::complex<double> PushAndExecute(const ::std::complex<double> &input) = 0;

    FilterBase(const FilterBase &) = delete;
    FilterBase &operator=(const FilterBase &) = delete;
};

HITDSP_REGISTER_REGISTER(FilterBase);
#define HITDSP_REGISTER_FILTER(name) \
    HITDSP_REGISTER_CLASS(FilterBase, name)

} // namespace hitdsp
} // namespace hitdsp