#include "pe_test.h"

namespace parallel_algo_test {

SL void ParallelExecuteTest() {
  // Empty task list.
  ParallelExecute({});
  ParallelExecute({}, 1);

  for (int tn : {1, 2, 4, 8}) {
    for (int n : {1, 3, 17, 64}) {
      std::vector<int64> out(n, -1);
      std::vector<std::function<void()>> tasks;
      for (int i = 0; i < n; ++i) {
        tasks.push_back([&out, i]() {
          int64 s = 0;
          for (int j = 0; j <= i; ++j) s += j;
          out[i] = s;
        });
      }
      ParallelExecute(tasks, tn);
      for (int i = 0; i < n; ++i) {
        assert(out[i] == static_cast<int64>(i) * (i + 1) / 2);
      }
    }
  }
}

template <int TN>
SL void ParallelSortSmallImpl() {
  for (int n : {0, 1, 2, 3, 5, 7, 8, 9, 31, 100, 1001}) {
    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = (i * 7919 + 13) % 97 - 48;
    std::vector<int> expected = arr;
    std::sort(std::begin(expected), std::end(expected));
    ParallelSort<TN>(std::data(arr), std::data(arr) + n);
    assert(arr == expected);
  }
}

SL void ParallelSortSmallTest() {
  ParallelSortSmallImpl<1>();
  ParallelSortSmallImpl<2>();
  ParallelSortSmallImpl<3>();
  ParallelSortSmallImpl<5>();
  ParallelSortSmallImpl<8>();
}

// Brute force reference for the bounded searches.
template <typename T>
SL T BruteFirst(T first, T last, const std::function<bool(T)>& f) {
  for (T i = first; i <= last; ++i) {
    if (f(i)) return i;
  }
  return last + 1;
}

template <typename T>
SL T BruteLast(T first, T last, const std::function<bool(T)>& f) {
  for (T i = last; i >= first; --i) {
    if (f(i)) return i;
  }
  return first - 1;
}

template <typename T>
SL void FindFirstLastSerialImpl() {
  // Predicate "x is a multiple of m", with m large enough to give gaps.
  for (T m : {T(1), T(7), T(50), T(1000)}) {
    std::function<bool(T)> f = [m](T x) { return x % m == 0; };
    for (T first : {T(-120), T(-7), T(0), T(1), T(33)}) {
      for (T len : {T(0), T(1), T(2), T(10), T(200)}) {
        const T last = first + len - 1;
        const T a = FindFirst<T>(first, last, f);
        assert(a == BruteFirst<T>(first, last, f));
        const T b = FindLast<T>(first, last, f);
        assert(b == BruteLast<T>(first, last, f));
      }
    }
  }

  // Empty range: first > last.
  std::function<bool(T)> always = [](T) { return true; };
  assert(FindFirst<T>(T(10), T(5), always) == T(6));
  assert(FindLast<T>(T(10), T(5), always) == T(9));
  assert(FindFirst<T>(T(-3), T(-4), always) == T(-3));
  assert(FindLast<T>(T(-3), T(-4), always) == T(-4));

  // Single element range.
  assert(FindFirst<T>(T(4), T(4), always) == T(4));
  assert(FindLast<T>(T(4), T(4), always) == T(4));
  std::function<bool(T)> never = [](T) { return false; };
  assert(FindFirst<T>(T(4), T(4), never) == T(5));
  assert(FindLast<T>(T(4), T(4), never) == T(3));

  // Unbounded searches with a predicate.
  std::function<bool(T)> sq = [](T x) { return x > 0 && x * x >= 1000; };
  assert(FindFirst<T>(T(-50), sq) == T(32));
  assert(FindFirst<T>(T(32), sq) == T(32));
  std::function<bool(T)> neg = [](T x) { return x * x <= 1000 && x < 0; };
  assert(FindLast<T>(T(-1), neg) == T(-1));
  assert(FindLast<T>(T(100), neg) == T(-1));
  std::function<bool(T)> below = [](T x) { return x < -37; };
  assert(FindLast<T>(T(50), below) == T(-38));

  // Unbounded block searches: f(a, b) returns the first (or last) match in
  // [a, b], or something outside [a, b] if not found.
  std::function<T(T, T)> block_first = [&](T a, T b) {
    while (a <= b && !sq(a)) ++a;
    return a;
  };
  assert((FindFirst<T, 1>(T(-50), block_first)) == T(32));
  assert((FindFirst<T, 7>(T(-50), block_first)) == T(32));
  assert((FindFirst<T, 64>(T(-50), block_first)) == T(32));
  assert((FindFirst<T>(T(-50), block_first)) == T(32));

  std::function<T(T, T)> block_last = [&](T a, T b) {
    while (b >= a && !below(b)) --b;
    return b;
  };
  assert((FindLast<T, 1>(T(50), block_last)) == T(-38));
  assert((FindLast<T, 7>(T(50), block_last)) == T(-38));
  assert((FindLast<T, 64>(T(50), block_last)) == T(-38));
  assert((FindLast<T>(T(50), block_last)) == T(-38));
}

SL void FindFirstLastSerialTest() {
  FindFirstLastSerialImpl<int>();
  FindFirstLastSerialImpl<int64>();
#if PE_HAS_INT128
  FindFirstLastSerialImpl<int128>();
#endif
}

template <int TN, int B, typename T>
SL void ParallelFindImpl() {
  // A sparse set of "hits" in a pseudo-random pattern.
  for (T m : {T(3), T(37), T(211), T(5000)}) {
    std::function<bool(T)> f = [m](T x) {
      const T y = x < 0 ? -x : x;
      return (y * 31 + 7) % m == 0;
    };
    std::function<T(T, T)> gf = [&](T a, T b) {
      while (a <= b && !f(a)) ++a;
      return a;
    };
    std::function<T(T, T)> gl = [&](T a, T b) {
      while (b >= a && !f(b)) --b;
      return b;
    };
    for (T first : {T(-500), T(-1), T(0), T(9)}) {
      for (T len : {T(1), T(B), T(B + 1), T(3 * B + 2), T(700)}) {
        const T last = first + len - 1;
        const T ef = BruteFirst<T>(first, last, f);
        const T el = BruteLast<T>(first, last, f);
        assert((ParallelFindFirst<TN, T, B>(first, last, f)) == ef);
        assert((ParallelFindFirst<TN, T, B>(first, last, gf)) == ef);
        assert((ParallelFindLast<TN, T, B>(first, last, f)) == el);
        assert((ParallelFindLast<TN, T, B>(first, last, gl)) == el);
      }
    }

    // Unbounded searches; every m here has a hit within m steps.
    // Tiny blocks with a large m cost thousands of lock round trips; the
    // few-threads case is covered separately below.
    if (m > 100 * B) continue;
    for (T start : {T(-300), T(0), T(123)}) {
      T ef = start;
      while (!f(ef)) ++ef;
      assert((ParallelFindFirst<TN, T, B>(start, f)) == ef);
      assert((ParallelFindFirst<TN, T, B>(start, gf)) == ef);
      T el = start;
      while (!f(el)) --el;
      assert((ParallelFindLast<TN, T, B>(start, f)) == el);
      assert((ParallelFindLast<TN, T, B>(start, gl)) == el);
    }
  }

  // No hit at all in a non-empty range.
  std::function<bool(T)> never = [](T) { return false; };
  assert((ParallelFindFirst<TN, T, B>(T(-100), T(400), never)) == T(401));
  assert((ParallelFindLast<TN, T, B>(T(-100), T(400), never)) == T(-101));

  // Hit only at the boundaries.
  std::function<bool(T)> at_first = [](T x) { return x == -100; };
  std::function<bool(T)> at_last = [](T x) { return x == 400; };
  assert((ParallelFindFirst<TN, T, B>(T(-100), T(400), at_first)) == T(-100));
  assert((ParallelFindLast<TN, T, B>(T(-100), T(400), at_first)) == T(-100));
  assert((ParallelFindFirst<TN, T, B>(T(-100), T(400), at_last)) == T(400));
  assert((ParallelFindLast<TN, T, B>(T(-100), T(400), at_last)) == T(400));

  // Empty range (first > last) is skipped here: see the TODO(bug) at
  // ParallelFindFirst/ParallelFindLast in pe_parallel_algo, the result
  // depends on ENABLE_OPENMP.
}

SL void ParallelFindTest() {
  ParallelFindImpl<1, 10, int64>();
  ParallelFindImpl<2, 1, int64>();
  ParallelFindImpl<3, 7, int>();
  ParallelFindImpl<4, 16, int64>();
  ParallelFindImpl<8, 5, int64>();
  ParallelFindImpl<8, 10000, int64>();
#if PE_HAS_INT128
  ParallelFindImpl<4, 9, int128>();
#endif

#if ENABLE_OPENMP
  // Unbounded searches with fewer threads than TN: the inner regions are
  // inactive, so each call runs on a single thread. Only odd numbers hit, so
  // with B == 1 half of the blocks never contain a hit.
  std::function<bool(int64)> is_odd = [](int64 x) { return x % 2 != 0; };
#if _OPENMP >= 201811
  const int old_levels = omp_get_max_active_levels();
  omp_set_max_active_levels(1);
#else
  // MSVC only supports OpenMP 2.0.
  const int old_nested = omp_get_nested();
  omp_set_nested(0);
#endif
  int64 res[2][4];
#pragma omp parallel for num_threads(2)
  for (int i = 0; i < 2; ++i) {
    res[i][0] = ParallelFindFirst<2, int64, 1>(int64(0), is_odd);
    res[i][1] = ParallelFindLast<2, int64, 1>(int64(0), is_odd);
    res[i][2] = ParallelFindFirst<8, int64, 3>(int64(-8), is_odd);
    res[i][3] = ParallelFindLast<8, int64, 3>(int64(8), is_odd);
  }
#if _OPENMP >= 201811
  omp_set_max_active_levels(old_levels);
#else
  omp_set_nested(old_nested);
#endif
  for (int i = 0; i < 2; ++i) {
    assert(res[i][0] == 1 && res[i][1] == -1);
    assert(res[i][2] == -7 && res[i][3] == 7);
  }
#endif
}

#if ENABLE_OPENMP
SL void PSumTest() {
  PSum<int64> s;
  assert(s.value() == 0);
  constexpr int n = 10000;
#pragma omp parallel for schedule(dynamic, 16) num_threads(8)
  for (int i = 1; i <= n; ++i) {
    s += i;
  }
  assert(s.CalSum() == static_cast<int64>(n) * (n + 1) / 2);
  assert(s.Cal() == static_cast<int64>(n) * (n + 1) / 2);

#pragma omp parallel for schedule(dynamic, 16) num_threads(8)
  for (int i = 1; i <= n; ++i) {
    if (i % 2 == 0) {
      s.Sub(i);
    } else {
      s -= 1;
    }
    ++s;
    s++;
    --s;
    s.Add(0);
  }
  // Removed all even i, and each iteration changed the sum by -1 + 1 for odd i
  // and by -i + 1 for even i.
  int64 expected = static_cast<int64>(n) * (n + 1) / 2;
  for (int i = 1; i <= n; ++i) {
    expected += (i % 2 == 0 ? -i : -1) + 1;
  }
  assert(s() == expected);
  assert(static_cast<int64>(s) == expected);
  s--;
  assert(s.value() == expected - 1);

  s.Reset();
  assert(s.value() == 0);
  s -= 5;
  assert(s.value() == -5);
}

SL void PSumModTest() {
  const int64 mod = 1000000007;
  PSumMod<int64> s(mod);
  assert(s.value() == 0);
  constexpr int n = 20000;
  int64 expected = 0;
  for (int i = 1; i <= n; ++i) {
    expected = (expected + static_cast<int64>(i) * i % mod) % mod;
  }
#pragma omp parallel for schedule(dynamic, 16) num_threads(8)
  for (int i = 1; i <= n; ++i) {
    s += static_cast<int64>(i) * i % mod;
  }
  assert(s.CalSum() == expected);
  assert(s.Cal() == expected);
  assert(s() == expected);
  assert(static_cast<int64>(s) == expected);

  s.Reset();
  assert(s.value() == 0);
  s -= 3;
  assert(s.value() == mod - 3);
  ++s;
  s++;
  s.Add(5);
  assert(s.value() == 4);
  --s;
  s--;
  s.Sub(2);
  assert(s.value() == 0);

  // Small modulus with wrap around.
  PSumMod<int> t(7);
#pragma omp parallel for num_threads(4)
  for (int i = 0; i < 100; ++i) {
    t += i % 7;
  }
  int e = 0;
  for (int i = 0; i < 100; ++i) e = (e + i % 7) % 7;
  assert(t.value() == e);
}

SL void PMinTest() {
  constexpr int n = 5000;
  auto val = [](int i) -> int64 {
    return (static_cast<int64>(i) * 7919 + 104729) % 100003 - 50000;
  };
  int64 mn = val(0), mx = val(0);
  for (int i = 0; i < n; ++i) {
    mn = std::min(mn, val(i));
    mx = std::max(mx, val(i));
  }

  PMin<int64> pmin;
  pmin.SetEnableLog(false);
#pragma omp parallel for schedule(dynamic, 8) num_threads(8)
  for (int i = 0; i < n; ++i) {
    pmin.CheckMin(val(i));
  }
  assert(pmin.value() == mn);
  assert(pmin() == mn);
  assert(static_cast<int64>(pmin) == mn);

  // Custom comparator: maximum.
  PMin<int64, std::greater<int64>> pmax;
  pmax.SetEnableLog(false);
#pragma omp parallel for schedule(dynamic, 8) num_threads(8)
  for (int i = 0; i < n; ++i) {
    pmax.CheckMin(val(i));
  }
  assert(pmax.value() == mx);

  // Inverted comparator also yields the maximum.
  pmin.Reset().SetInvertCmp(true);
#pragma omp parallel for schedule(dynamic, 8) num_threads(8)
  for (int i = 0; i < n; ++i) {
    pmin.CheckMin(val(i));
  }
  assert(pmin.value() == mx);

  // Back to non-inverted after reset, serial use.
  pmin.Reset().SetInvertCmp(false);
  assert(!pmin.result.has_value());
  pmin.CheckMin(3).CheckMin(-2).CheckMin(10);
  assert(pmin.value() == -2);
  pmin.CheckGlobalMin(std::optional<int64>(-9));
  assert(pmin.value() == -9);
  assert(pmin.Compare(1, 2) && !pmin.Compare(2, 1) && !pmin.Compare(2, 2));
  pmin.SetInvertCmp();
  assert(!pmin.Compare(1, 2) && pmin.Compare(2, 1) && pmin.Compare(2, 2));

  // Non-integer type.
  PMin<std::string> smin;
  smin.SetEnableLog(false);
  smin.CheckMin("pear").CheckMin("apple").CheckMin("zoo");
  assert(smin.value() == "apple");
}

SL void OmpLockTest() {
  OmpLock lock;
  int64 counter = 0;
  std::vector<int> order;
#pragma omp parallel for schedule(dynamic, 1) num_threads(8)
  for (int i = 0; i < 2000; ++i) {
    OmpGuard guard(lock);
    ++counter;
    order.push_back(i);
  }
  assert(counter == 2000);
  std::sort(std::begin(order), std::end(order));
  for (int i = 0; i < 2000; ++i) assert(order[i] == i);

  // Manual lock/unlock.
  int64 sum = 0;
#pragma omp parallel for num_threads(4)
  for (int i = 1; i <= 1000; ++i) {
    lock.lock();
    sum += i;
    lock.unlock();
  }
  assert(sum == 500500);
}
#endif

PE_REGISTER_TEST(&ParallelExecuteTest, "ParallelExecuteTest", SMALL);
PE_REGISTER_TEST(&ParallelSortSmallTest, "ParallelSortSmallTest", SMALL);
PE_REGISTER_TEST(&FindFirstLastSerialTest, "FindFirstLastSerialTest", SMALL);
PE_REGISTER_TEST(&ParallelFindTest, "ParallelFindTest", SMALL);
#if ENABLE_OPENMP
PE_REGISTER_TEST(&PSumTest, "PSumTest", SMALL);
PE_REGISTER_TEST(&PSumModTest, "PSumModTest", SMALL);
PE_REGISTER_TEST(&PMinTest, "PMinTest", SMALL);
PE_REGISTER_TEST(&OmpLockTest, "OmpLockTest", SMALL);
#endif
}  // namespace parallel_algo_test
