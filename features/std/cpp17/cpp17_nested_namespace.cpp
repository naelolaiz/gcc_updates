// description: Nested namespace declarations: 'namespace a::b::c { ... }' replaces three lines of nesting.
// reference: https://en.cppreference.com/w/cpp/language/namespace

#include "support/demo.hpp"
#include <cassert>

namespace company::project::detail {
    int internal_value() { return 7; }
}

namespace company::project {
    int public_value() { return detail::internal_value() * 2; }
}

int main() {
    demo::title("C++17 nested namespace");
    DEMO_ASSERT(company::project::detail::internal_value() == 7);
    DEMO_ASSERT(company::project::public_value() == 14);
    return 0;
}
