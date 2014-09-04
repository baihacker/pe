pe
==
It is my basic code for solve problem on project euler.
The old code has been treat as release 1.0.
The developing version is 2.0. It will be more interesting.

files list:
pe_base: some pre-including headers. some macros and typedef.
pe_nt: basic code of number theory.
pe_threads: a simple framework to solve problem with multi-threads.

design desitination:
fast and easy to use.

eg.
I change the factorizing interface to c++ style. It will be a little slower but it's eaty to use.
I keep the global variable plist, pcnt, because it is frequently used. phi and miu is a c pointer, because
they are not frequently used and vector<int> is a little heavier.
