// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'auto' deduces a variable's type from its initializer; 'decltype' deduces the type of an expression without evaluating it.
// reference: https://en.cppreference.com/w/cpp/language/auto

#include <cassert>
#include <map>
#include <type_traits>
#include <vector>

int main() {
    auto i = 42;          // int
    auto d = 3.14;        // double
    auto s = "hi";        // const char*

    static_assert(std::is_same<decltype(i), int>::value, "");
    static_assert(std::is_same<decltype(d), double>::value, "");
    static_assert(std::is_same<decltype(s), const char*>::value, "");

    std::vector<int> v{1, 2, 3};
    auto it = v.begin();  // no more 'std::vector<int>::iterator'

    // decltype on an expression -- doesn't evaluate it.
    int n = 0;
    decltype(n + 1.5) computed = 0;   // double
    static_assert(std::is_same<decltype(computed), double>::value, "");

    // Iterating a map without spelling iterator types.
    std::map<int, int> m{{1, 10}, {2, 20}};
    int sum = 0;
    for (auto p = m.begin(); p != m.end(); ++p) sum += p->second;
    assert(sum == 30);

    (void)it;
    return 0;
}
