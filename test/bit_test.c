#include "pe_test.h"

namespace bit_test {
#if defined(COMPILER_GNU)
SL void BitTest() {
  for (int i = 0; i < 65536; ++i) {
    if (i > 0) {
      assert(__pe_clz32(i) == __builtin_clz(i));
      assert(__pe_ctz32(i) == __builtin_ctz(i));
    }
    assert(__pe_popcount32(i) == __builtin_popcount(i));
    assert(__pe_ffs32(i) == __builtin_ffs(i));
    assert(__pe_parity32(i) == __builtin_parity(i));
#if defined(STL_GLIBCXX)
    if (i > 0) {
      assert(__pe_lg32(i) == std::__lg(i));
    }
#endif
  }

  for (int i = 0; i < 65536; ++i) {
    uint64 target = CRand63();
    if (target > 0) {
      assert(__pe_clz64(target) == __builtin_clzll(target));
      assert(__pe_ctz64(target) == __builtin_ctzll(target));
      assert(__pe_popcount64(target) == __builtin_popcountll(target));
      assert(__pe_ffs64(target) == __builtin_ffsll(target));
      assert(__pe_parity64(target) == __builtin_parityll(target));
#if defined(STL_GLIBCXX)
      assert(__pe_lg64(target) == static_cast<int>(std::__lg(target)));
#endif
    }
  }

  int x = 0;
  SetBit(x, 20);
  assert(x == (1 << 20));
  assert(GetBit(x, 20) == 1);

  RevBit(x, 20);
  assert(x == 0);
  assert(GetBit(x, 20) == 0);

  RevBit(x, 21);
  assert(x == (1 << 21));
  assert(GetBit(x, 21) == 1);

  ResetBit(x, 21);
  assert(x == 0);
  assert(GetBit(x, 21) == 0);
}

PE_REGISTER_TEST(&BitTest, "BitTest", SMALL);
#endif
// Reference implementations on the unsigned bit pattern.
template <typename T>
SL void TestBitFunctions() {
  using U = pe_make_unsigned_t<T>;
  constexpr int bits = sizeof(T) * 8;
  std::vector<U> values = {0, 1, 2, 3, 5, 0x80, 0xff, U(~U(0)), U(U(1) << (bits - 1))};
  uint64 seed = 12345;
  for (int i = 0; i < 300; ++i) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    U v = static_cast<U>(seed);
    if constexpr (bits > 64) v = (v << 64) | static_cast<U>(seed * 31 + 7);
    // Sparse values too
    values.push_back(v);
    values.push_back(v & (v >> 3) & (v >> 7));
  }
  for (U u : values) {
    const T x = static_cast<T>(u);
    int pop = 0, clz = bits, ctz = bits;
    std::vector<int> idx;
    for (int i = 0; i < bits; ++i) {
      if ((u >> i) & 1) {
        ++pop;
        idx.push_back(i);
        if (ctz == bits) ctz = i;
        clz = bits - 1 - i;
      }
    }
    assert(Popcount(x) == pop);
    assert(HasSingleBit(x) == (pop == 1));
    assert(CountLeftZero(x) == clz && CountRightZero(x) == ctz);
    assert(BitWidth(x) == bits - clz);
    assert(HighestBitIndex(x) == (u ? bits - 1 - clz : -1));
    assert(LowestBitIndex(x) == (u ? ctz : -1));
    assert(static_cast<U>(HighestBit(x)) == (u ? U(U(1) << (bits - 1 - clz)) : 0));
    assert(static_cast<U>(LowestBit(x)) == (u ? U(U(1) << ctz) : 0));
    assert(static_cast<U>(BitFloor(x)) == static_cast<U>(HighestBit(x)));
    if (u && clz > 0) {
      const U ceil = HasSingleBit(x) ? u : U(U(1) << (bits - clz));
      if constexpr (std::is_unsigned_v<T>) assert(BitCeil(x) == ceil);
    }
    assert(BitIndex(x) == idx);
    for (int s : {0, 1, 7, bits - 1, bits, bits + 3, -1, -bits - 2}) {
      const int r = ((s % bits) + bits) % bits;
      const U left = r ? U(U(u << r) | U(u >> (bits - r))) : u;
      const U right = r ? U(U(u >> r) | U(u << (bits - r))) : u;
      assert(static_cast<U>(RotateLeft(x, s)) == left);
      assert(static_cast<U>(RotateRight(x, s)) == right);
    }
    if constexpr (bits >= 32) {
      U swapped = 0;
      for (int i = 0; i < bits / 8; ++i) {
        swapped |= U((u >> (8 * i)) & 0xff) << (bits - 8 - 8 * i);
      }
      assert(static_cast<U>(Byteswap(x)) == swapped);
    }
    // SetBit / ResetBit / GetBit / RevBit
    T y = x;
    for (int i : {0, bits / 2, bits - 1}) {
      assert(GetBit(y, i) == static_cast<int>((u >> i) & 1));
      SetBit(y, i);
      assert(GetBit(y, i) == 1);
      ResetBit(y, i);
      assert(GetBit(y, i) == 0);
      RevBit(y, i);
      assert(GetBit(y, i) == 1);
    }
  }
  assert(BitCeil(T(0)) == 1 && BitFloor(T(0)) == 0);
}

