// gcc-test: std=c++23 min-gcc=14 topic=language experimental=false
// description: 1uz / 1z literal suffix yields a std::size_t / std::ptrdiff_t directly; no more static_cast<size_t>(0).
// reference: https://en.cppreference.com/w/cpp/language/integer_literal

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <vector>

int main() {
    auto a = 0uz;
    static_assert(std::is_same_v<decltype(a), std::size_t>);

    auto b = 1z;
    static_assert(std::is_same_v<decltype(b), std::ptrdiff_t>);

    std::vector<int> v{10, 20, 30};
    // No -Wsign-compare grumble: same type as v.size().
    for (auto i = 0uz; i < v.size(); ++i) {
        assert(v[i] == static_cast<int>((i + 1) * 10));
    }
    return 0;
}
