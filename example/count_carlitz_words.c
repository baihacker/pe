#include <pe.hpp>
using namespace pe;

// Given the number of each letter, find the number of words without adjacent
// letter of the same

int brute_force(const std::vector<int64>& vec) {
  int size = 0;
  for (auto& iter : vec) size += iter;

  std::vector<int> data;
  int id = 0;
  for (auto iter : vec) {
    for (int j = 0; j < iter; ++j) data.push_back(id);
    ++id;
  }

  int ret = 0;
  do {
    int ok = 1;
    for (int i = 0; i < size - 1; ++i) {
      int idx = (i + 1) % size;
      if (i != idx && data[i] == data[idx]) {
        ok = 0;
        break;
      }
    }
    ret += ok;
  } while (next_permutation(data.begin(), data.end()));
  return ret;
}

int main() {
  CarlitzWordsCounter counter(1000000007, 1000000);

  std::vector<std::vector<int64>> test_data{
      {1},    {2},          {1, 1},    {1, 3},       {2, 2},    {3, 7},
      {2, 3}, {2, 2, 2},    {2, 3, 3}, {3, 3, 2, 2}, {3, 3, 3}, {2, 2, 2, 2, 2},
      {4, 4}, {4, 4, 2, 2}, {5, 5, 5}};

  for (auto& iter : test_data) {
    std::cout << brute_force(iter) << " " << counter.Cal(iter) << std::endl;
  }
  return 0;
}