#include "pe_test.h"

namespace tree_test {
SL void RuBitTest() {
  const int n = 100;
  int data[n + 1] = {0};
  RUBit<int> tree(n);
  for (int iter = 0; iter < 100; ++iter) {
    if (rand() % 2 == 0) {
      for (int i = 0; i < 100; ++i) {
        int u = rand() % n + 1, v = rand() % n + 1;
        int w = rand() % n - 50;
        if (u > v) std::swap(u, v);
        tree.Update(u, v, w);
        for (int j = u; j <= v; ++j) data[j] += w;
      }
    } else {
      for (int i = 1; i <= 100; ++i) {
        assert(tree.Query(i) == data[i]);
      }
    }
  }
}

SL void RsqBitTest() {
  const int n = 100;
  int data[n + 1] = {0};
  RSQBit<int> tree(n);
  for (int iter = 0; iter < 100; ++iter) {
    if (rand() % 2 == 0) {
      for (int i = 0; i < 100; ++i) {
        int u = rand() % n + 1, v = rand() % n + 1;
        if (u > v) std::swap(u, v);
        int s = 0;
        for (int j = u; j <= v; ++j) s += data[j];
        assert(tree.Query(u, v) == s);
      }
    } else {
      for (int i = 1; i <= 100; ++i) {
        int w = rand() % n - 50;
        data[i] += w;
        tree.Update(i, w);
      }
    }
  }
}

SL void TreeTest() {
  RuBitTest();
  RsqBitTest();
}

PE_REGISTER_TEST(&TreeTest, "TreeTest", SMALL);
SL void IndexHelperTest() {
  const std::vector<int64> elements = {50, -3, 7, 50, 1000000000000, 7, 0};
  // Distinct sorted values: -3, 0, 7, 50, 1e12
  for (const IndexHelper& ih :
       {IndexHelper(elements), IndexHelper(std::vector<int64>(elements)),
        IndexHelper(std::begin(elements), std::end(elements))}) {
    assert(ih.size() == 5);
    assert(ih[-3] == 1 && ih[0] == 2 && ih.Index(7) == 3 && ih[50] == 4 &&
           ih[1000000000000] == 5);
  }
  IndexHelper ih;
  ih.Reset({9, 8, 9});
  assert(ih.size() == 2 && ih[8] == 1 && ih[9] == 2);
  ih.Reset(std::vector<int64>{4});
  assert(ih.size() == 1 && ih[4] == 1);

  // Bit trees sized by an IndexHelper, Clear and Reset.
  const IndexHelper h(elements);
  RSQBit<int64> rsq(h);
  RUBit<int64> ru(h);
  for (int64 v : elements) {
    rsq.Update(static_cast<int>(h[v]), v);
    ru.Update(static_cast<int>(h[v]), 5, 1);
  }
  assert(rsq.Query(1, 5) == 1000000000111LL && rsq.Query(3, 3) == 14);
  assert(ru.Query(5) == 7 && ru.Query(1) == 1);
  rsq.Clear();
  assert(rsq.Query(5) == 0);
  ru.Reset(3);
  assert(ru.size_ == 3 && ru.Query(3) == 0);
}

PE_REGISTER_TEST(&IndexHelperTest, "IndexHelperTest", SMALL);

SL void SplayMultiSetTest() {
  SplayMultiSet<int64> s;
  std::multiset<int64> ref;
  uint64 seed = 5;
  auto rnd = [&](int64 m) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    return static_cast<int64>((seed >> 33) % m);
  };
  for (int step = 0; step < 4000; ++step) {
    const int op = static_cast<int>(rnd(10));
    const int64 v = rnd(60) - 30;
    if (op < 6) {
      s.insert(v);
      ref.insert(v);
    } else if (op < 8) {
      // erase(value) removes every copy.
      s.erase(v);
      ref.erase(v);
    } else {
      // erase(iterator) removes one copy.
      auto it = s.FindOne(v);
      if (it != s.end()) {
        assert(*it == v);
        s.erase(it);
        ref.erase(ref.find(v));
      } else {
        assert(ref.count(v) == 0);
      }
    }
    assert(s.size() == static_cast<int64>(std::size(ref)));
    const int64 q = rnd(70) - 35;
    assert(s.FindCount(q) == static_cast<int64>(ref.count(q)));
    int64 less = 0, less_eq = 0;
    for (int64 x : ref) less += x < q, less_eq += x <= q;
    const int64 total = std::size(ref);
    assert(s.QueryLess(q) == less && s.QueryLessEqual(q) == less_eq);
    assert(s.QueryGreater(q) == total - less_eq);
    assert(s.QueryGreaterEqual(q) == total - less);
    const bool present = ref.count(q) > 0;
    assert((s.FindFirst(q) != s.end()) == present);
    assert((s.FindLast(q) != s.end()) == present);
    if (present) assert(*s.FindFirst(q) == q && *s.FindLast(q) == q);
    if (total > 0) {
      const int64 k = rnd(total) + 1;
      assert(*s.FindKth(k) == *std::next(std::begin(ref), k - 1));
    }
    assert(s.FindKth(0) == s.end() && s.FindKth(total + 1) == s.end());

    if (step % 200 == 0) {
      // In-order traversal, both directions.
      std::vector<int64> fwd, bwd;
      for (auto it = s.begin(); it != s.end(); ++it) fwd.push_back(*it);
      for (auto it = s.rbegin(); it != s.rend(); --it) bwd.push_back(*it);
      assert(fwd == std::vector<int64>(std::begin(ref), std::end(ref)));
      assert(bwd == std::vector<int64>(std::rbegin(ref), std::rend(ref)));
      auto it = s.begin();
      if (it != s.end()) {
        auto old = it++;
        assert(old == s.begin());
        it--;
        assert(it == s.begin());
      }
    }
  }
}

PE_REGISTER_TEST(&SplayMultiSetTest, "SplayMultiSetTest", SMALL);
}  // namespace tree_test
