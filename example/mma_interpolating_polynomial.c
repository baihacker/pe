#include <pe.hpp>
using namespace pe;

int64 f(int64 x) { return 1 + 2 * x + 3 * x * x + 4 * x * x * x; }

int64 g(int64 x, int64 y) {
  return 1 + 2 * x + 3 * y + 4 * x * y + 5 * x * x + 6 * y * y;
}

int main() {
  PE_INIT(maxp = 2000000);
  {
    mma::IntPoly ip;
    ip.Reset({"x"});  // one variable x.
    for (int64 x = 0; x <= 10; ++x) {
      ip.Add({x}, f(x));
    }
    std::cout << ip << std::endl;
    // output:
    // InterpolatingPolynomial[{{0, 1}, {1, 10}, {2, 49}, {3, 142}, {4, 313},
    // {5, 586}, {6, 985}, {7, 1534}, {8, 2257}, {9, 3178}, {10, 4321}}, x]
    // Simplified mma output:
    // 1 + 2 x + 3 x^2 + 4 x^3
  }
  {
    mma::IntPoly2D ip(2,  // x's degree
                      2,  // y's degree
                      2   // max a + b of terms in the form of x^a y^b
    );
    for (int64 x = 0; x <= 4; ++x)
      for (int64 y = 0; y <= 4; ++y) {
        ip.Add({x, y}, g(x, y));
      }
    std::cout << ip << std::endl;
    // output:
    // Values[Solve[{1 x0 + 0 x1 + 0 x2 + 0 x3 + 0 x4 + 0 x5 == 1 && 1 x0 + 1 x1
    // + 1 x2 + 0 x3 + 0 x4 + 0 x5 == 8 && 1 x0 + 2 x1 + 4 x2 + 0 x3 + 0 x4 + 0
    // x5 == 25 && 1 x0 + 3 x1 + 9 x2 + 0 x3 + 0 x4 + 0 x5 == 52 && 1 x0 + 4 x1
    // + 16 x2 + 0 x3 + 0 x4 + 0 x5 == 89 && 1 x0 + 0 x1 + 0 x2 + 1 x3 + 0 x4 +
    // 1 x5 == 6 && 1 x0 + 1 x1 + 1 x2 + 1 x3 + 1 x4 + 1 x5 == 16 && 1 x0 + 2 x1
    // + 4 x2 + 1 x3 + 2 x4 + 1 x5 == 36 && 1 x0 + 3 x1 + 9 x2 + 1 x3 + 3 x4 + 1
    // x5
    // == 66 && 1 x0 + 4 x1 + 16 x2 + 1 x3 + 4 x4 + 1 x5 == 106 && 1 x0 + 0 x1 +
    // 0 x2 + 2 x3 + 0 x4 + 4 x5 == 19&& 1 x0 + 1 x1 + 1 x2 + 2 x3 + 2 x4 + 4 x5
    // == 32 && 1 x0 + 2 x1 + 4 x2 + 2 x3 + 4 x4 + 4 x5 == 55 && 1 x0 + 3 x1 + 9
    // x2 + 2 x3 + 6 x4 + 4 x5 == 88 && 1 x0 + 4 x1 + 16 x2 + 2 x3 + 8 x4 + 4 x5
    // == 131 && 1 x0 + 0 x1 + 0 x2 + 3 x3 + 0 x4 + 9 x5 == 40 && 1 x0 + 1 x1 +
    // 1 x2 + 3 x3 + 3 x4 + 9 x5 == 56 && 1 x0 + 2 x1 + 4 x2+ 3 x3 + 6 x4 + 9 x5
    // == 82 && 1 x0 + 3 x1 + 9 x2 + 3 x3 + 9 x4 + 9 x5 == 118 && 1 x0 + 4 x1 +
    // 16 x2 + 3 x3 + 12 x4 + 9 x5 == 164 && 1 x0 + 0 x1 + 0 x2 + 4 x3 + 0 x4 +
    // 16 x5 == 69 && 1 x0 + 1 x1 + 1 x2 + 4 x3 + 4 x4 + 16 x5== 88 && 1 x0 + 2
    // x1 + 4 x2 + 4 x3 + 8 x4 + 16 x5 == 117 && 1 x0 + 3 x1 + 9 x2 + 4 x3 + 12
    // x4 + 16 x5 == 156 && 1 x0 + 4 x1 + 16 x2 + 4 x3 + 16 x4 + 16 x5 == 205},
    // {x0, x1, x2, x3, x4, x5}]] mma output:
    // {{1, 3, 6, 2, 4, 5}}
    ip.Show(std::cout, {1, 3, 6, 2, 4, 5});
    // output:
    // 1 + 3 y + 6 y^2 + 2 x + 4 x y + 5 x^2
  }
  return 0;
}