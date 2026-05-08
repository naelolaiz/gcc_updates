// description: __attribute__((packed)) removes padding between struct members; useful for binary protocols, but unaligned access can be slower.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>

struct Normal {
    char     a;
    uint32_t b;
    char     c;
};

struct __attribute__((packed)) Packed {
    char     a;
    uint32_t b;
    char     c;
};

int main() {
    demo::title("GCC extension attribute packed");
    // Normal: padding inserts 3 bytes after 'a' to align b, plus tail padding => 12.
    static_assert(sizeof(Normal) == 12, "");

    // Packed: no padding => 1 + 4 + 1 = 6.
    static_assert(sizeof(Packed) == 6, "");

    Packed p{};
    p.a = 'A'; p.b = 0xDEADBEEF; p.c = 'Z';
    DEMO_ASSERT(p.b == 0xDEADBEEF);
    DEMO_ASSERT(p.a == 'A' && p.c == 'Z');
    return 0;
}
