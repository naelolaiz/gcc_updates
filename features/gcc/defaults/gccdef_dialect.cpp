// description: Compiled WITHOUT any -std flag to test the compiler's default dialect: gnu++17 on GCC 13-15, gnu++20 from GCC 16 -- code that relies on "whatever the compiler defaults to" changes meaning across releases.
// reference: https://gcc.gnu.org/projects/cxx-status.html

#include "support/demo.hpp"

int main() {
    demo::title("GCC default C++ dialect (no -std flag)");

    demo::value("__GNUC__", __GNUC__);
    demo::value("__cplusplus", __cplusplus);

#if __GNUC__ >= 16
    static_assert(__cplusplus == 202002L, "GCC 16 changed the default to gnu++20");
#else
    static_assert(__cplusplus == 201703L, "GCC 13-15 default to gnu++17");
#endif

    // The default is always the GNU dialect (gnu++NN), never strict ISO:
    // __STRICT_ANSI__ is only defined under -std=c++NN.
#ifdef __STRICT_ANSI__
    constexpr bool strict_iso = true;
#else
    constexpr bool strict_iso = false;
#endif
    DEMO_ASSERT(!strict_iso);
    demo::text("dialect flavour", "GNU (gnu++NN), not strict ISO");
    return 0;
}
