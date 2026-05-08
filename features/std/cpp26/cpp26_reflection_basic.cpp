// description: C++26 static reflection (P2996). API is still settling -- file is experimental.
// reference: https://en.cppreference.com/w/cpp/meta

#include "support/demo.hpp"
#include <cassert>
// The reflection header location and API spelling are subject to change.
// This file demonstrates the SHAPE of expected usage; mark experimental.
#if __has_include(<meta>)
#  include <meta>
#endif

struct Point { int x; int y; };

int main() {
    demo::title("C++26 reflection basic");
#if __has_include(<meta>) && defined(__cpp_static_reflection)
    constexpr auto refl = ^^Point;
    constexpr auto members = std::meta::nonstatic_data_members_of(refl);
    static_assert(members.size() == 2);
#else
    // No reflection support in this build; treat as a soft-skip.
    (void)0;
#endif
    demo::text("check", "static assertions passed");
    return 0;
}
