#include <pe.hpp>

void test(const map<int, int>& mem) {
  for (auto iter : irange(mem)) {
    cout << iter.i << " " << iter.v << endl;
    // ++iter.v.second;
  }
  for (auto iter : irange(mem)) {
    cout << iter.i << " " << iter.v << endl;
  }
}

void test_irange() {
  int a[6] = {1, 2, 3, 4, 5, 6};
  vector<int> x{3, 4, 5, 6};
  map<int, int> mem;
  mem[1] = 2;
  mem[2] = 4;
  test(mem);

  for (auto i : irange(range(10, 20))) {
    cout << i.i << " " << i.v << endl;
  }
  for (auto i : irange(irange(static_cast<vector<int>&>(x)))) {
    cout << i.i << " " << i.v.i << " " << i.v.v << endl;
    ++i.v.v;
  }
  for (auto i : irange(irange(x))) {
    cout << i.i << " " << i.v.i << " " << i.v.v << endl;
  }
  for (auto i : irange(range(10, 1, -2))) {
    cout << i.i << " " << i.v << endl;
  }
  for (auto i : irange(a, a + 3)) {
    cout << i.i << " " << i.v << endl;
  }
  for (auto i : irange(a)) {
    cout << i.i << " " << i.v << endl;
  }
}

struct Pt {
  int a, b;
  operator int() const { return a; }
};

