// gcc-test: std=c++17 min-gcc=13 topic=templates experimental=false
// description: Fold expressions reduce parameter packs with a binary operator: (... op pack) and (init op ... op pack).
// reference: https://en.cppreference.com/w/cpp/language/fold

#include <cassert>
#include <string>

template <typename... Args>
constexpr auto sum(Args... args) {
    return (args + ...);                    // unary right fold: a1 + (a2 + (a3 + ...))
}

template <typename... Args>
constexpr auto sum_with_init(Args... args) {
    return (0 + ... + args);                // binary fold with identity 0
}

template <typename... Args>
constexpr bool all_truthy(Args... args) {
    return (... && static_cast<bool>(args));  // unary left fold of &&
}

template <typename... Args>
void push_each(std::string& dest, const Args&... args) {
    (dest += ... += args);                  // binary fold of compound assignment
}

int main() {
    static_assert(sum(1, 2, 3, 4) == 10);
    static_assert(sum_with_init(1, 2, 3) == 6);
    static_assert(sum_with_init() == 0);    // empty pack OK with init form

    static_assert(all_truthy(true, 1, "x"));
    static_assert(!all_truthy(true, 0, "x"));

    std::string out;
    push_each(out, std::string("a"), std::string("b"), std::string("c"));
    assert(out == "abc");
    return 0;
}
