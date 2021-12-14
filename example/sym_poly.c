#include <pe.hpp>
using namespace pe;

// Guess the solution to a^2=b^2+c^2+bc where
// a = t1, b = t2, c = t3, c7 = 1, c8 = 0, c9 = -1
SymPoly<int64> t1("c1 m^2 + c2 m n + c3 n^2");
SymPoly<int64> t2("c4 m^2 + c5 m n + c6 n^2");
SymPoly<int64> t3("c7 m^2 + c8 m n + c9 n^2");

SymPoly<int64> target = SymPoly<int64>("a^2-b^2-c^2-b c")
                            .Replace("a", t1)
                            .Replace("b", t2)
                            .Replace("c", t3);

int his[10];
void dfs(int now, const SymPoly<int64>& p) {
  if (now == 10) {
    if (p.terms().empty() && his[7] == 1 && his[8] == 0 && his[9] == -1) {
      auto aa =
          t1.Replace("c1", his[1]).Replace("c2", his[2]).Replace("c3", his[3]);
      auto bb =
          t2.Replace("c4", his[4]).Replace("c5", his[5]).Replace("c6", his[6]);
      auto cc =
          t3.Replace("c7", his[7]).Replace("c8", his[8]).Replace("c9", his[9]);
      if (aa.terms().empty() || bb.terms().empty() || cc.terms().empty())
        return;
      std::cout << "a = " << aa << std::endl;
      std::cout << "b = " << bb << std::endl;
      std::cout << "c = " << cc << std::endl;
      std::cout << std::endl;
    }
  } else {
    std::string me = "c" + ToString(now);
    for (int i = -1; i <= 2; ++i) {
      his[now] = i;
      dfs(now + 1, p.Replace(me, i));
    }
  }
}

int main() {
  std::cout << target.Replace("n", 1).Replace("m", 1) << std::endl;
  dfs(1, target);
  return 0;
}