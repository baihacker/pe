#include <pe>

const int64 mod = 1000000000 + 7;
const int n = 10000000;

int64 P[2*n+5];
int64 Q[2*n+5];
int64 R[2*n+5];
int64 RR[2*n+5];

int even_data[2][2*n+5];
int odd_data[2][2*n+5];
int my[n+1];

int64 U[n+5];
int64 V[n+5];
int w[n+5];

void pol_mul(const int* X, const int* Y, const int n, int* result) {
  if (n <= 49) {
    fill(result, result+2*n, 0);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) {
        result[i+j] = add_mod(result[i+j], (uint64)X[i] * Y[j] % mod, mod);
      }
    return;
  }
  
  if (n > 100000) {
    dbg(n);
  }
  const int m1 = n + 1 >> 1;
  const int m0 = n - m1;
  //m1 >= m0
  const int* x0 = X + m1;
  const int* y0 = Y + m1;
  const int* x1 = X;
  const int* y1 = Y;
  int* x0y0 = new int[2*m1];
  int* x1y1 = new int[2*m1];
  if (n > 5000) {
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < 2; ++i) {
      if (i&1) pol_mul(x0, y0, m0, x0y0);
      else pol_mul(x1, y1, m1, x1y1);
    }
  } else {
    pol_mul(x0, y0, m0, x0y0);
    pol_mul(x1, y1, m1, x1y1);
  }
  if (m0 != m1) {
    x0y0[2*m0] = 0;
    x0y0[2*m0+1] = 0;
  }
  int* magic = new int[2*m1];
  {
    int* u = new int[m1];
    int* v = new int[m1];
 
    for (int i = 0; i < m0; ++i)
      u[i] = add_mod(x0[i], x1[i], mod),
      v[i] = add_mod(y0[i], y1[i], mod);
    if (m0 != m1) {
      u[m1-1] = x1[m1-1];
      v[m1-1] = y1[m1-1];
    }
    pol_mul(u, v, m1, magic);
    delete[] u;
    delete[] v;
    for (int i = 0; i < m1*2; ++i)
      magic[i] = sub_mod(magic[i], add_mod(x0y0[i], x1y1[i], mod), mod);
  }
  fill(result, result + 2 * n, 0);
  for (int i = 0; i < m0*2; ++i)
    result[2*m1+i] = add_mod(result[2*m1+i], x0y0[i], mod);
  
  for (int i = 0; i < m1*2; ++i)
    result[m1+i] = add_mod(result[m1+i], magic[i], mod);
  
  for (int i = 0; i < m1*2; ++i)
    result[i] = add_mod(result[i], x1y1[i], mod);
  delete[] x0y0;
  delete[] x1y1;
  delete[] magic;
}

int main()
{
  R[0] = 1;
  RR[0] = 1;
  {
    int* ef = even_data[0];
    int* et = even_data[1];
    int* of = odd_data[0];
    int* ot = odd_data[1];
    even_data[0][0] = 1;
    const int maxsq = sqrt(2*n) + 1;
    for (int k = 1; k <= maxsq; ++k) {
      dbg(k);
      fill(et, et+n+1, 0);
      fill(ot, ot+n+1, 0);
      int k2 = k * k;
      for (int i = k2; i <= 2*n; ++i) {
        et[i] = ot[i-k];
        ot[i] = add_mod(et[i-k], ef[i-k], mod);
        R[i] = add_mod(R[i], ot[i], mod);
        RR[i] = add_mod(RR[i], et[i], mod);
      }
      swap(ef, et);
      swap(of, ot);
    }
  }
  Q[0] = 1;
  for (int i = 1; i <= n; ++i) {
    Q[i] = RR[2*i];
  }
  for (int i = 0; i <= n; i += 4) {
    U[i] = Q[i/4];
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
  auto w = poly_mul_ntt(u, v, mod);
  cerr << tr.elapsed().format() << endl;
  dbg(w.size());
  dbg(w[100]);
  int64 ans = 0;
  for (int i = 1; i <= n; ++i)
    ans += w[i];
  dbg(ans%mod);
  return 0;
}