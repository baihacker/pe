#include "pe_test.h"

namespace span_test {

template <typename T>
SL std::string ToStr(Span<T> s) {
  std::ostringstream ss;
  ss << s;
  return ss.str();
}

SL int64 SumConst(Span<const int64> s) {
  int64 ret = 0;
  for (auto x : s) ret += x;
  return ret;
}

template <typename T>
SL int64 SpanSum(Span<T> s) {
  int64 ret = 0;
  for (int64 i = 0; i < s.size(); ++i) ret += s[i];
  return ret;
}
ADAPT_TO_SPAN(SpanSum)

template <typename T>
SL void SpanDouble(Span<T> s) {
  for (auto& x : s) x *= 2;
}
ADAPT_TO_SPAN(SpanDouble)

template <typename T, typename U>
SL int64 SpanDot(Span<T> a, Span<U> b, int64 offset) {
  int64 ret = offset;
  for (int64 i = 0; i < a.size() && i < b.size(); ++i) ret += a[i] * b[i];
  return ret;
}
ADAPT_TO_SPAN(SpanDot)

SL void SpanBasicTest() {
  // Default.
  {
    Span<int> s;
    assert(s.size() == 0 && s.empty() && s.data() == nullptr);
    assert(s.size_bytes() == 0);
    assert(s.begin() == s.end() && s.cbegin() == s.cend());
    assert(ToStr(s) == "{}");
  }
  // Pointer and size.
  {
    int a[5] = {1, 2, 3, 4, 5};
    Span<int> s(a, 5);
    assert(s.size() == 5 && !s.empty() && s.data() == a);
    assert(s.size_bytes() == 5 * static_cast<int64>(sizeof(int)));
    assert(s.front() == 1 && s.back() == 5);
    assert(s[0] == 1 && s[4] == 5 && s.at(2) == 3);
    s[1] = 20;
    s.at(3) = 40;
    s.front() = 10;
    s.back() = 50;
    assert(a[0] == 10 && a[1] == 20 && a[3] == 40 && a[4] == 50);
    Span<int> t(a + 1, 0);
    assert(t.empty() && t.data() == a + 1);
    // Copy and assignment share the data.
    Span<int> u(s);
    assert(u.data() == a && u.size() == 5);
    Span<int> w;
    w = s;
    assert(w.data() == a && w.size() == 5);
    w[2] = 30;
    assert(s[2] == 30);
  }
  // C arrays.
  {
    int a[4] = {4, 3, 2, 1};
    Span<int> s(a);
    assert(s.size() == 4 && s.data() == a);
    const int ca[3] = {7, 8, 9};
    Span<const int> cs(ca);
    assert(cs.size() == 3 && cs[2] == 9);
    Span<const int> cs2(a);
    assert(cs2.size() == 4 && cs2.data() == a);
  }
  // std::vector.
  {
    std::vector<int> v{1, 2, 3};
    Span<int> s(v);
    assert(s.size() == 3 && s.data() == v.data());
    s[0] = 100;
    assert(v[0] == 100);
    const std::vector<int>& cv = v;
    Span<const int> cs(cv);
    assert(cs.size() == 3 && cs[0] == 100);
    Span<const int> cs2(v);
    assert(cs2.data() == v.data());
    std::vector<int> empty;
    assert(Span<int>(empty).empty());
    // A temporary vector lives until the end of the full expression.
    assert(SumConst(std::vector<int64>{1, 2, 3}) == 6);
  }
  // std::array.
  {
    std::array<int64, 3> a{5, 6, 7};
    Span<int64> s(a);
    assert(s.size() == 3 && s.data() == a.data());
    s[2] = 70;
    assert(a[2] == 70);
    const std::array<int64, 3>& ca = a;
    Span<const int64> cs(ca);
    assert(cs.size() == 3 && cs[2] == 70);
    assert(SumConst(std::array<int64, 2>{3, 4}) == 7);
  }
  // Initializer list (for const spans).
  {
    assert(SumConst({1, 2, 3, 4}) == 10);
    assert(SumConst({}) == 0);
  }
  // Stream output.
  {
    std::vector<int> v{1, 2, 3};
    assert(ToStr(Span<int>(v)) == "{1, 2, 3}");
    assert(ToStr(Span<int>(v.data(), 1)) == "{1}");
    assert(ToStr(Span<const int>(v)) == "{1, 2, 3}");
  }
}

PE_REGISTER_TEST(&SpanBasicTest, "SpanBasicTest", SMALL);

SL void SpanIteratorTest() {
  std::vector<int> v{1, 2, 3, 4, 5};
  Span<int> s(v);
  // Forward iteration.
  {
    std::vector<int> got;
    for (auto x : s) got.push_back(x);
    assert(got == v);
    for (auto& x : s) x += 10;
    assert((v == std::vector<int>{11, 12, 13, 14, 15}));
    auto it = s.begin();
    auto old = it++;
    assert(*old == 11 && *it == 12);
    ++it;
    assert(*it == 13);
    old = it--;
    assert(*old == 13 && *it == 12);
    --it;
    assert(it == s.begin() && it != s.end());
    *it = 1;
    assert(v[0] == 1);
    int64 cnt = 0;
    for (auto jt = s.cbegin(); jt != s.cend(); ++jt) {
      assert(*jt == v[cnt]);
      ++cnt;
    }
    assert(cnt == 5);
  }
  // Reverse iteration.
  {
    std::vector<int> got;
    for (auto it = s.rbegin(); it != s.rend(); ++it) got.push_back(*it);
    assert((got == std::vector<int>{15, 14, 13, 12, 1}));
    got.clear();
    for (auto it = s.crbegin(); it != s.crend(); it++) got.push_back(*it);
    assert((got == std::vector<int>{15, 14, 13, 12, 1}));
    auto it = s.rbegin();
    *it = 50;
    assert(v[4] == 50);
    auto old = it++;
    assert(*old == 50 && *it == 14);
    old = it--;
    assert(*old == 14 && *it == 50);
    ++it;
    --it;
    assert(it == s.rbegin());
  }
  // Indexed iteration.
  {
    std::vector<int> w{10, 20, 30};
    Span<int> t(w);
    int64 cnt = 0;
    for (auto [i, x] : t.Item()) {
      assert(i == cnt && x == w[cnt]);
      x += static_cast<int>(i);
      ++cnt;
    }
    assert(cnt == 3);
    assert((w == std::vector<int>{10, 21, 32}));
    cnt = 0;
    for (auto [i, x] : t.CItem()) {
      static_assert(std::is_same_v<decltype(x), const int&>);
      assert(i == cnt && x == w[cnt]);
      ++cnt;
    }
    assert(cnt == 3);
    // Reversed: i counts from 0 while the values go from the back.
    cnt = 0;
    for (auto [i, x] : t.RItem()) {
      assert(i == cnt && x == w[2 - cnt]);
      x = static_cast<int>(i);
      ++cnt;
    }
    assert(cnt == 3);
    assert((w == std::vector<int>{2, 1, 0}));
    cnt = 0;
    for (auto [i, x] : t.CRItem()) {
      static_assert(std::is_same_v<decltype(x), const int&>);
      assert(i == cnt && x == static_cast<int>(i));
      ++cnt;
    }
    assert(cnt == 3);
    // Iterator operations.
    auto items = t.Item();
    auto it = items.begin();
    auto old = it++;
    assert((*old).i == 0 && (*it).i == 1 && (*it).v == 1);
    old = it--;
    assert((*old).i == 1 && (*it).i == 0);
    ++it;
    --it;
    assert(it == items.begin() && it != items.end());
    auto ritems = t.RItem();
    auto rt = ritems.begin();
    auto rold = rt++;
    assert((*rold).i == 0 && (*rold).v == 0 && (*rt).i == 1 && (*rt).v == 1);
    rold = rt--;
    assert((*rold).i == 1 && (*rt).i == 0);
    ++rt;
    --rt;
    assert(rt == ritems.begin() && rt != ritems.end());
    // Empty spans produce no items.
    Span<int> e;
    for (auto item : e.Item()) {
      (void)item;
      assert(false);
    }
    for (auto item : e.CItem()) {
      (void)item;
      assert(false);
    }
  }
}

PE_REGISTER_TEST(&SpanIteratorTest, "SpanIteratorTest", SMALL);

SL void SpanSubviewTest() {
  std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7};
  Span<int> s(v);
  for (int64 c = 0; c <= 8; ++c) {
    Span<int> f = s.first(c);
    assert(f.size() == c && f.data() == v.data());
    Span<int> l = s.last(c);
    assert(l.size() == c && l.data() == v.data() + (8 - c));
    Span<int> sub = s.subspan(c);
    assert(sub.size() == 8 - c && sub.data() == v.data() + c);
    for (int64 n = 0; c + n <= 8; ++n) {
      Span<int> t = s.subspan(c, n);
      assert(t.size() == n && t.data() == v.data() + c);
      for (int64 i = 0; i < n; ++i) assert(t[i] == c + i);
    }
  }
  assert(ToStr(s.subspan(2, 3)) == "{2, 3, 4}");
  assert(ToStr(s.first(0)) == "{}");
  assert(ToStr(s.last(2)) == "{6, 7}");
  s.subspan(1, 2)[1] = 20;
  assert(v[2] == 20);
  assert(s.subspan(3).subspan(1).first(2).back() == 5);
}

