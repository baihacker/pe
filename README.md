pe
==

It is my basic code for solving problem on project euler.
The old code has been treated as release 1.0.
The developing version is 2.0. It will be more interesting.


File list:
-----------

* pe: Including all the files.
* pe_base: Some pre-including headers. Some macros and typedef.
* pe_nt: Basic code of number theory.
* pe_threads: A simple framework to solve problem with multi-threads.
* pe_bn: Big integer.
* pe_util: Utilities such as TimeDelta, KVPersistance.

Design rules:
--------------------
Fast and easy to use.

eg:

The factorizing interface is c++ style. It will be a little slower but it's easy to use.

Use global variable plist (array of integer), pcnt to store the prime list and prime count, because they are frequently used. phi and miu are a c-style pointers, because they are not frequently used while vector<int> is a little heavier.