void test_range() {
  int a[6] = {1, 2, 3, 4, 5, 6};
  vector<int> x{3, 4, 5, 6};
  map<int, int> mem;
  mem[1] = 2;
  mem[2] = 4;

  for (auto i : range(1, 10)) {
    dbg(i);
  }
  for (auto i : range(1, 10, 2)) {
    cout << i << endl;
  }
  for (auto i : range(0, 10, 2)) {
    cout << i << endl;
  }
  for (auto i : range(10, 0, -2)) {
    cout << i << endl;
  }
  for (auto i : range(10, 1, -2)) {
    cout << i << endl;
  }
  for (auto i : range(x).filter([](auto v) { return v % 2 == 0; })) {
    cout << i << endl;
  }

  cout << range(1, 10)
              .filter([](auto v) { return v % 2 == 0; })
              .reduce(0, [](auto a, auto b) { return a + b; })
       << endl;
  cout << range(1, 10)
              .filter([](auto v) { return v % 2 == 0; })
              .filter([](auto v) { return v % 3 == 0; })
              .map([](auto v) { return v * 3; })
              .map([](auto v) { return v * 3; })
              .reduce(0, [](auto a, auto b) { return a + b; })
       << endl;
  cout << range(1, 10).filter([](auto v) { return v % 2 == 0; }).toVector()
       << endl;
  cout << range(1, 10).filter([](auto v) { return v % 3 == 0; }).toSet()
       << endl;

  const int* a0 = a;
  const int* a1 = a + 6;

  cout << range(a0, a1).reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(a, a + 6).reduce(1, [](auto a, auto b) { return a * b; })
       << endl;
  cout << range(a).reduce(1, [](auto a, auto b) { return a + b; }) << endl;

  for (auto i : range(x)) {
    cout << i << endl;
  }

  cout << range(static_cast<const vector<int>&>(x))
              .reduce(1, [](auto a, auto b) { return a + b; })
       << endl;

  cout << range(x.begin(), x.end()).reduce(1, [](auto a, auto b) {
    return a + b;
  }) << endl;

  for (auto i : range(mem)) {
    cout << i.first << " " << i.second << endl;
  }

  // 1*2*3*...*9
  cout << range(1, 10).reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(1, 10).reduce([](auto a, auto b) { return a * b; }) << endl;
  cout << range(1, 10).reduce(&ru::mul) << endl;
  cout << range(1, 10).reduce(1, &ru::mul) << endl;

  cout << range(1, 10).preduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(1, 10).preduce([](auto a, auto b) { return a * b; }) << endl;
  cout << range(1, 10).preduce(&ru::mul) << endl;
  cout << range(1, 10).preduce(1, &ru::mul) << endl;

  // 1*2*3*...*999999
  const int64 mod = 1000000007;
  NMod64<mod> t(1);
  for (int i = 1; i < 1000000; ++i) t *= i;
  cout << t << endl;

  cout << range(1, 1000000).reduce(1, [=](auto a, auto b) {
    return a * b % mod;
  }) << endl;
  cout << range(1, 1000000).reduce([=](auto a, auto b) { return a * b % mod; })
       << endl;
  cout << range(1, 1000000).reduce(&ru::mul_mod<mod>) << endl;
  cout << range(1, 1000000).reduce(ru::mul_mod(mod)) << endl;

  cout << range(1, 1000000).preduce(1, [=](auto a, auto b) {
    return a * b % mod;
  }) << endl;
  cout << range(1, 1000000).preduce([=](auto a, auto b) { return a * b % mod; })
       << endl;
  cout << range(1, 1000000).preduce(&ru::mul_mod<mod>) << endl;
  cout << range(1, 1000000).preduce(ru::mul_mod(mod)) << endl;

  // Reduce on x
  cout << range(static_cast<const vector<int>&>(x)).reduce([](auto a, auto b) {
    return a * b;
  }) << endl;
  cout << range(x).reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(x).reduce([](auto a, auto b) { return a * b; }) << endl;
  cout << range(x).reduce(&ru::mul) << endl;

  cout << range(static_cast<const vector<int>&>(x)).preduce([](auto a, auto b) {
    return a * b;
  }) << endl;
  cout << range(x).preduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(x).preduce([](auto a, auto b) { return a * b; }) << endl;
  cout << range(x).preduce(&ru::mul) << endl;

  // Count prime.
  cout << range(1, 10000000 + 1)
              .map([](auto a) { return is_prime(a); })
              .reduce(&ru::add)
       << endl;
  cout << range(1, 10000000 + 1).reduce(0, [](auto a, auto b) {
    return a + is_prime(b);
  }) << endl;
  cout << range(1, 10000000 + 1)
              .preduce(
                  0, [](auto a, auto b) { return a + is_prime(b); }, &ru::add)
       << endl;

  // Reduce on mem
  cout << range(mem).preduce<int64>(
              0LL, [](int64 a, auto& b) -> int64 { return a + b.second; },
              &ru::add)
       << endl;
  // Error
  // cout << range(mem).preduce({0,0}, [](auto a, auto b) -> pair<const int,
  // int> {return {0, a.second + b.second};}, 2) << endl;
  vector<Pt> y;
  y.pb({1, 2});
  y.pb({3, 4});
  range(y).preduce(
      {0, 0},
      [](auto a, auto b) -> Pt {
        return {0, a.a + b.a};
      },
      2);

  // Inplace reduce
#if !PE_HAS_INT128
#define int128 int64
#endif
  cout << range(1, 101).ireduce<int128>(0, [](auto& a, auto b) { a += b; })
       << endl;
  cout << range(1, 101).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  cout << range(1, 101).ireduce(0, &ru::iadd) << endl;

  cout << range(1, 101).ireduce<int128>(0, [](auto& a, auto b) { a += b; })
       << endl;
  cout << range(1, 101).ireduce([](auto& a, auto b) { a += b; }) << endl;
  cout << range(1, 101).ireduce(&ru::iadd) << endl;

  cout << range(1, 101).pireduce<int128>(
              1, [](auto& a, auto b) { a *= b; },
              [](auto& a, auto b) { a *= b; })
       << endl;
  cout << range(1, 101).pireduce(
              1, [](auto& a, auto b) { a *= b; },
              [](auto& a, auto b) { a *= b; })
       << endl;
  cout << range(1, 101).pireduce(1, [](auto& a, auto b) { a *= b; }) << endl;

  cout << range(1, 101).pireduce<int128>([](auto& a, auto b) { a *= b; },
                                         [](auto& a, auto b) { a *= b; })
       << endl;
  cout << range(1, 101).pireduce([](auto& a, auto b) { a *= b; },
                                 [](auto& a, auto b) { a *= b; })
       << endl;
  cout << range(1, 101).pireduce([](auto& a, auto b) { a *= b; }) << endl;

  cout << range(y).ireduce<int128>(0, [](auto& a, auto b) { a += b.a; })
       << endl;
  cout << range(y).ireduce<int128>([](auto& a, auto b) { a += b.a; }) << endl;
  cout << range(y).ireduce({0, 0}, [](auto& a, auto b) { a.a += b.a; }).a
       << endl;
  cout << range(y).ireduce([](auto& a, auto b) { a.a += b.a; }).a << endl;

  cout << range(y).pireduce<int64>(
              0, [](auto& a, const Pt& b) { a += b.a; },
              [](auto& a, auto b) { a += b; })
       << endl;
  cout << range(y).pireduce<int64>([](auto& a, const Pt& b) { a += b.a; },
                                   [](auto& a, auto b) { a += b; })
       << endl;

  cout << range(y)
              .pireduce(
                  {0, 0}, [](auto& a, auto b) { a.a += b.a; },
                  [](auto& a, auto b) { a.a += b.a; })
              .a
       << endl;
  cout << range(y).pireduce({0, 0}, [](auto& a, auto b) { a.a += b.a; }).a
       << endl;

  cout << range(y)
              .pireduce([](auto& a, auto b) { a.a += b.a; },
                        [](auto& a, auto b) { a.a += b.a; })
              .a
       << endl;
  cout << range(y).pireduce([](auto& a, auto b) { a.a += b.a; }).a << endl;
}

int main() {
  pe().maxPrime(2000000).init();
  test_irange();
  test_range();
  return 0;
}