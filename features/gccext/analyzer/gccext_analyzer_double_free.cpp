// description: -fanalyzer flags a double-free that only fires on a control-flow branch the runtime might never hit; runtime sanitizers can miss this if the test inputs don't reach that path.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html
// why: Path analysis can prove that one branch releases the same allocation twice without running it.
// before: A runtime sanitizer found the defect only when test input selected the bad branch.
// pitfall: Analyzer diagnostics are release-dependent warnings, so tests match the warning category.

// Compiled under -fanalyzer only; binary is never executed.
// Expect a -Wanalyzer-double-free diagnostic on the argc>1 branch.

#include <cstdlib>

int main(int argc, char**) {
    int* p = static_cast<int*>(std::malloc(sizeof(int)));
    if (p == nullptr) {
        return 0;
    }
    std::free(p);
    if (argc > 1) {
        std::free(p);   // analyzer: p was already freed on the line above
    }
    return 0;
}
