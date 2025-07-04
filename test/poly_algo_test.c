#include "pe_test.h"

namespace poly_algo_test {
const int64 mod = 1000000007;

SL void PolyMultiPointEvaluationTest() {
  srand(123456789);
  std::vector<int64> data;
  int n = 5000;
  const int64 mod = 1000000007;
  for (int i = 1; i <= n; ++i) data.push_back(i);
  std::vector<int64> v;
  for (int i = 1; i <= n; ++i) v.push_back(i % 10007);
  {
    TimeRecorder tr;
    std::vector<int64> result = PolyMultipointEvaluateNormal(data, v, mod);
    // std::cout << tr.Elapsed().Format() << std::endl;
    for (int i = 1; i <= n; ++i) {
      int64 value = PolyEvaluate<int64>(data, i % 10007, mod);
      assert(value == result[i - 1]);
    }
  }
  {
    TimeRecorder tr;
    std::vector<int64> result = PolyMultipointEvaluateBls(data, v, mod);
    // std::cout << tr.Elapsed().Format() << std::endl;
    for (int i = 1; i <= n; ++i) {
      int64 value = PolyEvaluate<int64>(data, i % 10007, mod);
      assert(value == result[i - 1]);
    }
  }
#if HAS_POLY_FLINT
  {
    TimeRecorder tr;
    std::vector<int64> result = flint::PolyMultipointEvaluate(data, v, mod);
    // std::cout << tr.Elapsed().Format() << std::endl;
    for (int i = 1; i <= n; ++i) {
      int64 value = PolyEvaluate<int64>(data, i % 10007, mod);
      assert(value == result[i - 1]);
    }
  }
#endif
}
PE_REGISTER_TEST(&PolyMultiPointEvaluationTest, "PolyMultiPointEvaluationTest",
                 SMALL);

SL void PolyBatchMulTest() {
  const int mod = 10007;
  std::vector<int64> data = {1, 1, 2, 1, 3, 1};
  std::vector<int64> result = PolyBatchMul(data, mod);

  std::vector<int64> expected = {6, 11, 6, 1};
  assert(expected == result);
}
PE_REGISTER_TEST(&PolyBatchMulTest, "PolyBatchMulTest", SMALL);

SL void GenBernoulliNumberTest() {
  const int mod = 10007;
  assert((GenBernoulliNumber(7, mod) ==
          std::vector<int64>{1, 5003, 1668, 0, 7672, 0, 4527, 0}));
}
PE_REGISTER_TEST(&GenBernoulliNumberTest, "GenBernoulliNumberTest", SMALL);

SL void GenStirling1ColumnTest() {
  const int mod = 10007;
  assert((pmod::GenStirling1Column(3, 10, mod) ==
          std::vector<int64>{0, 0, 0, 1, 6, 35, 225, 1624, 3125, 8047, 1881}));
}
PE_REGISTER_TEST(&GenStirling1ColumnTest, "GenStirling1ColumnTest", SMALL);

SL void GenStirling1Test() {
  const int mod = 10007;
  assert((GenStirling1(7, mod) ==
          std::vector<int64>{0, 720, 1764, 1624, 735, 175, 21, 1}));
}
PE_REGISTER_TEST(&GenStirling1Test, "GenStirling1Test", SMALL);

SL void GenStirling2Test() {
  const int mod = 10007;
  assert((pmod::GenStirling2(7, mod) ==
          std::vector<int64>{0, 1, 63, 301, 350, 140, 21, 1}));
}
PE_REGISTER_TEST(&GenStirling2Test, "GenStirling2Test", SMALL);

SL void GetGFCoefficientTest() {
  {
    // Fibonacci sequence
    std::vector<int64> A = {1, -1, -1};
    std::vector<int64> B = {0, 1};
    std::vector<int64> result = {0, 1};
    for (int i = 2; i <= 30; ++i) {
      result.push_back(AddMod(result[i - 2], result[i - 1], mod));
    }
    std::vector<int64> x = GetGFCoefficientSeries(A, B, 30, mod);
    for (int i = 0; i <= 30; ++i) {
      assert(result[i] == x[i]);
    }
  }

  {
    // Dollar exchange.
    // Concret math
    // 7 Generating Functions
    // 7.3 Solving Recurrences
    // Example 4: A closed form for change.
    int64 dp[10000 + 1] = {1};
    int64 can[5] = {1, 5, 10, 25, 50};
    for (int64 each : can) {
      for (int j = 0; j + each <= 10000; ++j) {
        if (dp[j]) {
          dp[j + each] = AddMod(dp[j + each], dp[j], mod);
        }
      }
    }

    int64 coe[100] = {0};
    for (int i = 0; i < 1 << 5; ++i) {
      int s = 0;
      int bc = 0;
      for (int j = 0; j < 5; ++j) {
        if (i & (1 << j)) ++bc, s += (int)can[j];
      }
      if (bc & 1) {
        --coe[s];
      } else {
        ++coe[s];
      }
    }
    std::vector<int64> gfresult = GetGFCoefficientSeries(
        std::vector<int64>(coe, coe + 92), {1}, 10000, mod);
    for (int i = 0; i <= 10000; ++i) assert(dp[i] == gfresult[i]);

    std::string mine = ToString(GetGFCoefficientAt(
        std::vector<int64>(coe, coe + 92), {1}, 100000000, mod));
    std::string expected = ToString("66666793333412666685000001"_bi % mod);
    assert(mine == expected);
  }
}
PE_REGISTER_TEST(&GetGFCoefficientTest, "GetGFCoefficientTest", SMALL);

SL void LinearRecurrenceTest() {
  const int64 P = 1000000009;
  std::vector<int64> s = {0, 1, 1, 2, 3, 5};
  std::vector<int64> v = *FindLinearRecurrence(s, P);
  assert(v[0] == P - 1);
  assert(v[1] == P - 1);
  assert(v[2] == 1);
  const int n = static_cast<int>(std::size(v));
  int64 ans = 0;
  for (int i = 0; i < n; ++i) ans += v[i] * s[i];
  assert(ans == P);

  ans = LinearRecurrenceValueAt(v, s, 38, P);
  assert(ans == 39088169LL);

  std::vector<int64> t = *FindLinearRecurrence({0, 1, 1, 2, 3, 5, 8, 13}, 31);
  assert(t[0] == 30);
  assert(t[1] == 30);
  assert(t[2] == 1);
  assert(*FindLinearRecurrenceValueAt({0, 1, 1, 2, 3, 5, 8, 13}, 38, P) ==
         39088169);
}
PE_REGISTER_TEST(&LinearRecurrenceTest, "LinearRecurrenceTest", SMALL);

SL void SeqExprTest() {
  {
    Sequence a;
    (void)a;
    assert((a[1] + a[2]).ValueAt({0, 1}, 20, mod) == 6765);
    assert((a[1] + a[2]).ValueAtWithCharPoly({0, 1}, 20, mod) == 6765);
    assert((a[1] + a[2]).SumAt({0, 1}, 20, mod) == 17710);
    assert((a[1] + a[2]).SumAtWithCharPoly({0, 1}, 20, mod) == 17710);
    assert(((a[1] + a[2]).Generate({0, 1}, 20, mod) ==
            std::vector<int64>{0,   1,   1,   2,    3,    5,    8,
                               13,  21,  34,  55,   89,   144,  233,
                               377, 610, 987, 1597, 2584, 4181, 6765}));
  }
  {
    Sequence a;
    (void)a;
    assert((a[1] + a[2]).ValueAt({0, 1}, 1000, mod) == 517691607);
    assert((a[1] + a[2]).ValueAtWithCharPoly({0, 1}, 1000, mod) == 517691607);
    assert((a[1] + a[2]).SumAt({0, 1}, 1000, mod) == 625271545);
    assert((a[1] + a[2]).SumAtWithCharPoly({0, 1}, 1000, mod) == 625271545);
  }
  {
    using MT = NModCC64<mod>;
    Sequence<MT> a;
    (void)a;
    assert((a[1] + a[2]).ValueAt({0, 1}, 1000).value() == 517691607);
    assert((a[1] + a[2]).SumAt({0, 1}, 1000).value() == 625271545);
  }
}
PE_REGISTER_TEST(&SeqExprTest, "SeqExprTest", SMALL);
}  // namespace poly_algo_test
