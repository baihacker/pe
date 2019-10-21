#include <pe.hpp>

void irange_array() {
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
      ++iter.v;
    }
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
    }
  }
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_pointer() {
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a, a + 6)) {
      cout << iter.i << " " << iter.v << endl;
      ++iter.v;
    }
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
    }
  }
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a, a + 6)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_vector() {
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
      ++iter.v;
    }
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
    }
  }
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_set() {
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_map() {
  {
    map<int, int> a;
    a[5] = 10, a[7] = 20;
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v.first << " " << iter.v.second << endl;
      ++iter.v.second;
    }
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v.first << " " << iter.v.second << endl;
    }
  }
  {
    map<int, int> t;
    t[5] = 10, t[7] = 20;
    const map<int, int> a = t;
    for (auto iter : irange(a)) {
      cout << iter.i << " " << iter.v.first << " " << iter.v.second << endl;
      // ++iter.v.second; not allowed
    }
  }
}

void irange_number_range() {
  {
    for (auto iter : irange(range(1, 10))) {
      cout << iter.i << " " << iter.v << endl;
      // ++iter.v; not allowed
    }
  }
}

void irange_irange() {
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    for (auto iter : irange(irange(a))) {
      cout << iter.i << " " << iter.v.i << " " << iter.v.v << endl;
      ++iter.v.v;
      // ++iter.v.i;
    }
    for (auto iter : irange(irange(a))) {
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
    cout << range(a).reduce<int64>(0, &ru::add) << endl;
    cout << range(a).reduce(0, &ru::add) << endl;
    cout << range(a).reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).ireduce<int64>(0, &ru::iadd) << endl;
    cout << range(a).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).preduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << range(a).preduce<int64>(0LL, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add) << endl;
    cout << range(a).preduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << range(a).preduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).pireduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << range(a).pireduce<int64>(0LL, &ru::iadd) << endl;
    cout << range(a).pireduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << range(a).pireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // not inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).reduce<int64>(0, &ru::add) << endl;
    cout << range(a).reduce(0, &ru::add) << endl;
    cout << range(a).reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).ireduce<int64>(0, &ru::iadd) << endl;
    cout << range(a).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).preduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << range(a).preduce<int64>(0LL, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add) << endl;
    cout << range(a).preduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << range(a).preduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    const int a[6] = {1, 2, 3, 4, 5, 6};
    cout << range(a).pireduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << range(a).pireduce<int64>(0LL, &ru::iadd) << endl;
    cout << range(a).pireduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << range(a).pireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }
}

void range_vector_reduce() {
  // not inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).reduce<int64>(0, &ru::add) << endl;
    cout << range(a).reduce(0, &ru::add) << endl;
    cout << range(a).reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).ireduce<int64>(0, &ru::iadd) << endl;
    cout << range(a).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).preduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << range(a).preduce<int64>(0LL, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add) << endl;
    cout << range(a).preduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << range(a).preduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).pireduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << range(a).pireduce<int64>(0LL, &ru::iadd) << endl;
    cout << range(a).pireduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << range(a).pireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // not inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).reduce<int64>(0, &ru::add) << endl;
    cout << range(a).reduce(0, &ru::add) << endl;
    cout << range(a).reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).ireduce<int64>(0, &ru::iadd) << endl;
    cout << range(a).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).preduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << range(a).preduce<int64>(0LL, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add) << endl;
    cout << range(a).preduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << range(a).preduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    const vector<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).pireduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << range(a).pireduce<int64>(0LL, &ru::iadd) << endl;
    cout << range(a).pireduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << range(a).pireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }
}

void range_set_reduce() {
  // not inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).reduce<int64>(0, &ru::add) << endl;
    cout << range(a).reduce(0, &ru::add) << endl;
    cout << range(a).reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).ireduce<int64>(0, &ru::iadd) << endl;
    cout << range(a).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).preduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << range(a).preduce<int64>(0LL, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add) << endl;
    cout << range(a).preduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << range(a).preduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).pireduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << range(a).pireduce<int64>(0LL, &ru::iadd) << endl;
    cout << range(a).pireduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << range(a).pireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // not inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).reduce<int64>(0, &ru::add) << endl;
    cout << range(a).reduce(0, &ru::add) << endl;
    cout << range(a).reduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).ireduce<int64>(0, &ru::iadd) << endl;
    cout << range(a).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }

  // parallel, not inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).preduce<int64>(0LL, &ru::add, &ru::add) << endl;
    cout << range(a).preduce<int64>(0LL, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add, &ru::add) << endl;
    cout << range(a).preduce(0, &ru::add) << endl;
    cout << range(a).preduce(
                0, [](auto a, auto b) { return a + b; },
                [](auto a, auto b) { return a + b; })
         << endl;
    cout << range(a).preduce(0, [](auto a, auto b) { return a + b; }) << endl;
  }

  // parallel, inplace
  {
    const set<int> a{1, 2, 3, 4, 5, 6};
    cout << range(a).pireduce<int64>(0LL, &ru::iadd, &ru::iadd) << endl;
    cout << range(a).pireduce<int64>(0LL, &ru::iadd) << endl;
    cout << range(a).pireduce(
                0, [](auto& a, auto b) { a += b; },
                [](auto& a, auto b) { a += b; })
         << endl;
    cout << range(a).pireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  }
}

