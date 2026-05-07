// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: Lambdas: anonymous callables with captures. [=]/[&]/[x]/[&x] cover the four common capture modes.
// reference: https://en.cppreference.com/w/cpp/language/lambda

#include <algorithm>
#include <cassert>
#include <vector>

int main() {
    // Stateless lambda.
    auto add = [](int a, int b) { return a + b; };
    assert(add(2, 3) == 5);

    // Capture by value: lambda owns a copy of n.
    int n = 10;
    auto by_val = [n](int x) { return x + n; };
    n = 0;
    assert(by_val(5) == 15);   // still uses the captured 10

    // Capture by reference: lambda sees the live variable.
    int counter = 0;
    auto bump = [&counter] { ++counter; };
    bump(); bump(); bump();
    assert(counter == 3);

    // mutable: needed when modifying a value-captured copy. (Note: C++14 added
    // init-capture '[sum=0]'; in pure C++11 we capture an existing variable.)
    int running = 0;
    auto cumulative = [running](int x) mutable { running += x; return running; };
    assert(cumulative(1) == 1);
    assert(cumulative(2) == 3);
    assert(cumulative(3) == 6);
    assert(running == 0);   // outer 'running' untouched -- captured copy was bumped

    // Lambdas as predicates for STL algorithms.
    std::vector<int> v{1, 2, 3, 4, 5};
    auto count_evens = std::count_if(v.begin(), v.end(),
                                     [](int x) { return x % 2 == 0; });
    assert(count_evens == 2);
    return 0;
}
