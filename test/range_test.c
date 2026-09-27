#include "pe_test.h"

namespace range_test {

template <typename R>
SL std::string ToStr(const R& r) {
  std::ostringstream ss;
  ss << r;
  return ss.str();
}

template <typename T>
SL std::vector<T> Sorted(std::vector<T> v) {
  std::sort(std::begin(v), std::end(v));
  return v;
}

SL void NumberRangeTest() {
  static_assert(std::is_same_v<decltype(Range(1, 5)), NumberRange<int64>>);
  static_assert(
      std::is_same_v<decltype(Range(1, 5, 2)), NumberRangeD<int64>>);
  static_assert(std::is_same_v<decltype(XRange(1u, 5u)), NumberRange<int64>>);

  // Range(s, e) is [s, e).
  {
    std::vector<int64> got;
    for (auto i : Range(-2, 3)) got.push_back(i);
    assert((got == std::vector<int64>{-2, -1, 0, 1, 2}));
    assert(Range(-2, 3).Count() == 5);
    assert(Range(4, 4).Count() == 0 && Range(4, 4).IsEmpty());
    assert(!Range(4, 5).IsEmpty());
    assert(Range(0LL, 1000000000000LL).Count() == 1000000000000LL);
    assert(Range(static_cast<uint32>(3), static_cast<uint32>(7)).Count() ==
           4);
  }
  // XRange(s, e) is [s, e]; s > e is empty.
  {
    assert(XRange(1, 10).ToVector() == Range(1, 11).ToVector());
    assert(XRange(5, 5).ToVector() == std::vector<int64>{5});
    assert(XRange(5, 4).IsEmpty() && XRange(5, 4).Count() == 0);
    assert(XRange(5, -3).Count() == 0);
    assert(XRange(-3, 3).Sum() == 0);
  }
  // Range(s, e, delta) is s, s + delta, ... while before e.
  {
    assert((Range(1, 10, 2).ToVector() == std::vector<int64>{1, 3, 5, 7, 9}));
    assert((Range(0, 10, 2).ToVector() == std::vector<int64>{0, 2, 4, 6, 8}));
    assert((Range(10, 0, -2).ToVector() ==
            std::vector<int64>{10, 8, 6, 4, 2}));
    assert((Range(10, 1, -2).ToVector() ==
            std::vector<int64>{10, 8, 6, 4, 2}));
    assert(Range(1, 10, 2).Count() == 5);
    assert(Range(10, 1, -2).Count() == 5);
    assert(Range(3, 3, 1).IsEmpty() && Range(3, 3, 1).Count() == 0);
    // delta may be 0 for an empty range.
    assert(Range(3, 3, 0).IsEmpty() && Range(3, 3, 0).Count() == 0);
    assert(Range(0, 1, 100).ToVector() == std::vector<int64>{0});
    // XRange(s, e, delta) includes e when it is hit.
    assert((XRange(1, 10, 3).ToVector() == std::vector<int64>{1, 4, 7, 10}));
    assert((XRange(1, 9, 3).ToVector() == std::vector<int64>{1, 4, 7}));
    assert((XRange(10, 1, -3).ToVector() ==
            std::vector<int64>{10, 7, 4, 1}));
    assert(XRange(3, 3, 1).ToVector() == std::vector<int64>{3});
    assert(XRange(3, 3, -1).ToVector() == std::vector<int64>{3});
    assert(XRange(3, 3, -1).Count() == 1);
    // Brute force against a plain loop.
    for (int64 s = -6; s <= 6; ++s) {
      for (int64 e = -6; e <= 6; ++e) {
        for (int64 d = -4; d <= 4; ++d) {
          if (d == 0) continue;
          if ((s < e && d < 0) || (s > e && d > 0)) continue;
          std::vector<int64> expected, expected_x;
          if (d > 0) {
            for (int64 i = s; i < e; i += d) expected.push_back(i);
            for (int64 i = s; i <= e; i += d) expected_x.push_back(i);
          } else {
            for (int64 i = s; i > e; i += d) expected.push_back(i);
            for (int64 i = s; i >= e; i += d) expected_x.push_back(i);
          }
          auto r = Range(s, e, d);
          assert(r.ToVector() == expected);
          assert(r.Count() == static_cast<int64>(expected.size()));
          auto xr = XRange(s, e, d);
          assert(xr.ToVector() == expected_x);
          assert(xr.Count() == static_cast<int64>(expected_x.size()));
        }
      }
    }
  }
  // Iterator operations.
  {
    auto r = Range(10, 20);
    auto it = r.begin();
    assert(*it == 10);
    auto old = it++;
    assert(*old == 10 && *it == 11);
    ++it;
    assert(*it == 12);
    old = it--;
    assert(*old == 12 && *it == 11);
    --it;
    assert(it == r.begin() && it != r.end());

    auto rd = Range(0, 10, 3);
    auto jt = rd.begin();
    assert(*jt == 0);
    auto jold = jt++;
    assert(*jold == 0 && *jt == 3);
    ++jt;
    assert(*jt == 6);
    --jt;
    assert(*jt == 3);
    // NumberIterD has const members and is not assignable.
    auto jold2 = jt--;
    assert(*jold2 == 3 && *jt == 0 && jt == rd.begin());
  }
}

PE_REGISTER_TEST(&NumberRangeTest, "RangeNumberRangeTest", SMALL);

SL void ContainerRangeTest() {
  // Vector.
  {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    auto r = Range(v);
    assert(r.Count() == 8);
    assert(r.Sum() == 31);
    assert(r.ToVector() == v);
    // ForEach modifies the elements through a mutable range.
    Range(v).ForEach([](int& x) { x *= 2; }).ForEach([](int& x) { ++x; });
    assert((v == std::vector<int>{7, 3, 9, 3, 11, 19, 5, 13}));
    // Const container.
    const std::vector<int>& cv = v;
    static_assert(std::is_same_v<decltype(Range(cv)),
                                 ContainerRange<std::vector<int>::const_iterator>>);
    assert(Range(cv).Sum() == 70);
    // Iterator pair (non-integer, non-pointer).
    assert(Range(v.begin() + 1, v.end()).Count() == 7);
    assert(Range(v.begin() + 1, v.begin() + 3).Sum() == 12);
    assert(Range(v.begin(), v.begin()).IsEmpty());
    // Iterator operations.
    auto it = r.begin();
    assert(*it == 7);
    auto old = it++;
    assert(*old == 7 && *it == 3);
    old = it--;
    assert(*old == 3 && *it == 7);
    ++it;
    --it;
    assert(it == r.begin());
    *it = 100;
    assert(v[0] == 100);
  }
  // Empty container.
  {
    std::vector<int> v;
    auto r = Range(v);
    assert(r.IsEmpty() && r.Count() == 0 && r.Sum() == 0 && r.Prod() == 1);
    assert(!r.Max().has_value() && !r.Min().has_value());
    assert(r.ToVector().empty() && r.ToSet().empty());
    assert(ToStr(r) == "{}");
  }
  // std::array: iterator is a pointer in libstdc++, other libraries may use a
  // class; both go through ContainerRange.
  {
    std::array<int, 5> a{5, 4, 3, 2, 1};
    assert(Range(a).Count() == 5);
    assert(Range(a).Prod() == 120);
    const std::array<int, 5>& ca = a;
    assert(Range(ca).Sum() == 15);
  }
  // Set and map.
  {
    std::set<int> s{5, 1, 3};
    assert((Range(s).ToVector() == std::vector<int>{1, 3, 5}));
    assert(Range(s).Count() == 3);
    std::map<int, int> m{{1, 2}, {2, 3}, {7, 10}};
    assert(Range(m).Map<int64>([](const auto& p) { return p.second; }).Sum() ==
           15);
    assert(Range(m).Map<int64>([](const auto& p) { return p.first; }).Sum() ==
           10);
    Range(m).ForEach([](std::pair<const int, int>& p) { p.second *= 10; });
    assert(m[7] == 100 && m[1] == 20);
  }
  // Range of a range.
  {
    auto r = Range(Range(1, 6));
    assert(r.Count() == 5 && r.Sum() == 15);
  }
  // VectorRange owns its data.
  {
    VectorRange<int64> vr(std::vector<int64>{4, -2, 7});
    assert(vr.Count() == 3 && vr.Sum() == 9);
    const std::vector<int64> src{1, 2};
    VectorRange<int64> vr2(src);
    assert(vr2.ToVector() == src);
    assert(ToStr(vr2) == "{1, 2}");
  }
}

PE_REGISTER_TEST(&ContainerRangeTest, "RangeContainerRangeTest", SMALL);

SL void ArrayRangeTest() {
  int a[6] = {1, 2, 3, 4, 5, 6};
  static_assert(std::is_same_v<decltype(Range(a)), ArrayRange<int>>);
  assert(Range(a).Count() == 6);
  assert(Range(a).Sum() == 21);
  assert(Range(a).Prod() == 720);
  assert(Range(a, a + 3).Sum() == 6);
  assert(Range(a + 2, a + 2).IsEmpty() && Range(a + 2, a + 2).Count() == 0);

  const int* a0 = a;
  const int* a1 = a + 6;
  static_assert(std::is_same_v<decltype(Range(a0, a1)), ArrayRange<const int>>);
  assert(Range(a0, a1).Prod() == 720);

  const int ca[3] = {7, 8, 9};
  assert(Range(ca).Sum() == 24);

  // Modification through an ArrayRange<int>.
  Range(a).ForEach([](int& x) { x = -x; });
  assert(a[0] == -1 && a[5] == -6);
  auto r = Range(a);
  auto it = r.begin();
  auto old = it++;
  assert(*old == -1 && *it == -2);
  old = it--;
  assert(*old == -2 && *it == -1);
  ++it;
  --it;
  assert(it == r.begin() && it != r.end());
  *it = 11;
  assert(a[0] == 11);
}

PE_REGISTER_TEST(&ArrayRangeTest, "RangeArrayRangeTest", SMALL);

SL void AggregateTest() {
  // Sum / Prod / SumMod / ProdMod.
  assert(Range(1, 101).Sum() == 5050);
  assert(Range(1, 11).Prod() == 3628800);
  assert(Range(1, 21).Prod() == 2432902008176640000LL);
  assert(Range(1, 1).Prod() == 1);
  assert(Range(1, 11).Sum<int>() == 55);
  assert(Range(1, 11).Prod<double>() == 3628800.0);
  const int64 mod = 1000000007;
  {
    int64 expected = 1;
    for (int64 i = 1; i < 1000; ++i) expected = expected * i % mod;
    assert(Range(1, 1000).ProdMod(mod) == expected);
  }
  assert(Range(1, 1000).SumMod(mod) == 499500);
  assert(Range(1, 1000).SumMod(int64{7}) == 499500 % 7);
  // Negative values are reduced into [0, mod).
  assert(Range(-10, 1).SumMod(int64{7}) == 1);  // -55 mod 7
  assert(Range(-3, 0).ProdMod(int64{7}) == 1);  // -6 mod 7
  assert(Range(0, 0).SumMod(int64{7}) == 0);
  assert(Range(0, 0).ProdMod(int64{7}) == 1);
  assert(Range(0, 0).ProdMod(int64{1}) == 0);
  assert(Range(2, 5).ProdMod(int64{1}) == 0);

  // Max / Min.
  std::vector<int> v{3, -1, 4, 1, -5, 9, 2, 6};
  assert(*Range(v).Max() == 9 && *Range(v).Min() == -5);
  assert(*Range(7, 8).Max() == 7 && *Range(7, 8).Min() == 7);
  assert(!Range(7, 7).Max().has_value() && !Range(7, 7).Min().has_value());

  // Sort / Distinct / Reverse.
  std::vector<int> w{3, 1, 3, 2, 1};
  assert((Range(w).Sort().ToVector() == std::vector<int>{1, 1, 2, 3, 3}));
  assert((Range(w).Distinct().ToVector() == std::vector<int>{1, 2, 3}));
  assert((Range(w).Reverse().ToVector() == std::vector<int>{1, 2, 3, 1, 3}));
  assert((w == std::vector<int>{3, 1, 3, 2, 1}));  // Source untouched.
  assert(Range(0, 0).Sort().IsEmpty() && Range(0, 0).Distinct().IsEmpty() &&
         Range(0, 0).Reverse().IsEmpty());

  // Skip / Limit.
  assert((Range(0, 6).Skip(2).ToVector() == std::vector<int64>{2, 3, 4, 5}));
  assert(Range(0, 6).Skip(0).Count() == 6);
  assert(Range(0, 6).Skip(-3).Count() == 6);
  assert(Range(0, 6).Skip(6).IsEmpty() && Range(0, 6).Skip(100).IsEmpty());
  assert((Range(0, 6).Limit(3).ToVector() == std::vector<int64>{0, 1, 2}));
  assert(Range(0, 6).Limit(0).IsEmpty() && Range(0, 6).Limit(-1).IsEmpty());
  assert(Range(0, 6).Limit(100).Count() == 6);
  assert((Range(0, 10).Skip(3).Limit(2).ToVector() ==
          std::vector<int64>{3, 4}));

  // AnyMatch / AllMatch / NoneMatch.
  auto even = [](int64 x) { return x % 2 == 0; };
  auto neg = [](int64 x) { return x < 0; };
  assert(Range(1, 5).AnyMatch(even) && !Range(1, 5).AllMatch(even) &&
         !Range(1, 5).NoneMatch(even));
  assert(!Range(1, 5).AnyMatch(neg) && Range(1, 5).NoneMatch(neg));
  assert(Range(0, 10, 2).AllMatch(even));
  assert(!Range(0, 0).AnyMatch(even) && Range(0, 0).AllMatch(even) &&
         Range(0, 0).NoneMatch(even));

  // Fill / ToVector / ToSet.
  {
    std::vector<int64> out;
    Range(3, 6).Fill(std::back_inserter(out));
    assert((out == std::vector<int64>{3, 4, 5}));
    int64 buf[4] = {0, 0, 0, -1};
    Range(7, 10).Fill(buf);
    assert(buf[0] == 7 && buf[2] == 9 && buf[3] == -1);
    assert((Range(w).ToSet() == std::set<int>{1, 2, 3}));
  }

  // Reduce / InplaceReduce.
  assert(Range(1, 5).Reduce(int64{10}, std::plus<int64>()) == 20);
  assert(Range(1, 5).Reduce(std::string(),
                            [](const std::string& s, int64 x) {
                              return s + std::to_string(x);
                            }) == "1234");
  assert(Range(0, 0).Reduce(int64{42}, std::plus<int64>()) == 42);
  assert(Range(1, 6).InplaceReduce(int64{1},
                                   [](int64& a, int64 b) { a *= b; }) == 120);
  assert(Range(v).InplaceReduce(std::vector<int>{}, [](std::vector<int>& a,
                                                       int b) {
    a.push_back(b);
  }) == v);

  // Stream output.
  assert(ToStr(Range(1, 4)) == "{1, 2, 3}");
  assert(ToStr(Range(1, 2)) == "{1}");
  assert(ToStr(Range(0, 0)) == "{}");
  assert(ToStr(Range(10, 0, -4)) == "{10, 6, 2}");
  assert(ToStr(Range(w)) == "{3, 1, 3, 2, 1}");
}

PE_REGISTER_TEST(&AggregateTest, "RangeAggregateTest", SMALL);

SL void FilterMapTest() {
  auto even = [](int64 x) { return x % 2 == 0; };
  // Filter.
  {
    auto r = Range(1, 11).Filter(even);
    assert((r.ToVector() == std::vector<int64>{2, 4, 6, 8, 10}));
    assert(r.Count() == 5 && r.Sum() == 30);
    assert(*r.Max() == 10 && *r.Min() == 2);
    assert(ToStr(r) == "{2, 4, 6, 8, 10}");
    auto it = r.begin();
    auto old = it++;
    assert(*old == 2 && *it == 4);
    ++it;
    assert(*it == 6);
    // Nothing matches / everything matches / first and last elements.
    assert(Range(1, 11).Filter([](int64) { return 0; }).IsEmpty());
    assert(Range(1, 11).Filter([](int64) { return 0; }).Count() == 0);
    assert(Range(1, 11).Filter([](int64) { return 1; }).Count() == 10);
    assert(Range(0, 0).Filter(even).IsEmpty());
    assert((Range(1, 11)
                .Filter([](int64 x) { return x == 1 || x == 10; })
                .ToVector() == std::vector<int64>{1, 10}));
  }
  // Chained Filter / Map, stored and iterated after the temporaries are gone.
  {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto r = Range(v)
                 .Filter([](int x) { return x % 2 == 0; })
                 .Filter([](int x) { return x % 3 == 0; })
                 .Map<int64>([](int x) { return int64{x} * 3; })
                 .Map<int64>([](int64 x) { return x + 1; });
    assert((r.ToVector() == std::vector<int64>{19, 37}));
    assert(r.Count() == 2 && r.Sum() == 56);
    auto r2 = Range(1, 30)
                  .Map<int64>([](int64 x) { return x * x; })
                  .Filter([](int64 x) { return x % 10 == 1; });
    assert((r2.ToVector() == std::vector<int64>{1, 81, 121, 361, 441, 841}));
  }
  // Map.
  {
    auto sq = Range(0, 5).Map<int64>([](int64 x) { return x * x; });
    assert((sq.ToVector() == std::vector<int64>{0, 1, 4, 9, 16}));
    assert(sq.Count() == 5 && sq.Sum() == 30 && *sq.Max() == 16);
    auto it = sq.begin();
    auto old = it++;
    assert(*old == 0 && *it == 1);
    ++it;
    assert(*it == 4);
    old = it--;
    assert(*old == 4 && *it == 1);
    --it;
    assert(it == sq.begin());
    // Map to a different type.
    auto strs = Range(8, 11).Map<std::string>(
        [](int64 x) { return std::to_string(x); });
    assert((strs.ToVector() == std::vector<std::string>{"8", "9", "10"}));
    assert(Range(0, 0).Map<int64>([](int64 x) { return x; }).IsEmpty());
    // Map over an array.
    int a[3] = {1, 2, 3};
    assert(Range(a).Map<double>([](int x) { return x / 2.0; }).Sum<double>() ==
           3.0);
  }
}

PE_REGISTER_TEST(&FilterMapTest, "RangeFilterMapTest", SMALL);

SL void ParallelTest() {
  auto div3 = [](int64 x) { return x % 3 == 0; };
  // CalculateDefaultPartitionSize.
  {
    auto r = Range(0, 1);
    assert(r.CalculateDefaultPartitionSize(100, 8, 0) == 6);
    assert(r.CalculateDefaultPartitionSize(5, 8, 0) == 1);
    assert(r.CalculateDefaultPartitionSize(100, 8, 7) == 7);
  }
  const std::vector<int64> expected_div3 = Range(0, 1000).Filter(div3).ToVector();
  std::vector<int64> data = Range(0, 1000).ToVector();
  for (int tn : {1, 2, 3, 8}) {
    for (int64 ps : {0, 1, 7, 1000}) {
      // PFilter. With OpenMP the order is not kept (see TODO(bug) at
      // RangeBase::PFilter in pe_range), so compare sorted results there.
      {
        auto a = Range(0, 1000).PFilter(div3, tn, ps).ToVector();
        auto b = Range(data).PFilter(div3, tn, ps).ToVector();
#if ENABLE_OPENMP
        a = Sorted(a);
        b = Sorted(b);
#endif
        assert(a == expected_div3);
        assert(b == expected_div3);
        assert(Range(0, 0).PFilter(div3, tn, ps).IsEmpty());
        assert(Range(std::vector<int64>{}).PFilter(div3, tn, ps).IsEmpty());
      }
      // PMap keeps the order.
      {
        auto sq = [](int64 x) { return x * x; };
        auto a = Range(0, 1000).PMap<int64>(sq, tn, ps).ToVector();
        auto b = Range(data).PMap<int64>(sq, tn, ps).ToVector();
        for (int64 i = 0; i < 1000; ++i) {
          assert(a[i] == i * i && b[i] == i * i);
        }
        assert(a.size() == 1000u && b.size() == 1000u);
        assert(Range(5, 5).PMap<int64>(sq, tn, ps).IsEmpty());
        assert(Range(std::vector<int64>{}).PMap<int64>(sq, tn, ps).IsEmpty());
        // Non-zero start.
        auto c = Range(10, 13).PMap<int64>(sq, tn, ps).ToVector();
        assert((c == std::vector<int64>{100, 121, 144}));
      }
      // PReduce folds the seed in exactly once.
      {
        auto plus = std::plus<int64>();
        assert(Range(1, 4).PReduce(int64{10}, plus, tn, ps) == 16);
        assert(Range(0, 1000).PReduce(int64{7}, plus, tn, ps) == 499507);
        assert(Range(data).PReduce(int64{7}, plus, tn, ps) == 499507);
        assert(Range(5, 5).PReduce(int64{42}, plus, tn, ps) == 42);
        assert(Range(std::vector<int64>{}).PReduce(int64{42}, plus, tn, ps) ==
               42);
        auto mx = [](int64 a, int64 b) { return std::max(a, b); };
        assert(Range(0, 1000).PReduce(int64{-1}, mx, tn, ps) == 999);
        assert(Range(data).PReduce(int64{5000}, mx, tn, ps) == 5000);
      }
      // PInplaceReduce.
      {
        auto add = [](int64& a, int64 b) { a += b; };
        assert(Range(1, 4).PInplaceReduce(int64{10}, add, tn, ps) == 16);
        assert(Range(0, 1000).PInplaceReduce(int64{7}, add, tn, ps) == 499507);
        assert(Range(data).PInplaceReduce(int64{7}, add, tn, ps) == 499507);
        assert(Range(5, 5).PInplaceReduce(int64{42}, add, tn, ps) == 42);
        assert(Range(std::vector<int64>{}).PInplaceReduce(int64{42}, add, tn,
                                                          ps) == 42);
        const int64 mod = 1000000007;
        int64 fact = 1;
        for (int64 i = 1; i < 1000; ++i) fact = fact * i % mod;
        assert(Range(1, 1000).PInplaceReduce(
                   int64{1},
                   [&](int64& a, int64 b) { a = a * b % mod; }, tn, ps) ==
               fact);
      }
      // PForEach. With OpenMP the action runs on copies (see TODO(bug) at
      // RangeBase::PForEach), so only side effects outside the range are
      // checked.
      {
        std::atomic<int64> sum{0};
        auto r = Range(data);
        r.PForEach([&](int64 x) { sum += x; }, tn, ps);
        assert(sum == 499500);
        sum = 0;
        std::vector<int64> empty;
        Range(empty).PForEach([&](int64 x) { sum += x + 1; }, tn, ps);
        assert(sum == 0);
      }
    }
  }
  // Default arguments and the template variants.
  {
    assert(Range(1, 101).PReduce(int64{0}, std::plus<int64>()) == 5050);
    assert(Range(1, 101).PInplaceReduce(int64{0}, [](int64& a, int64 b) {
      a += b;
    }) == 5050);
    assert(Range(0, 100).PMap<int64>([](int64 x) { return 2 * x; }).Sum() ==
           9900);
    assert(Range(0, 100).PMapT<int64>([](int64 x) { return 2 * x; }).Sum() ==
           9900);
    assert((Range(0, 100).PMapT<int64, 3, 5>([](int64 x) { return x; })
                .ToVector() == Range(0, 100).ToVector()));
    assert(Range(0, 100).PFilterT(div3).Count() == 34);
    assert((Range(0, 100).PFilterT<3, 5>(div3).Sum() == 1683));
    assert(Range(data).PFilterT(div3).Count() == 334);
    assert((Range(data).PFilterT<2, 11>(div3).Sum() ==
            Range(0, 1000).Filter(div3).Sum()));
    assert(Range(data).PMapT<int64>([](int64 x) { return x + 1; }).Sum() ==
           500500);
    std::atomic<int64> cnt{0};
    Range(0, 64).PForEach([&](int64) { ++cnt; });
    assert(cnt == 64);
  }
  // Parallel operations on other range kinds.
  {
    std::map<int, int> m{{1, 2}, {2, 3}, {5, 7}};
    assert(Range(m).PMap<int64>([](const auto& p) { return p.second; }).Sum() ==
           12);
    int a[6] = {1, 2, 3, 4, 5, 6};
    assert(Range(a).PInplaceReduce(0, [](int& x, int y) { x += y; }) == 21);
    assert(Range(a).PReduce(int64{1}, std::multiplies<int64>(), 3, 1) == 720);
    assert(Range(1, 20, 2).PReduce(int64{0}, std::plus<int64>(), 4) == 100);
    assert(Range(1, 20)
               .Filter([](int64 x) { return x % 2 == 1; })
               .PMap<int64>([](int64 x) { return x; }, 4)
               .Sum() == 100);
  }
}

PE_REGISTER_TEST(&ParallelTest, "RangeParallelTest", SMALL);

SL void IRangeTest() {
  // Vector: iter.i is the index, iter.v a reference to the element.
  {
    std::vector<int> v{10, 20, 30};
    for (auto iter : IRange(v)) {
      assert(iter.v == 10 * (iter.i + 1));
      iter.v += static_cast<int>(iter.i);
    }
    assert((v == std::vector<int>{10, 21, 32}));
    const std::vector<int>& cv = v;
    int64 cnt = 0;
    for (auto iter : IRange(cv)) {
      assert(iter.i == cnt && iter.v == cv[cnt]);
      ++cnt;
    }
    assert(cnt == 3);
    std::vector<int> empty;
    for (auto iter : IRange(empty)) {
      (void)iter;
      assert(false);
    }
  }
  // C array and pointer pair.
  {
    int a[5] = {5, 4, 3, 2, 1};
    for (auto iter : IRange(a)) {
      assert(iter.v == 5 - iter.i);
      ++iter.v;
    }
    assert(a[0] == 6 && a[4] == 2);
    int64 cnt = 0;
    for (auto iter : IRange(a + 1, a + 4)) {
      assert(iter.i == cnt && iter.v == a[1 + cnt]);
      ++cnt;
    }
    assert(cnt == 3);
    for (auto iter : IRange(a + 2, a + 2)) {
      (void)iter;
      assert(false);
    }
    const int ca[3] = {1, 2, 3};
    int64 s = 0;
    for (auto iter : IRange(ca)) s += iter.i * iter.v;
    assert(s == 0 * 1 + 1 * 2 + 2 * 3);
    const int* p0 = ca;
    s = 0;
    for (auto iter : IRange(p0, p0 + 3)) s += iter.v;
    assert(s == 6);

    // Iterator operations.
    auto r = IRange(a);
    auto it = r.begin();
    auto old = it++;
    assert((*old).i == 0 && (*it).i == 1 && (*it).v == a[1]);
    old = it--;
    assert((*old).i == 1 && (*it).i == 0);
    ++it;
    --it;
    assert(it == r.begin() && it != r.end());
  }
  // Associative containers.
  {
    std::set<int> st{9, 3, 6};
    int64 cnt = 0;
    for (auto iter : IRange(st)) {
      assert(iter.i == cnt && iter.v == 3 * (cnt + 1));
      ++cnt;
    }
    assert(cnt == 3);
    std::map<int, int> m{{5, 10}, {7, 20}};
    for (auto iter : IRange(m)) {
      iter.v.second += static_cast<int>(iter.i);
    }
    assert(m[5] == 10 && m[7] == 21);
    std::list<int> lst{1, 2, 3};
    auto r = IRange(lst);
    auto it = r.begin();
    auto old = it++;
    assert((*old).i == 0 && (*old).v == 1 && (*it).i == 1 && (*it).v == 2);
    old = it--;
    assert((*old).i == 1 && (*it).i == 0 && (*it).v == 1);
    ++it;
    --it;
    assert(it == r.begin() && it != r.end());
  }
  // A number range and nested IRange.
  {
    int64 cnt = 0;
    for (auto iter : IRange(Range(100, 105))) {
      assert(iter.i == cnt && iter.v == 100 + cnt);
      ++cnt;
    }
    assert(cnt == 5);
    std::vector<int> v{1, 2, 3};
    for (auto iter : IRange(IRange(v))) {
      assert(iter.i == iter.v.i);
      ++iter.v.v;
    }
    assert((v == std::vector<int>{2, 3, 4}));
  }
}

PE_REGISTER_TEST(&IRangeTest, "RangeIRangeTest", SMALL);

SL void MakeSegmentTest() {
  using Seg = std::vector<std::pair<int64, int64>>;
  assert((MakeSegment(1, 10, 3) == Seg{{1, 3}, {4, 6}, {7, 9}, {10, 10}}));
  assert((MakeSegment(1, 9, 3) == Seg{{1, 3}, {4, 6}, {7, 9}}));
  assert((MakeSegment(5, 5, 1) == Seg{{5, 5}}));
  assert((MakeSegment(5, 5, 100) == Seg{{5, 5}}));
  assert((MakeSegment(-3, 2, 4) == Seg{{-3, 0}, {1, 2}}));
  assert(MakeSegment(5, 4, 1).empty());
  assert(MakeSegment(5, -10, 3).empty());
  for (int64 s = -5; s <= 5; ++s) {
    for (int64 e = s - 2; e <= s + 12; ++e) {
      for (int64 b = 1; b <= 6; ++b) {
        Seg seg = MakeSegment(s, e, b);
        if (s > e) {
          assert(seg.empty());
          continue;
        }
        assert(static_cast<int64>(seg.size()) == (e - s + b) / b);
        int64 next = s;
        for (int64 i = 0; i < static_cast<int64>(seg.size()); ++i) {
          assert(seg[i].first == next);
          assert(seg[i].first <= seg[i].second);
          const int64 len = seg[i].second - seg[i].first + 1;
          if (i + 1 < static_cast<int64>(seg.size())) {
            assert(len == b);
          } else {
            assert(len >= 1 && len <= b);
          }
          next = seg[i].second + 1;
        }
        assert(next == e + 1);
      }
    }
  }
}

PE_REGISTER_TEST(&MakeSegmentTest, "RangeMakeSegmentTest", SMALL);

}  // namespace range_test
