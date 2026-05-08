// description: std::string_view is a non-owning, copy-cheap view over a contiguous sequence of chars.
// reference: https://en.cppreference.com/w/cpp/string/basic_string_view

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <string_view>

// Take string_view to accept const char*, std::string, char arrays, and substrings -- without allocation.
std::size_t length(std::string_view sv) { return sv.size(); }

int main() {
    demo::title("C++17 string view");
    const char*    a  = "hello";
    std::string    b  = "hello, world";
    char           c[] = "hi";

    DEMO_ASSERT(length(a) == 5);
    DEMO_ASSERT(length(b) == 12);
    DEMO_ASSERT(length(c) == 2);

    // substr() on string_view returns another view -- O(1), no copy.
    std::string_view sv(b);
    auto greeting = sv.substr(0, 5);
    DEMO_ASSERT(greeting == "hello");
    DEMO_ASSERT(greeting.data() == b.data());   // points into the original

    // remove_prefix / remove_suffix mutate the view (NOT the source).
    sv.remove_prefix(7);
    DEMO_ASSERT(sv == "world");

    // string_view literal.
    using namespace std::literals;
    auto lit = "hello"sv;
    static_assert(std::is_same_v<decltype(lit), std::string_view>);
    return 0;
}
