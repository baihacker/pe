#include <pe.hpp>

const int64 mod = 1000000007;

int main() {
  {
    NMod64<mod> a(5);
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  {
    TLMod<int64>::Set(mod);
    TLNMod64<> a(5);
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  {
    DefaultMod::Set(mod);
    NModNumber<DefaultMod> a(5);
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  {
      // NModNumber<MemMod<int64>> a(5);
  }

  {
    NModM64<mod> a(5);
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  {
    TLMod<int64>::Set(mod);
    TLNModM64<> a(5);
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  {
    DefaultMod::Set(mod);
    NModNumberM<DefaultMod> a(5);
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  {
    // Not recommended.
    NModNumberM<MemMod<int64>> a(5, MemMod<int64>(mod));
    cout << a.Power(10) << endl;
    cout << Power(a, 10) << endl;
    cout << PowerMod(a, 10) << endl;
    cout << PowerMod(a, 10, mod) << endl;
    cout << PowerModEx(a, 10) << endl;
    cout << PowerModEx(a, 10, mod) << endl;
  }
  return 0;
}