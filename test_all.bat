bazel clean
bazel run test:test
bazel run test:test_tcmalloc
bazel run test:test_noint128_noopenmp
bazel run test:test_int128_noopenmp
bazel run test:test_int128_openmp
bazel run test:test_noint128_openmp