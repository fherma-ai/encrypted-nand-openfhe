// GENERATED for encrypted_nand/bit@1.0.0. Do not edit — `--update` rewrites it.
//
// The cryptographic envelope: context, keys, encryption, decryption. The only
// place a secret key exists. The measured call never enters this file.
//
// Every parameter comes from config.jsonc. For this scheme there are two: the
// parameter set, which carries the security level and the failure
// probability, and the blind-rotation method.
//
// Needs OpenFHE 1.6.0 or later: naming the parameter set in a file rather than
// at compile time wants `convertToBINFHE_PARAMSET`, which arrived in 1.6.0.
#pragma once

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "binfhecontext.h"

namespace fherma_envelope {

struct Config {
    std::string scheme = "fhew";
    std::string parameter_set = "STD128";
    std::string method = "GINX";
    uint32_t threads = 0;

    static Config load(const std::string& path);   // tiny JSONC reader in main
};

class Envelope {
  public:
    explicit Envelope(const Config& cfg) {
        if (cfg.scheme != "fhew")
            throw std::runtime_error("config.jsonc: this scaffold answers with fhew, not " + cfg.scheme);

        // Before the context, because the library reads the thread count when
        // it builds its own parallel regions.
#ifdef _OPENMP
        if (cfg.threads > 0) omp_set_num_threads(static_cast<int>(cfg.threads));
#endif

        // Throws on a name the library does not know, before anything runs.
        cc_.GenerateBinFHEContext(lbcrypto::convertToBINFHE_PARAMSET(cfg.parameter_set),
                                  method(cfg.method));
        sk_ = cc_.KeyGen();
        // The refresh and key-switching keys every gate consumes. Once per
        // point, outside the measured window.
        cc_.BTKeyGen(sk_);
    }

    lbcrypto::BinFHEContext cc() const { return cc_; }

    std::vector<lbcrypto::LWECiphertext> encrypt(const std::vector<lbcrypto::LWEPlaintext>& bits) {
        std::vector<lbcrypto::LWECiphertext> out;
        out.reserve(bits.size());
        for (const auto& bit : bits) out.push_back(cc_.Encrypt(sk_, bit));
        return out;
    }

    std::vector<lbcrypto::LWEPlaintext> decrypt(const std::vector<lbcrypto::LWECiphertext>& cts) {
        std::vector<lbcrypto::LWEPlaintext> out;
        out.reserve(cts.size());
        for (const auto& ct : cts) {
            lbcrypto::LWEPlaintext bit;
            cc_.Decrypt(sk_, ct, &bit);
            out.push_back(bit);
        }
        return out;
    }

  private:
    // Three of them, so a map rather than the library's own converter, which
    // it offers for parameter sets alone.
    static lbcrypto::BINFHE_METHOD method(const std::string& name) {
        static const std::map<std::string, lbcrypto::BINFHE_METHOD> M = {
            {"AP", lbcrypto::AP},
            {"GINX", lbcrypto::GINX},
            {"LMKCDEY", lbcrypto::LMKCDEY},
        };
        auto found = M.find(name);
        if (found == M.end())
            throw std::runtime_error("config.jsonc: unknown method " + name + " (AP, GINX, LMKCDEY)");
        return found->second;
    }

    lbcrypto::BinFHEContext cc_;
    lbcrypto::LWEPrivateKey sk_;   // never handed out
};

}  // namespace fherma_envelope
