// description: 'auto' deduces a variable's type from its initializer; 'decltype' deduces the type of an expression without evaluating it.
// reference: https://en.cppreference.com/w/cpp/language/auto
// why: Code can express the value it wants without repeating long or implementation-specific types.
// before: Iterator and expression-result types had to be spelled explicitly.
// pitfall: auto follows template deduction rules, so top-level const and references may be dropped.

#include "support/demo.hpp"
#include <cassert>
#include <map>
#include <type_traits>
#include <vector>

int main() {
    demo::title("C++11 auto");
    auto i = 42;          // int
    auto d = 3.14;        // double
    auto s = "hi";        // const char*

    static_assert(std::is_same<decltype(i), int>::value, "");
    static_assert(std::is_same<decltype(d), double>::value, "");
    static_assert(std::is_same<decltype(s), const char*>::value, "");

    std::vector<int> v{1, 2, 3};
    auto it = v.begin();  // no more 'std::vector<int>::iterator'

    // decltype on an expression -- doesn't evaluate it.
    int n = 0;
    decltype(n + 1.5) computed = 0;   // double
    static_assert(std::is_same<decltype(computed), double>::value, "");

    // Iterating a map without spelling iterator types.
    std::map<int, int> m{{1, 10}, {2, 20}};
    int sum = 0;
    for (auto p = m.begin(); p != m.end(); ++p) sum += p->second;
    DEMO_ASSERT(sum == 30);

    (void)it;
    return 0;
}
