#include "pe_test.h"

namespace mpf_test {
#if HAS_MPF
SL void mpf_test() {
  // cout << Mpf::getDefaultPrec() << endl;
  Mpf::setDefaultPrec(200);

  Mpf x(1);
  x /= 10;
#if 0
  cout << x.toString(20) << endl;
  x = -x.power(10);
  cout << x.toString(20) << endl;
  cout << x.toLongDouble() << endl;
  cout << Mpf().toString(20) << endl;
  cout << Mpf("-.1123456789e31").toString(30) << endl;
  cout << Mpf(".1123456789e31").toString(30) << endl;
  cout << Mpf(100).toString(30) << endl;
  mpf_t tester;
  mpf_init(tester);
  mpf_set_str(tester, "112345678987654321", 10);
  cout << Mpf("12345678987654321").toString() << endl;
  cout << mpf_get_d(tester) << endl;
  cout << mpf_get_prec(tester) << endl;
  Mpf y(1);
  y *= 123456789;
  y *= 1000000000;
  y += 123456789;
  cout << y.toString() << endl;
#endif
}

PE_REGISTER_TEST(&mpf_test, "mpf_test", SMALL);
#endif
}  // namespace mpf_test
