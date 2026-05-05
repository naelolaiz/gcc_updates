// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::string_view is a non-owning, copy-cheap view over a contiguous sequence of chars.
// reference: https://en.cppreference.com/w/cpp/string/basic_string_view

#include <cassert>
#include <string>
#include <string_view>

// Take string_view to accept const char*, std::string, char arrays, and substrings -- without allocation.
std::size_t length(std::string_view sv) { return sv.size(); }

int main() {
    const char*    a  = "hello";
    std::string    b  = "hello, world";
    char           c[] = "hi";

    assert(length(a) == 5);
    assert(length(b) == 12);
    assert(length(c) == 2);

    // substr() on string_view returns another view -- O(1), no copy.
    std::string_view sv(b);
    auto greeting = sv.substr(0, 5);
    assert(greeting == "hello");
    assert(greeting.data() == b.data());   // points into the original

    // remove_prefix / remove_suffix mutate the view (NOT the source).
    sv.remove_prefix(7);
    assert(sv == "world");

    // string_view literal.
    using namespace std::literals;
    auto lit = "hello"sv;
    static_assert(std::is_same_v<decltype(lit), std::string_view>);
    return 0;
}
