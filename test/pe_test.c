#include "pe_test.h"

#include "algo_test.c"
#include "array_test.c"
#include "bi_test.c"
#include "bi_test1.c"
#include "bit_test.c"
#include "comb_moder_test.c"
#include "dva_test.c"
#include "fact_ppower_mod_test.c"
#include "init_inv_test.c"
#include "int128_test.c"
#include "mat_mul_test.c"
#include "misc_test.c"
#include "mod_test.c"
#include "mpf_test.c"
#include "mpi_test.c"
#include "nt_test.c"
#include "ntt_test.c"
#include "parallel_sort_test.c"
#include "poly_test.c"
#include "poly_algo_test.c"
#include "fft_test.c"
#include "prime_pi_sum_test.c"
#include "square_root_test.c"
#include "tree_test.c"

static time_usage __time_usage;

PeTest& getTester() {
  static PeTest tester;
  return tester;
}

TestSize enabledTestSize[] {
#if defined(TEST_ALL)
  SMALL, MEDIUM, BIG, SPECIFIED,
#else
  SPECIFIED,
#endif
};

SL bool isEnabledTestSize(TestSize size) {
  for (auto& iter : enabledTestSize) {
    if (iter == size) {
      return true;
    }
  }
  return false;
}

int main() {
  pe().calPhi().calMu().fft().ntt32().ntt64().init();

  dbg(maxp);
  dbg(maxp2);

  auto& tester = getTester();
  const int size = (int)tester.tests.size();

  bool isFirstTest = true;

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