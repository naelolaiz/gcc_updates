// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: Raw string literal R"delim(...)delim" disables backslash escapes; great for regex, paths, embedded JSON.
// reference: https://en.cppreference.com/w/cpp/language/string_literal

#include <cassert>
#include <string>

int main() {
    std::string a = R"(C:\path\to\file)";
    assert(a == "C:\\path\\to\\file");

    // Custom delimiter when the content contains )" .
    std::string b = R"json({"key": "value with )\""})json";
    assert(b == R"json({"key": "value with )\""})json");

    // Multi-line raw string preserves newlines literally.
    std::string c = R"(
line1
line2
)";
    assert(c == "\nline1\nline2\n");
    return 0;
}
