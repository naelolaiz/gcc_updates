// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::invoke calls anything callable (function, member, member ptr) uniformly; std::apply unpacks a tuple as args.
// reference: https://en.cppreference.com/w/cpp/utility/functional/invoke

#include "support/demo.hpp"
#include <cassert>
#include <functional>
#include <tuple>

int add(int a, int b) { return a + b; }

struct Foo {
    int value = 0;
    int times(int n) const { return value * n; }
};

int main() {
    demo::title("C++17 invoke apply");
    // std::invoke handles a free function ...
    DEMO_ASSERT(std::invoke(add, 3, 4) == 7);

    // ... a member function pointer ...
    Foo f{.value = 10};
    DEMO_ASSERT(std::invoke(&Foo::times, f, 3) == 30);

    // ... a member data pointer (returns a reference) ...
    DEMO_ASSERT(std::invoke(&Foo::value, f) == 10);

    // ... and any plain callable.
    auto lam = [](int x) { return x * x; };
    DEMO_ASSERT(std::invoke(lam, 6) == 36);

    // std::apply: unpack a tuple as the argument list.
    auto t = std::make_tuple(2, 5);
    DEMO_ASSERT(std::apply(add, t) == 7);

    auto sum3 = [](int a, int b, int c) { return a + b + c; };
    DEMO_ASSERT(std::apply(sum3, std::make_tuple(1, 2, 3)) == 6);
    return 0;
}
