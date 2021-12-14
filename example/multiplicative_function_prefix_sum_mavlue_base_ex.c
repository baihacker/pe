#include <pe.hpp>
using namespace pe;

// f(n) = the number of divisors of n

using RT = int64;

struct Solver0 : public MValueBaseExLite<Solver0, RT, 8> {
  RT F(int64 p, int64 e) { return e + 1; }
};

struct Solver1 : public MValueBaseEx<Solver1, RT, 8> {
  RT F(int64 p, int64 e) { return e + 1; }
  RT BatchF(int imp, int64 vmp, int64 remain) {
    return F(2, 1) * RT(dva[remain] - (imp + 1));
  }
  void Init(int64 n) { dva = PrimeS0Ex<int64>(n); }
  DVA<int64> dva;
};

struct Solver2 : public MValueBaseEx<Solver2, RT, 8> {
  RT Batch(int64 n, int64 val, int imp, int64 vmp, int emp, RT now, RT now1) {
    RT ret = 0;
    int64 remain = n / val;
    // we have remain >= vmp
    // handle val * q where q > vmp
    if (remain > vmp) {
      ret += now * BatchF(imp, vmp, remain);
    }
    if (val > 1) {
      // handle val * vmp
      ret += now1 * F(vmp, emp + 1);
    } else {
      // handle f(1)
      ret += 1;
    }
    return ret;
  }
  RT F(int64 p, int64 e) { return e + 1; }
  RT BatchF(int imp, int64 vmp, int64 remain) {
    return F(2, 1) * RT(dva[remain] - (imp + 1));
  }
  void Init(int64 n) { dva = PrimeS0Ex<int64>(n); }
  DVA<int64> dva;
};

DVA<int64> dva;
MVVHistory history_[128];
int top_;
RT Dfs(int limit, int64 n, int64 val, int imp, int64 vmp, int emp, RT now,
       RT now1) {
  RT ret = 0;
  {
    int64 remain = n / val;
    // we have remain >= vmp
    // handle val * q where q > vmp
    if (remain > vmp) {
      ret += now * (dva[remain] - (imp + 1)) * 2;
    }
    if (val > 1) {
      // handle val * vmp
      ret += now1 * (emp + 2);
    } else {
      // handle f(1)
      ret += 1;
    }
  }
  for (int i = 0; i < limit; ++i) {
    const int64 p = plist[i];
    const int next_imp = imp == -1 ? i : imp;
    const int64 next_vmp = imp == -1 ? p : vmp;
    const int64 val_limit = n / p / next_vmp;
    if (val > val_limit) break;
    history_[top_].ip = i;
    history_[top_].p = p;
    history_[top_].e = 1;
    int& e = history_[top_++].e;
    for (int64 nextval = val * p;; ++e) {
      RT tmp = now * (e + 1);
      ret += Dfs(i, n, nextval, next_imp, next_vmp, imp == -1 ? e : emp, tmp,
                 imp == -1 ? 1 : now1 * (e + 1));
      if (nextval > val_limit) break;
      nextval *= p;
    }
    --top_;
  }
  return ret;
}

int main() {
  PE_INIT(maxp = 2000000);

  for (int64 n = 1; n <= 100000000; n *= 10) {
    int64 a = Solver0().Cal(n);
    int64 b = Solver1().Cal(n);
    int64 c = Solver2().Cal(n);
    ::dva = PrimeS0Ex<int64>(n);
    int64 d = Dfs(pcnt, n, 1, -1, 1, 0, 1, 1);
    std::cout << n << "\t" << a << "\t" << b << "\t" << c << " " << d
              << std::endl;
  }
  return 0;
}