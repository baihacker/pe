#include "pe_test.h"

namespace bit_test {
#if defined(COMPILER_GNU)
SL void BitTest() {
  for (int i = 1; i < 65536; ++i) {
    assert(__pe_clz32(i) == __builtin_clz(i));
    assert(__pe_ctz32(i) == __builtin_ctz(i));
    assert(__pe_bitcnt32(i) == __builtin_popcount(i));
    assert(__pe_ffs32(i) == __builtin_ffs(i));
    assert(__pe_parity32(i) == __builtin_parity(i));
    assert(__pe_lg32(i) == std::__lg(i));
  }

  assert(__pe_bitcnt32(0) == __builtin_popcount(0));
  assert(__pe_ffs32(0) == __builtin_ffs(0));
  assert(__pe_parity32(0) == __builtin_parity(0));

  for (int i = 0; i < 65536; ++i) {
    uint64 target = CRand63();
    if (target > 0) {
      assert(__pe_clz64(target) == __builtin_clzll(target));
      assert(__pe_ctz64(target) == __builtin_ctzll(target));
      assert(__pe_bitcnt64(target) == __builtin_popcountll(target));
      assert(__pe_ffs64(target) == __builtin_ffsll(target));
      assert(__pe_parity64(target) == __builtin_parityll(target));
      assert(__pe_lg64(target) == std::__lg(target));
    }
  }

  int x = 0;
  x = SetBit(x, 20);
  assert(x == (1 << 20));
  assert(GetBit(x, 20) == 1);

  x = RevBit(x, 20);
  assert(x == 0);
  assert(GetBit(x, 20) == 0);

  x = RevBit(x, 21);
  assert(x == (1 << 21));
  assert(GetBit(x, 21) == 1);

  x = ResetBit(x, 21);
  assert(x == 0);
  assert(GetBit(x, 21) == 0);
}

PE_REGISTER_TEST(&BitTest, "BitTest", SMALL);
#endif
}  // namespace bit_test
