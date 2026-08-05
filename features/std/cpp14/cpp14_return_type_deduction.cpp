// description: Function return type deduction: ordinary functions (not just lambdas) can declare 'auto' and let their return statements fix the type; decltype(auto) preserves references exactly.
// reference: https://en.cppreference.com/w/cpp/language/function

#include "support/demo.hpp"
#include <type_traits>
#include <vector>

// Deduced from the return statement -- C++11 needed 'auto f() -> decltype(...)'.
auto midpoint(double a, double b) { return (a + b) / 2; }

// Recursion is fine as long as a non-recursive return is seen first.
auto sum_to(int n) {
    if (n <= 0) return 0;
    return n + sum_to(n - 1);
}

// 'auto' always decays: references and top-level const are stripped.
auto first_by_value(std::vector<int>& v) { return v[0]; }

// 'decltype(auto)' keeps the exact type of the expression, reference and all.
decltype(auto) first_by_ref(std::vector<int>& v) { return (v[0]); }

int main() {
    demo::title("C++14 return type deduction");

    DEMO_ASSERT(midpoint(1.0, 4.0) == 2.5);
    DEMO_ASSERT(sum_to(100) == 5050);

    std::vector<int> v{10, 20, 30};
    static_assert(std::is_same<decltype(first_by_value(v)), int>::value,
                  "auto decays to a value");
    static_assert(std::is_same<decltype(first_by_ref(v)), int&>::value,
                  "decltype(auto) preserves the reference");

    first_by_ref(v) = 99;  // a real reference: assignment writes through
    DEMO_ASSERT(v[0] == 99);
    demo::range("v after write", v);
    return 0;
}
