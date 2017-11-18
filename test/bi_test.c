#include <pe>

int main()
{
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
  return 0;
}