void range_map_reduce() {
  // not inplace
  {
    map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    cout << range(a).reduce<int64>(0, [](auto a, auto b) {
      return a + b.second;
    }) << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).reduce<std::pair<int, int>>(
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
    cout << range(a).ireduce<int64>(0, [](auto& a, auto b) { a += b.second; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).ireduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
      a.first += b.first;
      a.second += b.second;
    }) << endl;
  }

  // parallel, not inplace
  {
    map<int, int> a;
    a[1] = 2;
    a[2] = 3;
    cout << range(a).preduce<int64>(
                0, [](auto a, auto b) { return a + b.second; },
                [](auto a, auto b) { return a + b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).preduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                },
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
    cout << range(a).preduce<std::pair<int, int>>(
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
    cout << range(a).pireduce<int64>(
                0, [](auto& a, auto b) { a += b.second; },
                [](auto& a, auto b) { a += b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).pireduce<std::pair<int, int>>(
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
    cout << range(a).pireduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
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
    cout << range(a).reduce<int64>(0, [](auto a, auto b) {
      return a + b.second;
    }) << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).reduce<std::pair<int, int>>(
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
    cout << range(a).ireduce<int64>(0, [](auto& a, auto b) { a += b.second; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).ireduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
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
    cout << range(a).preduce<int64>(
                0, [](auto a, auto b) { return a + b.second; },
                [](auto a, auto b) { return a + b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).preduce<std::pair<int, int>>(
                {0, 0},
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                },
                [](auto a, auto b) -> std::pair<int, int> {
                  return {a.first + b.first, a.second + b.second};
                })
         << endl;
    cout << range(a).preduce<std::pair<int, int>>(
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
    cout << range(a).pireduce<int64>(
                0, [](auto& a, auto b) { a += b.second; },
                [](auto& a, auto b) { a += b; })
         << endl;
    // Cannot reduce on pair<const int, int>
    // Need to specify std::pair<int, int>
    cout << range(a).pireduce<std::pair<int, int>>(
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
    cout << range(a).pireduce<std::pair<int, int>>({0, 0}, [](auto& a, auto b) {
      a.first += b.first;
      a.second += b.second;
    }) << endl;
  }
}

void range_number_range_reduce() {
  // not inplace
  {
    cout << range(range(1, 20)).reduce<int64>(1, &ru::mul) << endl;
    cout << range(range(1, 20)).reduce(1, &ru::mul) << endl;
  }

  // inplace
  {
    cout << range(range(1, 20)).ireduce<int64>(1, &ru::imul) << endl;
    cout << range(range(1, 20)).ireduce(1, &ru::imul) << endl;
  }

  // parallel, not inplace
  {
    cout << range(range(1, 20)).preduce<int64>(1LL, &ru::mul, &ru::mul) << endl;
    cout << range(range(1, 20)).preduce<int64>(1LL, &ru::mul) << endl;

    cout << range(range(1, 20)).preduce(1, &ru::mul, &ru::mul) << endl;
    cout << range(range(1, 20)).preduce(1, &ru::mul) << endl;
  }

  // parallel, inplace
  {
    cout << range(range(1, 20)).pireduce<int64>(1LL, &ru::imul, &ru::imul)
         << endl;
    cout << range(range(1, 20)).pireduce<int64>(1LL, &ru::imul) << endl;
    cout << range(range(1, 20)).pireduce(1LL, &ru::imul, &ru::imul) << endl;
    cout << range(range(1, 20)).pireduce(1LL, &ru::imul) << endl;
  }
}

void range_general_example() {
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
  cout << range(1, 10).reduce(1, &ru::mul) << endl;

  cout << range(1, 10).preduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(1, 10).preduce(1, &ru::mul) << endl;

  // 1*2*3*...*999999
  const int64 mod = 1000000007;
  NMod64<mod> t(1);
  for (int i = 1; i < 1000000; ++i) t *= i;
  cout << t << endl;

  cout << range(1, 1000000).reduce(1, [=](auto a, auto b) {
    return a * b % mod;
  }) << endl;

  cout << range(1, 1000000).preduce(1, [=](auto a, auto b) {
    return a * b % mod;
  }) << endl;

  // Reduce on x
  cout << range(x).reduce(1, [](auto a, auto b) { return a * b; }) << endl;
  cout << range(x).preduce(1, [](auto a, auto b) { return a * b; }) << endl;

  // Count prime.
  cout << range(1, 10000000 + 1)
              .map([](auto a) { return is_prime(a); })
              .reduce(0, &ru::add)
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

  cout << range(1, 101).ireduce<int64>(0, [](auto& a, auto b) { a += b; })
       << endl;
  cout << range(1, 101).ireduce(0, [](auto& a, auto b) { a += b; }) << endl;
  cout << range(1, 101).ireduce(0, &ru::iadd) << endl;

  cout << range(1, 101).pireduce<int64>(
              1, [](auto& a, auto b) { a *= b; },
              [](auto& a, auto b) { a *= b; })
       << endl;
  cout << range(1, 101).pireduce(
              1, [](auto& a, auto b) { a *= b; },
              [](auto& a, auto b) { a *= b; })
       << endl;
  cout << range(1, 101).pireduce(1, [](auto& a, auto b) { a *= b; }) << endl;
  cout << range(1, 101).pireduce(1, &ru::imul) << endl;

  cout << range(y).ireduce<int64>(0, [](auto& a, auto b) { a += b.a; }) << endl;
  cout << range(y).ireduce({0, 0}, [](auto& a, auto b) { a.a += b.a; }).a
       << endl;

  cout << range(y).pireduce<int64>(
              0, [](auto& a, const Pt& b) { a += b.a; },
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
  pe().maxPrime(2000000).init();
  // irange_example();
  range_example();
  return 0;
}