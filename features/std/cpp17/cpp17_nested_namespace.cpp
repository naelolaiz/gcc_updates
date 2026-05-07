// gcc-test: std=c++17 min-gcc=13 topic=language experimental=false
// description: Nested namespace declarations: 'namespace a::b::c { ... }' replaces three lines of nesting.
// reference: https://en.cppreference.com/w/cpp/language/namespace

#include <cassert>

namespace company::project::detail {
    int internal_value() { return 7; }
}

namespace company::project {
    int public_value() { return detail::internal_value() * 2; }
}

int main() {
    assert(company::project::detail::internal_value() == 7);
    assert(company::project::public_value() == 14);
    return 0;
}
