#include <pe.hpp>

void test() {
  bi N(1);
  for (int i = 1; i <= 20; ++i) N = N * i;

  bi v = 0;
  for (bi n = N; !n.isZero(); v = v + n, n = n / 2);
  dbg(v);
  bi mod = power(2_bi, 48);
  bi ans = 1;
  FactPPowerModer moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N = N / 2;
  }
  v = v % 4;
  dbg(ans);
  ans = ans * power(2_bi, v[0]) % mod;

  cout << ans << endl;
  cout << ans.toInt<int64>() << endl;
  printf("%llx\n", (unsigned long long)ans.toInt<unsigned long long>());
}

void facTest()
{
  TimeRecorder tr;
  BigInteger result = 1;
  for (int i = 1; i <= 100000; ++i) result = result * i;
  cout << tr.elapsed().format() << endl;
  cout << result.toString().size() << endl;
  cout << result.size() << endl;
}

int main()
{
//cout << GCDEX(281474976710656_bi, 207628560027673_bi) << endl;
// cout << "123455"_bi - 12345678_bi << endl;
// cout << 1_bi * -3 << endl;
  test();
cout << (1_bi << 64) << endl;
BigInteger f(32768);
for (int i = 1; i < 20; ++i)
cout << (f >> i) << " " << (f << i) << endl;
cout << (1_bi << 20) << endl;
for (int i = -10; i <= 10; ++i)
  for (int j = -10; j <= 10; ++j) if (j != 0)
    assert(i / j == BigInteger(i) / BigInteger(j)),
    assert(i % j == BigInteger(i) % BigInteger(j));

  cout << ~"1048576"_bi << endl;
  cout << ~"255"_bi << endl;
  cout << ~"1"_bi << endl;
  cout << ~"0"_bi << endl;
  cout << ~"5"_bi << endl;
  cout << "123123456789123456789"_bi << endl;
  //       123123456789123456789
  
  cout << "123123456789123456789"_bi + "123123456789123456789"_bi << endl;
  cout << "123123456789123456789"_bi - "113123456789123456788"_bi << endl;
  cout << "123123456789123456789"_bi * "113123456789123456788"_bi << endl;
  
  cout << BigInteger(vector<char>{0,0,0,0,0,0,0,0,0,0,0,1}) << endl;
  /*cout << BigInteger(vector<char>{0,0,0,0,0,0,0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,0,0,0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,0,0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,0,1}) << endl;
  cout << BigInteger(vector<char>{0,1}) << endl;
  cout << BigInteger(vector<char>{1}) << endl;*/

  BigInteger a = 123456789;
  BigInteger b = a * a;

  for (auto& iter: a.toBinVector<int>()) cerr << (int)iter;
  cerr << endl;
  cout << a << endl;
  cout << b << endl;
  //cout << b / a << endl;

  cout << "5"_bi % "4"_bi << endl;
  cout << ~"16384"_bi << endl;

  TimeRecorder tr;
  BigInteger result = 1;
  for (int i = 1; i <= 100; ++i) result = result * i;
  cout << tr.elapsed().format() << endl;
  cout << result.toString().size() << endl;
  cout << result.size() << endl;
  //cout << - "123"_bi << endl;

  //cout << BigInteger((int128)1000000000000 * (int128)999999999) << endl;
tie(a, b) = div("1234"_bi, "5"_bi);
cout << a << " " << b << endl;

cout << a / 123 << endl;

cout << a / 2 << endl;
cout << a / 5 << endl;

tie(a, b) = div("12340000000000000000000"_bi, 5);
cout << a << " " << b << endl;
cout << "12340000000000000000000"_bi % 5 << endl;
//  cout << div("1234"_bi, "5"_bi) << endl;

facTest();

cout << "1234556"_bi % (unsigned)123 << endl;
  return 0;
}
/*
13928151043811916475759392165831626733732
13928151043811916475759392165831626733732
*/
