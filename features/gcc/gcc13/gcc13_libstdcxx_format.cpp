// description: GCC 13 was the first release where libstdc++ shipped a usable <format> implementation by default.
// reference: https://gcc.gnu.org/gcc-13/changes.html

#include "support/demo.hpp"
#include <cassert>
#include <format>
#include <string>

int main() {
    demo::title("GCC 13 libstdcxx format");
    auto s = std::format("the answer is {}", 42);
    DEMO_ASSERT(s == "the answer is 42");

    // Field width / fill / alignment.
    auto padded = std::format("[{:*>5}]", 42);
    DEMO_ASSERT(padded == "[***42]");
    return 0;
}
