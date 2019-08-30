#include "ecc/reed_solomon.h"

namespace hitdsp {
namespace ecc {

ReedSolomon::ReedSolomon(int nmsg, int ngen, Gf28 gf) {
    gf_ = gf;
    nmsg_ = nmsg;
    ngen_ = ngen;
    uint8_t g = 1;
    uint8_t tmp[2];
    generator_ = Poly(&g, 0, gf_);
    for (int idx = 0; idx < ngen; ++idx) {
        tmp[0] = 1;
        tmp[1] = gf_.Pow(2, idx);
        generator_.Mul(Poly(&tmp[0], 1, gf_));
    }
    ::std::vector<uint8_t> msg(nmsg + ngen, 0);
    message_ = Poly(&msg[0], nmsg + ngen - 1, gf_);
}

void ReedSolomon::Encode(const uint8_t *input, uint8_t *output) {
    Poly tmp(message_);
    for (int idx = 0; idx < nmsg_; ++idx) {
        tmp[idx] = input[idx];
        output[idx] = input[idx];
    }
    tmp.Div(generator_);
    for (int idx = nmsg_; idx < ngen_ + nmsg_; ++idx) {
        output[idx] = tmp[idx - nmsg_];
    }
}

void ReedSolomon::Decode(const uint8_t *input, uint8_t *output) {
    ::std::vector<uint8_t> synds = CalcSyndromes(input);
    ::std::vector<uint8_t> err_loc = FindErrorLocator(synds);
    ::std::vector<uint8_t> err_loc_inv(err_loc.size(), 0);
    for (int idx = 0; idx < err_loc_inv.size(); ++idx) {
        err_loc_inv[idx] = err_loc[err_loc_inv.size() - 1 - idx];
    }
    ::std::vector<uint8_t> err_pos = FindErrors(err_loc_inv);
    CorrectErrata(input, synds, err_pos, output);
}

bool ReedSolomon::Check(const uint8_t *input) {
    ::std::vector<uint8_t> synds = CalcSyndromes(input);
    for (size_t idx = 0; idx < synds.size(); ++idx) {
        if (synds[idx] != 0) {
            return false;
        }
    }
    return true;
}

::std::vector<uint8_t> ReedSolomon::CalcSyndromes(const uint8_t *rcw) {
    ::std::vector<uint8_t> synds(ngen_, 0);
    Poly msg_poly(rcw, ngen_ + nmsg_ - 1, gf_);
    for (int idx = 0; idx < ngen_; ++idx) {
        synds[idx] = msg_poly.EvaluateAt(gf_.Pow(2, idx));
    }
    return synds;
}

::std::vector<uint8_t> ReedSolomon::FindErrorLocator(const ::std::vector<uint8_t> &synds) {
    ::std::vector<uint8_t> err_loc = {1};
    ::std::vector<uint8_t> old_loc = {1};
    for (int idx1 = 0; idx1 < ngen_; ++idx1) {
        uint8_t delta = synds[idx1];
        for (int idx2 = 1; idx2 < err_loc.size(); ++idx2) {
            delta ^= gf_.Mul(err_loc[(err_loc.size() - idx2 - 1) % err_loc.size()], synds[(idx1 - idx2 + synds.size()) % synds.size()]);
        }
        old_loc.push_back(0);
        if (delta != 0) {
            if (old_loc.size() > err_loc.size()) {
                Poly old_poly(&old_loc[0], old_loc.size() - 1, gf_);
                old_poly.MultScalar(delta);
                Poly err_poly(&err_loc[0], err_loc.size() - 1, gf_);
                err_poly.MultScalar(gf_.Inv(delta));
                err_loc = old_poly.GetPoly();
                old_loc = err_poly.GetPoly();
            }
            Poly old_poly(&old_loc[0], old_loc.size() - 1, gf_);
            Poly err_poly(&err_loc[0], err_loc.size() - 1, gf_);
            old_poly.MultScalar(delta);
            err_poly.Add(old_poly);
            err_loc = err_poly.GetPoly();
        }
    }
    return err_loc;
}

::std::vector<uint8_t> ReedSolomon::FindErrors(const ::std::vector<uint8_t> &err_loc) {
    Poly err_poly(&err_loc[0], err_loc.size() - 1, gf_);
    ::std::vector<uint8_t> err_pos;
    for (int idx = 0; idx < ngen_ + nmsg_; ++idx) {
        if (0 == err_poly.EvaluateAt(gf_.Pow(2, idx))) {
            err_pos.push_back(ngen_ + nmsg_ - 1 - idx);
        }
    }
    return err_pos;
}

::std::vector<uint8_t> ReedSolomon::FindErrataLocator(const ::std::vector<uint8_t> &err_pos) {
    ::std::vector<uint8_t> e_loc = {1};
    ::std::vector<uint8_t> tmp = {0, 1};
    Poly e_poly(&e_loc[0], e_loc.size() - 1, gf_);
    for (size_t idx = 0; idx < err_pos.size(); ++idx) {
        tmp[0] = gf_.Pow(2, err_pos[idx]);
        e_poly.Mul(Poly(&tmp[0], tmp.size() - 1, gf_));
    }
    return e_poly.GetPoly();
}

::std::vector<uint8_t> ReedSolomon::FindErrorEvaluator(const ::std::vector<uint8_t> &synds, const ::std::vector<uint8_t> &err_loc, int nsym) {
    Poly synd_poly(&synds[0], synds.size() - 1, gf_);
    Poly errl_poly(&err_loc[0], err_loc.size() - 1, gf_);
    ::std::vector<uint8_t> tmp(nsym + 2, 0);
    tmp[0] = 1;
    Poly tmp_poly(&tmp[0], tmp.size() - 1, gf_);
    synd_poly.Mul(errl_poly);
    synd_poly.Div(tmp_poly);
    return synd_poly.GetPoly();
}

void ReedSolomon::CorrectErrata(const uint8_t *input, const ::std::vector<uint8_t> &synds, const ::std::vector<uint8_t> &err_pos, uint8_t *output) {
    ::std::vector<uint8_t> coef_pos;
    for (size_t idx = 0; idx < err_pos.size(); ++idx) {
        coef_pos.push_back(ngen_ + nmsg_ - 1 - err_pos[idx]);
    }
    ::std::vector<uint8_t> err_loc = FindErrataLocator(coef_pos);
    ::std::vector<uint8_t> synds_rev(synds.size(), 0);
    for (size_t idx = 0; idx < synds_rev.size(); ++idx) {
        synds_rev[idx] = synds[synds_rev.size() - 1 - idx];
    }
    synds_rev.push_back(0);
    ::std::vector<uint8_t> err_eval_rev = FindErrorEvaluator(synds_rev, err_loc, err_loc.size() - 1);
    Poly err_eval_rev_poly(&err_eval_rev[0], err_eval_rev.size() - 1, gf_);
    ::std::vector<uint8_t> err_eval(err_eval_rev.size(), 0);
    for (size_t idx = 0; idx < err_eval.size(); ++idx) {
        err_eval[idx] = err_eval_rev[err_eval.size() - 1 - idx];
    }
    ::std::vector<uint8_t> x;
    for (size_t idx = 0; idx < coef_pos.size(); ++idx) {
        uint8_t l = 0xff - coef_pos[idx];
        x.push_back(gf_.Inv(gf_.Pow(2, l)));
    }
    ::std::vector<uint8_t> e(ngen_ + nmsg_, 0);
    int x_len = x.size();
    for (int idx = 0; idx < x_len; ++idx) {
        uint8_t xi = x[idx];
        uint8_t xi_inv = gf_.Inv(xi);
        ::std::vector<uint8_t> err_loc_prime_tmp;
        for (int xidx = 0; xidx < x_len; ++xidx) {
            if (idx != xidx) {
                err_loc_prime_tmp.push_back(gf_.Sub(1, gf_.Mul(xi_inv, x[xidx])));
            }
        }
        uint8_t err_loc_prime = 1;
        for (size_t idx = 0; idx < err_loc_prime_tmp.size(); ++idx) {
            err_loc_prime = gf_.Mul(err_loc_prime, err_loc_prime_tmp[idx]);
        }
        uint8_t y = err_eval_rev_poly.EvaluateAt(xi_inv);
        y = gf_.Mul(xi, y);
        uint8_t mag = gf_.Div(y, err_loc_prime);
        e[err_pos[idx]] = mag;
    }
    Poly msg_in_poly(input, ngen_ + nmsg_ - 1, gf_);
    Poly e_poly(&e[0], e.size() - 1, gf_);
    msg_in_poly.Add(e_poly);
    for (int idx = 0; idx < nmsg_; ++idx) {
        output[idx] = msg_in_poly[idx];
    }
}

} // namespace ecc
} // namespace hitdsp