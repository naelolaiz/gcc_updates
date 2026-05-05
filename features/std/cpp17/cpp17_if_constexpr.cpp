// gcc-test: std=c++17 min-gcc=13 topic=templates experimental=false
// description: 'if constexpr' discards the unselected branch at compile time, enabling clean template specialization.
// reference: https://en.cppreference.com/w/cpp/language/if#Constexpr_If

#include <cassert>
#include <string>
#include <type_traits>

template <typename T>
auto describe(T x) {
    if constexpr (std::is_integral_v<T>) {
        return x * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        return x + 0.5;
    } else if constexpr (std::is_same_v<T, std::string>) {
        return x + "!";
    } else {
        return x;
    }
}

int main() {
    auto a = describe(10);            // integral path -> 20
    auto b = describe(2.0);           // floating path -> 2.5
    auto c = describe(std::string("hi"));  // string path -> "hi!"

    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), double>);
    static_assert(std::is_same_v<decltype(c), std::string>);

    assert(a == 20);
    assert(b == 2.5);
    assert(c == "hi!");
    return 0;
}
