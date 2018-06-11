#include "pe_test.h"

#include "bi_test.c"
#include "comb_moder_test.c"
#include "dva_test.c"
#include "fac_mod_test.c"
#include "init_inv_test.c"
#include "mat_mul_test.c"
#include "misc_test.c"
#include "mod_test.c"
#include "nt_test.c"
#include "parallel_sort_test.c"
#include "poly_test.c"
#include "poly_algo_test.c"
#include "fft_test.c"
#include "prime_phi_sum_test.c"
#include "print_int128_test.c"
#include "square_root_test.c"
#include "tree_test.c"

static parallel_initialize_t __parallel_initializer;
static time_usage __time_usage;

PeTest& getTester() {
  static PeTest tester;
  return tester;
}

TestSize enabledTestSize[]{
//  SMALL,
//  MEDIUM,
//  BIG,
SPECIFIED,
};

SL bool isEnabledTestSize(TestSize size) {
  for (auto& iter: enabledTestSize) {
    if (iter == size) {
      return true;
    }
  }
  return false;
}

int main() {
  init_primes(1, 1);

  dbg(maxp);
  dbg(maxp2);

  auto& tester = getTester();
  const int size = tester.tests.size();
  
  bool isFirstTest = true;
  ntt::init_ntt();
  fft::init_fft();
  for (int i = 0; i < size; ++i) {
    auto& item = tester.tests[i];
    if (!isEnabledTestSize(item.testSize)) {
      continue;
    }
    if (!isFirstTest) {
      cout << endl;
    }
    cout << "Begin " << item.description << endl;
    TimeRecorder tr;
    item.test();
    cout << "End " << item.description << endl;
    cout << "Time usage " << tr.elapsed().format() << endl;
    isFirstTest = false;
  }
  return 0;
}