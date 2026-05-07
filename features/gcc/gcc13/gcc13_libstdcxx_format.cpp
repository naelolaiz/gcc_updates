// gcc-test: std=c++20 min-gcc=13 topic=gcc-release experimental=false
// description: GCC 13 was the first release where libstdc++ shipped a usable <format> implementation by default.
// reference: https://gcc.gnu.org/gcc-13/changes.html

#include <cassert>
#include <format>
#include <string>

int main() {
    auto s = std::format("the answer is {}", 42);
    assert(s == "the answer is 42");

    // Field width / fill / alignment.
    auto padded = std::format("[{:*>5}]", 42);
    assert(padded == "[***42]");
    return 0;
}
