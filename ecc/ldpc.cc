#include "ecc/ldpc.h"

namespace hitdsp {
namespace ecc {

void Ldpc::Init(const ::std::vector<::std::vector<int>> &base_graph, int z) {
    base_graph_ = base_graph;
    z_ = z;
    m_ = base_graph.size();
    n_ = base_graph[0].size();
    k_ = n_ - m_;
    col_weights_ = ::std::vector<int>(n_, 0);
    row_weights_ = ::std::vector<int>(m_, 0);
    row_mat_.resize(m_);
    col_mat_.resize(n_);
    for (size_t ridx = 0; ridx < base_graph.size(); ++ridx) {
        const ::std::vector<int> &row_tmp = base_graph[ridx];
        for (size_t cidx = 0; cidx < row_tmp.size(); ++cidx) {
            if (row_tmp[cidx] != -1) {
                col_weights_[cidx]++;
                row_weights_[ridx]++;
                row_mat_[ridx].push_back(cidx);
                col_mat_[cidx].push_back(ridx);
            }
        }
    }
}

void Ldpc::Encode(const uint8_t *data, uint8_t *symbols, int data_bits) {

}

void Ldpc::Decode(const uint8_t *symbols, uint8_t *data, int data_bits) {

}

void Ldpc::Decode(const float *symbols, uint8_t *data, int data_bits) {

}

bool Ldpc::CheckLdpc(const uint8_t *cw) const {
    ::std::vector<uint8_t> syn(m_ * z_, 0);
    ::std::vector<uint8_t> muls(z_, 0);
    for (int ridx = 0; ridx < m_; ++ridx) {
        for (int cidx = 0; cidx < n_; ++cidx) {
            MulSh(&cw[cidx * z_], &muls[0], base_graph_[ridx][cidx]);
            for (int zidx = 0; zidx < z_; ++zidx) {
                syn[ridx * z_ + zidx] = syn[ridx * z_ + zidx] ^ muls[zidx];
            }
        }
    }
    for (auto &tmp : syn) {
        if (tmp != 0) {
            return false;
        } else {
            return true;
        }
    }
}

void Ldpc::MulSh(const uint8_t *input, uint8_t *output, int k) const {
    if (k == -1) {
        for (int idx = 0; idx < z_; ++idx) {
            output[idx] = 0;
        }
    } else {
        int index = 0;
        for (int idx = k; idx < z_; ++idx) {
            output[index++] = output[idx];
        }
        for (int idx = 0; idx < k; ++idx) {
            output[index++] = output[idx];
        }
    }
}

HITDSP_REGISTER_BED(Ldpc);

} // namespace ecc
} // namespace hitdsp