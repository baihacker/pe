#include <pe.hpp>
using namespace pe;

// f(n) = the number of divisors of n

using RT = int64;

struct Solver0 : public MValueBaseLite<Solver0, RT, 8> {
  RT F(int64 p, int64 e) { return e + 1; }
};

struct Solver1 : public MValueBase<Solver1, RT, 8> {
  RT F(int64 p, int64 e) { return e + 1; }
  RT BatchF(int imp, int64 vmp, int64 remain) {
    return F(2, 1) * RT(dva[remain] - (imp + 1));
  }
  void Init(int64 n) {
    dva = PrimeS0Ex<int64>(n);
    tr.Record();
  }
  void Done() { usage = tr.Elapsed(); }
  DVA<int64> dva;
  TimeRecorder tr;
  TimeDelta usage;
};

struct Solver2 : public MValueBase<Solver2, RT, 8, /*small_to_large=*/false> {
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
  void Init(int64 n) {
    dva = PrimeS0Ex<int64>(n);
    tr.Record();
  }
  void Done() { usage = tr.Elapsed(); }
  DVA<int64> dva;
  TimeRecorder tr;
  TimeDelta usage;
};

struct Solver3 : public MValueBase<Solver3, RT, 8, /*small_to_large=*/true> {
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
  void Init(int64 n) {
    dva = PrimeS0Ex<int64>(n);
    tr.Record();
  }
  void Done() { usage = tr.Elapsed(); }
  DVA<int64> dva;
  TimeRecorder tr;
  TimeDelta usage;
};

struct Solver4 : public MValueVisitor<Solver4, RT, 8> {
  RT Visit(int64 n, int64 val, int imp, int64 vmp, int emp, MVVHistory* his,
           int top) {
    RT ret = 0;
    int64 remain = n / val;
    RT now = 1;
    RT now1 = 1;
    for (int i = 0; i < top; ++i) {
      now *= his[i].e + 1;
      if (his[i].p != vmp) {
        now1 *= his[i].e + 1;
      }
    }
    // we have remain >= vmp
    // handle val * q where q > vmp
    if (remain > vmp) {
      ret += now * 2 * RT(dva[remain] - (imp + 1));
    }
    if (val > 1) {
      // handle val * vmp
      ret += now1 * (emp + 2);
    } else {
      // handle f(1)
      ret += 1;
    }
    return ret;
  }
  void Init(int64 n) {
    dva = PrimeS0Ex<int64>(n);
    tr.Record();
  }
  void Done() { usage = tr.Elapsed(); }
  DVA<int64> dva;
  TimeRecorder tr;
  TimeDelta usage;
};

struct Solver5 : public MValueVisitorEx<Solver5, RT, int64, 8> {
  using AT = int64;
  RT Visit(int64 n, int64 val, int imp, int64 vmp, int emp, MVVHistory* his,
           int top, AT now, AT now1) {
    RT ret = 0;
    int64 remain = n / val;

    // we have remain >= vmp
    // handle val * q where q > vmp
    if (remain > vmp) {
      ret += RT(now) * 2 * RT(dva[remain] - (imp + 1));
    }
    if (val > 1) {
      // handle val * vmp
      ret += RT(now1) * (emp + 2);
    } else {
      // handle f(1)
      ret += 1;
    }
    return ret;
  }
  void Init(int64 n) {
    dva = PrimeS0Ex<int64>(n);
    tr.Record();
  }
  void Done() { usage = tr.Elapsed(); }

  AT GetInitValue() { return 1; }
  // pd = p^e
  AT AccumulateValue(AT init, int64 p, int64 e, int64 pd) {
    return init * (e + 1);
  }
  DVA<int64> dva;
  TimeRecorder tr;
  TimeDelta usage;
};

DVA<int64> dva;
MVVHistory history[128];
int top;
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
    history[top].ip = i;
    history[top].p = p;
    history[top].e = 1;
    history[top].pd = p;
    int& e = history[top].e;
    int64& pd = history[top++].pd;
    for (int64 nextval = val * p;; ++e) {
      RT tmp = now * (e + 1);
      ret += Dfs(i, n, nextval, next_imp, next_vmp, imp == -1 ? e : emp, tmp,
                 imp == -1 ? 1 : now1 * (e + 1));
      if (nextval > val_limit) break;
      pd *= p;
      nextval *= p;
    }
    --top;
  }
  return ret;
}

