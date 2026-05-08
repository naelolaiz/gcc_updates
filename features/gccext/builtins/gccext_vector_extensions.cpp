// description: __attribute__((vector_size(N))) makes a real SIMD vector type; element-wise +,-,*, etc. compile to one instruction.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html

#include "support/demo.hpp"
#include <cassert>

typedef int v4si __attribute__((vector_size(16)));   // 4 ints = 128 bits

int main() {
    demo::title("GCC extension vector extensions");
    v4si a = {1, 2, 3, 4};
    v4si b = {10, 20, 30, 40};

    v4si sum  = a + b;
    v4si prod = a * b;

    DEMO_ASSERT(sum[0] == 11);
    DEMO_ASSERT(sum[3] == 44);
    DEMO_ASSERT(prod[0] == 10);
    DEMO_ASSERT(prod[3] == 160);

    // Compare yields a vector mask (all-ones in lanes where true).
    v4si mask = (a < b);
    DEMO_ASSERT(mask[0] != 0);   // 1 < 10 -> all bits set
    return 0;
}
