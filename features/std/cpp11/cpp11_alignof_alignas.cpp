// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'alignof(T)' yields T's alignment; 'alignas(N)' constrains a variable or type's alignment.
// reference: https://en.cppreference.com/w/cpp/language/alignof

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>

struct alignas(16) Aligned16 {
    int a;
    int b;
};

int main() {
    demo::title("C++11 alignof alignas");
    static_assert(alignof(int) >= 1, "");
    static_assert(alignof(double) >= alignof(int), "");

    static_assert(alignof(Aligned16) == 16, "");

    Aligned16 x{};
    auto addr = reinterpret_cast<std::uintptr_t>(&x);
    DEMO_ASSERT(addr % 16 == 0);

    alignas(32) char buf[64]{};
    auto baddr = reinterpret_cast<std::uintptr_t>(&buf[0]);
    DEMO_ASSERT(baddr % 32 == 0);
    return 0;
}
