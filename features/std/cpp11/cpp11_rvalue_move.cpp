// description: Rvalue references && + std::move enable transferring resources without copying. std::forward preserves value category in templates.
// reference: https://en.cppreference.com/w/cpp/utility/move

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

void use(int&)  { /* lvalue path */ }
void use(int&&) { /* rvalue path */ }

// Perfect forwarding: T deduces to int& for lvalues and int for rvalues; the
// std::forward<T>(x) call restores the original value category, then the
// overload set picks the right use() at compile time.
template <typename T>
void perfect_forward(T&& x) {
    use(std::forward<T>(x));
}

int main() {
    demo::title("C++11 rvalue move");
    std::string a = "hello world, this is a longer-than-SSO string";
    std::string b = std::move(a);     // moves a's heap buffer into b
    // a is in a valid-but-unspecified state.
    DEMO_ASSERT(b.size() > 0);
    DEMO_ASSERT(b.find("hello") != std::string::npos);

    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2 = std::move(v1);
    DEMO_ASSERT(v2.size() == 5);

    // Perfect forwarding via universal reference + std::forward.
    int n = 1;
    perfect_forward(n);          // lvalue branch
    perfect_forward(2);          // rvalue branch
    perfect_forward(std::move(n));
    return 0;
}
