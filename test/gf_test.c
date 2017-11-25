#include "pe_test.h"

const int64 mod = 1000000007;

SL void gf_test() {
  {
    // Fibonacci sequence
    vector<int64> A{1, -1, -1};
    vector<int64> B{0, 1};
    vector<int64> result{0, 1};
    for (int i = 2; i <= 30; ++i) {
      result.push_back(add_mod(result[i-2], result[i-1], mod));
    }
    auto x = gf_first<mod>(A, B, 30);
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
    int64 dp[10000+1] = {1};
    int64 can[5] = {1, 5, 10, 25, 50};
    for (auto each: can)
    {
      for (int j = 0; j + each <= 10000; ++j) if (dp[j])
      {
        dp[j+each] = add_mod(dp[j+each], dp[j], mod);
      }
    }

    int64 coe[100] = {0};
    for (int i = 0; i < 1 << 5; ++i)
    {
      int s = 0;
      int bc = 0;
      for (int j = 0; j < 5; ++j)
        if (i & (1<<j)) ++bc, s += can[j];
      if (bc & 1) --coe[s]; else ++coe[s];
    }
    auto gfresult = gf_first<mod>(vector<int64>(coe, coe+92), {1}, 10000);
    for (int i = 0; i <= 10000; ++i)
      assert(dp[i] == gfresult[i]);

    string mine = to_string(gf_at<mod>(vector<int64>(coe, coe+92), {1}, 100000000));
    string expected = ("66666793333412666685000001"_bi % mod).toString();
    assert(mine == expected);
  }
}

PE_REGISTER_TEST(&gf_test, "gf_test");