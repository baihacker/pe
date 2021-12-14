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
#include "parallel_sort_test.c"
#include "poly_test.c"
#include "poly_algo_test.c"
#include "poly_div_test.c"
#include "poly_mul_test.c"
#include "fft_test.c"
#include "prime_pi_sum_test.c"
#include "square_root_test.c"
#include "tree_test.c"

static TimeUsage __time_usage;

PeTest& GetTester() {
  static PeTest tester;
  return tester;
}

TestSize enabled_test_size[] {
#if defined(TEST_ALL)
  SMALL, MEDIUM, BIG, SUPER, SPECIFIED,
#else
  SPECIFIED,
#endif
};

SL bool IsEnabledTestSize(TestSize size) {
  for (auto& iter : enabled_test_size) {
    if (iter == size) {
      return true;
    }
  }
  return false;
}

int main() {
  PeInitializer()
      .set_cal_phi()
      .set_cal_mu()
      .set_fft_k()
      .set_ntt32_k()
      .set_ntt64_k()
      .Init();

  dbg(maxp);
  dbg(maxp2);

  auto& tester = GetTester();
  const int size = (int)tester.tests.size();

  bool isFirstTest = true;

  for (int i = 0; i < size; ++i) {
    auto& item = tester.tests[i];
    if (!IsEnabledTestSize(item.testSize)) {
      continue;
    }
#ifdef NO_SMALL_TEST
    if (item.testSize == SMALL) {
      continue;
    }
#endif
#ifdef NO_MEDIUM_TEST
    if (item.testSize == MEDIUM) {
      continue;
    }
#endif
#ifdef NO_BIG_TEST
    if (item.testSize == BIG) {
      continue;
    }
#endif
#ifdef NO_SUPER_TEST
    if (item.testSize == SUPER) {
      continue;
    }
#endif
    if (!isFirstTest) {
      std::cout << std::endl;
    }
    std::cout << "Begin " << item.description << std::endl;
    TimeRecorder tr;
    item.test();
    std::cout << "End " << item.description << std::endl;
    std::cout << "Time usage " << tr.Elapsed().Format() << std::endl;
    isFirstTest = false;
  }
  return 0;
}