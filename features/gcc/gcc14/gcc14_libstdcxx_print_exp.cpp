// description: GCC 14 first shipped std::print/std::println in libstdc++, still in the experimental library -- linking -lstdc++exp is required (GCC 15 later moved it into the main library; see gcc15_default_print).
// reference: https://gcc.gnu.org/gcc-14/changes.html

#include "support/demo.hpp"
#include <format>
#include <print>

int main() {
    demo::title("GCC 14: std::print via -lstdc++exp");

    std::println("  println straight to stdout: {} + {} = {}", 20, 22, 20 + 22);
    DEMO_ASSERT(std::format("{:#06x}", 255) == "0x00ff");
    return 0;
}
