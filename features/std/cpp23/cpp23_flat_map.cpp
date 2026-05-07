// gcc-test: std=c++23 min-gcc=15 topic=stl experimental=false
// description: std::flat_map keeps two parallel sorted vectors instead of a tree -- cache-friendly, lookup O(log n), insert O(n).
// reference: https://en.cppreference.com/w/cpp/container/flat_map

#include <cassert>
#include <flat_map>
#include <string>

int main() {
    std::flat_map<int, std::string> fm;
    fm.insert({3, "three"});
    fm.insert({1, "one"});
    fm.insert({2, "two"});

    // Stored sorted by key.
    auto it = fm.begin();
    assert(it->first == 1 && it->second == "one"); ++it;
    assert(it->first == 2);                        ++it;
    assert(it->first == 3);

    assert(fm.size() == 3);
    assert(fm.contains(2));
    assert(fm[3] == "three");
    fm.erase(2);
    assert(!fm.contains(2));

    // Bulk insert from sorted input is much cheaper than one-by-one.
    std::flat_map<int, int> bulk;
    bulk.insert(std::sorted_unique, {{10, 1}, {20, 2}, {30, 3}});
    assert(bulk.size() == 3);
    return 0;
}
