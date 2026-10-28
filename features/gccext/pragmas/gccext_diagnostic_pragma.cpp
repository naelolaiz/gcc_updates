// description: '#pragma GCC diagnostic push/pop/ignored' silences a warning locally without -Wno-* polluting the rest of the file.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
// why: A known-safe warning in one line or third-party header can be suppressed without weakening diagnostics across the whole translation unit.
// before: Build files passed '-Wno-...' globally to the compiler, hiding real occurrences of the warning elsewhere in the file.
// pitfall: Always pair '#pragma GCC diagnostic ignored' with push and pop so the suppression does not leak past the intended scope.

#include "support/demo.hpp"
#include <cassert>

int unused_in_default_build() { return 0; }

int main() {
    demo::title("GCC extension diagnostic pragma");
    // Locally suppress -Wunused-variable for one block.
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    int intentionally_unused = 42;
    #pragma GCC diagnostic pop

    (void)intentionally_unused;
    DEMO_ASSERT(unused_in_default_build() == 0);
    return 0;
}
