// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: views::as_const yields const references; views::as_rvalue yields rvalue references (enables move-iteration).
// reference: https://en.cppreference.com/w/cpp/ranges/as_const_view

#include "support/demo.hpp"
#include <cassert>
#include <ranges>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 views as const as rvalue");
    std::vector<int> v{1, 2, 3, 4};

    // as_const: prevents accidental modification through the view.
    for (auto& x : v | std::views::as_const) {
        // x is 'const int&'; assigning to it would not compile.
        DEMO_ASSERT(x > 0);
    }

    // as_rvalue: each element is yielded as an rvalue. Useful with move-only / costly types.
    std::vector<std::string> src{"alpha", "beta", "gamma"};
    std::vector<std::string> dst;
    for (auto&& s : src | std::views::as_rvalue) {
        dst.push_back(std::move(s));
    }
    DEMO_ASSERT(dst.size() == 3);
    DEMO_ASSERT(dst[0] == "alpha");
    // Source strings are valid-but-unspecified after the move.
    return 0;
}
