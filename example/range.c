#include <pe.hpp>
using namespace pe;

void irange_array() {
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
    }
  }
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_pointer() {
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a, a + 6)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
    }
  }
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a, a + 6)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_vector() {
  {
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
    }
  }
  {
    const std::vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_set() {
  {
    std::set<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
  {
    const std::set<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_map() {
  {
    std::map<int, int> a;
    a[5] = 10, a[7] = 20;
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v.first << " " << iter.v.second
                << std::endl;
      ++iter.v.second;
    }
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v.first << " " << iter.v.second
                << std::endl;
    }
  }
  {
    std::map<int, int> t;
    t[5] = 10, t[7] = 20;
    const std::map<int, int> a = t;
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v.first << " " << iter.v.second
                << std::endl;
      // ++iter.v.second; not allowed
    }
  }
}

void irange_number_range() {
  {
    for (auto iter : IRange(Range(1, 10))) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_irange() {
  {
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(IRange(a))) {
      std::cout << iter.i << " " << iter.v.i << " " << iter.v.v << std::endl;
      ++iter.v.v;
      // ++iter.v.i;
    }
    for (auto iter : IRange(IRange(a))) {
      std::cout << iter.i << " " << iter.v.i << " " << iter.v.v << std::endl;
    }
  }
}

struct Pt {
  int a, b;
  // operator int() const { return a; }
};

std::ostream& operator<<(std::ostream& o, const Pt& p) {
  return o << p.a << " " << p.b;
}

void range_array_reduce() {
  // Sequential
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).Reduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(a).Reduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
  }

  // Parallel
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PReduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(a).PReduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
    std::cout << Range(a).PReduce(
                     0, [](int& a, int b) -> void { a += b; },
                     [](int& a, int b) -> void { a += b; })
              << std::endl;
  }
}

void range_vector_reduce() {
  // Sequential
  {
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    std::cout << Range(a).Reduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(a).Reduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
  }

  // Parallel
  {
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PReduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(a).PReduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
    std::cout << Range(a).PReduce(
                     0, [](int& a, int b) -> void { a += b; },
                     [](int& a, int b) -> void { a += b; })
              << std::endl;
  }
}

