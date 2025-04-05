load("//toolchain:pe_toolchain.bzl", "pe_binary", "pe_library")

package(
    default_visibility = [
        "//visibility:public",
    ],
)

pe_binary(
  name = "test_perf",
  srcs = ["test_perf.c"],
  defines = [
    "TEST_ALL",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=1",
    "ENABLE_OPENMP=1",
  ]
)

pe_binary(
  name = "test",
  srcs = ["pe_test.c"],
  defines = [
    "ENABLED_TEST=SMALL,MEDIUM,BIG,SUPER,SPECIFIED",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=1",
    "ENABLE_OPENMP=1",
  ]
)

pe_binary(
  name = "test_tcmalloc",
  srcs = ["pe_test.c"],
  defines = [
    "ENABLED_TEST=SMALL,MEDIUM,BIG,SUPER,SPECIFIED",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=1",
    "ENABLE_OPENMP=1",
    "ENABLE_TCMALLOC=1",
  ],
  libs = [
    "tcmalloc_minimal",
    "synchronization",
    "psapi",
  ]
)

pe_binary(
  name = "test_noint128_noopenmp",
  srcs = ["pe_test.c"],
  defines = [
    "ENABLED_TEST=SMALL,MEDIUM,BIG,SUPER,SPECIFIED",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=0",
    "ENABLE_OPENMP=0",
  ]
)

pe_binary(
  name = "test_int128_noopenmp",
  srcs = ["pe_test.c"],
  defines = [
    "ENABLED_TEST=SMALL,MEDIUM,BIG,SUPER,SPECIFIED",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=1",
    "ENABLE_OPENMP=0",
  ]
)

pe_binary(
  name = "test_int128_openmp",
  srcs = ["pe_test.c"],
  defines = [
    "ENABLED_TEST=SMALL,MEDIUM,BIG,SUPER,SPECIFIED",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=1",
    "ENABLE_OPENMP=1",
  ]
)

pe_binary(
  name = "test_noint128_openmp",
  srcs = ["pe_test.c"],
  defines = [
    "TEST_ALL",
    "ENABLE_ASSERT=0",
    "TRY_TO_USE_INT128=0",
    "ENABLE_OPENMP=1",
  ]
)