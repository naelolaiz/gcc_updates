// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: std::bind partially applies arguments; placeholders _1, _2, ... thread remaining args. std::ref/cref pass references through copies.
// reference: https://en.cppreference.com/w/cpp/utility/functional/bind

#include <cassert>
#include <functional>

int triple_add(int a, int b, int c) { return a + b + c; }

int main() {
    using namespace std::placeholders;

    auto add_5 = std::bind(triple_add, 5, _1, _2);
    assert(add_5(2, 3) == 10);

    auto reorder = std::bind(triple_add, _2, _1, 100);
    assert(reorder(1, 2) == 2 + 1 + 100);

    // std::ref makes bind store a reference (bind defaults to copies).
    int counter = 0;
    auto bump = std::bind([](int& r) { ++r; }, std::ref(counter));
    bump(); bump(); bump();
    assert(counter == 3);

    // Without std::ref, the lambda captures a *copy* of counter.
    int outer = 0;
    auto miss = std::bind([](int& r) { ++r; }, outer);
    miss(); miss();
    assert(outer == 0);   // unchanged
    return 0;
}
