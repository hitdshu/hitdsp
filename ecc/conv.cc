#include <cassert>
#include <algorithm>
#include "ecc/conv.h"
#include "common/maths.h"

namespace hitdsp {
namespace ecc {

void Conv::Init(int nmem, int rate, const int *polys) {
    nmem_ = nmem;
    rate_ = rate;
    for (int idx = 0; idx < rate; ++idx) {
        polys_.push_back(polys[idx]);
    }
}

void Conv::Encode(const uint8_t *data, uint8_t *symbols, int data_bits) {
    assert(data_bits > 3 * nmem_);
    int out_index = 0;
    int sr = 0;
    for (int idx = 0; idx < data_bits + nmem_; ++idx) {
        int bit;
        if (idx < data_bits) {
            int b = data[idx / 8];
            int j = idx % 8;
            bit = (b >> (7 - j)) & 1;
        } else {
            bit = 0;
        }
        sr = (sr << 1) | bit;
        for (int ridx = 0; ridx < rate_; ++ridx) {
            int m = sr & polys_[ridx];
            int par = ::hitdsp::common::Parity(m);
            symbols[out_index++] = par;
        }
    }
}

void Conv::Decode(const uint8_t *symbols, uint8_t *data, int data_bits) {
    assert(data_bits > 3 * nmem_);
    int total_symb_count = rate_ * (data_bits + nmem_);
    int total_states_num = 1 << nmem_;
    int total_states_num_half = 1 << (nmem_ - 1);
    int states_mask = total_states_num - 1;
    int bit0 = 0;
    int bit1 = 1;
    ::std::vector<double> cost_prev_(total_states_num, 1e8);
    ::std::vector<double> cost_cur_(total_states_num, 0);
    ::std::vector<int> state_ind(total_states_num * (data_bits + nmem_), 0);
    ::std::vector<int> data_dec(data_bits, 0);
    cost_prev_[0] = 0;
    for (int idx = 0; idx < data_bits; ++idx) {
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            int cur_in_bit = sidx & 1;
            int last_state1 = sidx >> 1;
            int last_state2 = (sidx >> 1) + total_states_num_half;
            double cur_cost1 = cost_prev_[last_state1];
            double cur_cost2 = cost_prev_[last_state2];
            for (int ridx = 0; ridx < rate_; ++ridx) {
                int m1 = ((last_state1 << 1) + cur_in_bit) & polys_[ridx];
                int par1 = ::hitdsp::common::Parity(m1);
                double ro1 = par1 ? bit1 : bit0;
                double tmp_cost1 = ::std::pow(ro1 - (double)symbols[idx * rate_ + ridx], 2);
                cur_cost1 += tmp_cost1;
                int m2 = ((last_state2 << 1) + cur_in_bit) & polys_[ridx];
                int par2 = ::hitdsp::common::Parity(m2);
                double ro2 = par2 ? bit1 : bit0;
                double tmp_cost2 = ::std::pow(ro2 - (double)symbols[idx * rate_ + ridx], 2);
                cur_cost2 += tmp_cost2;
            }
            cost_cur_[sidx] = ::std::min(cur_cost1, cur_cost2);
            state_ind[idx * total_states_num + sidx] = (cur_cost1 < cur_cost2) ? 0 : 1;
        }
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            cost_prev_[sidx] = cost_cur_[sidx];
        }
    }
    int min_state = 0;
    for (int idx = 0; idx < nmem_; ++idx) {
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            int cur_in_bit = sidx & 1;
            int last_state1 = sidx >> 1;
            int last_state2 = (sidx >> 1) + total_states_num_half;
            double cur_cost1 = cost_prev_[last_state1];
            double cur_cost2 = cost_prev_[last_state2];
            for (int ridx = 0; ridx < rate_; ++ridx) {
                int m1 = ((last_state1 << 1) + cur_in_bit) & polys_[ridx];
                int par1 = ::hitdsp::common::Parity(m1);
                double ro1 = par1 ? bit1 : bit0;
                double tmp_cost1 = ::std::pow(ro1 - (double)symbols[(idx + data_bits) * rate_ + ridx], 2);
                cur_cost1 += tmp_cost1;
                int m2 = ((last_state2 << 1) + cur_in_bit) & polys_[ridx];
                int par2 = ::hitdsp::common::Parity(m2);
                double ro2 = par2 ? bit1 : bit0;
                double tmp_cost2 = ::std::pow(ro2 - (double)symbols[(idx + data_bits) * rate_ + ridx], 2);
                cur_cost2 += tmp_cost2;
            }
            cost_cur_[sidx] = ::std::min(cur_cost1, cur_cost2);
            state_ind[(idx + data_bits) * total_states_num + sidx] = (cur_cost1 < cur_cost2) ? 0 : 1;
            if (cur_in_bit) {
                cost_cur_[sidx] += 1e8;
            }
        }
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            cost_prev_[sidx] = cost_cur_[sidx];
        }
        if (nmem_ - 1 == idx) {
            double cost_min = 1e10;
            for (int sidx = 0; sidx < total_states_num; ++sidx) {
                if (cost_prev_[sidx] < cost_min) {
                    cost_min = cost_prev_[sidx];
                    min_state = sidx;
                }
            }
        }
    }
    for (int idx = data_bits + nmem_ - 1; idx >= nmem_; --idx) {
        int pop_out_bit = state_ind[idx * total_states_num + min_state];
        data_dec[idx - nmem_] = pop_out_bit;
        min_state = (min_state >> 1) + (pop_out_bit << (nmem_ - 1));
    }
    for (int idx = 0; idx < (data_bits + 7) / 8; ++idx) {
        data[idx] = 0;
    }
    for (int idx = 0; idx < data_bits; ++idx) {
        if (data_dec[idx]) {
            data[idx / 8] ^= 1 << (7 - (idx % 8));
        }
    }
}

