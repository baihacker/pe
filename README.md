pe
==

It is my basic code for solving problem on project euler.
The old code has been treated as release 1.0.
The developing version is 2.0. It will be more interesting.

Installation:
-------------
* Put all the files in a folder (directory). For example, we put them in the directory D:\usr\include.
* Make sure the environment variable CPLUS_INCLUDE_PATH contain D:\usr\include.
* In [pe](https://github.com/baihacker/pe/blob/master/pe), configure this library. 
  > ENABLE_EIGEN should be 0 if you don't have the [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) library.

  > ENABLE_OPENMP should be 0 if you don't want to use [openmp](http://www.openmp.org).
  
  > ENABLE_GMP should be 0 if you don't want to use [gmp](https://gmplib.org).

* [optional, recommended] Run "g++ -xc++-header "pe.hpp" --std=c++11 -O3 -march=native -fopenmp" in D:\usr\include to generate precompile header "pe.hpp.gch". The compile options are the same as the options you will compile your target files.

Use:
----
#include <pe.hpp> in your source code.

Prerequirements:
----------------
C++11 or above

File list:
-----------
* pe: Including all the files.
* pe.hpp: The file for generating precompile header.
* pe_base: Some pre-including headers. Some macros and typedef.
* pe_bi: Big integer whose base is 1 << 30.
* pe_bi32: Big integer whose base is 1 << 32.
* pe_bn: Old big integer implementation.
* pe_geometry: Support Point2D and Point3D.
* pe_mat: Matrix operations.
* pe_memory: Memory manipulation such as allocating large memory. (windows only)
* pe_mod: Modular arithmetic.
* pe_nt: Basic code of number theory.
* pe_ntt: Number theoretic transform.
* pe_parallel: A simple framework to solve problem with multi-threads. (windows only)
* pe_parallel_algo: Parallel algorithms.
* pe_poly: Polynomial.
* pe_util: Utilities such as TimeDelta, KVPersistance. (it may support linux but we need change the generated cmdline and check the other codes)

Design principle:
--------------------
Fast and easy to use.

eg:

The factorizing interface is c++ style. It will be a little slower but it's easy to use.

[Use global variable plist (array of integers), pcnt to store the prime list and prime count, because they are frequently used. phi and miu are a c-style pointers, because they are not frequently used while vector<int> is a little heavier.] But now, the type of plist can be both array of integers and c-stype pointer, because the performance is nearly the same and sometimes we need dynamic allocation to obtain much more primes where a static allocation will make the binary not work.

Example:
--------
```cpp
#include <pe.hpp>

int main()
{
  init_primes();

  int64 result = PARALLEL_RESULT(
  BEGIN_PARALLEL
    FROM 1 TO 100000000 EACH_BLOCK_IS 10000000 CACHE ""
    THREADS 10
    MAP
      {
        return is_prime_ex(key);
      }
    REDUCE
      {
        result += value;
        return result;
      }
  END_PARALLEL);
  dbg(result);

  return 0;
}
```
