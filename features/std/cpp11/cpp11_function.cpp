// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: std::function<R(Args...)> is a polymorphic wrapper for any callable matching the signature; type-erases the actual type.
// reference: https://en.cppreference.com/w/cpp/utility/functional/function

#include <cassert>
#include <functional>

int free_fn(int a, int b) { return a + b; }

struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    std::function<int(int, int)> f;

    f = free_fn;
    assert(f(2, 3) == 5);

    f = [](int a, int b) { return a - b; };
    assert(f(10, 4) == 6);

    std::function<int(int)> g = Multiplier{7};
    assert(g(3) == 21);

    // Empty function -- calling throws std::bad_function_call.
    std::function<void()> empty;
    assert(!empty);
    bool threw = false;
    try { empty(); } catch (const std::bad_function_call&) { threw = true; }
    assert(threw);
    return 0;
}
