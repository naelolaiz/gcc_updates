// description: views::split lazily divides a range on a delimiter (single element or a whole subrange) into subranges -- tokenise a string_view without allocating a token vector up front.
// reference: https://en.cppreference.com/w/cpp/ranges/split_view

#include "support/demo.hpp"
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

int main() {
    demo::title("C++20 views::split");

    // Split on a single element; each piece is a contiguous subrange over
    // the original characters -- no copies until we materialise strings.
    std::string_view csv = "alpha,beta,gamma";
    std::vector<std::string> tokens;
    for (auto part : csv | std::views::split(',')) {
        tokens.emplace_back(part.begin(), part.end());
    }
    DEMO_ASSERT(tokens.size() == 3);
    DEMO_ASSERT(tokens[1] == "beta");
    demo::range("tokens", tokens);

    // The delimiter can also be a whole range (multi-character separator).
    std::string_view text = "one::two::three";
    int pieces = 0;
    for (auto part : text | std::views::split(std::string_view("::"))) {
        (void)part;
        ++pieces;
    }
    DEMO_ASSERT(pieces == 3);
    return 0;
}