SL void BitFunctionsTest() {
  TestBitFunctions<int8_t>();
  TestBitFunctions<uint8_t>();
  TestBitFunctions<int16_t>();
  TestBitFunctions<uint16_t>();
  TestBitFunctions<int32>();
  TestBitFunctions<uint32>();
  TestBitFunctions<int64>();
  TestBitFunctions<uint64>();
#if PE_HAS_INT128
  TestBitFunctions<int128>();
  TestBitFunctions<uint128>();
#endif

  // Bit reverse and byte swap of the fixed width helpers.
  uint64 seed = 99;
  for (int i = 0; i < 1000; ++i) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    const uint32 a = static_cast<uint32>(seed >> 7);
    const uint64 b = seed;
    uint32 ra = 0;
    uint64 rb = 0;
    for (int j = 0; j < 32; ++j) ra |= ((a >> j) & 1u) << (31 - j);
    for (int j = 0; j < 64; ++j) rb |= ((b >> j) & 1ull) << (63 - j);
    assert(__pe_bitreverse32(a) == ra && __pe_bitreverse64(b) == rb);
    assert(__pe_bswap32(a) == Byteswap(a) && __pe_bswap64(b) == Byteswap(b));
    assert(__pe_clz64(b | 1) == CountLeftZero(b | 1));
    assert(__pe_lg64(b | 1) == HighestBitIndex(b | 1));
    assert(__pe_lg32(a | 1) == HighestBitIndex(a | 1));
#if PE_HAS_INT128
    const uint128 c = (static_cast<uint128>(b) << 64) | a;
    assert(__pe_bitreverse128(c) ==
           ((static_cast<uint128>(__pe_bitreverse64(static_cast<uint64>(c))) << 64) |
            __pe_bitreverse64(static_cast<uint64>(c >> 64))));
    assert(__pe_popcount128(c) == Popcount(c) && __pe_clz128(c | 1) == CountLeftZero(c | 1));
    assert(__pe_ctz128(c | 1) == CountRightZero(c | 1));
    assert(__pe_ffs128(c) == (c ? CountRightZero(c) + 1 : 0));
    assert(__pe_parity128(c) == (Popcount(c) & 1));
    assert(__pe_lg128(c | 1) == HighestBitIndex(c | 1));
    assert(__pe_bswap128(c) == Byteswap(c));
#endif
  }

  // NextComb enumerates the k-subsets of n bits in increasing order.
  for (int n = 1; n <= 10; ++n) {
    for (int k = 1; k <= n; ++k) {
      std::vector<int64> expected;
      for (int64 m = 0; m < (int64(1) << n); ++m) {
        if (Popcount(m) == k) expected.push_back(m);
      }
      std::vector<int64> got;
      for (int64 m = (int64(1) << k) - 1; m < (int64(1) << n);
           m = NextComb(m)) {
        got.push_back(m);
      }
      assert(got == expected);
    }
  }
}

PE_REGISTER_TEST(&BitFunctionsTest, "BitFunctionsTest", SMALL);
}  // namespace bit_test
