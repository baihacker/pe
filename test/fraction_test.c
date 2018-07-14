#include "pe_test.h"

namespace fraction_test {

SL void fraction_test() {
  auto now = from_continued_fraction<int>({1,2,3,4,5,6,7,8,9,10});
  //cout << now << endl;
  //cout << now.toDouble() << endl;
  
  //cout << to_continued_fraction(1, 2, 10) << endl;
  
  //cout << from_continued_fraction(to_continued_fraction(1,2,50)) << endl;
}

PE_REGISTER_TEST(&fraction_test, "fraction_test", SMALL);
}
