#include <pe.hpp>
using namespace pe;

const int64 mod = 1000000007;

int64 solve_nth(int64 n) {
  auto ans = MatrixPowerMod<mod>(
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
  auto ans = MatrixPowerMod<mod>(
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
  const std::vector<int64> init{0LL, 1LL, 1LL, 2LL, 3LL, 5LL, 8LL};

  for (int64 n = 1; n <= 1000000000; n *= 10) {
    // Use Berlekamp Massey algorithm to find the recurrence.
    // The minimal initial element count is: 2 * order + 1
    const int64 ans0 = NthElement(init, mod, n);
    // Compute the nth element assuming the recurrence is known.
    const int64 ans1 = LinearRecurrence({1LL, 1LL}, init, mod, n);
    // Use matric multiplication to compute the nth element.
    const int64 ans2 = solve_nth(n);
    std::cout << "n = " << n << std::endl;
    std::cout << "Ans0 = " << ans0 << std::endl;
    std::cout << "Ans1 = " << ans1 << std::endl;
    std::cout << "Ans2 = " << ans2 << std::endl;
    std::cout << std::endl;
  }

  for (int64 n = 1; n <= 1000000000; n *= 10) {
    // The minimal initial element count is: 2 * (order + 1) + 1
    const int64 ans1 = LinearRecurrenceSum({1LL, 1LL}, init, mod, n);
    const int64 ans2 = solve_nth_sum(n);
    std::cout << "n = " << n << std::endl;
    std::cout << "Ans1 = " << ans1 << std::endl;
    std::cout << "Ans2 = " << ans2 << std::endl;
    std::cout << std::endl;
  }
  return 0;
}