#include "pe_test.h"

namespace rand_test {
SL void CRandTest() {
  int or15 = 0, or31 = 0, bits_seen = 0;
  int64 or63 = 0;
  for (int i = 0; i < 4000; ++i) {
    const int b = CRandBit();
    assert(b == 0 || b == 1);
    bits_seen |= 1 << b;
    const int r15 = CRand15();
    assert(0 <= r15 && r15 <= 32767);
    or15 |= r15;
    const int r31 = CRand31();
    assert(r31 >= 0);
    or31 |= r31;
    const int64 r63 = CRand63();
    assert(r63 >= 0);
    or63 |= r63;
    assert(CRandI() >= 0);
    const double d = CRandD();
    assert(0 <= d && d < 1);
  }
  // Every bit is produced at least once.
  assert(bits_seen == 3);
  assert(or15 == 32767);
  assert(or31 == std::numeric_limits<int>::max());
  assert(or63 == std::numeric_limits<int64>::max());

  // The C generator is deterministic for a fixed seed.
  auto take = [](unsigned seed) {
    srand(seed);
    std::vector<int64> v;
    for (int i = 0; i < 8; ++i) {
      v.push_back(CRand15());
      v.push_back(CRand31());
      v.push_back(CRand63());
      v.push_back(CRandBit());
      v.push_back(CRandI());
      v.push_back(static_cast<int64>(CRandD() * (1 << 30)));
    }
    return v;
  };
  const auto s1 = take(12345);
  const auto s2 = take(12345);
  const auto s3 = take(54321);
  assert(s1 == s2 && s1 != s3);
  srand(static_cast<unsigned>(time(nullptr)));
}

SL void RandomGeneratorTest() {
  // Same seed, same sequence; and it matches the std components.
  auto g1 = MakeUniformGenerator(2024, -5, 5);
  auto g2 = MakeUniformGenerator(2024, -5, 5);
  std::mt19937 engine(2024);
  std::uniform_int_distribution<int> dist(-5, 5);
  std::set<int> seen;
  for (int i = 0; i < 2000; ++i) {
    const int x = g1();
    assert(-5 <= x && x <= 5);
    assert(x == g2() && x == dist(engine));
    seen.insert(x);
  }
  assert(std::size(seen) == 11);

  // Different seeds give different sequences.
  auto g3 = MakeUniformGenerator(1, 0, 1000000);
  auto g4 = MakeUniformGenerator(2, 0, 1000000);
  int same = 0;
  for (int i = 0; i < 100; ++i) same += g3() == g4();
  assert(same < 5);

  // A single value range.
  auto g5 = MakeUniformGenerator(7, 42, 42);
  for (int i = 0; i < 10; ++i) assert(g5() == 42);

  // Full int range.
  auto g6 = MakeUniformGenerator(9, std::numeric_limits<int>::min(),
                                 std::numeric_limits<int>::max());
  bool has_negative = false, has_positive = false;
  for (int i = 0; i < 100; ++i) {
    const int x = g6();
    has_negative |= x < 0;
    has_positive |= x > 0;
  }
  assert(has_negative && has_positive);

  // Seeded from std::random_device: only the range can be checked.
  auto g7 = MakeUniformGenerator(3, 9);
  std::set<int> seen7;
  for (int i = 0; i < 1000; ++i) {
    const int x = g7();
    assert(3 <= x && x <= 9);
    seen7.insert(x);
  }
  assert(std::size(seen7) == 7);
  auto g8 = MakeUniformGenerator(-1, -1);
  assert(g8() == -1);

  // RandomGenerator with custom components.
  RandomGenerator<std::minstd_rand, std::uniform_int_distribution<int>> g9(
      std::minstd_rand(5), std::uniform_int_distribution<int>(10, 20));
  std::minstd_rand e9(5);
  std::uniform_int_distribution<int> d9(10, 20);
  for (int i = 0; i < 100; ++i) assert(g9() == d9(e9));
}

// The radical inverse of idx in the given base.
SL double RadicalInverse(int64 idx, int64 base) {
  double ret = 0, f = 1.0 / base;
  for (; idx > 0; idx /= base, f /= base) ret += (idx % base) * f;
  return ret;
}

SL void HaltonTest() {
  auto close = [](double a, double b) { return std::fabs(a - b) < 1e-12; };
  // Index 0 is the origin.
  const std::vector<double> h0 = Halton(0, 3);
  assert((h0 == std::vector<double>{0, 0, 0}));
  // Zero dimensions.
  assert(Halton(5, 0).empty());

  const std::vector<double> h1 = Halton(1, 3);
  assert(close(h1[0], 0.5) && close(h1[1], 1. / 3) && close(h1[2], 0.2));
  const std::vector<double> h5 = Halton(5, 2);
  // 5 = 101_2 -> 0.101_2; 5 = 12_3 -> 0.21_3.
  assert(close(h5[0], 0.625) && close(h5[1], 7. / 9));
  const std::vector<double> h3 = Halton(3, 1);
  assert(std::size(h3) == 1 && close(h3[0], 0.75));

  // Against the radical inverse with the first primes as bases.
  const int64 primes[] = {2, 3, 5, 7, 11, 13};
  for (int64 idx = 0; idx <= 200; ++idx) {
    const std::vector<double> h = Halton(idx, 6);
    assert(std::size(h) == 6);
    for (int i = 0; i < 6; ++i) {
      assert(close(h[i], RadicalInverse(idx, primes[i])));
      assert(0 <= h[i] && h[i] < 1);
    }
  }
  // A larger index.
  const int64 big = 123456789012LL;
  const std::vector<double> hb = Halton(big, 4);
  for (int i = 0; i < 4; ++i)
    assert(close(hb[i], RadicalInverse(big, primes[i])));

  // Template forms with other index and result types.
  const std::vector<float> hf = Halton<int, float>(6, 2);
  assert(std::fabs(hf[0] - 0.375f) < 1e-6f &&
         std::fabs(hf[1] - 2.f / 9) < 1e-6f);
  long double out[3] = {-1, -1, -1};
  Halton<int64, long double>(10, 3, out);
  // 10 = 1010_2, 101_3, 20_5.
  assert(std::fabs(out[0] - 0.3125L) < 1e-15L);
  assert(std::fabs(out[1] - (1.0L / 3 + 1.0L / 27)) < 1e-15L);
  assert(std::fabs(out[2] - 0.08L) < 1e-15L);
  double out1[2] = {9, 9};
  Halton(int64(0), 2, out1);
  assert(out1[0] == 0 && out1[1] == 0);
}

SL void MemoryTest() {
  // StdAllocator.
  char* p = static_cast<char*>(StdAllocator::Allocate(64));
  assert(p != nullptr);
  for (int i = 0; i < 64; ++i) p[i] = static_cast<char>(i);
  for (int i = 0; i < 64; ++i) assert(p[i] == i);
  StdAllocator::Deallocate(p);

#if OS_TYPE_WIN
  {
    LargeMemory lm;
    // Non-positive sizes allocate nothing.
    assert(lm.Allocate(0) == nullptr && lm.Allocate(-5) == nullptr);
    int64* a = static_cast<int64*>(lm.Allocate(4096 * sizeof(int64)));
    int64* b = static_cast<int64*>(lm.Allocate(100));
    assert(a != nullptr && b != nullptr && a != b);
    // The memory is zero initialized.
    for (int i = 0; i < 4096; ++i) assert(a[i] == 0);
    for (int i = 0; i < 4096; ++i) a[i] = i * i;
    for (int i = 0; i < 12; ++i) b[i] = -i;
    for (int i = 0; i < 4096; ++i) assert(a[i] == int64(i) * i);
    for (int i = 0; i < 12; ++i) assert(b[i] == -i);
    lm.Deallocate(a);
    // Unknown pointers and nullptr are ignored.
    int local = 0;
    lm.Deallocate(&local);
    lm.Deallocate(nullptr);
    // b is released by the destructor.
  }
  {
    // The singleton and the allocator struct.
    LargeMemory& lm1 = LmAllocator();
    LargeMemory& lm2 = LmAllocator();
    assert(&lm1 == &lm2);
    int* q = static_cast<int*>(pe::LmAllocator::Allocate(1000 * sizeof(int)));
    assert(q != nullptr);
    for (int i = 0; i < 1000; ++i) q[i] = 3 * i;
    assert(q[999] == 2997);
    pe::LmAllocator::Deallocate(q);
    assert(pe::LmAllocator::Allocate(0) == nullptr);

    // Arrays backed by LmAllocator.
    DArray<int64, 2, struct pe::LmAllocator> d({30, 40}, int64(5));
    for (int i = 0; i < 30; ++i)
      for (int j = 0; j < 40; ++j) {
        assert(d[i][j] == 5);
        d[i][j] = i * 40 + j;
      }
    assert(d.data()[1199] == 1199);
    AArray<int, struct pe::LmAllocator, 5, 6> f(-1);
    assert(f[4][5] == -1);
    f[4][5] = 7;
    assert(f.data()[29] == 7);
  }
#endif
}

SL void ArrayRefTest() {
  static_assert(std::is_same_v<DArrayRef<int, 1>::ValueType, int&>);
  static_assert(
      std::is_same_v<DArrayRef<int, 1>::ConstValueType, const int&>);
  static_assert(
      std::is_same_v<DArrayRef<int, 3>::ValueType, DArrayRef<int, 2>>);
  static_assert(
      std::is_same_v<FArrayRef<int, ArrayShape<4>>::ValueType, int&>);
  static_assert(std::is_same_v<FArrayRef<int, ArrayShape<2, 3, 4>>::ValueType,
                               FArrayRef<int, ArrayShape<3, 4>>>);

  // DArrayRef over a raw buffer, row-major.
  int buf[24];
  for (int i = 0; i < 24; ++i) buf[i] = i;
  const int64 dims[3] = {2, 3, 4};
  const int64 ec[4] = {24, 12, 4, 1};
  DArrayRef<int, 3> r(buf, 0, dims, ec);
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 4; ++k) assert(r[i][j][k] == i * 12 + j * 4 + k);
  r[1][2][3] = -1;
  assert(buf[23] == -1);
  auto row = r[1][0];
  row[2] = 100;
  assert(buf[14] == 100 && r[1][0][2] == 100);
  const DArrayRef<int, 1> crow = r[0][1];
  static_assert(std::is_same_v<decltype(crow[0]), const int&>);
  assert(crow[0] == 4 && crow[3] == 7);
  // A lower dimensional view of the same buffer.
  const int64 dims2[2] = {6, 4};
  const int64 ec2[3] = {24, 4, 1};
  DArrayRef<int, 2> r2(buf, 0, dims2, ec2);
  assert(r2[3][2] == 100 && r2[5][3] == -1 && r2[0][0] == 0);
  // One dimension.
  DArrayRef<int, 1> r1(buf, 0, dims, ec);
  r1[5] = 55;
  assert(buf[5] == 55 && r[0][1][1] == 55);