void Conv::Decode(const float *symbols, uint8_t *data, int data_bits) {
    assert(data_bits > 3 * nmem_);
    int total_symb_count = rate_ * (data_bits + nmem_);
    int total_states_num = 1 << nmem_;
    int total_states_num_half = 1 << (nmem_ - 1);
    int states_mask = total_states_num - 1;
    float bit0 = 0;
    float bit1 = 1;
    ::std::vector<double> cost_prev_(total_states_num, 1e8);
    ::std::vector<double> cost_cur_(total_states_num, 0);
    ::std::vector<int> state_ind(total_states_num * (data_bits + nmem_), 0);
    ::std::vector<int> data_dec(data_bits, 0);
    cost_prev_[0] = 0;
    for (int idx = 0; idx < data_bits; ++idx) {
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            int cur_in_bit = sidx & 1;
            int last_state1 = sidx >> 1;
            int last_state2 = (sidx >> 1) + total_states_num_half;
            double cur_cost1 = cost_prev_[last_state1];
            double cur_cost2 = cost_prev_[last_state2];
            for (int ridx = 0; ridx < rate_; ++ridx) {
                int m1 = ((last_state1 << 1) + cur_in_bit) & polys_[ridx];
                int par1 = ::hitdsp::common::Parity(m1);
                double ro1 = par1 ? bit1 : bit0;
                double tmp_cost1 = ::std::pow(ro1 - (double)symbols[idx * rate_ + ridx], 2);
                cur_cost1 += tmp_cost1;
                int m2 = ((last_state2 << 1) + cur_in_bit) & polys_[ridx];
                int par2 = ::hitdsp::common::Parity(m2);
                double ro2 = par2 ? bit1 : bit0;
                double tmp_cost2 = ::std::pow(ro2 - (double)symbols[idx * rate_ + ridx], 2);
                cur_cost2 += tmp_cost2;
            }
            cost_cur_[sidx] = ::std::min(cur_cost1, cur_cost2);
            state_ind[idx * total_states_num + sidx] = (cur_cost1 < cur_cost2) ? 0 : 1;
        }
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            cost_prev_[sidx] = cost_cur_[sidx];
        }
    }
    int min_state = 0;
    for (int idx = 0; idx < nmem_; ++idx) {
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            int cur_in_bit = sidx & 1;
            int last_state1 = sidx >> 1;
            int last_state2 = (sidx >> 1) + total_states_num_half;
            double cur_cost1 = cost_prev_[last_state1];
            double cur_cost2 = cost_prev_[last_state2];
            for (int ridx = 0; ridx < rate_; ++ridx) {
                int m1 = ((last_state1 << 1) + cur_in_bit) & polys_[ridx];
                int par1 = ::hitdsp::common::Parity(m1);
                double ro1 = par1 ? bit1 : bit0;
                double tmp_cost1 = ::std::pow(ro1 - (double)symbols[(idx + data_bits) * rate_ + ridx], 2);
                cur_cost1 += tmp_cost1;
                int m2 = ((last_state2 << 1) + cur_in_bit) & polys_[ridx];
                int par2 = ::hitdsp::common::Parity(m2);
                double ro2 = par2 ? bit1 : bit0;
                double tmp_cost2 = ::std::pow(ro2 - (double)symbols[(idx + data_bits) * rate_ + ridx], 2);
                cur_cost2 += tmp_cost2;
            }
            cost_cur_[sidx] = ::std::min(cur_cost1, cur_cost2);
            state_ind[(idx + data_bits) * total_states_num + sidx] = (cur_cost1 < cur_cost2) ? 0 : 1;
            if (cur_in_bit) {
                cost_cur_[sidx] += 1e8;
            }
        }
        for (int sidx = 0; sidx < total_states_num; ++sidx) {
            cost_prev_[sidx] = cost_cur_[sidx];
        }
        if (nmem_ - 1 == idx) {
            double cost_min = 1e10;
            for (int sidx = 0; sidx < total_states_num; ++sidx) {
                if (cost_prev_[sidx] < cost_min) {
                    cost_min = cost_prev_[sidx];
                    min_state = sidx;
                }
            }
        }
    }
    for (int idx = data_bits + nmem_ - 1; idx >= nmem_; --idx) {
        int pop_out_bit = state_ind[idx * total_states_num + min_state];
        data_dec[idx - nmem_] = pop_out_bit;
        min_state = (min_state >> 1) + (pop_out_bit << (nmem_ - 1));
    }
    for (int idx = 0; idx < (data_bits + 7) / 8; ++idx) {
        data[idx] = 0;
    }
    for (int idx = 0; idx < data_bits; ++idx) {
        if (data_dec[idx]) {
            data[idx / 8] ^= 1 << (7 - (idx % 8));
        }
    }
}

HITDSP_REGISTER_BED(Conv);

} // namespace ecc
} // namespace hitdsp