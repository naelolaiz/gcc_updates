// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::optional<T> represents a value-or-nothing; use has_value(), value_or(), and emplace().
// reference: https://en.cppreference.com/w/cpp/utility/optional

#include <cassert>
#include <optional>
#include <string>

std::optional<int> safe_div(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

int main() {
    auto ok  = safe_div(10, 2);
    auto bad = safe_div(10, 0);

    assert(ok.has_value());
    assert(*ok == 5);
    assert(ok.value() == 5);

    assert(!bad.has_value());
    assert(bad.value_or(-1) == -1);

    // emplace constructs in place.
    std::optional<std::string> s;
    s.emplace("hello");
    assert(s && *s == "hello");

    // Reset.
    s.reset();
    assert(!s);

    // std::in_place_t avoids a temporary when constructing.
    std::optional<std::string> t(std::in_place, 5, 'x');
    assert(*t == "xxxxx");
    return 0;
}
