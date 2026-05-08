// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: Raw string literal R"delim(...)delim" disables backslash escapes; great for regex, paths, embedded JSON.
// reference: https://en.cppreference.com/w/cpp/language/string_literal

#include "support/demo.hpp"
#include <cassert>
#include <string>

int main() {
    demo::title("C++11 raw string");
    std::string a = R"(C:\path\to\file)";
    DEMO_ASSERT(a == "C:\\path\\to\\file");

    // Custom delimiter when the content contains )" .
    std::string b = R"json({"key": "value with )\""})json";
    DEMO_ASSERT(b == R"json({"key": "value with )\""})json");

    // Multi-line raw string preserves newlines literally.
    std::string c = R"(
line1
line2
)";
    DEMO_ASSERT(c == "\nline1\nline2\n");
    return 0;
}
