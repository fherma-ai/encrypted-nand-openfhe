// GENERATED. The four functions that are yours, declared once.
#pragma once

#include "fherma.h"
#include "binfhecontext.h"

using lbcrypto::BinFHEContext;
using lbcrypto::LWECiphertext;
using lbcrypto::LWEPlaintext;

// Setup over public material: the context and its keys are already made.
// Once per point, not measured.
void* solve_init(const fherma::Point& p, BinFHEContext cc);

// The layout: cleartext into the bits that will be encrypted, one per
// ciphertext. binfhe does not pack, so this is the order and nothing else.
std::vector<LWEPlaintext> solve_encoding(BinFHEContext cc,
                                         const fherma::Inputs& inp);

// The answer, over ciphertexts. Measured — and only this.
std::vector<LWECiphertext> solve_run(
    void* state,
    BinFHEContext cc,
    const std::vector<LWECiphertext>& cts);

// Reading the answer out of the decrypted bits. Not measured.
fherma::Outputs solve_decoding(const fherma::Point& p,
                               BinFHEContext cc,
                               const std::vector<LWEPlaintext>& pts);

void solve_free(void* state);
