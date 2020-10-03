#include <pe.hpp>

void irange_array() {
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
    }
  }
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_pointer() {
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a, a + 6)) {
      cout << iter.i << " " << iter.v << endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
    }
  }
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a, a + 6)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_vector() {
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
      ++iter.v;
    }
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
    }
  }
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_set() {
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_map() {
  {
    map<int, int> a;
    a[5] = 10, a[7] = 20;
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v.first << " " << iter.v.second << endl;
      ++iter.v.second;
    }
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v.first << " " << iter.v.second << endl;
    }
  }
  {
    map<int, int> t;
    t[5] = 10, t[7] = 20;
    const map<int, int> a = t;
    for (auto iter : IRange(a)) {
      cout << iter.i << " " << iter.v.first << " " << iter.v.second << endl;
      // ++iter.v.second; not allowed
    }
  }
}

void irange_number_range() {
  {
    for (auto iter : IRange(Range(1, 10))) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_irange() {
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : IRange(IRange(a))) {
      cout << iter.i << " " << iter.v.i << " " << iter.v.v << endl;
      ++iter.v.v;
      // ++iter.v.i;
    }
    for (auto iter : IRange(IRange(a))) {
      cout << iter.i << " " << iter.v.i << " " << iter.v.v << endl;
    }
  }
}

struct Pt {
  int a, b;
  // operator int() const { return a; }
};

void range_array_reduce() {
  // not inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).Reduce<int64>(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).IReduce<int64>(0, &ru::iadd) << endl;
    cout << Range(a).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).PReduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce<int64>(0LL, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add) << endl;
    cout << Range(a).PReduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << Range(a).PReduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd) << endl;
    cout << Range(a).PIReduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << Range(a).PIReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // not inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).Reduce<int64>(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).IReduce<int64>(0, &ru::iadd) << endl;
    cout << Range(a).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).PReduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce<int64>(0LL, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add) << endl;
    cout << Range(a).PReduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << Range(a).PReduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd) << endl;
    cout << Range(a).PIReduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << Range(a).PIReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }
}

void range_vector_reduce() {
  // not inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).Reduce<int64>(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).IReduce<int64>(0, &ru::iadd) << endl;
    cout << Range(a).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PReduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce<int64>(0LL, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add) << endl;
    cout << Range(a).PReduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << Range(a).PReduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd) << endl;
    cout << Range(a).PIReduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << Range(a).PIReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // not inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).Reduce<int64>(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).IReduce<int64>(0, &ru::iadd) << endl;
    cout << Range(a).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PReduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce<int64>(0LL, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add) << endl;
    cout << Range(a).PReduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << Range(a).PReduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd) << endl;
    cout << Range(a).PIReduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << Range(a).PIReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }
}

void range_set_reduce() {
  // not inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).Reduce<int64>(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).IReduce<int64>(0, &ru::iadd) << endl;
    cout << Range(a).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PReduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce<int64>(0LL, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add) << endl;
    cout << Range(a).PReduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << Range(a).PReduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd) << endl;
    cout << Range(a).PIReduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << Range(a).PIReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // not inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).Reduce<int64>(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, &ru::add) << endl;
    cout << Range(a).Reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).IReduce<int64>(0, &ru::iadd) << endl;
    cout << Range(a).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PReduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce<int64>(0LL, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add, &ru::add) << endl;
    cout << Range(a).PReduce(0, &ru::add) << endl;
    cout << Range(a).PReduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << Range(a).PReduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << Range(a).PIReduce<int64>(0LL, &ru::iadd) << endl;
    cout << Range(a).PIReduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << Range(a).PIReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }
}

