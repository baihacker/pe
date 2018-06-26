# Benchmark

## Ntt benchmark

### Test Environment:

* OS:  Win10 Pro 1803
* CPU: i7-4790K (MMX, SSE, SSE2, SSE3, SSE4.1, SSE4.2, EM64T, VT-x, AES, AVX, AVX2, FMA3)
* Compiler: MinGW-x86_64-8.1.0-win32-seh-rt_v6-rev0
* Msys2: msys2-x86_64-20180531
* Build libbf:
 > gcc -Wall -O3 -mavx -mavx2 -mfma -mbmi2 -c -o libbf.avx2.o libbf.c
 
 > gcc -Wall -O3 -mavx -mavx2 -mfma -mbmi2 -c -o cutils.avx2.o cutils.c
 
 > gcc-ar crv libbf.avx2.a cutils.avx2.o libbf.avx2.o
* Run tests:
 > -o a.exe --std=c++11 -O3 -march=native -mtune=native -fopenmp -Wl,--stack,268435456 -lbf -lgmpxx -lflint -lgmp -lmpfr -lmpir
* Test code: [Ntt test](https://github.com/baihacker/pe/blob/master/test/ntt_test.c)

### Test result

#### Openmp enabled
```cpp
ntt test : data = random, size = 0, n = 1000000, mod = 100019
flint n  : 0.409
flint p  : 1.145
ntt32 s  : 1.002
ntt32 l  : 1.170
ntt64 s  : 1.581
ntt64 l  : 1.683
Min_25 s : 0.157
Min_25 l : 0.178
libbf    : 0.819
ntt test : data = random, size = 1, n = 1479725, mod = 100000000003
flint n  : 1.232
flint p  : 2.923
ntt32 l  : 2.378
ntt64 l  : 3.467
Min_25 l : 0.343
libbf    : 1.362
ntt test : data = random, size = 2, n = 1000000, mod = 316227766016779
flint n  : 0.840
flint p  : 2.081
ntt64 l  : 1.709
Min_25 l : 0.182
libbf    : 0.866
ntt test : data = max mod, size = 0, n = 999996, mod = 1000003
flint n  : 0.418
flint p  : 1.231
ntt32 s  : 1.011
ntt32 l  : 1.160
ntt64 s  : 1.568
ntt64 l  : 1.670
Min_25 s : 0.157
Min_25 l : 0.185
libbf    : 0.821
ntt test : data = max mod, size = 1, n = 1479725, mod = 100000000003
flint n  : 1.235
flint p  : 2.778
ntt32 l  : 2.373
ntt64 l  : 3.375
Min_25 l : 0.348
libbf    : 1.360
ntt test : data = max mod, size = 2, n = 1000000, mod = 316227766016779
flint n  : 0.843
flint p  : 2.086
ntt64 l  : 1.663
Min_25 l : 0.184
libbf    : 0.865
```

#### Openmp disabled (option "-fopenmp" removed)
```cpp
ntt test : data = random, size = 0, n = 1000000, mod = 100019
flint n  : 0.405
flint p  : 1.159
ntt32 s  : 3.135
ntt32 l  : 4.723
ntt64 s  : 2.509
ntt64 l  : 5.001
Min_25 s : 0.168
Min_25 l : 0.360
libbf    : 0.818
ntt test : data = random, size = 1, n = 1479725, mod = 100000000003
flint n  : 1.236
flint p  : 2.956
ntt32 l  : 9.907
ntt64 l  : 10.506
Min_25 l : 0.744
libbf    : 1.361
ntt test : data = random, size = 2, n = 1000000, mod = 316227766016779
flint n  : 0.831
flint p  : 2.084
ntt64 l  : 5.077
Min_25 l : 0.422
libbf    : 0.860
ntt test : data = max mod, size = 0, n = 999996, mod = 1000003
flint n  : 0.421
flint p  : 1.224
ntt32 s  : 3.126
ntt32 l  : 4.723
ntt64 s  : 2.465
ntt64 l  : 4.992
Min_25 s : 0.174
Min_25 l : 0.355
libbf    : 0.825
ntt test : data = max mod, size = 1, n = 1479725, mod = 100000000003
flint n  : 1.229
flint p  : 2.760
ntt32 l  : 9.954
ntt64 l  : 10.497
Min_25 l : 0.728
libbf    : 1.368
ntt test : data = max mod, size = 2, n = 1000000, mod = 316227766016779
flint n  : 0.839
flint p  : 2.081
ntt64 l  : 5.037
Min_25 l : 0.428
libbf    : 0.862
```
