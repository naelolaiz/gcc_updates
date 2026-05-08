// description: 'noexcept' is a specifier (promises a function does not throw) and an operator (asks at compile time whether an expression can throw).
// reference: https://en.cppreference.com/w/cpp/language/noexcept_spec

#include "support/demo.hpp"
#include <cassert>

void cannot_throw() noexcept { /* must not throw */ }
void may_throw()             {  throw 1; }

void conditional_noexcept() noexcept(noexcept(cannot_throw())) {}

int main() {
    demo::title("C++11 noexcept");
    // The 'noexcept' operator: a compile-time bool predicate on an expression.
    static_assert( noexcept(cannot_throw()),  "");
    static_assert(!noexcept(may_throw()),     "");
    static_assert( noexcept(conditional_noexcept()), "");

    cannot_throw();   // runtime call -- no actual throw, so OK

    bool caught = false;
    try { may_throw(); } catch (int) { caught = true; }
    DEMO_ASSERT(caught);
    return 0;
}
