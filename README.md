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

#### File List

*   **pe**: Contains all implementation files.
*   **pe.hpp**: Header file for generating the precompiled header. It includes the core library components required for most functionalities.
*   **pe_algo**: Contains various algorithms, including binary search, combinatorial calculations (`CombModer`, `CombModerEx`), modular factorial (`FactModer`), modular power sum (`PowerSumModer`), and advanced algorithms for number theory problems like counting Pythagorean triples and perfect powers.
*   **pe_array**: Implements compile-time (`FArray`) and runtime (`DArray`) dimensioned arrays. Supports element counts exceeding `int32` limits and allows for custom allocators.
*   **pe_base**: A foundational header that pre-includes standard C++ library headers, defines common macros (like `PE_ASSERT`), typedefs for integer types (`int64`, `uint128`), and basic inline utility functions.
*   **pe_bi32**: Provides a big integer implementation (`BigInteger`) with a base of 1 << 32, using `uint32` arrays for storage. It supports standard arithmetic, bitwise operations, and conversion from native integer types and strings.
*   **pe_bit**: A utility for bit manipulation, offering functions to count leading/trailing zeros (`pe_clzll`, `pe_ctzll`), population count (`pe_popcountll`), and determine bit width for various integer types, including `__int128`.
*   **pe_config**: A centralized configuration file for the PE library. It contains macros to enable or disable features like `int128` support and third-party libraries such as GMP and FLINT.
*   **pe_db**: Provides functionality to load and save pre-calculated results, such as the prime-counting function (`PrimePi`) and the sum of primes (`PrimeSum`), from a database to speed up computations.
*   **pe_dpe**: A framework for distributed computation, leveraging ZeroMQ (`ENABLE_ZMQ`) for communication between a server and clients to solve tasks in parallel.
*   **pe_extended_int**: A header file that includes and consolidates extended-precision signed and unsigned integer types (`ExtendedSignedIntImpl`, `ExtendedUnsignedIntImpl`).
*   **pe_extended_signed_int**: Implements extended-precision signed integer types by building upon the unsigned versions, providing support for larger-than-native integer arithmetic.
*   **pe_extended_unsigned_int**: Implements extended-precision unsigned integer types (`ExtendedUnsignedIntImpl`) that can be larger than standard 64-bit integers.
*   **pe_fft**: Contains utilities for Fast Fourier Transform and polynomial multiplication, offering multiple implementations based on different precision and performance trade-offs.
*   **pe_float**: Provides unified functions for floating-point operations, including support for the `__float128` type if available.
*   **pe_fraction**: Supports arithmetic operations for fractions, templated to work with various integer types.
*   **pe_gbi**: Defines a concept for General Big Integers (`is_gbi_v`) and provides a unified interface for operations on different big integer types, such as those from GMP or custom implementations.
*   **pe_geometry**: Provides support for 2D (`Point2D`) and 3D (`Point3D`) geometric points and related operations like dot and cross products.
*   **pe_initializer**: Offers helper classes and macros (`PE_INIT`) for initializing library components at program startup, such as generating prime lists or configuring parallel execution environments.
*   **pe_int**: Contains basic utility functions for integer types, including modular arithmetic helpers and functions like `Gcd` and `Lcm`.
*   **pe_int_algo**: Provides integer algorithms specifically for extended integer types (`is_extended_integer_v`) and general big integers (`is_gbi_v`), such as the Extended Euclidean Algorithm (`ExGcd`) and the Chinese Remainder Theorem (`Crt`).
*   **pe_internal**: An internal header that includes the `pe_config` file, performs compiler and platform checks, and manages the inclusion of third-party libraries.
*   **pe_io**: Contains methods and macros for simplified or accelerated I/O operations, such as fast integer reading (`ReadInt`) and a helper for printing answers (`PromptAnswer`).
*   **pe_mat**: Provides functions for matrix operations, including matrix multiplication (`MatMulMat`), matrix-vector multiplication (`MatMulVec`), and their modular arithmetic versions.
*   **pe_memory**: Contains memory management utilities, including a `LargeMemory` allocator for Windows environments.
*   **pe_misc**: A collection of miscellaneous utility functions that do not fit into other categories, such as Gaussian elimination (`GaussianEliminationMod2`).
*   **pe_mma**: Includes helper methods and classes for generating Mathematica (MMA) code, such as finding recurrence formulas (`FRHelper`).
*   **pe_mod**: Offers utilities for modular arithmetic, such as modular addition (`AddMod`), multiplication (`MulMod`), and exponentiation (`PowerMod`).
*   **pe_mpf**: A wrapper for multi-precision floating-point numbers based on the GMP library (`mpf_t`), enabled by `ENABLE_GMP`.
*   **pe_mpz**: A wrapper for multi-precision integers based on the GMP library (`mpz_t`), enabled by `ENABLE_GMP`.
*   **pe_nt**: Core number theory utilities, extending `pe_nt_base`. It includes functions like integer square root (`SqrtI`), prime factorization (`Factorize`), and primality testing (`IsPrimeEx`).
*   **pe_nt_base**: Basic number theory functionalities, including prime list generation (`InitPrimes`), integer factorization, and computation of number-theoretic functions like Euler's totient (`phi`) and Möbius (`mu`).
*   **pe_ntf**: Provides implementations for calculating prefix sums of various number-theoretic functions, such as the sum of `mu` (`MuSummer`) and the sum of `sigma0` (`Sigma0Summer`).
*   **pe_parallel**: A simple framework for multi-threaded problem-solving (currently Windows only).
*   **pe_parallel_algo**: Contains parallel algorithms such as parallel sort (`ParallelSort`) and parallel find (`ParallelFindFirst`), which require OpenMP to be enabled.
*   **pe_persistance**: Offers key-value persistence functionality, which may require adjustments to support Linux.
*   **pe_poly_algo**: Contains advanced polynomial algorithms, such as generating Bernoulli numbers (`GenBernoulliNumber`) and Stirling numbers (`GenStirling1`, `GenStirling2`).
*   **pe_poly_base**: Provides basic polynomial algorithms and serves as a hub for including various underlying implementations (e.g., from FLINT, NTL) for polynomial operations.
*   **pe_poly_base_flint**: Implements polynomial algorithms using the FLINT library, enabled by `ENABLE_FLINT`.
*   **pe_poly_base_gmp**: Implements polynomial algorithms based on the GMP library, enabled by `ENABLE_GMP`.
*   **pe_poly_base_libbf**: Implements polynomial algorithms based on the libbf library, enabled by `ENABLE_LIBBF`.
*   **pe_poly_base_min25**: Contains polynomial algorithms from user [Min_25](https://github.com/min-25), noted for including the fastest polynomial multiplication.
*   **pe_poly_base_ntl**: Implements polynomial algorithms using the NTL library, enabled by `ENABLE_NTL`.
*   **pe_poly_base_ntt**: Provides Number Theoretic Transform (NTT) based polynomial multiplication, with multiple implementations (`ntt32`, `ntt64`) optimized for different modulus sizes and coefficient ranges.
*   **pe_rand**: Provides random number generation utilities.
*   **pe_range**: An implementation for creating and manipulating ranges, offering a fluent API for operations like `Filter`, `Map`, and `Reduce`.
*   **pe_serialization**: Provides functionality to serialize and deserialize objects into sequences of integers.
*   **pe_span**: An implementation of a `Span` class, which provides a non-owning view over a contiguous sequence of objects.
*   **pe_sym_poly**: Supports symbolic polynomial operations, allowing for manipulation of polynomials with variables as symbols.
*   **pe_time**: Contains time-related utilities like `TimeDelta` for measuring durations and `TimeRecorder` for timing code execution.
*   **pe_tree**: Implements tree-based data structures, such as Fenwick trees (`RUBit`, `RSQBit`) and Splay trees (`SplayMultiSet`).
*   **pe_type_traits**: Provides custom type trait utilities for compile-time type introspection.
*   **pe_vector**: Offers utilities for vector operations like addition, subtraction, scaling, and dot product.

