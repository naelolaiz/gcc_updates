// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::invoke calls anything callable (function, member, member ptr) uniformly; std::apply unpacks a tuple as args.
// reference: https://en.cppreference.com/w/cpp/utility/functional/invoke

#include <cassert>
#include <functional>
#include <tuple>

int add(int a, int b) { return a + b; }

struct Foo {
    int value = 0;
    int times(int n) const { return value * n; }
};

int main() {
    // std::invoke handles a free function ...
    assert(std::invoke(add, 3, 4) == 7);

    // ... a member function pointer ...
    Foo f{.value = 10};
    assert(std::invoke(&Foo::times, f, 3) == 30);

    // ... a member data pointer (returns a reference) ...
    assert(std::invoke(&Foo::value, f) == 10);

    // ... and any plain callable.
    auto lam = [](int x) { return x * x; };
    assert(std::invoke(lam, 6) == 36);

    // std::apply: unpack a tuple as the argument list.
    auto t = std::make_tuple(2, 5);
    assert(std::apply(add, t) == 7);

    auto sum3 = [](int a, int b, int c) { return a + b + c; };
    assert(std::apply(sum3, std::make_tuple(1, 2, 3)) == 6);
    return 0;
}
