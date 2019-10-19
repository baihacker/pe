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
}

int main() {
  test_irange();
  test_range();
  return 0;
}