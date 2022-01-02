#include "pe_test.h"

#define ONLY_TEST_PE_POLY_MUL 1
#include "poly_mul_test.c"

PeTest& GetTester() {
  static PeTest tester;
  return tester;
}

static inline std::string FormatSecond(double s) {
  char buff[256];
  sprintf(buff, "%.3f", s);
  return buff;
}

static inline std::string FormatSecond(TimeDelta d) {
  char buff[256];
  sprintf(buff, "%.3f", d.ToSeconds());
  return buff;
}

int main(int argc, char* argv[]) {
  int run_count = 2;

  for (int i = 1; i < argc;) {
    std::string c = argv[i];
    const int size = static_cast<int>(c.size());
    int j = 0;
    while (j < size && c[j] == '-') ++j;
    c = c.substr(j);
    if (c == "r") {
      if (i + 1 < argc) {
        run_count = atoi(argv[i + 1]);
        i += 2;
      } else {
        fprintf(stderr, "Run count is unspecified");
        exit(-1);
      }
    } else {
      fprintf(stderr, "Unknown flags: %s", argv[i]);
      exit(-1);
    }
  }

  std::cout << std::endl;
  std::cout << "run_count: " << run_count << std::endl;
  std::cout << std::endl;

  PeInitializer()
      .set_cal_phi()
      .set_cal_mu()
      .set_fft_k()
      .set_ntt32_k()
      .set_ntt64_k()
      .Init();

  auto& tester = GetTester();
  const int size = (int)tester.tests.size();

  TableFormatter tf;
  auto& header = tf.AppendLine();
  std::vector<TimeDelta> total_timings;
  for (int i = 0; i < size; ++i) {
    auto& item = tester.tests[i];
    header.push_back(item.description);
    total_timings.push_back({});
  }
  header.push_back("Total");
  total_timings.push_back({});

  for (int _ = 0; _ < run_count; ++_) {
    std::cout << "Test run " << _ << std::endl;
    bool isFirstTest = true;
    TimeDelta test_suite_timing;
    auto& line = tf.AppendLine();
    for (int i = 0; i < size; ++i) {
      auto& item = tester.tests[i];
      if (!isFirstTest) {
        std::cout << std::endl;
      }
      std::cout << "Begin " << item.description << std::endl;
      TimeRecorder tr;
      item.test();
      std::cout << "End " << item.description << std::endl;
      auto usage = tr.Elapsed();
      std::cout << "Time usage " << usage.Format() << std::endl;

      line.push_back(FormatSecond(usage.ToSeconds()));
      total_timings[i].Add(usage);
      test_suite_timing.Add(usage);
      isFirstTest = false;
    }
    line.push_back(FormatSecond(test_suite_timing.ToSeconds()));
    total_timings[size].Add(test_suite_timing);

    std::cout << std::endl << "Test run " << _ << " finished" << std::endl;
    std::cout << "Time usage " << test_suite_timing.Format() << std::endl
              << std::endl;
  }

  {
    auto& line = tf.AppendLine();
    for (int i = 0; i <= size; ++i) {
      line.push_back("");
    }
  }

  {
    auto& line = tf.AppendLine();
    for (int i = 0; i <= size; ++i) {
      line.push_back(FormatSecond(total_timings[i].ToSeconds()));
    }
  }

  {
    auto& line = tf.AppendLine();
    for (int i = 0; i <= size; ++i) {
      line.push_back(FormatSecond(total_timings[i].ToSeconds() / run_count));
    }
  }

  tf.Render(std::cout);

  std::cout << std::endl;

  tf.SetDefaultFormat(TableFormatter::NoAlign())
      .SetSeparator(",")
      .Render(std::cout);
  return 0;
}