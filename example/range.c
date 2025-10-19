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
    std::array<int, 6> a = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
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
  {
    std::array<int, 6> a = {1, 2, 3, 4, 5, 6};
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
    std::vector<int> a = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
    }
  }
  {
    const std::vector<int> a = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_set() {
  {
    std::set<int> a = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      std::cout << iter.i << " " << iter.v << std::endl;
      // ++iter.v; not allowed
    }
  }
  {
    const std::set<int> a = {1, 2, 3, 4, 5, 6};
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
    std::vector<int> a = {1, 2, 3, 4, 5, 6};
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
    std::cout << Range(a).InplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }
  {
    std::array<int, 6> a = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).InplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }

  // Parallel
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PInplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }
  {
    std::array<int, 6> a = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PInplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }
}

void range_vector_reduce() {
  // Sequential
  {
    std::vector<int> a = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).InplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }

  // Parallel
  {
    std::vector<int> a = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PInplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }
}

void range_set_reduce() {
  // Sequential
  {
    std::set<int> a = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).InplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }

  // Parallel
  {
    std::set<int> a = {1, 2, 3, 4, 5, 6};
    std::cout << Range(a).PInplaceReduce(0, [](int& a, int b) -> void {
      a += b;
    }) << std::endl;
  }
}

void range_map_reduce() {
  // Sequential
  {
    std::map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    std::cout
        << Range(a).Map<int64>([](const auto& a) { return a.second; }).Sum()
        << std::endl;
  }

  // Parallel
  {
    std::map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    std::cout
        << Range(a).PMap<int64>([](const auto& a) { return a.second; }).Sum()
        << std::endl;
  }
}

void range_number_range_reduce() {
  // Sequential
  {
    std::cout << Range(Range(1, 20))
                     .InplaceReduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;
    std::cout << Range(Range(1, 20))
                     .InplaceReduce<int64>(
                         1, [](int64& a, int b) -> void { a *= b; })
              << std::endl;
  }

  // Parallel
  {
    std::cout << Range(Range(1, 20))
                     .PInplaceReduce(0, [](int& a, int b) -> void { a += b; })
              << std::endl;

    std::cout << Range(Range(1, 20))
                     .PInplaceReduce<int64>(
                         1, [](int64& a, int64 b) -> void { a *= b; })
              << std::endl;
  }
}

void range_general_example() {
  int a[6] = {1, 2, 3, 4, 5, 6};
  std::vector<int> x = {3, 4, 5, 6};
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

  std::cout
      << Range(1, 10).Filter([](auto v) { return v % 2 == 0; }).Sum<int64>()
      << std::endl;
  std::cout << Range(1, 10)
                   .Filter([](auto v) { return v % 2 == 0; })
                   .Filter([](auto v) { return v % 3 == 0; })
                   .Map<int64>([](auto v) { return v * 3; })
                   .Map<int64>([](auto v) { return v * 3; })
                   .Sum<int64>()
            << std::endl;
  std::cout << Range(1, 10).Filter([](auto v) { return v % 2 == 0; }).ToVector()
            << std::endl;
  std::cout << Range(1, 10).Filter([](auto v) { return v % 3 == 0; }).ToSet()
            << std::endl;

  const int* a0 = a;
  const int* a1 = a + 6;

  std::cout << Range(a0, a1).Prod<int64>() << std::endl;
  std::cout << Range(a, a + 6).Prod<int64>() << std::endl;
  std::cout << Range(a).Sum<int64>() << std::endl;

  for (auto i : Range(x)) {
    std::cout << i << std::endl;
  }

  std::cout << Range(static_cast<const std::vector<int>&>(x)).Sum<int64>()
            << std::endl;

  std::cout << Range(std::begin(x), std::end(x)).Sum<int64>() << std::endl;

  for (auto i : Range(mem)) {
    std::cout << i.first << " " << i.second << std::endl;
  }

  // 1*2*3*...*9
  std::cout << Range(1, 10).Prod<int64>() << std::endl;
  std::cout << Range(1, 10).PReduce<int64>(1, std::multiplies<int64>())
            << std::endl;

  // 1*2*3*...*999999
  const int64 mod = 1000000007;
  NModCC64<mod> t(1);
  for (int i = 1; i < 1000000; ++i) t *= i;
  std::cout << t << std::endl;

  std::cout << Range(1, 1000000).ProdMod(mod) << std::endl;
  std::cout << Range(1, 1000000)
                   .PInplaceReduce<int64>(
                       1, [&](int64& a, int64 b) { a = MulMod(a, b, mod); })
            << std::endl;

  // InplaceReduce on x
  std::cout << Range(x).Prod<int64>() << std::endl;
  std::cout << Range(x).PReduce<int64>(1, std::multiplies<int64>())
            << std::endl;

  // Count prime.
  std::cout << Range(1, 10000000 + 1)
                   .Map<int64>([](auto a) { return IsPrime(a); })
                   .Sum<int64>()
            << std::endl;

  // InplaceReduce on mem
  std::cout << Range(mem)
                   .PMap<int64>([=](std::pair<const int, int> x) -> int64 {
                     return x.second;
                   })
                   .Sum<int64>()
            << std::endl;

  std::cout << Range(mem).PInplaceReduce(
                   std::pair<int, int>{0, 0},
                   [](auto& a, auto b) { a.second += b.second; }, 2)
            << std::endl;
  std::vector<Pt> y;
  y.push_back({1, 2});
  y.push_back({3, 4});
  std::cout << Range(y).PInplaceReduce(Pt{0, 0}, [=](Pt& r, Pt v) {
    r.a += v.a;
    r.b += v.b;
  }) << std::endl;
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
  PE_INIT(maxp = 2000000);
  irange_example();
  range_example();
  return 0;
}