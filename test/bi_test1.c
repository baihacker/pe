#include "pe_test.h"

namespace bi_test {
SL void BiTestSmall() {
  for (int i = -100; i <= 100; ++i)
    for (int j = -100; j <= 100; ++j) {
      bi a(i), b(j);
      assert(i + j == a + b);
      assert(i - j == a - b);
      assert(i * j == a * b);
      if (j != 0) {
        assert(i / j == a / b);
        assert(i % j == a % b);
      }
      if (i >= 0 && j >= 0) {
        assert((i & j) == (a & b));
        assert((i ^ j) == (a ^ b));
        assert((i | j) == (a | b));
      }
      assert((i > j) == (bool)(a > b));
      assert((i < j) == (bool)(a < b));
      assert((i == j) == (bool)(a == b));
      assert((i >= j) == (bool)(a >= b));
      assert((i <= j) == (bool)(a <= b));
    }
}

PE_REGISTER_TEST(&BiTestSmall, "BiTestSmall", SMALL);

#if ENABLE_GMP
SL void BiMulTestImpl(int x, int y) {
  for (int s1 = -1; s1 <= 1; ++s1)
    for (int s2 = -1; s2 <= 1; ++s2)
      for (int id = 0; id < x; ++id) {
        std::vector<int> A, B;
        for (int i = 0; i < y; ++i) {
          A.push_back(rand());
          B.push_back(rand());
        }
        std::string expectedResult;
        {
          mpz_class a = s1;
          mpz_class b = s2;
          for (auto& iter : A) a *= iter;
          for (auto& iter : B) b *= iter;
          mpz_class c = a * b;
          std::stringstream ss;
          ss << c;
          ss >> expectedResult;
        }
        std::string myResult;
        {
          bi a = s1;
          bi b = s2;
          for (auto& iter : A) a *= iter;
          for (auto& iter : B) b *= iter;
          bi c = a * b;
          std::stringstream ss;
          ss << c;
          ss >> myResult;
        }
        assert(expectedResult == myResult);
      }
}

SL void BiMulTestMedium() { BiMulTestImpl(1000, 500); }

PE_REGISTER_TEST(&BiMulTestMedium, "BiMulTestMedium", MEDIUM);

SL void BiMulTestBig() { BiMulTestImpl(10, 10000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestBig, "BiMulTestBig", BIG);
#endif

SL void BiDivTestMediumImpl(int x, int y) {
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
            std::string expectedResult1;
            std::string expectedResult2;
            {
              mpz_class a = s1;
              mpz_class b = s2;
              for (auto& iter : A) a *= iter;
              for (auto& iter : B) b *= iter;
              mpz_class c = a / b;
              mpz_class d = a % b;
              std::stringstream ss;
              ss << c;
              ss >> expectedResult1;
              ss << d;
              ss >> expectedResult2;
            }
            std::string myResult1;
            std::string myResult2;
            {
              bi a = s1;
              bi b = s2;
              for (auto& iter : A) a *= iter;
              for (auto& iter : B) b *= iter;
              bi c, d;
              std::tie(c, d) = Div(a, b);
              std::stringstream ss;
              ss << c;
              ss >> myResult1;
              ss << d;
              ss >> myResult2;
            }
            assert(expectedResult1 == myResult1);
            assert(expectedResult2 == myResult2);
          }
}

SL void BiDivTestMedium() { BiDivTestMediumImpl(100, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestMedium, "BiDivTestMedium", MEDIUM);
#endif

SL void BiDivTestBig() { BiDivTestMediumImpl(10, 2000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestBig, "BiDivTestBig", BIG);
#endif

#endif
}  // namespace bi_test
