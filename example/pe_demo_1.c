const int maxp = 100000;
#include <pe>

int main()
{
  init_primes(true, true);
  auto x = get_factors(1);
  for (auto i: x) cerr << i << endl;
  cerr << pr::find_pr(37) << endl;
  cerr << phi[3350] << endl;
  cerr << cal_phi(3350) << endl;
  cerr << cal_phi(1) << endl;
  
  int s = pr::find_pr(37);
  pr::ind_solver orz(37);
  for (int i = 1; i < 37; ++i)
  {
    cerr << orz(i) << " " << pr::ind(i, s, 37) << endl;
  }
  {
    //comb_solver sv(37);
    comb_moder<37> sv1;
    for (int i = 0; i < 100; ++i)
    for (int j = 0; j <= i; ++j)
    {
      //int a = sv.comb(i, j);
      int b = sv1.comb(i, j);
      //assert(a == b);
    }
  }
  return 0;
}