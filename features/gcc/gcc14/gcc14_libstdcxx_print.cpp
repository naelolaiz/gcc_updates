// description: GCC 14 first shipped std::print/std::println in libstdc++; on Linux and other POSIX targets they link with default flags (only Windows needs -lstdc++exp, for Unicode console output).
// reference: https://gcc.gnu.org/gcc-14/changes.html

#include "support/demo.hpp"
#include <format>
#include <print>

int main() {
    demo::title("GCC 14: std::print with no extra library");

    // No EXTRA_LIBS in this example's registration: if it links on the gcc:14
    // image, the formatting and stdout path live in the default libstdc++.
    std::println("  println straight to stdout: {} + {} = {}", 20, 22, 20 + 22);
    DEMO_ASSERT(std::format("{:#06x}", 255) == "0x00ff");
    return 0;
}
