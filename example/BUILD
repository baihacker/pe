load("//toolchain:pe_toolchain.bzl", "pe_binary")

package(
    default_visibility = [
        "//visibility:public",
    ],
)

[pe_binary(name = x[:-2], srcs = [x]) for x in glob(["*.c"])]
[pe_binary(name = x[:-2] + "_gcc",
           srcs = [x],
           executable_suffix = ".out",
           enable_pe_flags = False,
           copts = [
             "-std=c++17",
             "-Wno-delete-incomplete",
             "-Wno-shift-count-overflow",
             "-O3",
             "-march=native",
             "-mtune=native",
             "-fopenmp"],
           defines = [
             "ENABLE_ASSERT=0",
             "TRY_TO_USE_INT128=1",
             "ENABLE_OPENMP=1",
             "ENABLE_EIGEN=1",
             "ENABLE_GMP=1",
             "ENABLE_FLINT=1",
             "ENABLE_MPFR=1",
             "ENABLE_NTL=1",
             "ENABLE_ZMQ=0",
             "ENABLE_LIBBF=0",
             "ENABLE_PRIME_COUNT=0",
             "ENABLE_PRIME_SIEVE=0",
             "TEST_ALL",
             "CONTINUOUS_INTEGRATION_TEST",
             "NO_SUPER_TEST"],
           linkopts = [
             "-fopenmp",
             "-lflint",
             "-lmpfr",
             "-lntl",
             "-lgmp"],
           cc_path = "g++",
           ) for x in glob(["*.c"])]

filegroup(name ="gcc_builds", srcs = [x[:-2] + "_gcc" for x in glob(["*.c"])])