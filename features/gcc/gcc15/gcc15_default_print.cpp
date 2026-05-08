// description: GCC 15 promoted std::print/std::println from libstdc++exp into the main libstdc++ -- no extra link flags needed.
// reference: https://gcc.gnu.org/gcc-15/changes.html

#include "support/demo.hpp"
#include <cassert>
#include <print>
#include <format>

int main() {
    demo::title("GCC 15 default print");
    // No -lstdc++exp on GCC 15+. If this file links cleanly with default
    // settings, the impl-promotion is in effect.
    std::println("gcc-15: print is in main libstdc++");

    auto s = std::format("{:>5}", 42);
    DEMO_ASSERT(s == "   42");
    return 0;
}
