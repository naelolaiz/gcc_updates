// description: Lambdas: anonymous callables with captures. [=]/[&]/[x]/[&x] cover the four common capture modes.
// reference: https://en.cppreference.com/w/cpp/language/lambda
// why: Small behavior can stay next to the algorithm or callback that consumes it.
// before: Even a one-line operation required a named function object or free function.
// pitfall: Reference captures must not outlive the objects they refer to.

#include "support/demo.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

int main() {
    demo::title("C++11 lambda");
    // Stateless lambda.
    auto add = [](int a, int b) { return a + b; };
    DEMO_ASSERT(add(2, 3) == 5);

    // Capture by value: lambda owns a copy of n.
    int n = 10;
    auto by_val = [n](int x) { return x + n; };
    n = 0;
    DEMO_ASSERT(by_val(5) == 15);   // still uses the captured 10

    // Capture by reference: lambda sees the live variable.
    int counter = 0;
    auto bump = [&counter] { ++counter; };
    bump(); bump(); bump();
    DEMO_ASSERT(counter == 3);

    // mutable: needed when modifying a value-captured copy. (Note: C++14 added
    // init-capture '[sum=0]'; in pure C++11 we capture an existing variable.)
    int running = 0;
    auto cumulative = [running](int x) mutable { running += x; return running; };
    DEMO_ASSERT(cumulative(1) == 1);
    DEMO_ASSERT(cumulative(2) == 3);
    DEMO_ASSERT(cumulative(3) == 6);
    DEMO_ASSERT(running == 0);   // outer 'running' untouched -- captured copy was bumped

    // Lambdas as predicates for STL algorithms.
    std::vector<int> v{1, 2, 3, 4, 5};
    auto count_evens = std::count_if(v.begin(), v.end(),
                                     [](int x) { return x % 2 == 0; });
    DEMO_ASSERT(count_evens == 2);
    return 0;
}
