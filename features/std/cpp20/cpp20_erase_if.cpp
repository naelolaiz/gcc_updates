// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::erase / std::erase_if removes the erase-remove idiom for every standard container.
// reference: https://en.cppreference.com/w/cpp/container/vector/erase2

#include "support/demo.hpp"
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>

int main() {
    demo::title("C++20 erase if");
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    auto removed = std::erase_if(v, [](int x) { return x % 2 == 0; });
    DEMO_ASSERT(removed == 4);
    DEMO_ASSERT((v == std::vector<int>{1, 3, 5, 7}));

    // std::erase by value (not predicate).
    std::vector<int> w{1, 2, 3, 2, 1};
    auto r2 = std::erase(w, 2);
    DEMO_ASSERT(r2 == 2);
    DEMO_ASSERT((w == std::vector<int>{1, 3, 1}));

    // Works on associative containers.
    std::map<std::string, int> m{{"a",1}, {"b",2}, {"c",3}};
    std::erase_if(m, [](auto const& kv) { return kv.second >= 2; });
    DEMO_ASSERT(m.size() == 1);
    DEMO_ASSERT(m.contains("a"));

    std::set<int> s{1,2,3,4,5};
    std::erase_if(s, [](int x) { return x > 2; });
    DEMO_ASSERT(s == std::set<int>({1,2}));

    return 0;
}
