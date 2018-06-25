#include "pe_test.h"

namespace bit_test {
#if defined(COMPILER_GNU)
SL void bit_test() {
  for (int i = 1; i < 65536; ++i) {
    assert(__pe_clz32(i) == __builtin_clz(i));
    assert(__pe_ctz32(i) == __builtin_ctz(i));
    assert(__pe_bitcnt32(i) == __builtin_popcount(i));
    assert(__pe_ffs32(i) == __builtin_ffs(i));
    assert(__pe_parity32(i) == __builtin_parity(i));
    assert(__pe_lg32(i) == __lg(i));
  }

  assert(__pe_bitcnt32(0) == __builtin_popcount(0));
  assert(__pe_ffs32(0) == __builtin_ffs(0));
  assert(__pe_parity32(0) == __builtin_parity(0));

  for (int i = 0; i < 65536; ++i) {
    uint64 target = crand63();
    if (target > 0) {
      assert(__pe_clz64(target) == __builtin_clzll(target));
      assert(__pe_ctz64(target) == __builtin_ctzll(target));
      assert(__pe_bitcnt64(target) == __builtin_popcountll(target));
      assert(__pe_ffs64(target) == __builtin_ffsll(target));
      assert(__pe_parity64(target) == __builtin_parityll(target));
      assert(__pe_lg64(target) == __lg(target));
    }
  }
}

PE_REGISTER_TEST(&bit_test, "bit_test", SMALL);
#endif
}  // namespace bit_test
