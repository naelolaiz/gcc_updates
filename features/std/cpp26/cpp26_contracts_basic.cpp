// description: C++26 contracts attach checked preconditions and postconditions to a function; GCC 16 implements the adopted P2900 syntax.
// reference: https://en.cppreference.com/w/cpp/language/contracts
// why: Interface assumptions and guarantees can live beside the function they describe.
// before: Assertions inside the body could not express caller and return-value roles as directly.
// pitfall: Contract semantics depend on evaluation mode; a violation handler must not return normally.

#include "support/demo.hpp"
#include <contracts>
#include <exception>

// P2900 makes the violation handler replaceable. Even a program whose
// contracts all pass must provide it because evaluated checks reference it.
void handle_contract_violation(const std::contracts::contract_violation&) {
    std::terminate();
}

int divide(const int a, const int b)
    pre(b != 0)
    post(r: r * b == a || a % b != 0)
{
    return a / b;
}

int main() {
    demo::title("C++26 contracts basic");
    DEMO_ASSERT(divide(10, 2) == 5);
    DEMO_ASSERT(divide(7, 3) == 2);
    contract_assert(divide(12, 3) == 4);
    return 0;
}
