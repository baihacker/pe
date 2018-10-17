#include <pe.hpp>

const int64 mod = 1000000000 + 7;
const int n = 100000;

int64 P[2 * n + 5];
int64 Q[2 * n + 5];
int64 R[2 * n + 5];
int64 RR[2 * n + 5];

int even_data[2][2 * n + 5];
int odd_data[2][2 * n + 5];
int my[n + 1];

int64 U[n + 5];
int64 V[n + 5];

int main() {
  R[0] = 1;
  RR[0] = 1;
  {
    int* ef = even_data[0];
    int* et = even_data[1];
    int* of = odd_data[0];
    int* ot = odd_data[1];
    even_data[0][0] = 1;
    const int maxsq = sqrt(2 * n) + 1;
    for (int k = 1; k <= maxsq; ++k) {
      dbg(k);
      fill(et, et + n + 1, 0);
      fill(ot, ot + n + 1, 0);
      int k2 = k * k;
      for (int i = k2; i <= 2 * n; ++i) {
        et[i] = ot[i - k];
        ot[i] = add_mod(et[i - k], ef[i - k], mod);
        R[i] = add_mod(R[i], ot[i], mod);
        RR[i] = add_mod(RR[i], et[i], mod);
      }
      swap(ef, et);
      swap(of, ot);
    }
  }
  Q[0] = 1;
  for (int i = 1; i <= n; ++i) {
    Q[i] = RR[2 * i];
  }
  for (int i = 0; i <= n; i += 4) {
    U[i] = Q[i / 4];
  }
  for (int i = 0; i <= n; ++i) {
    V[i] = R[i];
  }
  vector<int> u, v;
  for (int i = 0; i <= n; ++i) {
    u.pb(U[i]), v.pb(V[i]);
  }

  dbg(u.size());
  dbg(v.size());
  TimeRecorder tr;
  auto w = ntt32::poly_mul_ntt(u, v, mod);
  cerr << tr.elapsed().format() << endl;
  dbg(w.size());
  dbg(w[100]);  // 37076
  int64 ans = 0;
  for (int i = 1; i <= n; ++i) ans += w[i];
  dbg(ans % mod);
  return 0;
}