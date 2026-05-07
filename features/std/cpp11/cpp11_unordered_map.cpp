// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: std::unordered_map / unordered_set are hash-based containers; O(1) average lookup, O(n) worst case.
// reference: https://en.cppreference.com/w/cpp/container/unordered_map

#include <cassert>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main() {
    std::unordered_map<std::string, int> ages{{"alice", 30}, {"bob", 25}};
    ages["carol"] = 40;

    assert(ages.size() == 3);
    assert(ages["alice"] == 30);
    assert(ages.count("bob") == 1);
    assert(ages.count("dave") == 0);

    auto it = ages.find("carol");
    assert(it != ages.end());
    assert(it->second == 40);

    // Iteration order is unspecified -- don't rely on it.

    std::unordered_set<int> seen{1, 2, 3};
    seen.insert(4);
    seen.insert(2);   // duplicate is a no-op
    assert(seen.size() == 4);
    return 0;
}