RT Dfs1(int start, int limit, int64 n, int64 val, int imp, int64 vmp, int emp,
        RT now, RT now1) {
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
  for (int i = start; i < limit; ++i) {
    const int64 p = plist[i];
    const int64 val_limit = n / p / p;
    if (val > val_limit) break;
    history[top].ip = i;
    history[top].p = p;
    history[top].e = 1;
    history[top].pd = p;
    int& e = history[top].e;
    int64& pd = history[top++].pd;
    for (int64 nextval = val * p;; ++e) {
      RT tmp = now * (e + 1);
      ret += Dfs1(i + 1, limit, n, nextval, i, p, e, tmp, now);
      if (nextval > val_limit) break;
      pd *= p;
      nextval *= p;
    }
    --top;
  }
  return ret;
}

void RunCorrectnessTest() {
  TableFormatter tf;
  std::vector<std::string>& line = tf.AppendLine();
  line.push_back("n");
  line.push_back("Solver0");
  line.push_back("Solver1");
  line.push_back("Solver2");
  line.push_back("Solver3");
  line.push_back("Solver4");
  line.push_back("Solver5");
  line.push_back("Dfs");
  line.push_back("Dfs1");
  for (int64 n = 1, e = 0; n <= 100000000; n *= 10, ++e) {
    int64 a0 = Solver0().Cal(n);
    int64 a1 = Solver1().Cal(n);
    int64 a2 = Solver2().Cal(n);
    int64 a3 = Solver3().Cal(n);
    int64 a4 = Solver4().Cal(n);
    int64 a5 = Solver5().Cal(n);
    ::dva = PrimeS0Ex<int64>(n);
    int64 a6 = Dfs(pcnt, n, 1, -1, 1, 0, 1, 1);
    int64 a7 = Dfs1(0, pcnt, n, 1, -1, 1, 0, 1, 1);
    std::vector<std::string>& line = tf.AppendLine();
    line.push_back("1e" + pe::ToString(e));
    line.push_back(pe::ToString(a0));
    line.push_back(pe::ToString(a1));
    line.push_back(pe::ToString(a2));
    line.push_back(pe::ToString(a3));
    line.push_back(pe::ToString(a4));
    line.push_back(pe::ToString(a5));
    line.push_back(pe::ToString(a6));
    line.push_back(pe::ToString(a7));
  }
  tf.Render(std::cout);
}

void RunPerfTest() {
  TableFormatter tf;
  std::vector<std::string>& line = tf.AppendLine();
  line.push_back("n");
  line.push_back("Solver2");
  line.push_back("Solver3");
  line.push_back("Solver4");
  line.push_back("Solver5");
  line.push_back("Dfs");
  line.push_back("Dfs1");
  for (int64 n = 1000000, e = 6; n <= 1000000000000; n *= 10, ++e) {
    // std::cout << "n = " << n << std::endl;
    std::vector<std::string>& line = tf.AppendLine();
    line.push_back("1e" + pe::ToString(e));
    {
      Solver2 solve;
      RT ans = solve.Cal(n);
      // std::cout << ans << std::endl;
      // std::cout << solve.usage.Format() << std::endl;
      line.push_back(solve.usage.Format());
    }

    {
      Solver3 solve;
      RT ans = solve.Cal(n);
      // std::cout << ans << std::endl;
      // std::cout << solve.usage.Format() << std::endl;
      line.push_back(solve.usage.Format());
    }

    {
      Solver4 solve;
      RT ans = solve.Cal(n);
      // std::cout << ans << std::endl;
      // std::cout << solve.usage.Format() << std::endl;
      line.push_back(solve.usage.Format());
    }

    {
      Solver5 solve;
      RT ans = solve.Cal(n);
      // std::cout << ans << std::endl;
      // std::cout << solve.usage.Format() << std::endl;
      line.push_back(solve.usage.Format());
    }

    {
      ::dva = PrimeS0Ex<int64>(n);
      TimeRecorder tr;
      RT ans = Dfs(/*limit=*/pcnt, /*n=*/n, /*val=*/1, /*imp=*/-1, /*vmp=*/1,
                   /*emp=*/0, /*now=*/1, /*now1=*/1);
      // std::cout << ans << std::endl;
      // std::cout << tr.Elapsed().Format() << std::endl;
      line.push_back(tr.Elapsed().Format());
    }

    {
      ::dva = PrimeS0Ex<int64>(n);
      TimeRecorder tr;
      RT ans = Dfs1(/*start=*/0, /*limit=*/pcnt, /*n=*/n, /*val=*/1, /*imp=*/-1,
                    /*vmp=*/1, /*emp=*/0, /*now=*/1, /*now1=*/1);
      // std::cout << ans << std::endl;
      // std::cout << tr.Elapsed().Format() << std::endl;
      line.push_back(tr.Elapsed().Format());
    }

    // std::cout << std::endl;
  }

  tf.Render(std::cout);
}
int main() {
  PE_INIT(maxp = 2000000);
  RunCorrectnessTest();
  std::cout << std::endl;
  RunPerfTest();
  return 0;
}