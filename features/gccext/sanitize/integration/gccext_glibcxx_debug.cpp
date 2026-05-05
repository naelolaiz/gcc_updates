// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false extra-flags=-D_GLIBCXX_DEBUG
// description: -D_GLIBCXX_DEBUG turns on libstdc++'s debug mode -- iterator misuse, out-of-range access, mismatched containers all assert at runtime.
// reference: https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html

#include <cassert>
#include <vector>

int main() {
    // In debug mode the standard containers carry extra invariants and runtime
    // checks. This file just verifies the program still compiles and runs
    // cleanly under the flag (i.e. our examples are debug-mode safe).
    std::vector<int> v{1, 2, 3, 4, 5};

    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) sum += *it;
    assert(sum == 15);

    // Mutating-during-iteration via ranges API would assert in debug mode; we
    // do the safe thing: re-fetch iterators around the mutation.
    v.erase(v.begin() + 2);
    assert(v.size() == 4);
    return 0;
}
