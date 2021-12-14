#include <pe.hpp>
using namespace pe;

const int S = 500;
// The number of non-negative solution for
// 1 x1 + 2 x2 + 3 x3 + 4 x4 + 5 x5 = S
// x1, x2, ..., x5 are distinct.

const int64 mod = 1000000007;
using MT = NMod64<mod>;

char used[S + 1];
int64 dfs(int now, int s) {
  if (now == 6) return s == 0;
  int64 ret = 0;
  for (int i = 0; i <= S && now * i <= s; ++i)
    if (used[i] == 0) {
      used[i] = 1;
      ret += dfs(now + 1, s - now * i);
      used[i] = 0;
    }
  return ret;
}

int64 solve0() { return dfs(1, S); }

std::map<vi, int64> cache;
int64 compute(vi coe) {
  // sum(coe[i] * x_i) = S
  auto where = cache.find(coe);
  if (where != cache.end()) {
    return where->second;
  }
  int64 dp[S + 1] = {1};
  for (auto& iter : coe)
    for (int i = 0; i + iter <= S; ++i) {
      dp[i + iter] += dp[i];
    }
  return dp[S];
}

int64 solve1() {
  PartitionMobius pm(mod);
  vi pattern{1, 2, 3, 4, 5};
  MT ret = 0;
  for (auto p : Partition::GenPartitions(5)) {
    std::map<int, int> mem;
    for (int i = 0; i < 5; ++i) mem[p.colors[i]] += pattern[i];
    vi key;
    for (auto& i : mem) key.pb(i.second);
    std::sort(all(key));
    ret += compute(key) * pm.Cal(p) % mod;
  }
  return ret.value();
}

int main() {
  std::cout << solve0() << std::endl;
  std::cout << solve1() << std::endl;
  return 0;
}