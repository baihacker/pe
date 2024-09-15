#include <pe.hpp>
using namespace pe;

// http://oeis.org/A001499
// Oeis gives two recurrence formula:
// 2 a[n] = 2 n (n-1) a[n-1] + n (n-1)^2 a[n-2]
// 2 a[n] = n (n-1)^2 ((2 n - 3) a[n-2] + (n-2)^2 a[n-3])
//
// This example uses some sample data to guess the formula.
// Note: the result may not be unique.

int main() {
  mma::FRHelper<bi> helper;
  helper
      .set_values({1, 0, 1, 6, 90, 2040, 67950, 3110940, 187530840, 14398171200,
                   1371785398200})
      // In most cases we can use all the available check points, but
      // sometimes it has different recurrence according to n. For example, when
      // n is odd, it has a recurrence formula. When n is even, it has a
      // different one.
      .set_check_points({3, 5, 7})
      .set_offsets({1, 2})  // Tune the offset manually
      .set_max_degree(5)    // Tune the polynomial degree manually
      .set_max_abs_coe(4)   // Tune the coefficient manually
      .set_leading(2);      // Tune the leading manually

  std::cout << helper << std::endl << std::endl;

  // The output of the generated mathematica command is
  const std::string result =
      "{{x1p0 -> 0, x1p1 -> -2, x1p2 -> 2, x1p3 -> 0, x1p4 -> 0, x1p5 "
      "-> 0, x2p0 -> 0, x2p1 -> 1, x2p2 -> -2, x2p3 -> 1, x2p4 -> 0, "
      "x2p5 -> 0}}";

  // Validate the result by sample data.
  helper.Validate(result);

  // Validate the result by more data.
  std::vector<bi> dp0{1, 0, 1, 6, 90, 2040, 67950};
  for (int64 i = 7; i < 100; ++i) {
    dp0.push_back((2 * i * (i - 1) * dp0[i - 1] + i * sq(i - 1) * dp0[i - 2]) /
                  2);
  }

  std::vector<bi> dp1{1, 0, 1, 6, 90, 2040, 67950};
  for (int64 i = 7; i < 100; ++i) {
    dp1.push_back(i * sq(i - 1) *
                  ((2 * i - 3) * dp1[i - 2] + sq(i - 2) * dp1[i - 3]) / 2);
  }

  helper.Validate(dp0, result);
  helper.Validate(dp1, result);

  return 0;
}