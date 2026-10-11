// GENERATED from encrypted_nand/bit@1.0.0. Do not edit — `--update` rewrites it.
//
// The types your answer is written against, derived from the signature: one
// field per value parameter, per argument, per result. A tensor is typed,
// because the signature already settled what its elements are.
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace fherma {

template <class T>
struct Tensor {
    std::vector<int64_t> shape;
    std::vector<T> data;          // row-major

    int64_t count() const {
        int64_t total = 1;
        for (auto d : shape) total *= d;
        return total;
    }
};

struct Point {
    uint32_t N = 0;   // u32
};

struct Inputs {
    Tensor<uint8_t> a;   // tensor<N x u8>
    Tensor<uint8_t> b;   // tensor<N x u8>
};

struct Outputs {
    Tensor<uint8_t> c;   // tensor<N x u8>
};

}  // namespace fherma

// The four functions you write are declared in solve.h.
