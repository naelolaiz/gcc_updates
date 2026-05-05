// gcc-test: std=c++23 min-gcc=14 topic=language experimental=false
// description: auto(x) and auto{x} produce a decayed prvalue copy of x -- handy for materialising a copy in generic code.
// reference: https://en.cppreference.com/w/cpp/language/auto

#include <cassert>
#include <type_traits>

template <typename T>
auto first_value(T&& v) {
    // Used to require std::decay_t / std::remove_cvref. auto(x) does it inline.
    return auto(v);
}

int main() {
    int n = 7;
    int& nr = n;
    auto a = auto(nr);              // decayed prvalue copy of nr -> int
    static_assert(std::is_same_v<decltype(a), int>);
    assert(a == 7);

    const int cn = 42;
    auto b = auto{cn};              // strips const, makes prvalue
    static_assert(std::is_same_v<decltype(b), int>);
    assert(b == 42);

    // Modify the copy without affecting the source.
    int original = 5;
    auto copy = auto(original);
    copy += 1;
    assert(original == 5);
    assert(copy == 6);

    auto fv = first_value(n);
    static_assert(std::is_same_v<decltype(fv), int>);
    assert(fv == 7);
    return 0;
}
