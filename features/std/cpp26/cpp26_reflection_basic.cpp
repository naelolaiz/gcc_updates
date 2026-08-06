// description: C++26 static reflection inspects a type's members at compile time and synthesizes a projected array without hand-written field enumeration.
// reference: https://en.cppreference.com/w/cpp/meta
// why: Generic code can derive structure from program declarations without macros or generated files.
// before: Libraries used registration macros, tuples, or a separate code-generation step.
// pitfall: GCC 16 requires -freflection, and reflected entities obey access-control context.

#include "support/demo.hpp"
#include <meta>

struct Point { int x; int y; };

int main() {
    demo::title("C++26 reflection basic");
    constexpr auto refl = ^^Point;
    // The query returns a temporary vector. Keep it inside one constant
    // expression so its transient allocation is released before evaluation ends.
    static_assert(std::meta::nonstatic_data_members_of(
                      refl, std::meta::access_context::current()).size() == 2);
    demo::text("check", "static assertions passed");
    return 0;
}
