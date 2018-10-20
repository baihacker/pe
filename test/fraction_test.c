#include "pe_test.h"

namespace fraction_test {

SL void fraction_test() {
  /*vi data{1,2,2,2,2,2,2,2,2,2};
  for (int i = 0; i < 10; ++i) {
    cout << from_continued_fraction<int>(data, i) << endl;
  }
  for (int i = 50; i <= 50; ++i) {
  cout << i << " " << to_continued_fraction(1, 6, i) << endl;
  }*/
  // cout << now << endl;
  // cout << now.toDouble() << endl;

  // cout << to_continued_fraction(1, 2, 10) << endl;

  // cout << from_continued_fraction(to_continued_fraction(1,2,50)) << endl;
}

PE_REGISTER_TEST(&fraction_test, "fraction_test", SMALL);
}  // namespace fraction_test
