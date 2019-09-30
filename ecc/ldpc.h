#pragma once

#include <vector>
#include <cmath>
#include "ecc/bed_base.h"

namespace hitdsp {
namespace ecc {

class Ldpc : public BedBase {
public:
    void Init(const ::std::vector<::std::vector<int>> &base_graph, int z, int max_iter = 8);

    virtual void Encode(const uint8_t *data, uint8_t *symbols, int data_bits = -1) override;
    virtual void Decode(const uint8_t *symbols, uint8_t *data, int data_bits = -1) override;
    virtual void Decode(const float *symbols, uint8_t *data, int data_bits = -1) override;

    bool CheckLdpc(const uint8_t *cw) const;

protected:
    template <typename ValueType>
    void MulSh(const ValueType *input, ValueType *output, int k) const {
        if (k == -1) {
            for (int idx = 0; idx < z_; ++idx) {
                output[idx] = 0;
            }
        } else {
            int index = 0;
            for (int idx = k; idx < z_; ++idx) {
                output[index++] = input[idx];
            }
            for (int idx = 0; idx < k; ++idx) {
                output[index++] = input[idx];
            }
        }
    }
    template <typename ValueType>
    ::std::vector<::std::pair<::std::pair<int, ValueType>, ValueType>> Minimum(const ::std::vector<::std::vector<ValueType>> &llr, int max_rows, bool col_wise = true) const {
        int rsize = 0;
        if (col_wise) {
            rsize = llr[0].size();
        } else {
            rsize = max_rows;
        }
        ::std::vector<::std::pair<::std::pair<int, ValueType>, ValueType>> result(rsize, ::std::pair<::std::pair<int, ValueType>, ValueType>(::std::pair<int, ValueType>(0, 1e5), 1e6));
        if (col_wise) {
            for (size_t cidx = 0; cidx < llr[0].size(); ++cidx) {
                for (size_t ridx = 0; ridx < max_rows; ++ridx) {
                    if (fabs(llr[ridx][cidx]) < result[cidx].first.second) {
                        result[cidx].second = result[cidx].first.second;
                        result[cidx].first.second = fabs(llr[ridx][cidx]);
                        result[cidx].first.first = ridx;
                    } else if (fabs(llr[ridx][cidx]) < result[cidx].second) {
                        result[cidx].second = fabs(llr[ridx][cidx]);
                    }
                }
            }
        } else {
            for (size_t ridx = 0; ridx < max_rows; ++ridx) {
                const ::std::vector<ValueType> &tmp_row = llr[ridx];
                for (size_t cidx = 0; cidx < tmp_row.size(); ++cidx) {
                    if (fabs(tmp_row[cidx]) < result[ridx].first.second) {
                        result[ridx].second = result[ridx].first.second;
                        result[ridx].first.second = fabs(tmp_row[cidx]);
                        result[ridx].first.first = cidx;
                    } else if (fabs(tmp_row[cidx]) < result[ridx].second) {
                        result[ridx].second = fabs(tmp_row[cidx]);
                    }
                }
            }
        }
        return result;
    }
    template <typename ValueType>
    ::std::vector<int> Sign(const ::std::vector<::std::vector<ValueType>> &llr, int max_rows, bool col_wise = true) const {
        int rsize = 0;
        if (col_wise) {
            rsize = llr[0].size();
        } else {
            rsize = max_rows;
        }
        ::std::vector<int> result(rsize, 1);
        if (col_wise) {
            for (size_t cidx = 0; cidx < llr[0].size(); ++cidx) {
                for (size_t ridx = 0; ridx < max_rows; ++ridx) {
                    if (llr[ridx][cidx] < 0) {
                        result[cidx] *= -1;
                    }
                }
            }
        } else {
            for (size_t ridx = 0; ridx < max_rows; ++ridx) {
                const ::std::vector<ValueType> &tmp_row = llr[ridx];
                for (size_t cidx = 0; cidx < tmp_row.size(); ++cidx) {
                    if (tmp_row[cidx] < 0) {
                        result[ridx] *= -1;
                    }
                }
            }
        }
        return result;
    }

    ::std::vector<::std::vector<int>> base_graph_;
    ::std::vector<int> col_weights_;
    ::std::vector<int> row_weights_;
    ::std::vector<::std::vector<int>> row_mat_;
    ::std::vector<::std::vector<int>> col_mat_;
    int k_;
    int m_;
    int n_;
    int z_;
    int max_iter_;
    int slen_;
    int max_row_weights_;
};

} // namespace ecc
} // namespace hitdsp