#include <pe_bi>

int main()
{
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
  return 0;
}
/*
13928151043811916475759392165831626733732
13928151043811916475759392165831626733732
*/