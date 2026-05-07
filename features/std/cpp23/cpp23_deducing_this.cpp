// gcc-test: std=c++23 min-gcc=14 topic=templates experimental=false
// description: Explicit object parameter ('deducing this') replaces ref-qual overload sets and enables CRTP-without-CRTP.
// reference: https://en.cppreference.com/w/cpp/language/member_functions#Explicit_object_parameter

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

struct Box {
    std::string s;

    // ONE function definition replaces the four ref-qualified overloads
    // (&, const&, &&, const&&). Self deduces to the correct cvref type.
    template <typename Self>
    auto&& payload(this Self&& self) {
        return std::forward<Self>(self).s;
    }
};

// CRTP-without-CRTP: a 'mixin' that knows the derived type via Self deduction.
struct Doublable {
    template <typename Self>
    auto twice(this const Self& self) {
        return self.value() + self.value();
    }
};

struct Counter : Doublable {
    int n;
    int value() const { return n; }
};

int main() {
    Box b{"hi"};
    auto& got_lvalue = b.payload();
    static_assert(std::is_same_v<decltype(got_lvalue), std::string&>);
    assert(got_lvalue == "hi");

    auto moved = Box{"yo"}.payload();   // Self = Box, returns rvalue ref
    assert(moved == "yo");

    Counter c{.n = 21};
    assert(c.twice() == 42);
    return 0;
}
