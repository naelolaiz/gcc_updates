// gcc-test: std=c++20 min-gcc=14 topic=language experimental=false
// description: C++20 added template parameter lists on lambdas, default-construction, and capture of *this by value.
// reference: https://en.cppreference.com/w/cpp/language/lambda

#include <cassert>
#include <type_traits>
#include <vector>

struct Counter {
    int n = 0;
    auto bump_by_value_self() {
        // Capture *this by VALUE: lambda owns a copy of Counter, safe even if 'this' dies.
        return [*this](int x) mutable {
            n += x;
            return n;
        };
    }
};

int main() {
    // Templated lambda: pin the type of args without auto-deduction quirks.
    auto first_of = []<typename T>(const std::vector<T>& v) -> T {
        return v.front();
    };
    assert(first_of(std::vector<int>{4, 5, 6}) == 4);
    assert(first_of(std::vector<double>{1.5, 2.5}) == 1.5);

    // Stateless lambdas are now default-constructible and assignable.
    auto add = [](int a, int b) { return a + b; };
    decltype(add) add2;          // default ctor - C++20 feature
    add2 = add;                  // copy assign - also C++20
    assert(add2(3, 4) == 7);

    // *this capture survives the parent.
    Counter c{.n = 10};
    auto fn = c.bump_by_value_self();
    c.n = 9999;                  // doesn't affect the captured copy
    assert(fn(5) == 15);
    assert(fn(2) == 17);

    return 0;
}