PE_REGISTER_TEST(&SpanSubviewTest, "SpanSubviewTest", SMALL);

SL void CSpanTest() {
  std::vector<int> v{1, 2, 3};
  // From a mutable span, a const span and (inherited constructors) containers.
  Span<int> s(v);
  CSpan<int> c1(s);
  CSpan<int> c2{Span<const int>(v)};
  CSpan<int> c3(v);
  const int a[2] = {8, 9};
  CSpan<int> c4(a);
  CSpan<int> c5;
  static_assert(std::is_same_v<CSpan<int>::element_type, const int>);
  static_assert(std::is_same_v<CSpan<const int>::element_type, const int>);
  static_assert(std::is_same_v<decltype(c1[0]), const int&>);
  assert(c1.data() == v.data() && c1.size() == 3);
  assert(c2.data() == v.data() && c2.size() == 3);
  assert(c3.data() == v.data() && c3.size() == 3);
  assert(c4.size() == 2 && c4[1] == 9);
  assert(c5.empty());
  v[0] = 7;
  assert(c1.front() == 7);
  assert(ToStr<const int>(c1) == "{7, 2, 3}");
  // A CSpan is usable where a Span<const T> is expected.
  std::vector<int64> w{4, 5};
  CSpan<int64> cw(w);
  assert(SumConst(cw) == 9);
  assert(SumConst(CSpan<int64>(Span<int64>(w))) == 9);
}

