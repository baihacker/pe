# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**PE** is a C++ header-only library for solving [Project Euler](https://projecteuler.net) problems. All modules are `pe_*` files without extensions (they are C++ headers named without `.hpp`). The public entry point is `#include <pe.hpp>`; that file only does `#include <pe>`, and `pe` is the header that actually pulls in every `pe_*` module in dependency order (see Architecture below).

## Build & Run

### Windows (Bazel — primary)

```bash
# Build all library targets
bazel build //...

# Build a specific example
bazel build //example:example

# Build all examples (GCC variant)
bazel build //example:gcc_builds

# Run the test suite
bazel run //test:test

# Run performance tests
bazel run //test:test_perf
```

The `.bazelrc` hardcodes paths for this machine (Bazel output root `D:/bazel-output`, `C_INCLUDE_PATH` under `D:/Hilbert/usr/include`, `LIBRARY_PATH` at `D:/Hilbert/usr/lib`). On another machine, update `.bazelrc` or override via `--action_env`.

### Linux/CI (direct compiler)

```bash
# Set include path so pe.hpp is findable
export CPLUS_INCLUDE_PATH="$(pwd):/usr/include/flint:/usr/include/eigen3:${CPLUS_INCLUDE_PATH}"
export LIBRARY_PATH="/usr/lib:${LIBRARY_PATH}"

# Build and run tests (GCC)
g++ test/pe_test.c -o ./pe_test.out --std=c++20 -O3 -march=native -mtune=native \
    -fopenmp -lmpfr -lflint -lntl -lgmp \
    -DENABLE_ASSERT=0 -DTRY_TO_USE_INT128=1 -DENABLE_OPENMP=1 \
    -DENABLE_GMP=1 -DENABLE_FLINT=0 -DENABLE_MPFR=1 -DENABLE_NTL=1 \
    -DENABLE_ZMQ=0 -DENABLE_LIBBF=0 -DENABLE_PRIME_COUNT=0 -DENABLE_PRIME_SIEVE=0 \
    -DENABLED_TEST=SMALL,MEDIUM,BIG
./pe_test.out

# Compile the example
g++ example/example.c -o example.out --std=c++17 -O3 -march=native -fopenmp \
    -lquadmath -Wl,--stack,268435456 -static \
    -lbf -lgmpxx -lflint -lmpfr -lntl -lgmp -lprimesieve -lprimecount
```

### Precompiled header (optional, speeds up compilation)

```bash
g++ -xc++-header pe.hpp --std=c++17 -O3 -march=native -fopenmp
```

## Configuration

`pe_config` is the centralized feature-flag file. **Do not hand-edit it before running the generator.** To (re-)generate it:

```bash
python gen_config.py
```

The generator probes `CPLUS_INCLUDE_PATH` to auto-detect third-party libraries and sets flags accordingly. `ENABLE_ASSERT` and `TRY_TO_USE_INT128` are always written (not probed) and `ENABLE_OPENMP` isn't emitted at all — edit `pe_config` or pass `-D` overrides for these:

| Flag | Default | Meaning |
|------|---------|---------|
| `ENABLE_ASSERT` | 1 | Enable PE_ASSERT checks |
| `TRY_TO_USE_INT128` | 1 | Use `__int128` when available |
| `ENABLE_OPENMP` | — | Not auto-detected; add manually |

All other `ENABLE_*` flags (`ENABLE_GMP`, `ENABLE_FLINT`, `ENABLE_MPFR`, `ENABLE_NTL`, `ENABLE_ZMQ`, `ENABLE_LIBBF`, `ENABLE_EIGEN`, `ENABLE_PRIME_COUNT`, `ENABLE_PRIME_SIEVE`, `ENABLE_TCMALLOC`) can be overridden at compile time via `-D` defines without editing `pe_config`.

## Test Categories

Tests in `test/pe_test.c` are tagged. Control which run via `ENABLED_TEST`:

