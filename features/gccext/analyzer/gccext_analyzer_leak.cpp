// description: -fanalyzer follows an early-return path that loses the last pointer to an allocation and reports a leak without executing the program.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html
// why: A path-sensitive compile check can find leaks that a runtime test never happens to execute.
// before: Reviewers or runtime leak detection had to exercise the early-return path.
// pitfall: Analyzer findings are warnings and may change with optimization or compiler release.

#include <cstdlib>

void allocate_then_maybe_return(const bool return_early) {
    int* value = static_cast<int*>(std::malloc(sizeof(int)));
    if (return_early) {
        return;   // analyzer: the allocation is lost on this path
    }
    std::free(value);
}

int main(int argc, char**) {
    allocate_then_maybe_return(argc > 1);
    return 0;
}