  // FArrayRef over a raw buffer.
  int64 fbuf[30];
  for (int i = 0; i < 30; ++i) fbuf[i] = 2 * i;
  FArrayRef<int64, ArrayShape<2, 3, 5>> f(fbuf);
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 5; ++k)
        assert(f[i][j][k] == 2 * (i * 15 + j * 5 + k));
  f[1][1][4] = -9;
  assert(fbuf[24] == -9);
  auto frow = f[0][2];
  frow[0] = 77;
  assert(fbuf[10] == 77);
  const FArrayRef<int64, ArrayShape<5>> cfrow(fbuf + 5);
  static_assert(std::is_same_v<decltype(cfrow[0]), const int64&>);
  assert(cfrow[0] == 10 && cfrow[4] == 18);
  FArrayRef<int64, ArrayShape<30>> flat(fbuf);
  assert(flat[24] == -9 && flat[29] == 58);

  // Ref() of owning arrays aliases their storage.
  DArray<int, 2> da({3, 4}, 1);
  DArrayRef<int, 2> dref = da.Ref();
  dref[2][3] = 9;
  assert(da[2][3] == 9 && da.data()[11] == 9);
  Array<int, 3, 4> fa(2);
  FArrayRef<int, ArrayShape<3, 4>> fref = fa.Ref();
  fref[1][1] = 8;
  assert(fa[1][1] == 8 && fa.data()[5] == 8 && fa[0][0] == 2);
}

SL void RandTest() {
  CRandTest();
  RandomGeneratorTest();
  HaltonTest();
  MemoryTest();
  ArrayRefTest();
}

PE_REGISTER_TEST(&RandTest, "RandTest", SMALL);
}  // namespace rand_test
