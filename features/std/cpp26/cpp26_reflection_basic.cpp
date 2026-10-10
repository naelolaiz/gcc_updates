// description: C++26 static reflection enumerates a type's data members at compile time, so one generic loop can read every field without hand-written field lists.
// reference: https://en.cppreference.com/w/cpp/meta
// why: Generic code can derive structure from program declarations without macros or generated files.
// before: Libraries used registration macros, tuples, or a separate code-generation step.
// pitfall: GCC 16 requires -freflection, and reflected entities obey access-control context.

#include "support/demo.hpp"
#include <meta>
#include <string>

struct Point { int x; int y; };

// Sums every non-static data member of any aggregate of ints: the member list
// comes from reflection, and the expansion statement walks it at compile time.
template <typename T>
int sum_members(const T& obj) {
    constexpr auto ctx = std::meta::access_context::current();
    int total = 0;
    template for (constexpr auto member :
                  std::define_static_array(std::meta::nonstatic_data_members_of(^^T, ctx))) {
        total += obj.[:member:];
    }
    return total;
}

int main() {
    demo::title("C++26 reflection basic");
    constexpr auto refl = ^^Point;
    // The query returns a temporary vector. Keep it inside one constant
    // expression so its transient allocation is released before evaluation ends.
    static_assert(std::meta::nonstatic_data_members_of(
                      refl, std::meta::access_context::current()).size() == 2);
    static_assert(std::meta::identifier_of(
                      std::meta::nonstatic_data_members_of(
                          refl, std::meta::access_context::current())[0]) == "x");

    Point p{3, 4};
    DEMO_ASSERT(sum_members(p) == 7);
    return 0;
}
