# PE: C++ Library for Project Euler

[![Build Status (Windows/MSVC)](https://ci.appveyor.com/api/projects/status/scaji00tde2gb7uy?svg=true)](https://ci.appveyor.com/project/baihacker/pe-win-msvc)
[![Build Status (Ubuntu/GCC)](https://ci.appveyor.com/api/projects/status/9bt606nax24anyen?svg=true)](https://ci.appveyor.com/project/baihacker/pe-ubuntu-gcc)
[![Build Status (Ubuntu/Clang)](https://ci.appveyor.com/api/projects/status/nw243uvs95i0bj85?svg=true)](https://ci.appveyor.com/project/baihacker/pe-ubuntu-clang)
[![Build Status](https://github.com/baihacker/pe/actions/workflows/ci.yml/badge.svg)](https://github.com/baihacker/pe/actions)
[![GitHub Releases](https://img.shields.io/github/release/baihacker/pe.svg)](https://github.com/baihacker/pe/releases)
<a href="https://projecteuler.net/recent" target="_blank">![Project Euler](https://projecteuler.net/profile/baihacker.png?)</a>

**PE** is a C++ library designed to solve problems on [Project Euler](https://projecteuler.net/recent).

## Prerequisites

To use this library, you need a C++ development environment that supports:
* C++17 or later.
* Building `x86_64` targets.

## Installation

1. **Include the Library:**
   - Place all the library files into a directory of your choice.
   - Ensure that `#include <pe.hpp>` is by adding the directory to the `CPLUS_INCLUDE_PATH` environment variable.

2. **Configure the Library:**
   - Run **[gen_config.py](https://github.com/baihacker/pe/blob/master/gen_config.py)** from the installation directory to generate **[pe_config](https://github.com/baihacker/pe/blob/master/pe_config)**.
     - This script generates a static configuration file with default values. You can manually edit this file after generation.
       - `ENABLE_ASSERT`: Enable assertions for certain inputs or conditions.
       - `TRY_TO_USE_INT128`: Check if the compiler supports `int128` and enable it. Set to `0` to disable `int128` even if supported.
     - The script also automatically detects the presence of third-party libraries and sets the appropriate flags:
       - `ENABLE_EIGEN`: Use [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page).
       - `ENABLE_GMP`: Use [GMP](https://gmplib.org).
       - `ENABLE_FLINT`: Use [FLINT](http://www.flintlib.org).
       - `ENABLE_MPFR`: Use [MPFR](https://www.mpfr.org).
       - `ENABLE_LIBBF`: Use [libbf](https://bellard.org/libbf).
       - `ENABLE_NTL`: Use [NTL](https://www.shoup.net/ntl/download.html).
       - `ENABLE_ZMQ`: Use [ZeroMQ](https://zeromq.org/).
       - `ENABLE_PRIME_COUNT`: Use [PrimeCount](https://github.com/kimwalisch/primecount).
       - `ENABLE_PRIME_SIEVE`: Use [PrimeSieve](https://github.com/kimwalisch/primesieve).
       - `ENABLE_TCMALLOC`: Use [tcmalloc](https://github.com/gperftools/gperftools).
   - Manually edit **[pe_config](https://github.com/baihacker/pe/blob/master/pe_config)** to add or modify configuration items as needed:
     - `ENABLE_OPENMP`: Enable [OpenMP](http://www.openmp.org). The script doesn't generate the default config for OpenMP.

3. **(Optional) Generate Precompiled Header:**
   - Run `g++ -xc++-header pe.hpp` in the installation directory to create a precompiled header (`pe.hpp.gch`).
   - You may add additional compiler options if required (e.g., `g++ -xc++-header pe.hpp --std=c++17 -O3 -march=native -fopenmp`).

## Usage

For a quick start, refer to [example.c](https://github.com/baihacker/pe/blob/master/example/example.c).

## File List

This list categorizes the library files by their functional modules for easier understanding and navigation.

**Core & Base**

*   `pe.hpp`: **Core header file**. Used to generate a precompiled header, it includes the main functional modules of the library.
*   `pe_base`: **Base components**. Includes standard library headers, common macros (e.g., `PE_ASSERT`), type definitions (`int64`, `uint128`), and basic inline functions.
*   `pe_internal`: **Internal implementation details**. Includes the configuration file (`pe_config`), performs compiler/platform checks, and manages the inclusion of third-party libraries.
*   `pe_config`: **Centralized configuration file**. Used to configure various features of the PE library, such as enabling third-party libraries (`ENABLE_GMP`), `int128` support, and assertions.
*   `pe_initializer`: **Library initialization helper**. Provides helper classes and macros (`PE_INIT`) to initialize library settings at program startup, such as prime table generation and parallel environment setup.
*   `pe_type_traits`: **Type trait utilities**. Offers custom type traits for compile-time type introspection.
*   `pe_time`: **Time utilities**. Provides `TimeDelta` and `TimeRecorder` for timing code execution.
*   `pe_io`: **I/O operations**. Provides simplified or accelerated I/O methods and macros, such as `ReadInt` and `PromptAnswer`.

**Data Structures**

*   `pe_array`: **Multi-dimensional arrays**. Implements compile-time (`FArray`) and runtime (`DArray`) dimensioned arrays, supporting element counts beyond `int32` limits and custom allocators.
*   `pe_vector`: **Vector operations**. Provides utilities for vector addition, subtraction, scaling, and dot product.
*   `pe_span`: **Span implementation**. Provides an implementation of `Span` to represent a non-owning view over a contiguous sequence of objects.
*   `pe_range`: **Range implementation**. Used for creating and operating on sequence ranges with a fluent API.
*   `pe_tree`: **Tree-based data structures**. Contains implementations such as Fenwick trees (`RUBit`, `RSQBit`) and Splay trees (`SplayMultiSet`).
*   `pe_persistance`: **Key-value persistence**. Offers key-value persistence functionality, which may require adjustments to support Linux.

**Integer & Numerical Computing**

*   `pe_int`: **Basic integer utilities**. Provides fundamental integer operation utility functions.
*   `pe_extended_int`: **Extended integer types**. A header that consolidates extended-precision signed and unsigned integer types.
*   `pe_extended_unsigned_int`: **Extended unsigned integers**. Implements arbitrarily large unsigned integer types.
*   `pe_extended_signed_int`: **Extended signed integers**. Implements arbitrarily large signed integer types by building upon the unsigned versions.
*   `pe_bi32`: **Big integer (Base 1<<32)**. A big integer implementation (`BigInteger`) using an array of `uint32` values.
*   `pe_mpz`: **Multi-precision integers (GMP-based)**. A wrapper for the GMP library's `mpz_t` type for high-precision integer arithmetic.
*   `pe_gbi`: **General big integer operations**. Provides a unified interface for operations on various big integer types defined in the library.
*   `pe_fraction`: **Fraction arithmetic**. Supports arithmetic operations for fractions (`Fraction`).
*   `pe_float`: **Unified float operations**. Provides unified functions for floating-point types, including `__float128`.
*   `pe_mpf`: **Multi-precision floats (GMP-based)**. A wrapper for the GMP library's `mpf_t` type for high-precision floating-point calculations.
*   `pe_mod`: **Modular arithmetic utilities**. Offers modular addition, multiplication, and exponentiation.
*   `pe_mat`: **Matrix operations**. Provides matrix multiplication, matrix-vector multiplication, and matrix power, with support for modular arithmetic.

**Algorithms**

*   `pe_algo`: **General algorithms collection**. Contains various algorithms like binary search, combinatorial functions, power sums, and prefix sums of number theory functions.
*   `pe_int_algo`: **Integer algorithms**. Algorithms for extended and general big integers, such as the Extended Euclidean Algorithm (`ExGcd`) and the Chinese Remainder Theorem (`Crt`).
*   `pe_bit`: **Bit manipulation utilities**. Provides functions for counting leading/trailing zeros (`pe_clzll`, `pe_ctzll`), population count, and determining bit width.

**Number Theory**

*   `pe_nt_base`: **Basic number theory**. Includes prime list generation, integer factorization, primality testing, and computation of Euler's totient (`phi`) and Möbius (`mu`) functions.
*   `pe_nt`: **Core number theory utilities**. An extension of `pe_nt_base`, providing more number theory functions like integer square root (`SqrtI`) and advanced primality testing (`IsPrimeEx`).
*   `pe_ntf`: **Number-theoretic functions**. Contains algorithms for prefix sums of advanced number-theoretic functions like `MuSummer`, `MuPhiSummer`, and `Sigma0Summer`.
*   `pe_db`: **Pre-calculated results database**. Loads and saves pre-calculated results, such as the prime-counting function `π(x)` and the sum of primes, to speed up computations.

**Polynomial & Fast Fourier Transform**

*   `pe_fft`: **Fast Fourier Transform**. Provides FFT utilities and polynomial multiplication.
*   `pe_poly_base`: **Basic polynomial algorithms**. Provides fundamental polynomial operations like addition, subtraction, multiplication, division, inversion, and exponentiation, serving as an entry point for various underlying implementations.
*   `pe_poly_base_flint`: **FLINT-based** polynomial algorithm implementation.
*   `pe_poly_base_gmp`: **GMP-based** polynomial algorithm implementation.
*   `pe_poly_base_libbf`: **libbf-based** polynomial algorithm implementation.
*   `pe_poly_base_min25`: Contains polynomial algorithms from user **[Min_25](https://github.com/min-25)**, including a very fast polynomial multiplication.
*   `pe_poly_base_ntl`: **NTL-based** polynomial algorithm implementation.
*   `pe_poly_base_ntt`: Provides **Number Theoretic Transform (NTT)** based polynomial multiplication with multiple implementations (`ntt32`, `ntt64`) for different modulus sizes.
*   `pe_poly_algo`: **Advanced polynomial algorithms**. Provides higher-level polynomial algorithms, such as generating Bernoulli and Stirling numbers.
*   `pe_sym_poly`: **Symbolic polynomial operations**. Supports symbolic manipulation of polynomials.

**Parallel & Distributed Computing**

*   `pe_parallel`: **Multi-threading framework**. A simple framework for multi-threaded problem-solving (Windows only).
*   `pe_parallel_algo`: **Parallel algorithms**. Contains parallel sort (`ParallelSort`) and parallel find (`ParallelFindFirst`) algorithms.
*   `pe_dpe`: **Distributed computation**. Provides a framework for distributed computing using ZeroMQ (`ENABLE_ZMQ`).

**Miscellaneous**

*   `pe_geometry`: **Geometric computations**. Supports `Point2D` and `Point3D` objects and their operations.
*   `pe_memory`: **Memory management**. Provides memory management utilities (Windows only).
*   `pe_misc`: **Miscellaneous utility functions**. Contains useful functions that don't fit into other categories, such as Gaussian elimination (`GaussianEliminationMod2`).
*   `pe_mma`: **Mathematica (MMA) support**. Provides helper methods or classes for generating MMA code, like `FRHelper`.
*   `pe_rand`: **Random number generation**. Includes random number generation utilities.
*   `pe_serialization`: **Object serialization**. Provides functionality to serialize objects into sequences of integers.




