// description: C++20 modules compile an interface before an importer; this two-file fixture proves export, import, link, and execution with GCC's modules implementation.
// reference: https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Modules.html
// why: Modules replace repeated textual inclusion with an explicit compiled interface.
// before: A header declaration plus a separately compiled implementation file.
// pitfall: GCC module support (this fixture passes from GCC 15) remains experimental and requires -fmodules.

#include "support/demo.hpp"

import cpp20_modules_math;

int main() {
    demo::title("C++20 modules basic");
    static_assert(square(6) == 36);
    DEMO_ASSERT(square(6) == 36);
    return 0;
}