void range_map_reduce() {
  // not inplace
  {
    map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    cout << Range(a).Reduce<int64>(0, [](auto a, auto b) {
      return a + b.second;
    }) << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).Reduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
  }

  // inplace
  {
    map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    cout << Range(a).IReduce<int64>(0, [](auto& a, auto b) { a += b.second; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).IReduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
      a.first += b.first;
      a.second += b.second;
    }) << endl;
  }

  // parallel, not inplace
  {
    map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    cout << Range(a).PReduce<int64>(
                0, [](auto a, auto b) -> int64 { return a + b.second; },
                [](auto a, auto b) { return a + b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).PReduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                },
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
    cout << Range(a).PReduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
  }

  // parallel, inplace
  {
    map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    cout << Range(a).PIReduce<int64>(
                0, [](auto& a, auto b) -> void { a += b.second; },
                [](auto& a, auto b) { a += b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).PIReduce<std::pair<int, int>>(
                {0, 0},
                [](auto& a, auto b) {
                  a.first += b.first;
                  a.second += b.second;
                },
                [](auto& a, auto b) {
                  a.first += b.first;
                  a.second += b.second;
                })
         << endl;
    cout << Range(a).PIReduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
      a.first += b.first;
      a.second += b.second;
    }) << endl;
  }

  // not inplace
  {
    map<int, int> t;
    t[1] = 2;
    t[2] = 3;
    const map<int, int> a = t;
    cout << Range(a).Reduce<int64>(0, [](auto a, auto b) {
      return a + b.second;
    }) << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).Reduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
  }

  // inplace
  {
    map<int, int> t;
    t[1] = 2;
    t[2] = 3;
    const map<int, int> a = t;
    cout << Range(a).IReduce<int64>(0, [](auto& a, auto b) { a += b.second; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).IReduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
      a.first += b.first;
      a.second += b.second;
    }) << endl;
  }

  // parallel, not inplace
  {
    map<int, int> t;
    t[1] = 2;
    t[2] = 3;
    const map<int, int> a = t;
    cout << Range(a).PReduce<int64>(
                0, [](auto a, auto b) -> int64 { return a + b.second; },
                [](auto a, auto b) { return a + b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).PReduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                },
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
    cout << Range(a).PReduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
  }

  // parallel, inplace
  {
    map<int, int> t;
    t[1] = 2;
    t[2] = 3;
    const map<int, int> a = t;
    cout << Range(a).PIReduce<int64>(
                0, [](auto& a, auto b) -> void { a += b.second; },
                [](auto& a, auto b) { a += b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << Range(a).PIReduce<std::pair<int, int>>(
                {0, 0},
                [](auto& a, auto b) {
                  a.first += b.first;
                  a.second += b.second;
                },
                [](auto& a, auto b) {
                  a.first += b.first;
                  a.second += b.second;
                })
         << endl;
    cout << Range(a).PIReduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
      a.first += b.first;
      a.second += b.second;
    }) << endl;
  }
}

void range_number_range_reduce() {
  // not inplace
  {
    cout << Range(Range(1, 20)).Reduce<int64>(1, &ru::mul) << endl;
    cout << Range(Range(1, 20)).Reduce(1, &ru::mul) << endl;
  }

  // inplace
  {
    cout << Range(Range(1, 20)).IReduce<int64>(1, &ru::imul) << endl;
    cout << Range(Range(1, 20)).IReduce(1, &ru::imul) << endl;
  }

  // parallel, not inplace
  {
    cout << Range(Range(1, 20)).PReduce<int64>(1LL, &ru::mul, &ru::mul) << endl;
    cout << Range(Range(1, 20)).PReduce<int64>(1LL, &ru::mul) << endl;

    cout << Range(Range(1, 20)).PReduce(1, &ru::mul, &ru::mul) << endl;
    cout << Range(Range(1, 20)).PReduce(1, &ru::mul) << endl;
  }

  // parallel, inplace
  {
    cout << Range(Range(1, 20)).PIReduce<int64>(1LL, &ru::imul, &ru::imul)
         << endl;
    cout << Range(Range(1, 20)).PIReduce<int64>(1LL, &ru::imul) << endl;
    cout << Range(Range(1, 20)).PIReduce(1LL, &ru::imul, &ru::imul) << endl;
    cout << Range(Range(1, 20)).PIReduce(1LL, &ru::imul) << endl;
  }
}

