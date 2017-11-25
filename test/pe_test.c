#include "pe_test.h"

static parallel_initialize_t __parallel_initializer;
static time_usage __time_usage;

PeTest& getTester() {
  static PeTest tester;
  return tester;
}

int main() {
  auto& tester = getTester();
  const int size = tester.tests.size();
  
  for (int i = 0; i < size; ++i) {
    auto& item = tester.tests[i];
    cout << "Begin " << item.description << endl;
    TimeRecorder tr;
    item.test();
    cout << "End " << item.description << endl;
    cout << "Time usage " << tr.elapsed().format() << endl;
    if (i < size - 1) {
      cout << endl;
    }
  }
  return 0;
}