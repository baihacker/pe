pe
==

It is my basic code for solving problem on project euler.
The old code has been treated as release 1.0.
The developing version is 2.0. It will be more interesting.


File list:
-----------

* pe: Including all the files.
* pe_base: Some pre-including headers. Some macros and typedef. (c++11 or above).
* pe_util: Utilities such as TimeDelta, KVPersistance. (c++11 or above)
* pe_nt: Basic code of number theory. (c++11 or above)
* pe_threads: A simple framework to solve problem with multi-threads. (c++11 or above for class MultiThreadsTask)
* pe_bn: Big integer.
* pe_memory: Memory manipulation such as allocating large memory.


Design rules:
--------------------
Fast and easy to use.

eg:

The factorizing interface is c++ style. It will be a little slower but it's easy to use.

[Use global variable plist (array of integers), pcnt to store the prime list and prime count, because they are frequently used. phi and miu are a c-style pointers, because they are not frequently used while vector<int> is a little heavier.] But now, the type of plist can be both array of integers and c-stype pointer, because the performance is nearly the same and sometimes we need dynamic allocation to obtain much more primes where a static allocation will make the binary not work.
