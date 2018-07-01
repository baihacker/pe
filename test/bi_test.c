#include "pe_test.h"

namespace bi_test {
SL void bi_test_small() {
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
        assert((i&j) == (a&b));
        assert((i^j) == (a^b));
        assert((i|j) == (a|b));
      }
      assert((i > j) == (bool)(a > b));
      assert((i < j) == (bool)(a < b));
      assert((i == j) == (bool)(a == b));
      assert((i >= j) == (bool)(a >= b));
      assert((i <= j) == (bool)(a <= b));
    }
}

PE_REGISTER_TEST(&bi_test_small, "bi_test_small", SMALL);

#if ENABLE_GMP
SL void bi_mul_test_impl(int x, int y) {
  for (int s1 = -1; s1 <= 1; ++s1)
  for (int s2 = -1; s2 <= 1; ++s2)
  for (int id = 0; id < x; ++id) {
    std::vector<int> A, B;
    for (int i = 0; i < y; ++i) {
      A.push_back(rand());
      B.push_back(rand());
    }
    string expectedResult;
    {
      mpz_class a = s1;
      mpz_class b = s2;
      for (auto& iter: A) a *= iter;
      for (auto& iter: B) b *= iter;
      mpz_class c = a * b;
      stringstream ss;
      ss << c;
      ss >> expectedResult;
    }
    string myResult;
    {
      bi a = s1;
      bi b = s2;
      for (auto& iter: A) a *= iter;
      for (auto& iter: B) b *= iter;
      bi c = a * b;
      stringstream ss;
      ss << c;
      ss >> myResult;
    }
    assert(expectedResult == myResult);
  }
}

SL void bi_mul_test_medium() {
  bi_mul_test_impl(1000, 500);
}

PE_REGISTER_TEST(&bi_mul_test_medium, "bi_mul_test_medium", MEDIUM);

SL void bi_mul_test_big() {
  bi_mul_test_impl(10, 10000);
}

#if !defined(TEST_ON_TRAVIS)
PE_REGISTER_TEST(&bi_mul_test_big, "bi_mul_test_big", BIG);
#endif

SL void bi_div_test_medium_impl(int x, int y) {
  for (int strategy = 0; strategy < 2; ++ strategy)
  for (int s1 = -1; s1 <= 1; ++s1)
  for (int s2 = -1; s2 <= 1; ++s2) if (s2 != 0)
  for (int id = 0; id < x; ++id) {
    std::vector<int> A, B;
    if (strategy == 0) {
      for (int i = 0; i < y; ++i) {
        int t = rand() + 1;
        A.push_back(t);
        if (i&1) {
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
    string expectedResult1;
    string expectedResult2;
    {
      mpz_class a = s1;
      mpz_class b = s2;
      for (auto& iter: A) a *= iter;
      for (auto& iter: B) b *= iter;
      mpz_class c = a / b;
      mpz_class d = a % b;
      stringstream ss;
      ss << c;
      ss >> expectedResult1;
      ss << d;
      ss >> expectedResult2;
    }
    string myResult1;
    string myResult2;
    {
      bi a = s1;
      bi b = s2;
      for (auto& iter: A) a *= iter;
      for (auto& iter: B) b *= iter;
      bi c, d;
      tie(c, d) = div(a, b);
      stringstream ss;
      ss << c;
      ss >> myResult1;
      ss << d;
      ss >> myResult2;
    }
    assert(expectedResult1 == myResult1);
    assert(expectedResult2 == myResult2);
  }
}

SL void bi_div_test_medium() {
  bi_div_test_medium_impl(100, 500);
}

#if !defined(TEST_ON_TRAVIS)
PE_REGISTER_TEST(&bi_div_test_medium, "bi_div_test_medium", MEDIUM);
#endif

SL void bi_div_test_big() {
  bi_div_test_medium_impl(10, 2000);
}

#if !defined(TEST_ON_TRAVIS)
PE_REGISTER_TEST(&bi_div_test_big, "bi_div_test_big", BIG);
#endif

#endif
}
