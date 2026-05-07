// gcc-test: std=c++23 min-gcc=14 topic=gcc-sanitize experimental=false
// description: [[assume(expr)]] is a hint to the optimizer; under -fsanitize=undefined GCC additionally checks the assumption at runtime.
// reference: https://en.cppreference.com/w/cpp/language/attributes/assume

#include <cassert>

int divide_known_safe(int a, int b) {
    // Without sanitizer: compiler trusts the assumption; never evaluates b!=0.
    // Under UBSan: assumption is checked; if it ever fires false, UBSan reports.
    [[assume(b != 0)]];
    return a / b;
}

int main() {
    assert(divide_known_safe(10, 2) == 5);
    assert(divide_known_safe(7, 3) == 2);
    return 0;
}
