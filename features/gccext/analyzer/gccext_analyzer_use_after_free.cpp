// description: -fanalyzer warns at compile time about use-after-free on a path through main(), without requiring that path to run.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html
// why: Compile-time path exploration can find a stale pointer before a runtime input reaches it.
// before: ASan required an execution that followed the allocation, release, and reuse path.
// pitfall: Setting one pointer alias to null does not repair other aliases to the freed storage.

// This file is compiled-only under the analyzer job; the binary is never run.
// Expect a -Wanalyzer-use-after-free diagnostic in the build log.

#include <cstdlib>

int main() {
    int* p = static_cast<int*>(std::malloc(sizeof(int)));
    if (p == nullptr) {
        return 0;
    }
    *p = 42;
    std::free(p);
    return *p;   // analyzer: dereferences memory freed on the line above
}
