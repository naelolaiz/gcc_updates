// description: -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html
// why: A nullable return can be dangerous only on one path several calls away from its dereference.
// before: Tests had to produce the null path, or review had to trace it manually.
// pitfall: A pointer check must dominate every dereference, not merely the common path.

// Compiled under -fanalyzer only; binary is never executed.
// Expect a -Wanalyzer-null-dereference diagnostic on the path where allocate() returns nullptr.

#include <cstdlib>

int main(int argc, char**) {
    int* p = nullptr;
    if (argc > 1) {
        p = static_cast<int*>(std::malloc(sizeof(int)));
        if (p == nullptr) {
            return 0;
        }
        *p = 7;
    }
    return *p;   // analyzer: null pointer dereference on the argc <= 1 path
}
