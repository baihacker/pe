#include "pe_test.h"

#include "bi_test.c"
#include "dva_test.c"
#include "gf_test.c"
#include "init_inv_test.c"
#include "mat_mul_test.c"
#include "mp_test.c"
#include "parallel_sort_test.c"
//#include "prime_phi_sum_test.c"
#include "print_int128_test.c"
#include "square_root_test.c"

static parallel_initialize_t __parallel_initializer;
static time_usage __time_usage;

PeTest& getTester() {
  static PeTest tester;
  return tester;
}

int main() {
  init_primes();

  dbg(maxp);
  dbg(maxp2);

  auto& tester = getTester();
  const int size = tester.tests.size();
  
  for (int i = 0; i < size; ++i) {
    auto& item = tester.tests[i];
    cout << "Begin " << item.description << endl;
    TimeRecorder tr;
    item.test();
    cout << "End " << item.description << endl;
    cout << "Time usage " << tr.elapsed().format() << endl;
    if (i < size - 1) {
      cout << endl;
    }
  }
  return 0;
}