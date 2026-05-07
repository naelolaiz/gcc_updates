// gcc-test: std=c++26 min-gcc=16 topic=templates experimental=false extra-flags=-freflection
// description: C++26 static reflection (P2996). API is still settling -- file is experimental.
// reference: https://en.cppreference.com/w/cpp/meta

#include <cassert>
// The reflection header location and API spelling are subject to change.
// This file demonstrates the SHAPE of expected usage; mark experimental.
#if __has_include(<meta>)
#  include <meta>
#endif

struct Point { int x; int y; };

int main() {
#if __has_include(<meta>) && defined(__cpp_static_reflection)
    constexpr auto refl = ^^Point;
    constexpr auto members = std::meta::nonstatic_data_members_of(refl);
    static_assert(members.size() == 2);
#else
    // No reflection support in this build; treat as a soft-skip.
    (void)0;
#endif
    return 0;
}
