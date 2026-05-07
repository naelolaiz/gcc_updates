// gcc-test: std=c++20 min-gcc=14 topic=templates experimental=false
// description: C++20 lets CTAD work through alias templates -- no manual deduction guide needed.
// reference: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction

#include <cassert>
#include <memory>
#include <type_traits>

template <typename T, typename Alloc = std::allocator<T>>
class Box {
public:
    Box(T v, Alloc = {}) : value_(v) {}
    const T& get() const { return value_; }
private:
    T value_;
};

// Alias templates that CTAD can deduce through (C++20).
template <typename T>
using IntAllocBox = Box<T, std::allocator<int>>;

template <typename T>
using SimpleBox = Box<T>;

int main() {
    Box b1(42);
    static_assert(std::is_same_v<decltype(b1), Box<int>>);
    assert(b1.get() == 42);

    IntAllocBox b2(100);
    static_assert(std::is_same_v<decltype(b2), Box<int, std::allocator<int>>>);
    assert(b2.get() == 100);

    SimpleBox b3(3.14);
    static_assert(std::is_same_v<decltype(b3), Box<double>>);
    assert(b3.get() == 3.14);

    return 0;
}
