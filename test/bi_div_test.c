#include "pe_test.h"

namespace bi_test {
template <typename T>
SL void BiDivTestImpl(int x, int y) {
  for (int strategy = 0; strategy < 2; ++strategy)
    for (int s1 = -1; s1 <= 1; ++s1)
      for (int s2 = -1; s2 <= 1; ++s2)
        if (s2 != 0)
          for (int id = 0; id < x; ++id) {
            std::vector<int> A, B;
            if (strategy == 0) {
              for (int i = 0; i < y; ++i) {
                int t = rand() + 1;
                A.push_back(t);
                if (i & 1) {
                  B.push_back(t);
                }
              }
            } else {
              for (int i = 0; i < y; ++i) {
                A.push_back(rand() + 1);
                if (i & 1) {
                  B.push_back(rand() + 1);
                }
              }
            }
            std::string expected_result1;
            std::string expected_result2;
            {
              T a = s1;
              T b = s2;
              for (auto& iter : A) a *= iter;
              for (auto& iter : B) b *= iter;
              T c = a / b;
              T d = a % b;
              expected_result1 = ToString(c);
              expected_result2 = ToString(d);
            }
            std::string actual_result1;
            std::string actual_result2;
            {
              BigInteger a = s1;
              BigInteger b = s2;
              for (auto& iter : A) a *= iter;
              for (auto& iter : B) b *= iter;
              auto [c, d] = Div(a, b);
              actual_result1 = ToString(c);
              actual_result2 = ToString(d);
            }
            assert(expected_result1 == actual_result1);
            assert(expected_result2 == actual_result2);
          }
}

SL void BiDivTestMedium_BigInteger() { BiDivTestImpl<BigInteger>(100, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestMedium_BigInteger, "BiDivTestMedium_BigInteger",
                 MEDIUM);
#endif

SL void BiDivTestBig_BigInteger() { BiDivTestImpl<BigInteger>(10, 2000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestBig_BigInteger, "BiDivTestBig_BigInteger", BIG);
#endif

#if ENABLE_GMP
SL void BiDivTestMedium_MpInteger() { BiDivTestImpl<MpInteger>(100, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestMedium_MpInteger, "BiDivTestMedium_MpInteger",
                 MEDIUM);
#endif

SL void BiDivTestBig_MpInteger() { BiDivTestImpl<MpInteger>(10, 2000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestBig_MpInteger, "BiDivTestBig_MpInteger", BIG);
#endif
#endif

}  // namespace bi_test
