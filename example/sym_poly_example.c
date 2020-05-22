#include <pe.hpp>

/**
  Guess the solution to a^2=b^2+c^2+bc where
  a = t1, b = t2, c = t3, c7 = 1, c8 = 0, c9 = -1
 */
SymPoly<int64> t1("c1 m^2 + c2 m n + c3 n^2");
SymPoly<int64> t2("c4 m^2 + c5 m n + c6 n^2");
SymPoly<int64> t3("c7 m^2 + c8 m n + c9 n^2");

SymPoly<int64> target = SymPoly<int64>("a^2-b^2-c^2-b c")
                            .replace("a", t1)
                            .replace("b", t2)
                            .replace("c", t3);

int his[10];
void dfs(int now, const SymPoly<int64>& p) {
  if (now == 10) {
    if (p.terms().empty() && his[7] == 1 && his[8] == 0 && his[9] == -1) {
      auto aa =
          t1.replace("c1", his[1]).replace("c2", his[2]).replace("c3", his[3]);
      auto bb =
          t2.replace("c4", his[4]).replace("c5", his[5]).replace("c6", his[6]);
      auto cc =
          t3.replace("c7", his[7]).replace("c8", his[8]).replace("c9", his[9]);
      if (aa.terms().empty() || bb.terms().empty() || cc.terms().empty())
        return;
      cout << "a = " << aa << endl;
      cout << "b = " << bb << endl;
      cout << "c = " << cc << endl;
      cout << endl;
    }
  } else {
    string me = "c" + to_string(now);
    for (int i = -1; i <= 2; ++i) {
      his[now] = i;
      dfs(now + 1, p.replace(me, i));
    }
  }
}

int main() {
  cout << target.replace("n", 1).replace("m", 1) << endl;
  dfs(1, target);
  return 0;
}