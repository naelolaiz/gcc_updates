// gcc-test: std=c++11 min-gcc=13 topic=templates experimental=false
// description: 'using' creates type aliases that work as templates -- the modern replacement for 'typedef ... ::type' (e.g. std::add_const_t).
// reference: https://en.cppreference.com/w/cpp/language/type_alias

#include <type_traits>
#include <vector>

// Plain alias: equivalent to 'typedef ... Vec;' but reads left-to-right.
using IntVec = std::vector<int>;

// Alias template (typedef cannot do this).
template <typename T>
using StableMap = std::vector<std::pair<int, T>>;

// Compose with type traits to build _t conveniences (the standard library does
// the same for std::remove_const_t etc.).
template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

int main() {
    IntVec v{1, 2, 3};
    StableMap<double> m;
    m.emplace_back(1, 1.5);

    static_assert(std::is_same<remove_const_t<const int>, int>::value, "");
    return 0;
}
