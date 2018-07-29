#include "pe_test.h"

namespace mpf_test {
#if HAS_MPF
SL void mpf_test() {
  //cout << Mpf::getDefaultPrec() << endl;
  Mpf::setDefaultPrec(100);

  Mpf x(1);
  x /= 10;
  /*cout << x.toString(20) << endl;
  x = -x.power(10);
  cout << x.toString(20) << endl;
  cout << x.toLongDouble() << endl;
  cout << Mpf().toString(20) << endl;
  
  cout << Mpf("-.1123456789e31").toString(30) << endl;
  cout << Mpf(".1123456789e31").toString(30) << endl;
  cout << Mpf(100).toString(30) << endl;*/
}

PE_REGISTER_TEST(&mpf_test, "mpf_test", SMALL);
#endif
}
