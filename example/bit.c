#include <pe.hpp>
using namespace pe;
using namespace std;

int main() {
  TableFormatter tf;
  auto& line = tf.AppendLine();
  line.push_back("n");
  line.push_back("BitWidth");
  line.push_back("BitFloor");
  line.push_back("BitCeil");
  line.push_back("HighestBitIndex");
  line.push_back("HighestBit");
  line.push_back("LowestBitIndex");
  line.push_back("LowestBit");
  for (int n = 0; n < 32; ++n) {
    auto& line = tf.AppendLine();
    line.push_back(ToString(n));
    line.push_back(ToString(BitWidth(n)));
    line.push_back(ToString(BitFloor(n)));
    line.push_back(ToString(BitCeil(n)));
    line.push_back(ToString(HighestBitIndex(n)));
    line.push_back(ToString(HighestBit(n)));
    line.push_back(ToString(LowestBitIndex(n)));
    line.push_back(ToString(LowestBit(n)));
  }
  tf.Render(std::cout);
  return 0;
}

/*
n     BitWidth    BitFloor    BitCeil    HighestBitIndex    HighestBit    LowestBitIndex    LowestBit
0     0           0           1          -1                 0             -1                0
1     1           1           1          0                  1             0                 1
2     2           2           2          1                  2             1                 2
3     2           2           4          1                  2             0                 1
4     3           4           4          2                  4             2                 4
5     3           4           8          2                  4             0                 1
6     3           4           8          2                  4             1                 2
7     3           4           8          2                  4             0                 1
8     4           8           8          3                  8             3                 8
9     4           8           16         3                  8             0                 1
10    4           8           16         3                  8             1                 2
11    4           8           16         3                  8             0                 1
12    4           8           16         3                  8             2                 4
13    4           8           16         3                  8             0                 1
14    4           8           16         3                  8             1                 2
15    4           8           16         3                  8             0                 1
16    5           16          16         4                  16            4                 16
17    5           16          32         4                  16            0                 1
18    5           16          32         4                  16            1                 2
19    5           16          32         4                  16            0                 1
20    5           16          32         4                  16            2                 4
21    5           16          32         4                  16            0                 1
22    5           16          32         4                  16            1                 2
23    5           16          32         4                  16            0                 1
24    5           16          32         4                  16            3                 8
25    5           16          32         4                  16            0                 1
26    5           16          32         4                  16            1                 2
27    5           16          32         4                  16            0                 1
28    5           16          32         4                  16            2                 4
29    5           16          32         4                  16            0                 1
30    5           16          32         4                  16            1                 2
31    5           16          32         4                  16            0                 1
*/