// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=undefined expect-exit=1 extra-flags=-fno-sanitize-recover=undefined
// description: UBSan demo: signed integer overflow. Runs only under -fsanitize=undefined; -fno-sanitize-recover=undefined makes the handler fatal (exit 1).
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include <climits>

[[gnu::noinline]] int add_one_unchecked(int x) { return x + 1; }   // UB if x == INT_MAX

int main() {
    volatile int x = INT_MAX;       // hide from constant folding
    volatile int y;                 // volatile sink: forbids dead-call elimination
    y = add_one_unchecked(x);       // UBSan: signed-integer-overflow fires here
    return y;                       // unreachable in practice -- UBSan aborts above
}
