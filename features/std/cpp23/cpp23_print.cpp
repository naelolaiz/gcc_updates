// description: std::print / std::println write std::format-style output directly to stdout or any FILE*.
// reference: https://en.cppreference.com/w/cpp/io/print
// why: Programs can emit type-safe formatted text without building a temporary string or stream chain.
// before: Code used printf varargs, iostream insertion, or std::cout << std::format(...).
// pitfall: libstdc++ needs -lstdc++exp for these only on Windows (Unicode console output); Linux builds link them by default since GCC 14.

#include "support/demo.hpp"
#include <cassert>
#include <print>
#include <sstream>
#include <format>
#include <string>

int main() {
    demo::title("C++23 print");
    // std::format shares std::print's formatting backend, so the string can be
    // asserted in-process; the printed line itself is checked by CTest.
    std::string s = std::format("{} + {} = {}", 1, 2, 3);
    DEMO_ASSERT(s == "1 + 2 = 3");

    // EXPECT_RUN_OUTPUT in CMakeLists.txt asserts this line reaches stdout.
    std::println("hello from std::println, year {}", 2026);
    return 0;
}