- `SMALL` — fast unit tests (always run in CI)
- `MEDIUM` — moderate tests (run in CI)
- `BIG` — slow tests (run in CI)
- `SUPER` — very slow, excluded from CI (`NO_SUPER_TEST` or `CONTINUOUS_INTEGRATION_TEST` skips them)
- `SPECIFIED` — individual tests selected by name

## Architecture

### Module layers (load order in `pe`, included by `pe.hpp`)

```
pe_internal  →  pe_config + compiler/platform detection + third-party includes
pe_base      →  #includes pe_internal; STL includes, core macros (PE_ASSERT,
                SL, REQUIRES, RETURN), type aliases (int64, uint64, uint128)
pe_type_traits → compile-time type introspection
```

Then functional modules, in the actual `#include` order from the `pe` header:

| Layer | Modules |
|-------|---------|
| Base | `pe_base`, `pe_type_traits`, `pe_span`, `pe_bit`, `pe_mod`, `pe_int`, `pe_extended_int` (→ `pe_extended_unsigned_int`, `pe_extended_signed_int`), `pe_float`, `pe_vector` |
| General util | `pe_io`, `pe_time`, `pe_persistance`, `pe_tree`, `pe_rand` |
| Range | `pe_range` |
| Matrix | `pe_mat` |
| Number theory | `pe_nt_base` → `pe_nt` |
| Fraction | `pe_fraction` |
| Parallel | `pe_parallel`, `pe_parallel_algo` |
| Polynomial | `pe_poly_base` (dispatches to `pe_poly_base_*`), `pe_poly_algo` |
| FFT | `pe_fft` |
| Big integer | `pe_gbi`, `pe_bi32`, `pe_mpz` |
| Geometry / memory / MP float | `pe_geometry`, `pe_memory`, `pe_mpf` |
| Serialization / misc | `pe_serialization`, `pe_misc`, `pe_mma` |
| Late-bound algorithms | `pe_array`, `pe_ntf`, `pe_algo`, `pe_sym_poly`, `pe_db`, `pe_int_algo` |
| Distributed | `pe_dpe` |
| Initialization | `pe_initializer` (defines the `PE_INIT` macro; included last since it depends on `pe_nt`, `pe_fft`, `pe_parallel`) |

### Key conventions

- **`SL`** = `static inline` — used on most utility functions.
- **`REQUIRES(...)`** — SFINAE condition shorthand (expands to `std::enable_if_t<..., int> = 0`).
- **`RETURN(...)`** — trailing return type deduction helper.
- **`PE_INIT(...)`** — macro defined in `pe_initializer` (built on `PeInitializer`); must be called at program start to configure prime tables (`maxp`), phi/mu/rad/sigma arrays, FFT/NTT tables, the default modulus, and OpenMP/FLINT thread settings.
- **`using namespace pe;`** — all public symbols live in the `pe` namespace; examples always import it.
- **`pe_` prefix** — every module header follows this naming convention.
- Source files use the `.c` extension even though they are C++ (compiled with `g++` or `cl /TP`).

### Polynomial backend selection

`pe_poly_base` is a dispatcher. Which backend is active depends on which `ENABLE_*` flags are set (checked in priority order: FLINT → NTL → GMP → libbf → min25 → NTT). The NTT backend (`pe_poly_base_ntt`) is always available and provides `ntt32`/`ntt64` variants.

### Big integer unified interface

`pe_gbi` provides `GBIAbs`, `GBISign`, `GBIIsZero`, etc. that dispatch to the correct type (`BigInteger`, `MpInteger`, extended int types) via template specialization, so algorithms written against `pe_gbi` work with any big integer type.

### DVA (Divided Value Array)

Used extensively in prime-counting / prime-sum algorithms (`PrimeS0`, `PrimeS1`, etc.). A DVA stores values indexed by `n/k` for `k = 1..sqrt(n)` in a single flat array, accessed via `arr[k]` where `k` can be larger than `sqrt(n)` — the indexing scheme transparently maps large indices to the small-index region.
