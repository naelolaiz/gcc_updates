// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: views::join_with flattens a range-of-ranges with a separator (single element OR a range).
// reference: https://en.cppreference.com/w/cpp/ranges/join_with_view

#include <cassert>
#include <ranges>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> parts{"foo", "bar", "baz"};
    std::string joined;
    for (char c : parts | std::views::join_with(',')) {
        joined.push_back(c);
    }
    assert(joined == "foo,bar,baz");

    // Multi-char separator.
    std::vector<std::string> parts2{"alpha", "beta"};
    std::string sep = " :: ";
    std::string joined2;
    for (char c : parts2 | std::views::join_with(sep)) {
        joined2.push_back(c);
    }
    assert(joined2 == "alpha :: beta");
    return 0;
}
