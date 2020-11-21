#include <pe.hpp>

const int64 mod = 1000000007;

int64 solve_nth(int64 n) {
  auto ans = PowerMod<mod>(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        v[0] = 1;
        v[1] = 0;
      },
      2, n);
  return ans[1];
}

int64 solve_nth_sum(int64 n) {
  auto ans = PowerMod<mod>(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        m(2, 0) = 1;
        m(2, 2) = 1;
        v[0] = 1;
        v[1] = 0;
        v[2] = 0;
      },
      3, n);
  return ans[2];
}

int main() {
  const vector<int64> init{0LL, 1LL, 1LL, 2LL, 3LL, 5LL, 8LL};

  for (int64 n = 1; n <= 1000000000; n *= 10) {
    // Use Berlekamp Massey algorithm to find the recurrence.
    // The minimal initial element count is: 2 * order + 1
    const int64 ans0 = NthElement(init, mod, n);
    // Compute the nth element assuming the recurrence is known.
    const int64 ans1 = LinearRecurrence({1LL, 1LL}, init, mod, n);
    // Use matric multiplication to compute the nth element.
    const int64 ans2 = solve_nth(n);
    cout << "n = " << n << endl;
    cout << "Ans0 = " << ans0 << endl;
    cout << "Ans1 = " << ans1 << endl;
    cout << "Ans2 = " << ans2 << endl;
    cout << endl;
  }

  for (int64 n = 1; n <= 1000000000; n *= 10) {
    // The minimal initial element count is: 2 * (order + 1) + 1
    const int64 ans1 = LinearRecurrenceSum({1LL, 1LL}, init, mod, n);
    const int64 ans2 = solve_nth_sum(n);
    cout << "n = " << n << endl;
    cout << "Ans1 = " << ans1 << endl;
    cout << "Ans2 = " << ans2 << endl;
    cout << endl;
  }
  return 0;
}