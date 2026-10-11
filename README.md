# Encrypted NAND on OpenFHE

An answer to `encrypted-nand/bit@1.0.0` on FHERMA: NAND over encrypted bits
through OpenFHE's `binfhe` module, where every two-input gate is one blind
rotation and therefore one bootstrap.

`src` is three functions. `solve_run` is the only one measured, and it is a
loop of `EvalBinGate(NAND, …)` — the whole cost of the answer is there.

## What varies between the rows

The branch `rows` carries one commit per row of the board, and every one of
them changes `config.jsonc` and nothing else:

| row | `method` | `threads` |
|---|---|---|
| GINX, 1 thread | `GINX` | 1 |
| GINX, 8 threads | `GINX` | 8 |
| GINX, every core | `GINX` | 24 |
| LMKCDEY, 1 thread | `LMKCDEY` | 1 |
| LMKCDEY, 8 threads | `LMKCDEY` | 8 |
| LMKCDEY, every core | `LMKCDEY` | 24 |

GINX takes `STD128`; LMKCDEY takes `STD128_LMKCDEY`, the set tuned for it.
That is the shape of the table in OpenFHE's own
[Best_Performance.md](https://github.com/openfheorg/openfhe-development/blob/main/docs/static_docs/Best_Performance.md),
which is what these rows are meant to be read against.

## Building it outside the platform

```
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
./build/solution <point directory>
```

The generated files — `main.cpp`, `fherma.h`, `solve.h`, `envelope.h` — are
the platform's and are written over this clone at every measurement. They are
kept here so a plain clone builds; editing them changes nothing that is
measured.
