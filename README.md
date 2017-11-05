pe
==

It is my basic code for solving problem on project euler.
The old code has been treated as release 1.0.
The developing version is 2.0. It will be more interesting.

Installation:
-------------
Put all the files in a folder (directory).
For example, we put them in the directory D:\usr\include.
Make sure CPLUS_INCLUDE_PATH contain D:\usr\include.
In pe_base, configure this library.

Prerequirements:
----------------
C++11 or above

File list:
-----------

* pe: Including all the files.
* pe_base: Some pre-including headers. Some macros and typedef.
* pe_bn: Big integer.
* pe_geometry: Support Point2D and Point3D.
* pe_mat: Matrix operations.
* pe_memory: Memory manipulation such as allocating large memory. (windows only)
* pe_mod: Modular arithmetic.
* pe_nt: Basic code of number theory.
* pe_parallel: A simple framework to solve problem with multi-threads. (windows only)
* pe_parallel_algo: Parallel algorithms.
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
const int maxp = 100000;
#include <pe>

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
