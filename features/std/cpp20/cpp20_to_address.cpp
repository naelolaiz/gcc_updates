// gcc-test: std=c++20 min-gcc=13 topic=stl experimental=false
// description: std::to_address turns any 'fancy pointer' (smart, iterator) into a raw pointer without dereferencing.
// reference: https://en.cppreference.com/w/cpp/memory/to_address

#include <cassert>
#include <memory>
#include <vector>

int main() {
    int x = 42;
    int* raw = &x;
    assert(std::to_address(raw) == &x);

    auto sp = std::make_shared<int>(7);
    int* p1 = std::to_address(sp);
    assert(*p1 == 7);

    auto up = std::make_unique<int>(99);
    int* p2 = std::to_address(up);
    assert(*p2 == 99);

    // Works on iterators -- no need to dereference (handles end-of-range safely).
    std::vector<int> v{1, 2, 3, 4};
    auto it = v.begin() + 2;
    int* it_raw = std::to_address(it);
    assert(*it_raw == 3);
    return 0;
}
