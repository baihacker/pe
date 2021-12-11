#include "pe_test.h"

namespace mpf_test {
#if HAS_MPF
SL void MpfTest() {
  // std::cout << Mpf::getDefaultPrec() << std::endl;
  Mpf::SetDefaultPrec(200);

  Mpf x(1);
  x /= 10;
#if 0
  std::cout << x.toString(20) << std::endl;
  x = -x.Power(10);
  std::cout << x.toString(20) << std::endl;
  std::cout << x.toLongDouble() << std::endl;
  std::cout << Mpf().toString(20) << std::endl;
  std::cout << Mpf("-.1123456789e31").toString(30) << std::endl;
  std::cout << Mpf(".1123456789e31").toString(30) << std::endl;
  std::cout << Mpf(100).toString(30) << std::endl;
  mpf_t tester;
  mpf_init(tester);
  mpf_set_str(tester, "112345678987654321", 10);
  std::cout << Mpf("12345678987654321").toString() << std::endl;
  std::cout << mpf_get_d(tester) << std::endl;
  std::cout << mpf_get_prec(tester) << std::endl;
  Mpf y(1);
  y *= 123456789;
  y *= 1000000000;
  y += 123456789;
  std::cout << y.toString() << std::endl;
#endif
}

PE_REGISTER_TEST(&MpfTest, "MpfTest", SMALL);
#endif
}  // namespace mpf_test
