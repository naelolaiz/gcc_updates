// description: std::reference_wrapper<T> = an assignable, copyable handle that *behaves like* a reference. std::ref / std::cref construct it; how you put 'real references' into a std::vector or pass them through std::bind/std::thread without decay-to-copy.
// reference: https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper

#include "support/demo.hpp"
#include <functional>
#include <thread>
#include <vector>

void increment(int& x) { x += 1; }

int main() {
    demo::title("C++11 reference_wrapper");

    int a = 1, b = 2, c = 3;

    // std::vector<int&> is illegal -- references aren't object-like. But
    // std::vector<std::reference_wrapper<int>> works, because the wrapper IS
    // an object.
    std::vector<std::reference_wrapper<int>> refs{a, b, c};
    for (auto& r : refs) {
        r.get() *= 10;     // .get() yields the underlying T&
    }
    DEMO_ASSERT(a == 10 && b == 20 && c == 30);

    // std::thread / std::bind decay arguments to copies by default. Use
    // std::ref to forward as a reference. Without it, the thread would
    // increment a *temporary copy* and the original would stay 10.
    std::thread t(increment, std::ref(a));
    t.join();
    DEMO_ASSERT(a == 11);

    // std::cref is the const-ref counterpart -- useful with bind to capture
    // a large object by reference rather than value-copy it.
    int big = 99;
    auto bound = std::bind([](const int& x) { return x + 1; }, std::cref(big));
    DEMO_ASSERT(bound() == 100);

    // reference_wrapper has an implicit conversion back to T& -- it's
    // pretty much invisible at call sites that take T&.
    int& ref_to_a = refs[0];
    DEMO_ASSERT(&ref_to_a == &a);
    return 0;
}
