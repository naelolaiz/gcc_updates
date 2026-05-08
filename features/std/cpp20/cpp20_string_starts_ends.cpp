// description: std::string and std::string_view gained starts_with / ends_with member functions.
// reference: https://en.cppreference.com/w/cpp/string/basic_string/starts_with

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <string_view>

int main() {
    demo::title("C++20 string starts ends");
    std::string s = "hello, world";
    DEMO_ASSERT(s.starts_with("hello"));
    DEMO_ASSERT(s.starts_with('h'));
    DEMO_ASSERT(!s.starts_with("world"));

    DEMO_ASSERT(s.ends_with("world"));
    DEMO_ASSERT(s.ends_with('d'));
    DEMO_ASSERT(!s.ends_with("hello"));

    std::string_view sv = s;
    DEMO_ASSERT(sv.starts_with("hello,"));
    DEMO_ASSERT(sv.ends_with(", world"));
    return 0;
}