void range_general_example() {
  int a[6] = {1, 2, 3, 4, 5, 6};
  vector<int> x{3, 4, 5, 6};
  map<int, int> mem;
  mem[1] = 2;
  mem[2] = 4;

  for (auto i : Range(1, 10)) {
    dbg(i);
  }
  for (auto i : Range(1, 10, 2)) {
    cout << i << endl;
  }
  for (auto i : Range(0, 10, 2)) {
    cout << i << endl;
  }
  for (auto i : Range(10, 0, -2)) {
    cout << i << endl;
  }
  for (auto i : Range(10, 1, -2)) {
    cout << i << endl;
  }
  for (auto i : Range(x).Filter([](auto v) { return v % 2 == 0; })) {
    cout << i << endl;
  }

  cout << Range(1, 10)
              .Filter([](auto v) { return v % 2 == 0; })
              .Reduce(0, [](auto a, auto b) { return a + b; })
       << endl;
  cout << Range(1, 10)
              .Filter([](auto v) { return v % 2 == 0; })
              .Filter([](auto v) { return v % 3 == 0; })
              .Map<int64>([](auto v) { return v * 3; })
              .Map<int64>([](auto v) { return v * 3; })
              .Reduce(0, [](auto a, auto b) { return a + b; })
       << endl;
  cout << Range(1, 10).Filter([](auto v) { return v % 2 == 0; }).ToVector()
       << endl;
  cout << Range(1, 10).Filter([](auto v) { return v % 3 == 0; }).ToSet()
       << endl;

  const int* a0 = a;
  const int* a1 = a + 6;

  cout << Range(a0, a1).Reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << Range(a, a + 6).Reduce(1, [](auto a, auto b) { return a * b; })
       << endl;
  cout << Range(a).Reduce(1, [](auto a, auto b) { return a + b; }) << endl;

  for (auto i : Range(x)) {
    cout << i << endl;
  }

  cout << Range(static_cast<const vector<int>&>(x))
              .Reduce(1, [](auto a, auto b) { return a + b; })
       << endl;

  cout << Range(x.begin(), x.end()).Reduce(1, [](auto a, auto b) {
    return a + b;
  }) << endl;

  for (auto i : Range(mem)) {
    cout << i.first << " " << i.second << endl;
  }

  // 1*2*3*...*9
  cout << Range(1, 10).Reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << Range(1, 10).Reduce(1, &ru::mul) << endl;

  cout << Range(1, 10).PReduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << Range(1, 10).PReduce(1, &ru::mul) << endl;

  // 1*2*3*...*999999
  const int64 mod = 1000000007;
  NMod64<mod> t(1);
  for (int i = 1; i < 1000000; ++i) t *= i;
  cout << t << endl;

  cout << Range(1, 1000000).Reduce(1, [=](auto a, auto b) {
    return a * b % mod;
  }) << endl;

  cout << Range(1, 1000000).PReduce(1, [=](auto a, auto b) {
    return a * b % mod;
  }) << endl;

  // Reduce on x
  cout << Range(x).Reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << Range(x).PReduce(1, [](auto a, auto b) { return a * b; }) << endl;

  // Count prime.
  cout << Range(1, 10000000 + 1)
              .Map<int64>([](auto a) { return IsPrime(a); })
              .Reduce(0, &ru::add)
       << endl;
  cout << Range(1, 10000000 + 1).Reduce(0, [](auto a, auto b) {
    return a + IsPrime(b);
  }) << endl;
  cout << Range(1, 10000000 + 1)
              .PReduce(
                  0, [](auto a, auto b) { return a + IsPrime(b); }, &ru::add)
       << endl;

  // Reduce on mem
  cout << Range(mem).PReduce<int64>(
              0LL, [](int64 a, auto& b) -> int64 { return a + b.second; },
              &ru::add)
       << endl;
  // Error
  // cout << range(mem).PReduce({0,0}, [](auto a, auto b) -> pair<const int,
  // int> {return {0, a.second + b.second};}, 2) << endl;
  vector<Pt> y;
  y.pb({1, 2});
  y.pb({3, 4});
  Range(y).PReduce(
      {0, 0},
      [](auto a, auto b) -> Pt {
        return {0, a.a + b.a};
      },
      2);

  // Inplace reduce

  cout << Range(1, 101).IReduce<int64>(0, [](auto& a, auto b) { a += b; })
       << endl;
  cout << Range(1, 101).IReduce(0, [](auto& a, auto b) { a += b; }) << endl;
  cout << Range(1, 101).IReduce(0, &ru::iadd) << endl;

  cout << Range(1, 101).PIReduce<int64>(
              1, [](auto& a, auto b) { a *= b; },
              [](auto& a, auto b) { a *= b; })
       << endl;
  cout << Range(1, 101).PIReduce(
              1, [](auto& a, auto b) { a *= b; },
              [](auto& a, auto b) { a *= b; })
       << endl;
  cout << Range(1, 101).PIReduce(1, [](auto& a, auto b) { a *= b; }) << endl;
  cout << Range(1, 101).PIReduce(1, &ru::imul) << endl;

  cout << Range(y).IReduce<int64>(0, [](auto& a, auto b) { a += b.a; }) << endl;
  cout << Range(y).IReduce({0, 0}, [](auto& a, auto b) { a.a += b.a; }).a
       << endl;

  cout << Range(y).PIReduce<int64>(
              0, [](auto& a, const Pt& b) { a += b.a; },
              [](auto& a, auto b) { a += b; })
       << endl;

  cout << Range(y)
              .PIReduce(
                  {0, 0}, [](auto& a, auto b) { a.a += b.a; },
                  [](auto& a, auto b) { a.a += b.a; })
              .a
       << endl;
  cout << Range(y).PIReduce({0, 0}, [](auto& a, auto b) { a.a += b.a; }).a
       << endl;
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
  pe().set_max_prime(2000000).Init();
  // irange_example();
  range_example();
  return 0;
}