// GENERATED for encrypted_nand/bit@1.0.0. Do not edit — `--update` rewrites it.
//
// The cryptographic envelope: context, keys, encryption, decryption. The only
// place a secret key exists. The measured call never enters this file.
//
// Every parameter comes from config.jsonc. For this scheme there are two: the
// parameter set, which carries the security level and the failure
// probability, and the blind-rotation method.
#pragma once

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "binfhecontext.h"
#include "utils/parallel.h"

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

        // Through the library's own control, not omp_set_num_threads: every
        // parallel region in OpenFHE is written
        // `num_threads(OpenFHEParallelControls.GetThreadLimit(n))`, and that
        // limit is a member it keeps rather than anything OpenMP is asked for.
        // Setting OpenMP alone leaves the limit at the machine's thread count,
        // which is what every row would then have run at.
        if (cfg.threads > 0)
            lbcrypto::OpenFHEParallelControls.SetNumThreads(static_cast<int>(cfg.threads));

        cc_.GenerateBinFHEContext(paramset(cfg.parameter_set), method(cfg.method));
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
    // By name, so config.jsonc reads as the library's own vocabulary and an
    // unknown one fails before anything runs.
    //
    // A table rather than `convertToBINFHE_PARAMSET`, which does this in one
    // line but only from OpenFHE 1.6.0. Comparing a version against an earlier
    // one is the whole reason this scheme is here, and the converter would
    // have made every version before 1.6.0 unbuildable. These are the sets
    // 1.4.2 already had, so every image in the catalogue takes them.
    static lbcrypto::BINFHE_PARAMSET paramset(const std::string& name) {
        static const std::map<std::string, lbcrypto::BINFHE_PARAMSET> SETS = {
            {"LPF_STD128", lbcrypto::LPF_STD128},
            {"LPF_STD128Q", lbcrypto::LPF_STD128Q},
            {"LPF_STD128Q_LMKCDEY", lbcrypto::LPF_STD128Q_LMKCDEY},
            {"LPF_STD128_LMKCDEY", lbcrypto::LPF_STD128_LMKCDEY},
            {"MEDIUM", lbcrypto::MEDIUM},
            {"STD128", lbcrypto::STD128},
            {"STD128Q", lbcrypto::STD128Q},
            {"STD128Q_3", lbcrypto::STD128Q_3},
            {"STD128Q_3_LMKCDEY", lbcrypto::STD128Q_3_LMKCDEY},
            {"STD128Q_4", lbcrypto::STD128Q_4},
            {"STD128Q_4_LMKCDEY", lbcrypto::STD128Q_4_LMKCDEY},
            {"STD128Q_LMKCDEY", lbcrypto::STD128Q_LMKCDEY},
            {"STD128_3", lbcrypto::STD128_3},
            {"STD128_3_LMKCDEY", lbcrypto::STD128_3_LMKCDEY},
            {"STD128_4", lbcrypto::STD128_4},
            {"STD128_4_LMKCDEY", lbcrypto::STD128_4_LMKCDEY},
            {"STD128_AP", lbcrypto::STD128_AP},
            {"STD128_LMKCDEY", lbcrypto::STD128_LMKCDEY},
            {"STD192", lbcrypto::STD192},
            {"STD192Q", lbcrypto::STD192Q},
            {"STD192Q_3", lbcrypto::STD192Q_3},
            {"STD192Q_3_LMKCDEY", lbcrypto::STD192Q_3_LMKCDEY},
            {"STD192Q_4", lbcrypto::STD192Q_4},
            {"STD192Q_4_LMKCDEY", lbcrypto::STD192Q_4_LMKCDEY},
            {"STD192Q_LMKCDEY", lbcrypto::STD192Q_LMKCDEY},
            {"STD192_3", lbcrypto::STD192_3},
            {"STD192_3_LMKCDEY", lbcrypto::STD192_3_LMKCDEY},
            {"STD192_4", lbcrypto::STD192_4},
            {"STD192_4_LMKCDEY", lbcrypto::STD192_4_LMKCDEY},
            {"STD192_LMKCDEY", lbcrypto::STD192_LMKCDEY},
            {"STD256", lbcrypto::STD256},
            {"STD256Q", lbcrypto::STD256Q},
            {"STD256Q_3", lbcrypto::STD256Q_3},
            {"STD256Q_3_LMKCDEY", lbcrypto::STD256Q_3_LMKCDEY},
            {"STD256Q_4", lbcrypto::STD256Q_4},
            {"STD256Q_4_LMKCDEY", lbcrypto::STD256Q_4_LMKCDEY},
            {"STD256Q_LMKCDEY", lbcrypto::STD256Q_LMKCDEY},
            {"STD256_3", lbcrypto::STD256_3},
            {"STD256_3_LMKCDEY", lbcrypto::STD256_3_LMKCDEY},
            {"STD256_4", lbcrypto::STD256_4},
            {"STD256_4_LMKCDEY", lbcrypto::STD256_4_LMKCDEY},
            {"STD256_LMKCDEY", lbcrypto::STD256_LMKCDEY},
            {"TOY", lbcrypto::TOY},
        };
        auto found = SETS.find(name);
        if (found == SETS.end())
            throw std::runtime_error("config.jsonc: unknown parameter_set " + name);
        return found->second;
    }

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
