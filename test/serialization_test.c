#include "pe_test.h"

namespace serialization_test {

struct Point {
  int x;
  int64 y;
  DECLARE_SERIALIZATION(x, y)
  bool operator==(const Point& o) const { return x == o.x && y == o.y; }
  bool operator<(const Point& o) const {
    return x != o.x ? x < o.x : y < o.y;
  }
};

struct Record {
  std::string name;
  std::vector<int> values;
  Point origin;
  std::map<std::string, std::vector<Point>> groups;
  double weight;
  DECLARE_SERIALIZATION(name, values, origin, groups, weight)
  bool operator==(const Record& o) const {
    return name == o.name && values == o.values && origin == o.origin &&
           groups == o.groups && weight == o.weight;
  }
};

struct Empty {
  auto AsTuple() { return std::tuple<>(); }
  auto AsTuple() const { return std::tuple<>(); }
};

struct Raw16 {
  int64 a;
  int64 b;
};

struct Raw24 {
  int32 a;
  int32 b;
  double c;
  uint64 d;
};

static_assert(has_as_tuple_v<Point>);
static_assert(has_as_tuple_v<Record>);
static_assert(has_as_tuple_v<Empty>);
static_assert(!has_as_tuple_v<int>);
static_assert(!has_as_tuple_v<Raw16>);
static_assert(can_copy_by_byte_v<int>);
static_assert(can_copy_by_byte_v<double>);
static_assert(can_copy_by_byte_v<Raw16>);
static_assert(!can_copy_by_byte_v<Point>);
static_assert(!can_copy_by_byte_v<std::string>);
static_assert(support_serialization_v<int64>);
static_assert(support_serialization_v<Point>);
static_assert(support_serialization_v<Record>);
static_assert(!support_serialization_v<std::string>);
static_assert(!support_serialization_v<std::vector<int>>);

// Serializes value, deserializes it into a fresh T through the index-based
// API and checks that the whole buffer is consumed and the value round trips.
template <typename T>
SL void CheckRoundTrip(const T& value) {
  const std::vector<int64> data = SerializeObject(value);
  T out{};
  int64 idx = 0;
  DeserializeObject(data, idx, out);
  assert(idx == static_cast<int64>(std::size(data)));
  assert(out == value);
}

template <typename T>
SL void CheckScalar(T value) {
  CheckRoundTrip(value);
  const std::vector<int64> data = SerializeObject(value);
  assert(std::size(data) == (sizeof(T) + 7) / 8);
  assert(DeserializeObject<T>(data) == value);
  T out{};
  DeserializeObject(data, out);
  assert(out == value);
  assert(DeserializeObjectFrom<T>(data, 0) == value);
}

SL void ScalarTest() {
  for (int v : {0, 1, -1, 123456, -123456, std::numeric_limits<int>::max(),
                std::numeric_limits<int>::min()}) {
    CheckScalar<int>(v);
    CheckScalar<int32>(v);
  }
  for (uint32 v : {0U, 1U, 4000000000U, std::numeric_limits<uint32>::max()}) {
    CheckScalar<uint32>(v);
  }
  for (int64 v : std::initializer_list<int64>{
           0, 1, -1, 1234567890123LL, std::numeric_limits<int64>::max(),
           std::numeric_limits<int64>::min()}) {
    CheckScalar<int64>(v);
  }
  for (uint64 v : std::initializer_list<uint64>{
           0, 1, std::numeric_limits<uint64>::max()}) {
    CheckScalar<uint64>(v);
  }
  for (double v : {0.0, -0.0, 1.5, -3.25e100, 1e-300}) {
    CheckScalar<double>(v);
  }
  CheckScalar<double>(std::numeric_limits<double>::infinity());
  for (float v : {0.0f, 2.5f, -7.125f}) {
    CheckScalar<float>(v);
  }
#if PE_HAS_INT128
  CheckScalar<int128>(0);
  CheckScalar<int128>(-1);
  CheckScalar<int128>(static_cast<int128>(1) << 100);
  CheckScalar<int128>(-(static_cast<int128>(123456789) << 70) + 5);
  CheckScalar<uint128>(~static_cast<uint128>(0));
  assert(std::size(SerializeObject(static_cast<int128>(3))) == 2);
#endif

  // Exact encoding of 4 byte and 8 byte values.
  assert(SerializeObject(-1) == std::vector<int64>{4294967295LL});
  assert(SerializeObject(7) == std::vector<int64>{7});
  assert(SerializeObject(-1LL) == std::vector<int64>{-1});
  assert(SerializeObject(std::numeric_limits<uint64>::max()) ==
         std::vector<int64>{-1});
  assert(DeserializeObject<int>(std::vector<int64>{4294967295LL}) == -1);
  assert(DeserializeObject<int>(std::vector<int64>{-1}) == -1);
}

SL void TrivialStructTest() {
  const Raw16 a{-5, 1LL << 60};
  const std::vector<int64> da = SerializeObject(a);
  assert((da == std::vector<int64>{-5, 1LL << 60}));
  const Raw16 ra = DeserializeObject<Raw16>(da);
  assert(ra.a == a.a && ra.b == a.b);

  const Raw24 b{-3, 9, 2.75, 1ULL << 63};
  const std::vector<int64> db = SerializeObject(b);
  assert(std::size(db) == 3);
  Raw24 rb{};
  DeserializeObject(db, rb);
  assert(rb.a == b.a && rb.b == b.b && rb.c == b.c && rb.d == b.d);
}

SL void StringTest() {
  CheckRoundTrip(std::string());
  CheckRoundTrip(std::string("a"));
  CheckRoundTrip(std::string("hello, world"));
  std::string bin;
  for (int i = 0; i < 256; ++i) bin.push_back(static_cast<char>(i));
  CheckRoundTrip(bin);
  CheckRoundTrip(std::string("with\0nul", 8));

  assert((SerializeObject(std::string("ab")) == std::vector<int64>{2, 'a', 'b'}));
  assert(SerializeObject(std::string()) == std::vector<int64>{0});

  // Deserializing into a non-empty string appends (intended behavior).
  std::string s = "xy";
  int64 idx = 0;
  DeserializeObject(SerializeObject(std::string("zw")), idx, s);
  assert(s == "xyzw" && idx == 3);
}

SL void ContainerTest() {
  CheckRoundTrip(std::vector<int>());
  CheckRoundTrip(std::vector<int>{1, -2, 3, std::numeric_limits<int>::min()});
  CheckRoundTrip(std::vector<int64>{-1, 0, 1LL << 62});
  CheckRoundTrip(std::vector<double>{0.5, -1.25});
  CheckRoundTrip(std::vector<std::string>{"", "a", "bc", ""});
  CheckRoundTrip(std::vector<std::vector<int>>{{}, {1}, {2, 3}, {}});
  CheckRoundTrip(std::vector<std::vector<std::string>>{{"x", ""}, {}});

  CheckRoundTrip(std::set<int>());
  CheckRoundTrip(std::set<int>{5, -3, 9, 0});
  CheckRoundTrip(std::set<std::string>{"b", "a", ""});
  CheckRoundTrip(std::unordered_set<int64>());
  CheckRoundTrip(std::unordered_set<int64>{7, -7, 1LL << 40});
  CheckRoundTrip(std::unordered_set<std::string>{"p", "q"});

  CheckRoundTrip(std::map<int, int>());
  CheckRoundTrip(std::map<int, std::string>{{1, "one"}, {-2, ""}, {3, "t"}});
  CheckRoundTrip(std::map<std::string, std::vector<int64>>{
      {"a", {}}, {"b", {1, 2}}, {"", {-1}}});
  CheckRoundTrip(std::unordered_map<int64, int>());
  CheckRoundTrip(std::unordered_map<int64, int>{{1, 2}, {-3, 4}, {5, -6}});
  CheckRoundTrip(std::unordered_map<std::string, std::set<int>>{
      {"k", {3, 1}}, {"", {}}});
  CheckRoundTrip(std::map<int, std::map<int, std::vector<std::string>>>{
      {1, {{2, {"x"}}, {3, {}}}}, {4, {}}});

  // Exact layout: size followed by elements.
  assert((SerializeObject(std::vector<int>{1, -1}) ==
          std::vector<int64>{2, 1, 4294967295LL}));
  assert(SerializeObject(std::vector<int>()) == std::vector<int64>{0});
  assert((SerializeObject(std::map<int64, int64>{{2, 3}, {1, 4}}) ==
          std::vector<int64>{2, 1, 4, 2, 3}));
  assert((SerializeObject(std::vector<std::string>{"a", ""}) ==
          std::vector<int64>{2, 1, 'a', 0}));

  // Deserializing into a non-empty container appends / merges (intended).
  {
    std::vector<int> v{9};
    int64 idx = 0;
    DeserializeObject(SerializeObject(std::vector<int>{1, 2}), idx, v);
    assert((v == std::vector<int>{9, 1, 2}));
  }
  {
    std::set<int> s{1, 5};
    int64 idx = 0;
    DeserializeObject(SerializeObject(std::set<int>{1, 2}), idx, s);
    assert((s == std::set<int>{1, 2, 5}));
  }
  {
    std::map<int, int> m{{1, 10}, {2, 20}};
    int64 idx = 0;
    DeserializeObject(SerializeObject(std::map<int, int>{{2, 99}, {3, 30}}),
                      idx, m);
    assert((m == std::map<int, int>{{1, 10}, {2, 99}, {3, 30}}));
  }
}

SL void PairTest() {
  CheckRoundTrip(std::pair<int, int64>(-1, 1LL << 50));
  CheckRoundTrip(std::pair<std::string, int>("key", 42));
  CheckRoundTrip(std::pair<std::vector<int>, std::string>({1, 2}, ""));
  CheckRoundTrip(std::pair<std::pair<int, int>, std::pair<int64, int64>>(
      {1, 2}, {3, 4}));
  CheckRoundTrip(std::vector<std::pair<int, std::string>>{{1, "a"}, {2, ""}});
  CheckRoundTrip(std::map<std::pair<int, int>, std::pair<std::string, int64>>{
      {{1, 2}, {"x", 3}}, {{-1, 0}, {"", -4}}});

  // A pair is just the concatenation of its members.
  assert((SerializeObject(std::pair<int64, std::string>(5, "ab")) ==
          std::vector<int64>{5, 2, 'a', 'b'}));
}

SL void AsTupleTest() {
  const Point p{-7, 1LL << 45};
  assert((SerializeObject(p) == std::vector<int64>{2, 4294967289LL, 1LL << 45}));
  CheckRoundTrip(p);
  assert(DeserializeObject<Point>(SerializeObject(p)) == p);
  Point q{};
  DeserializeObject(SerializeObject(p), q);
  assert(q == p);

  assert(SerializeObject(Empty{}) == std::vector<int64>{0});
  {
    Empty e;
    int64 idx = 0;
    DeserializeObject(std::vector<int64>{0}, idx, e);
    assert(idx == 1);
  }

  Record r;
  r.name = "record";
  r.values = {3, -1, 4};
  r.origin = {1, -2};
  r.groups["a"] = {{1, 2}, {3, 4}};
  r.groups["empty"] = {};
  r.weight = 0.125;
  CheckRoundTrip(r);
  CheckRoundTrip(Record{});
  assert(DeserializeObject<Record>(SerializeObject(r)) == r);

  // Containers of AsTuple structs.
  CheckRoundTrip(std::vector<Point>{{1, 2}, {-3, -4}});
  CheckRoundTrip(std::set<Point>{{1, 2}, {0, 9}});
  CheckRoundTrip(std::map<int, Record>{{1, r}, {2, Record{}}});
  CheckRoundTrip(std::pair<Point, Record>(p, r));
}

SL void StreamTest() {
  // Several objects written back to back and read in order.
  const Point p{11, -12};
  const std::string s = "mid";
  const std::vector<int64> v{1, 2, 3};
  const int64 x = -99;
  std::vector<int64> data;
  for (const auto& part : {SerializeObject(p), SerializeObject(s),
                           SerializeObject(v), SerializeObject(x)}) {
    data.insert(std::end(data), std::begin(part), std::end(part));
  }

  int64 idx = 0;
  Point rp{};
  std::string rs;
  std::vector<int64> rv;
  int64 rx = 0;
  DeserializeObject(data, idx, rp);
  const int64 after_point = idx;
  assert(after_point == 3);
  DeserializeObject(data, idx, rs);
  DeserializeObject(data, idx, rv);
  DeserializeObject(data, idx, rx);
  assert(idx == static_cast<int64>(std::size(data)));
  assert(rp == p && rs == s && rv == v && rx == x);

  // DeserializeObjectFrom starts at a given offset.
  assert(DeserializeObjectFrom<Point>(data, 0) == p);
  assert(DeserializeObjectFrom<int64>(data, std::size(data) - 1) == x);
  assert(DeserializeObjectFrom<int64>(data, after_point) == 3);
}

PE_REGISTER_TEST(&ScalarTest, "SerializationScalarTest", SMALL);
PE_REGISTER_TEST(&TrivialStructTest, "SerializationTrivialStructTest", SMALL);
PE_REGISTER_TEST(&StringTest, "SerializationStringTest", SMALL);
PE_REGISTER_TEST(&ContainerTest, "SerializationContainerTest", SMALL);
PE_REGISTER_TEST(&PairTest, "SerializationPairTest", SMALL);
PE_REGISTER_TEST(&AsTupleTest, "SerializationAsTupleTest", SMALL);
PE_REGISTER_TEST(&StreamTest, "SerializationStreamTest", SMALL);
}  // namespace serialization_test
