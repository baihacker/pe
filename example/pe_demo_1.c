#include <pe.hpp>

int main() {
  InitPrimes(true, true, true);
  auto x = Factors(1);
  for (auto i : x) cerr << i << endl;
  cerr << pr::FindPr(37) << endl;
  cerr << phi[3350] << endl;
  cerr << CalPhi(3350) << endl;
  cerr << CalPhi(1) << endl;

  int s = pr::FindPr(37);
  pr::IndSolver orz(37);
  for (int i = 1; i < 37; ++i) {
    cerr << orz(i) << " " << pr::Ind(i, s, 37) << endl;
  }
  {
    // comb_solver sv(37);
    CombModer sv1(37);
    for (int i = 0; i < 100; ++i)
      for (int j = 0; j <= i; ++j) {
        // int a = sv.comb(i, j);
        int b = sv1.Comb(i, j);
        // assert(a == b);
      }
  }
  return 0;
}