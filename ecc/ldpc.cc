#include <cassert>
#include "ecc/ldpc.h"

namespace hitdsp {
namespace ecc {

void Ldpc::Init(const ::std::vector<::std::vector<int>> &base_graph, int z, int max_iter) {
    base_graph_ = base_graph;
    z_ = z;
    m_ = base_graph.size();
    n_ = base_graph[0].size();
    k_ = n_ - m_;
    max_iter_ = max_iter;
    col_weights_ = ::std::vector<int>(n_, 0);
    row_weights_ = ::std::vector<int>(m_, 0);
    row_mat_.resize(m_);
    col_mat_.resize(n_);
    int index = 0;
    slen_ = 0;
    max_row_weights_ = 0;
    for (size_t ridx = 0; ridx < base_graph.size(); ++ridx) {
        const ::std::vector<int> &row_tmp = base_graph[ridx];
        assert(n_ == row_tmp.size());
        for (size_t cidx = 0; cidx < n_; ++cidx) {
            if (row_tmp[cidx] != -1) {
                col_weights_[cidx]++;
                row_weights_[ridx]++;
                row_mat_[ridx].push_back(index);
                col_mat_[cidx].push_back(index);
                slen_++;
            }
            index++;
        }
        if (row_weights_[ridx] > max_row_weights_) {
            max_row_weights_ = row_weights_[ridx];
        }
    }
}

void Ldpc::Encode(const uint8_t *data, uint8_t *symbols, int data_bits) {
    for (int idx = 0; idx < k_ * z_; ++idx) {
        symbols[idx] = data[idx];
    }
    ::std::vector<::std::vector<uint8_t>> parity(m_, ::std::vector<uint8_t>(z_, 0));
    ::std::vector<uint8_t> tmp(z_, 0);
    for (int ridx = 0; ridx < m_; ++ridx) {
        for (int ccidx = 0; ccidx < row_weights_[ridx]; ++ccidx) {
            int index = row_mat_[ridx][ccidx];
            int row = index / n_;
            int col = index % n_;
            if (col >= k_) {
                break;
            }
            MulSh(&symbols[col * z_], &tmp[0], base_graph_[row][col]);
            for (int pidx = 0; pidx < z_; ++pidx) {
                parity[ridx][pidx] ^= tmp[pidx];
            }
        }
    }
    for (int idx1 = 0; idx1 < z_; ++idx1) {
        tmp[idx1] = 0;
        for (int idx2 = 0; idx2 < m_; ++idx2) {
            tmp[idx1] ^= parity[idx2][idx1];
        }
        symbols[k_ * z_ + idx1] = tmp[idx1];
    }
    for (int ridx = 0; ridx < m_; ++ridx) {
        MulSh(&symbols[k_ * z_], &tmp[0], base_graph_[ridx][k_]);
        for (int pidx = 0; pidx < z_; ++pidx) {
            parity[ridx][pidx] ^= tmp[pidx];
        }
    }
    ::std::vector<uint8_t> prevp(z_, 0);
    int index = (k_ + 1) * z_;
    for (int idx = 0; idx < m_ - 1; ++idx) {
        for (int pidx = 0; pidx < z_; ++pidx) {
            prevp[pidx] ^= parity[idx][pidx];
            symbols[index++] = prevp[pidx];
        }
    }
}

void Ldpc::Decode(const uint8_t *symbols, uint8_t *data, int data_bits) {
}

void Ldpc::Decode(const float *symbols, uint8_t *data, int data_bits) {
    int kbits = k_ * z_;
    int nbits = n_ * z_;
    ::std::vector<::std::vector<float>> treg(max_row_weights_, ::std::vector<float>(z_, 0));
    ::std::vector<::std::vector<float>> r(slen_, ::std::vector<float>(z_, 0));
    ::std::vector<float> llr(symbols, symbols + nbits);
    for (int iter_idx = 0; iter_idx < max_iter_; ++iter_idx) {
        int ri = 0;
        for (int midx = 0; midx < m_; ++midx) {
            int ti = 0;
            for (int cidx = 0; cidx < n_; ++cidx) {
                if (base_graph_[midx][cidx] == -1) {
                    continue;
                }
                ti++;
                ri++;
                for (int zidx = 0; zidx < z_; ++zidx) {
                    llr[cidx * z_ + zidx] -= r[ri - 1][zidx];
                }
                MulSh<float>(&llr[cidx * z_], &treg[ti - 1][0], base_graph_[midx][cidx]);
            }
            ::std::vector<::std::pair<::std::pair<int, float>, float>> min2s = Minimum<float>(treg, ti);
            ::std::vector<int> signs = Sign<float>(treg, ti);
            for (int tidx = 0; tidx < ti; ++tidx) {
                for (int zidx = 0; zidx < z_; ++zidx) {
                    int tmp_sign = treg[tidx][zidx] > 0 ? 1 : -1;
                    treg[tidx][zidx] = min2s[zidx].first.second;
                    if (tidx == min2s[zidx].first.first) {
                        treg[tidx][zidx] = min2s[zidx].second;
                    }
                    treg[tidx][zidx] *= signs[zidx] * tmp_sign;
                }
            }
            ri -= ti;
            ti = 0;
            for (int cidx = 0; cidx < n_; ++cidx) {
                if (base_graph_[midx][cidx] == -1) {
                    continue;
                }
                ri++;
                ti++;
                MulSh<float>(&treg[ti - 1][0], &r[ri - 1][0], z_ - base_graph_[midx][cidx]);
                for (int zidx = 0; zidx < z_; ++zidx) {
                    llr[cidx * z_ + zidx] += r[ri - 1][zidx];
                }
            }
        }
    }
    for (int idx = 0; idx < kbits; ++idx) {
        if (llr[idx] < 0) {
            data[idx] = 1;
        } else {
            data[idx] = 0;
        }
    }
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
        }
    }
    return true;
}

HITDSP_REGISTER_BED(Ldpc);

} // namespace ecc
} // namespace hitdsp