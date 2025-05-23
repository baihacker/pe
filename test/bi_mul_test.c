#include "pe_test.h"

namespace bi_test {
template <typename T>
SL void BiMulTestImpl(int x, int y) {
  for (int s1 = -1; s1 <= 1; ++s1)
    for (int s2 = -1; s2 <= 1; ++s2)
      for (int id = 0; id < x; ++id) {
        std::vector<int> A, B;
        for (int i = 0; i < y; ++i) {
          A.push_back(rand());
          B.push_back(rand());
        }
        std::string expected_result;
        {
          T a = s1;
          T b = s2;
          for (auto& iter : A) a *= iter;
          for (auto& iter : B) b *= iter;

          T c = a * b;
          expected_result = ToString(c);
        }
        std::string actual_result;
        {
          BigInteger a = s1;
          BigInteger b = s2;
          for (auto& iter : A) a *= iter;
          for (auto& iter : B) b *= iter;
          BigInteger c = a * b;
          std::stringstream ss;
          ss << c;
          ss >> actual_result;
        }
        assert(expected_result == actual_result);
      }
}

SL void BiMulTestMedium_BigInteger() { BiMulTestImpl<BigInteger>(1000, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestMedium_BigInteger, "BiMulTestMedium_BigInteger",
                 MEDIUM);
#endif

SL void BiMulTestBig_BigInteger() { BiMulTestImpl<BigInteger>(10, 10000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestBig_BigInteger, "BiMulTestBig_BigInteger", BIG);
#endif

#if ENABLE_GMP
SL void BiMulTestMedium_MpInteger() { BiMulTestImpl<MpInteger>(1000, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestMedium_MpInteger, "BiMulTestMedium_MpInteger",
                 MEDIUM);
#endif

SL void BiMulTestBig_MpInteger() { BiMulTestImpl<MpInteger>(10, 10000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestBig_MpInteger, "BiMulTestBig_MpInteger", BIG);
#endif
#endif
}  // namespace bi_test
