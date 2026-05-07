// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::string and std::string_view gained .contains() in C++23 (starts_with/ends_with were C++20).
// reference: https://en.cppreference.com/w/cpp/string/basic_string/contains

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <string_view>

int main() {
    demo::title("C++23 string contains");
    std::string s = "hello, world";
    DEMO_ASSERT(s.contains("world"));
    DEMO_ASSERT(s.contains(','));
    DEMO_ASSERT(!s.contains("xyz"));

    std::string_view sv = s;
    DEMO_ASSERT(sv.contains("hello"));
    DEMO_ASSERT(!sv.contains("HELLO"));
    return 0;
}
