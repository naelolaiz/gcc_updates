// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false extra-flags=-lstdc++exp
// description: std::print / std::println write directly to stdout using std::format syntax. On GCC 14 the impl lives in libstdc++exp.
// reference: https://en.cppreference.com/w/cpp/io/print

#include <cassert>
#include <print>
#include <sstream>
#include <format>
#include <string>

int main() {
    // We can't easily capture stdout from std::print in a self-test, but we
    // can verify the formatting machinery via std::format (same backend), and
    // exercise std::print at runtime to confirm linkage works.
    std::string s = std::format("{} + {} = {}", 1, 2, 3);
    assert(s == "1 + 2 = 3");

    // Side effect: emit one line. CI captures stdout but doesn't assert on it.
    std::println("hello from std::println, year {}", 2026);
    return 0;
}
