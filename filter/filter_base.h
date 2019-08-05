#pragma once

#include <complex>

namespace hitdsp {
namespace filter {

class FilterBase {
public:
    FilterBase() = default;
    virtual ~FilterBase() = default;

    ::std::complex<float> PushAndExecute(const ::std::complex<float> &input);
    ::std::complex<double> PushAndExecute(const ::std::complex<double> &input);

    FilterBase(const FilterBase &) = delete;
    FilterBase &operator=(const FilterBase &) = delete;
};

} // namespace hitdsp
} // namespace hitdsp