PE_REGISTER_TEST(&CSpanTest, "SpanCSpanTest", SMALL);

SL void MakeSpanTest() {
  std::vector<int> v{1, 2, 3};
  const std::vector<int>& cv = v;
  static_assert(std::is_same_v<decltype(MakeSpan(v)), Span<int>>);
  static_assert(std::is_same_v<decltype(MakeSpan(cv)), Span<const int>>);
  assert(MakeSpan(v).data() == v.data() && MakeSpan(v).size() == 3);
  assert(MakeSpan(cv).data() == v.data());

  std::array<int, 4> ar{1, 2, 3, 4};
  const std::array<int, 4>& car = ar;
  static_assert(std::is_same_v<decltype(MakeSpan(ar)), Span<int>>);
  static_assert(std::is_same_v<decltype(MakeSpan(car)), Span<const int>>);
  assert(MakeSpan(ar).size() == 4 && MakeSpan(car).data() == ar.data());

  int a[5] = {0};
  const int ca[2] = {1, 2};
  static_assert(std::is_same_v<decltype(MakeSpan(a)), Span<int>>);
  static_assert(std::is_same_v<decltype(MakeSpan(ca)), Span<const int>>);
  assert(MakeSpan(a).size() == 5 && MakeSpan(ca).size() == 2);
  MakeSpan(a)[4] = 9;
  assert(a[4] == 9);

  static_assert(std::is_same_v<decltype(MakeSpan(a + 1, 2)), Span<int>>);
  assert(MakeSpan(a + 1, 2).data() == a + 1 && MakeSpan(a + 1, 2).size() == 2);
  assert(MakeSpan(ca + 0, 2)[1] == 2);
}