void range_set_reduce() {
  // Sequential
  {
    std::set<int> a{1, 2, 3, 4, 5, 6};
    std::cout << Range(a).Reduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(a).Reduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
  }

  // Parallel
  {
    std::set<int> a{1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PReduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(a).PReduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
    std::cout << Range(a).PReduce(
                     0, [](int& a, int b) -> void { a += b; },
                     [](int& a, int b) -> void { a += b; })
              << std::endl;
  }
}

void range_map_reduce() {
  // Sequential
  {
    std::map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    std::cout << Range(a).Reduce(ru::Add<int64, std::pair<const int, int>>(
                     [=](std::pair<const int, int> x) -> int64 {
                       return x.second;
                     }))
              << std::endl;

    std::cout << Range(a).Map<int64>([](auto& a) { return a.second; }).Sum()
              << std::endl;
  }

  // Parallel
  {
    std::map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    std::cout << Range(a).PReduce(ru::Add<int64, std::pair<const int, int>>(
                     [=](std::pair<const int, int> x) -> int64 {
                       return x.second;
                     }))
              << std::endl;
    std::cout << Range(a).PMap<int64>([](auto& a) { return a.second; }).Sum()
              << std::endl;
  }
}

void range_number_range_reduce() {
  // Sequential
  {
    std::cout << Range(Range(1, 20)).Reduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(Range(1, 20)).Reduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
    std::cout << Range(Range(1, 20)).Reduce(ru::Mul<int64>()) << std::endl;
    std::cout << Range(Range(1, 20))
                     .Reduce<int64>(1, [](int64& a, int b) -> void { a *= b; })
              << std::endl;
  }

  // Parallel
  {
    std::cout << Range(Range(1, 20)).PReduce(ru::Add<int64>()) << std::endl;
    std::cout << Range(Range(1, 20)).PReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
    std::cout << Range(Range(1, 20))
                     .PReduce(
                         0, [](int& a, int b) -> void { a += b; },
                         [](int& a, int b) -> void { a += b; })
              << std::endl;

    std::cout << Range(Range(1, 20)).PReduce(ru::Mul<int64>()) << std::endl;
    std::cout << Range(Range(1, 20))
                     .PReduce<int64>(1,
                                     [](int64& a, int64 b) -> void { a *= b; })
              << std::endl;
    std::cout << Range(Range(1, 20))
                     .PReduce<int64>(
                         1, [](int64& a, int64 b) -> void { a *= b; },
                         [](int64& a, int64 b) -> void { a *= b; })
              << std::endl;
  }
}

void range_general_example() {
  int a[6] = {1, 2, 3, 4, 5, 6};
  std::vector<int> x{3, 4, 5, 6};
  std::map<int, int> mem;
  mem[1] = 2;
  mem[2] = 4;

  for (auto i : Range(1, 10)) {
    dbg(i);
  }
  for (auto i : Range(1, 10, 2)) {
    std::cout << i << std::endl;
  }
  for (auto i : Range(0, 10, 2)) {
    std::cout << i << std::endl;
  }
  for (auto i : Range(10, 0, -2)) {
    std::cout << i << std::endl;
  }
  for (auto i : Range(10, 1, -2)) {
    std::cout << i << std::endl;
  }
  for (auto i : Range(x).Filter([](auto v) { return v % 2 == 0; })) {
    std::cout << i << std::endl;
  }

  std::cout << Range(1, 10)
                   .Filter([](auto v) { return v % 2 == 0; })
                   .Reduce(ru::Add<int64>())
            << std::endl;
  std::cout << Range(1, 10)
                   .Filter([](auto v) { return v % 2 == 0; })
                   .Filter([](auto v) { return v % 3 == 0; })
                   .Map<int64>([](auto v) { return v * 3; })
                   .Map<int64>([](auto v) { return v * 3; })
                   .Reduce(ru::Add<int64>())
            << std::endl;
  std::cout << Range(1, 10).Filter([](auto v) { return v % 2 == 0; }).ToVector()
            << std::endl;
  std::cout << Range(1, 10).Filter([](auto v) { return v % 3 == 0; }).ToSet()
            << std::endl;

  const int* a0 = a;
  const int* a1 = a + 6;

  std::cout << Range(a0, a1).Reduce(ru::Mul<int64>()) << std::endl;
  std::cout << Range(a, a + 6).Reduce(ru::Mul<int64>()) << std::endl;
  std::cout << Range(a).Reduce(ru::Add<int64>()) << std::endl;

  for (auto i : Range(x)) {
    std::cout << i << std::endl;
  }

  std::cout
      << Range(static_cast<const std::vector<int>&>(x)).Reduce(ru::Add<int64>())
      << std::endl;

  std::cout << Range(x.begin(), x.end()).Reduce(ru::Add<int64>()) << std::endl;

  for (auto i : Range(mem)) {
    std::cout << i.first << " " << i.second << std::endl;
  }

  // 1*2*3*...*9
  std::cout << Range(1, 10).Reduce(ru::Mul<int64>()) << std::endl;
  std::cout << Range(1, 10).PReduce(ru::Mul<int64>()) << std::endl;

  // 1*2*3*...*999999
  const int64 mod = 1000000007;
  NMod64<mod> t(1);
  for (int i = 1; i < 1000000; ++i) t *= i;
  std::cout << t << std::endl;

  std::cout << Range(1, 1000000).Reduce(ru::MulMod<int64>(mod)) << std::endl;

  std::cout << Range(1, 1000000).PReduce(ru::MulMod<int64>(mod)) << std::endl;

  // Reduce on x
  std::cout << Range(x).Reduce(ru::Mul<int64>()) << std::endl;
  std::cout << Range(x).PReduce(ru::Mul<int64>()) << std::endl;

  // Count prime.
  std::cout << Range(1, 10000000 + 1)
                   .Map<int64>([](auto a) { return IsPrime(a); })
                   .Reduce(ru::Add<int64>())
            << std::endl;
  std::cout << Range(1, 10000000 + 1).PReduce(ru::Add<int64>()) << std::endl;

  // Reduce on mem
  std::cout << Range(mem).PReduce(ru::Add<int64, std::pair<const int, int>>(
                   [=](std::pair<const int, int> x) -> int64 {
                     return x.second;
                   }))
            << std::endl;
  std::cout << Range(1, 10000000 + 1)
                   .Map<int64>([](auto a) { return a; })
                   .Reduce(ru::Reducer<int64, int>(
                       0, [=](int64& r, int v) { r += v; }))
            << std::endl;
  // Error
  // std::cout << range(mem).PReduce({0,0}, [](auto a, auto b) ->
  // std::pair<const int, int> {return {0, a.second + b.second};}, 2) <<
  // std::endl;
  std::vector<Pt> y;
  y.pb({1, 2});
  y.pb({3, 4});
  std::cout << Range(y).PReduce(ru::Reducer<Pt>({0, 0}, [=](Pt& r, Pt v) {
    r.a += v.a;
    r.b += v.b;
  })) << std::endl;
}

void irange_example() {
  irange_array();
  irange_pointer();
  irange_vector();
  irange_set();
  irange_map();
  irange_number_range();
  irange_irange();
}

void range_example() {
  range_general_example();
  range_array_reduce();
  range_vector_reduce();
  range_set_reduce();
  range_map_reduce();
  range_number_range_reduce();
}

int main() {
  PeInitializer().set_max_prime(2000000).Init();
  irange_example();
  range_example();
  return 0;
}