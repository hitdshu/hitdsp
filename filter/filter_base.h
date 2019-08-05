#pragma once

#include <complex>
#include <vector>

namespace hitdsp {
namespace filter {

class FilterBase {
public:
    FilterBase() = default;
    virtual ~FilterBase() = default;

    virtual ::std::complex<float> PushAndExecute(const ::std::complex<float> &input) const = 0;
    virtual ::std::complex<double> PushAndExecute(const ::std::complex<double> &input) const = 0;
    virtual void BlockExecute(const ::std::vector<::std::complex<float>> inputs[], ::std::vector<::std::complex<float>> outputs[]) const = 0;

    virtual void SetBlockParam(const int &block_size) { block_size_ = block_size; }

    FilterBase(const FilterBase &) = delete;
    FilterBase &operator=(const FilterBase &) = delete;

protected:
    int block_size_;
};

HITDSP_REGISTER_REGISTER(FilterBase);
#define HITDSP_REGISTER_FILTER(name) \
    HITDSP_REGISTER_CLASS(FilterBase, name)

} // namespace hitdsp
} // namespace hitdsp