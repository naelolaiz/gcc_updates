// description: __attribute__((constructor)) / ((destructor)) run a function before main() and after it returns -- the C-linkage cousin of static-object constructors, with explicit ordering priorities.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include "support/demo.hpp"
#include <cstdio>

namespace {

int init_order[2] = {0, 0};
int next_slot = 0;

// Priorities 101+ are available to applications; lower runs earlier.
__attribute__((constructor(101))) void early_init() { init_order[next_slot++] = 1; }
__attribute__((constructor(102))) void late_init() { init_order[next_slot++] = 2; }

__attribute__((destructor)) void teardown() {
    // Runs after main() has returned -- keep such hooks minimal.
    std::puts("  teardown: destructor attribute ran after main()");
}

}  // namespace

int main() {
    demo::title("GCC constructor/destructor attributes");

    DEMO_ASSERT(next_slot == 2);                         // both ran before main
    DEMO_ASSERT(init_order[0] == 1 && init_order[1] == 2);  // in priority order
    return 0;
}
