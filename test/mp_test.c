#include <pe.hpp>

int main()
{
  const int64 P = 1000000009;
  Poly<P> s{0,1,1,2,3,5};
  cout << s << endl;
  auto v = find_minimal_poly(s);
  cout << v << endl;
  const int n = v.data.size();
  int64 ans = 0;
  for (int i = 0; i < n; ++i)
    ans += v.at(i) * s.at(i);
  dbg(ans);
  dbg(ans%P);
  
  ans = nth_element(s, 38, v);
  dbg(ans);
  
  cout << find_linear_recurrence<31>({0,1,1,2,3,5,8,13}) << endl;
  cout << nth_element<1000000009>({0,1,1,2,3,5,8,13}, 38) << endl;
  return 0;
}