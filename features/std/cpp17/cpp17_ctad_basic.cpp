// gcc-test: std=c++17 min-gcc=13 topic=templates experimental=false
// description: Class Template Argument Deduction (CTAD) lets you write 'std::pair p{1, 2.0};' without spelling the types.
// reference: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction

#include <cassert>
#include <map>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
struct Holder {
    T value;
};

// User-defined deduction guide.
template <typename T>
Holder(T) -> Holder<T>;

int main() {
    // Standard library CTAD.
    std::vector v{1, 2, 3};
    static_assert(std::is_same_v<decltype(v), std::vector<int>>);
    assert(v.size() == 3);

    std::pair p{1, std::string("two")};
    static_assert(std::is_same_v<decltype(p), std::pair<int, std::string>>);

    // CTAD on std::lock_guard / std::unique_lock saves repeating the mutex type.
    std::mutex m;
    std::lock_guard lk(m);

    // User-defined CTAD.
    Holder h{3.14};
    static_assert(std::is_same_v<decltype(h), Holder<double>>);
    assert(h.value == 3.14);
    return 0;
}
