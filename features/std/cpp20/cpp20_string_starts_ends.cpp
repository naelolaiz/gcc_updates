// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::string and std::string_view gained starts_with / ends_with member functions.
// reference: https://en.cppreference.com/w/cpp/string/basic_string/starts_with

#include <cassert>
#include <string>
#include <string_view>

int main() {
    std::string s = "hello, world";
    assert(s.starts_with("hello"));
    assert(s.starts_with('h'));
    assert(!s.starts_with("world"));

    assert(s.ends_with("world"));
    assert(s.ends_with('d'));
    assert(!s.ends_with("hello"));

    std::string_view sv = s;
    assert(sv.starts_with("hello,"));
    assert(sv.ends_with(", world"));
    return 0;
}
