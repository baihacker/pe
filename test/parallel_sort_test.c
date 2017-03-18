const int maxp = 100000;
#include "pe"
int arr[100000000];

int main()
{
  for (int i = 0; i < 100000000; ++i) arr[i] = rand();
  TimeRecorder tr;
  parallel_sort<8>(arr, arr+100000000);
  cerr << tr.elapsed().format() << endl;
  //return 0;
  for (int i = 1; i < 100000000; ++i)
  {
    assert(arr[i-1] <= arr[i]);
  }
  return 0;
}