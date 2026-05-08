// description: [[assume(expr)]] tells the optimizer 'expr is true here' WITHOUT evaluating it -- a portable hint.
// reference: https://en.cppreference.com/w/cpp/language/attributes/assume

#include "support/demo.hpp"
#include <cassert>

int divide_known_safe(int a, int b) {
    [[assume(b != 0)]];   // promise to the compiler; evaluating it is UB if false
    return a / b;
}

int clamp_to_byte(int v) {
    [[assume(v >= 0)]];
    [[assume(v <= 255)]];
    return v;
}

int main() {
    demo::title("C++23 assume");
    DEMO_ASSERT(divide_known_safe(10, 2) == 5);
    DEMO_ASSERT(clamp_to_byte(200) == 200);
    return 0;
}
