// NAND over encrypted bits, one gate to a pair of ciphertexts.
//
// Every two-input gate in FHEW/TFHE is one blind rotation, so the whole cost
// of this answer is in EvalBinGate — which is the only call the clock sees.
#include "solve.h"

void* solve_init(const fherma::Point& p, BinFHEContext cc) {
    return nullptr;
}

std::vector<LWEPlaintext> solve_encoding(BinFHEContext cc,
                                         const fherma::Inputs& inp) {
    // The order run() expects: every a, then every b.
    std::vector<LWEPlaintext> bits;
    bits.reserve(inp.a.data.size() + inp.b.data.size());
    for (auto bit : inp.a.data) bits.push_back(static_cast<LWEPlaintext>(bit));
    for (auto bit : inp.b.data) bits.push_back(static_cast<LWEPlaintext>(bit));
    return bits;
}

std::vector<LWECiphertext> solve_run(
    void* state,
    BinFHEContext cc,
    const std::vector<LWECiphertext>& cts) {
    const size_t gates = cts.size() / 2;
    std::vector<LWECiphertext> out(gates);
    for (size_t i = 0; i < gates; ++i)
        out[i] = cc.EvalBinGate(lbcrypto::NAND, cts[i], cts[gates + i]);
    return out;
}

fherma::Outputs solve_decoding(const fherma::Point& p,
                               BinFHEContext cc,
                               const std::vector<LWEPlaintext>& pts) {
    fherma::Outputs out;
    out.c.shape = {static_cast<int64_t>(pts.size())};
    out.c.data.reserve(pts.size());
    for (auto bit : pts) out.c.data.push_back(static_cast<uint8_t>(bit));
    return out;
}

void solve_free(void* state) {}