PE_REGISTER_TEST(&MakeSpanTest, "SpanMakeSpanTest", SMALL);

SL void AdaptToSpanTest() {
  // AdaptToSpanTransform.
  static_assert(
      std::is_same_v<AdaptToSpanTransformT<std::vector<int>&>, Span<int>>);
  static_assert(
      std::is_same_v<AdaptToSpanTransformT<std::vector<int>&&>, Span<int>>);
  static_assert(std::is_same_v<AdaptToSpanTransformT<const std::vector<int>&>,
                               Span<const int>>);
  static_assert(std::is_same_v<AdaptToSpanTransformT<const std::vector<int>&&>,
                               Span<const int>>);
  static_assert(std::is_same_v<AdaptToSpanTransformT<std::array<int, 3>&>,
                               Span<int>>);
  static_assert(std::is_same_v<AdaptToSpanTransformT<std::array<int, 3>&&>,
                               Span<int>>);
  static_assert(
      std::is_same_v<AdaptToSpanTransformT<const std::array<int, 3>&>,
                     Span<const int>>);
  static_assert(
      std::is_same_v<AdaptToSpanTransformT<const std::array<int, 3>&&>,
                     Span<const int>>);
  static_assert(std::is_same_v<AdaptToSpanTransformT<int (&)[3]>, Span<int>>);
  static_assert(std::is_same_v<AdaptToSpanTransformT<const int (&)[3]>,
                               Span<const int>>);
  static_assert(AdaptToSpanTransform<std::vector<int>&>::value);
  static_assert(AdaptToSpanTransform<const int (&)[3]>::value);
  static_assert(!AdaptToSpanTransform<int>::value);
  static_assert(!AdaptToSpanTransform<Span<int>>::value);
  static_assert(!AdaptToSpanTransform<std::vector<int>>::value);
  static_assert(std::is_same_v<AdaptToSpanTransformT<int&>, int&>);

  // span_like recognizes Span and CSpan.
  static_assert(is_span_like_v<Span<int>>);
  static_assert(is_span_like_v<Span<int>&>);
  static_assert(is_span_like_v<CSpan<int>>);
  static_assert(std::is_same_v<span_like<Span<int>>::element_type, int>);
  static_assert(
      std::is_same_v<span_like<Span<const int>>::element_type, const int>);
  static_assert(std::is_same_v<span_like<CSpan<int>>::element_type, const int>);

  // ADAPT_TO_SPAN.
  std::vector<int> v{1, 2, 3};
  const std::vector<int>& cv = v;
  std::array<int64, 2> ar{10, 20};
  int a[3] = {4, 5, 6};
  const int ca[2] = {7, 8};
  assert(SpanSum(v) == 6);
  assert(SpanSum(cv) == 6);
  assert(SpanSum(ar) == 30);
  assert(SpanSum(a) == 15);
  assert(SpanSum(ca) == 15);
  assert(SpanSum(Span<int>(v)) == 6);
  assert(SpanSum(std::vector<int>{2, 3}) == 5);
  SpanDouble(v);
  SpanDouble(a);
  SpanDouble(ar);
  assert((v == std::vector<int>{2, 4, 6}));
  assert(a[0] == 8 && a[2] == 12);
  assert(ar[1] == 40);
  // Mixed arguments: containers, spans and non-span values.
  assert(SpanDot(v, a, 1) == 1 + 2 * 8 + 4 * 10 + 6 * 12);
  assert(SpanDot(Span<int>(v), cv, 0) == 4 + 16 + 36);
  assert(SpanDot(ar, Span<const int>(ca), 0) == 20 * 7 + 40 * 8);
}

PE_REGISTER_TEST(&AdaptToSpanTest, "SpanAdaptToSpanTest", SMALL);

}  // namespace span_test
