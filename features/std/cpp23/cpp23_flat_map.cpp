// description: std::flat_map keeps two parallel sorted vectors instead of a tree -- cache-friendly, lookup O(log n), insert O(n).
// reference: https://en.cppreference.com/w/cpp/container/flat_map

#include "support/demo.hpp"
#include <cassert>
#include <flat_map>
#include <string>

int main() {
    demo::title("C++23 flat map");
    std::flat_map<int, std::string> fm;
    fm.insert({3, "three"});
    fm.insert({1, "one"});
    fm.insert({2, "two"});

    // Stored sorted by key.
    auto it = fm.begin();
    DEMO_ASSERT(it->first == 1 && it->second == "one"); ++it;
    DEMO_ASSERT(it->first == 2);                        ++it;
    DEMO_ASSERT(it->first == 3);

    DEMO_ASSERT(fm.size() == 3);
    DEMO_ASSERT(fm.contains(2));
    DEMO_ASSERT(fm[3] == "three");
    fm.erase(2);
    DEMO_ASSERT(!fm.contains(2));

    // Bulk insert from sorted input is much cheaper than one-by-one.
    std::flat_map<int, int> bulk;
    bulk.insert(std::sorted_unique, {{10, 1}, {20, 2}, {30, 3}});
    DEMO_ASSERT(bulk.size() == 3);
    return 0;
}
