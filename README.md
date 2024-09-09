# PE: C++ Library for Project Euler

[![Build Status (Windows/MSVC)](https://ci.appveyor.com/api/projects/status/scaji00tde2gb7uy?svg=true)](https://ci.appveyor.com/project/baihacker/pe-win-msvc)
[![Build Status (Ubuntu/GCC)](https://ci.appveyor.com/api/projects/status/9bt606nax24anyen?svg=true)](https://ci.appveyor.com/project/baihacker/pe-ubuntu-gcc)
[![Build Status (Ubuntu/Clang)](https://ci.appveyor.com/api/projects/status/nw243uvs95i0bj85?svg=true)](https://ci.appveyor.com/project/baihacker/pe-ubuntu-clang)
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
   - Ensure that `#include <pe.hpp>` is correctly recognized.
   - Add the directory to the `CPLUS_INCLUDE_PATH` environment variable or use an alternative method to include the files.

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
   - Manually edit **[pe_config](https://github.com/baihacker/pe/blob/master/pe_config)** to add or modify configuration items as needed:
     - `ENABLE_OPENMP`: Enable [OpenMP](http://www.openmp.org). This flag is not set by the script and must be defined manually if needed. If enabled but not supported, a warning will be displayed.

3. **(Optional) Generate Precompiled Header:**
   - Run `g++ -xc++-header pe.hpp` in the installation directory to create a precompiled header (`pe.hpp.gch`).
   - You may add additional compiler options if required (e.g., `g++ -xc++-header pe.hpp --std=c++17 -O3 -march=native -fopenmp`).

## Usage

For a quick start, refer to [example.c](https://github.com/baihacker/pe/blob/master/example/example.c).

## File List

- **pe**: Contains all implementation files.
- **pe.hpp**: Header file for generating the precompiled header. Includes the core library.
- **pe_algo**: Contains various algorithms.
- **pe_array**: Array implementation with compile-time dimension length. Supports element counts exceeding `int32` limits and custom allocators.
- **pe_base**: Pre-included headers, macros, typedefs, and basic inline functions.
- **pe_bi32**: Big integer implementation with base `1 << 32`.
- **pe_bit**: Bit manipulation utilities.
- **pe_config**: Centralized configuration file for PE.
- **pe_db**: Load and save pre-calculated results, such as prime pi and prime sum.
- **pe_extended_int**: Extended integer types.
- **pe_extended_signed_int**: Extended signed integer types.
- **pe_extended_unsigned_int**: Extended unsigned integer types.
- **pe_fft**: Fast Fourier Transform and polynomial multiplication utilities.
- **pe_float128**: Functions for unified `__float128` operations.
- **pe_fraction**: Fraction arithmetic operations.
- **pe_gbi**: General big integer operations, corresponding to `pe_nt`.
- **pe_geometry**: Support for `Point2D` and `Point3D`.
- **pe_initializer**: Helper classes and macros for library initialization.
- **pe_int**: Basic integer utilities.
- **pe_int128**: Support for `int128` and corresponding type traits.
- **pe_internal**: Includes configuration, defines necessary types/macros, and third-party libraries.
- **pe_io**: Methods and macros for simplified or accelerated I/O operations.
- **pe_mat**: Matrix operations.
- **pe_memory**: Memory management utilities (Windows only).
- **pe_misc**: Miscellaneous utility functions.
- **pe_mma**: Support for MMA: helper methods or classes for MMA code generation.
- **pe_mod**: Modular arithmetic utilities.
- **pe_nt**: Core number theory utilities.
- **pe_nt_base**: Prime list generation, integer factorization, prime testing, and computations of φ and μ.
- **pe_parallel**: Simple framework for multi-threaded problem-solving (Windows only).
- **pe_parallel_algo**: Parallel algorithms.
- **pe_persistance**: Key-Value Persistence (may support Linux with adjustments).
- **pe_poly**: Polynomial wrapper for C++.
- **pe_poly_algo**: Polynomial algorithms.
- **pe_poly_base**: Basic polynomial algorithms.
- **pe_poly_base_flint**: Polynomial algorithms based on FLINT.
- **pe_poly_base_libbf**: Polynomial algorithms based on libbf.
- **pe_poly_base_min25**: Polynomial algorithms from [Min_25](https://github.com/min-25), including the fastest polynomial multiplication.
- **pe_poly_base_ntl**: Polynomial algorithms based on NTL.
- **pe_rand**: Random number generation utilities.
- **pe_range**: Range generation and container iteration with indices.
- **pe_span**: Implementation of `Span`.
- **pe_sym_poly**: Symbolic polynomial operations.
- **pe_time**: Utilities for `TimeDelta` and `TimeRecorder`.
- **pe_tree**: Tree-based data structures.
- **pe_type_traits**: Type trait utilities.
