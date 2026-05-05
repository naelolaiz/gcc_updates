// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::string and std::string_view gained .contains() in C++23 (starts_with/ends_with were C++20).
// reference: https://en.cppreference.com/w/cpp/string/basic_string/contains

#include <cassert>
#include <string>
#include <string_view>

int main() {
    std::string s = "hello, world";
    assert(s.contains("world"));
    assert(s.contains(','));
    assert(!s.contains("xyz"));

    std::string_view sv = s;
    assert(sv.contains("hello"));
    assert(!sv.contains("HELLO"));
    return 0;
}
