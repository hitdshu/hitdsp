#pragma once

#include <vector>
#include "ecc/bed_base.h"

namespace hitdsp {
namespace ecc {

class Ldpc : public BedBase {
public:
    void Init(const ::std::vector<::std::vector<int>> &base_graph, int z);

    virtual void Encode(const uint8_t *data, uint8_t *symbols, int data_bits = -1) override;
    virtual void Decode(const uint8_t *symbols, uint8_t *data, int data_bits = -1) override;
    virtual void Decode(const float *symbols, uint8_t *data, int data_bits = -1) override;

    bool CheckLdpc(const uint8_t *cw) const;

protected:
    void MulSh(const uint8_t *input, uint8_t *output, int k) const;

    ::std::vector<::std::vector<int>> base_graph_;
    ::std::vector<int> col_weights_;
    ::std::vector<int> row_weights_;
    ::std::vector<::std::vector<int>> row_mat_;
    ::std::vector<::std::vector<int>> col_mat_;
    int k_;
    int m_;
    int n_;
    int z_;
};

} // namespace ecc
} // namespace hitdsp