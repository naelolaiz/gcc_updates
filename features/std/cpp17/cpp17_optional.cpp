// description: std::optional<T> represents a value-or-nothing; use has_value(), value_or(), and emplace().
// reference: https://en.cppreference.com/w/cpp/utility/optional
// why: A function can represent absence without sentinel values or a separate output parameter.
// before: APIs returned magic values, nullable owning pointers, or bool-plus-output pairs.
// pitfall: Dereferencing a disengaged optional is undefined; value() instead throws bad_optional_access.

#include "support/demo.hpp"
#include <cassert>
#include <optional>
#include <string>

std::optional<int> safe_div(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

int main() {
    demo::title("C++17 optional");
    auto ok  = safe_div(10, 2);
    auto bad = safe_div(10, 0);

    DEMO_ASSERT(ok.has_value());
    DEMO_ASSERT(*ok == 5);
    DEMO_ASSERT(ok.value() == 5);

    DEMO_ASSERT(!bad.has_value());
    DEMO_ASSERT(bad.value_or(-1) == -1);

    // emplace constructs in place.
    std::optional<std::string> s;
    s.emplace("hello");
    DEMO_ASSERT(s && *s == "hello");

    // Reset.
    s.reset();
    DEMO_ASSERT(!s);

    // std::in_place_t avoids a temporary when constructing.
    std::optional<std::string> t(std::in_place, 5, 'x');
    DEMO_ASSERT(*t == "xxxxx");
    return 0;
}
