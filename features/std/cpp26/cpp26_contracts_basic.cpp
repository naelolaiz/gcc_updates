// gcc-test: std=c++26 min-gcc=15 topic=language experimental=true extra-flags=-fcontracts expect-error="(expected initializer before .pre.|value parameter used in a postcondition must be const)"
// description: C++26 contracts: pre/post/contract_assert. Implementation status varies; this file is allowed to fail on partial support.
// reference: https://en.cppreference.com/w/cpp/language/contracts

#include "support/demo.hpp"
#include <cassert>

// The exact contracts syntax has shifted across drafts. The form below targets
// the C++26 spelling adopted in 2025; older toolchains may need -fcontracts or
// reject parts of this file -- hence experimental=true.
int divide(int a, int b)
    pre(b != 0)
    post(r: r * b == a || a % b != 0)
{
    return a / b;
}

int main() {
    demo::title("C++26 contracts basic");
    DEMO_ASSERT(divide(10, 2) == 5);
    DEMO_ASSERT(divide(7, 3) == 2);
    return 0;
}
