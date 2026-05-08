// description: std::hash<T> is the customisation point that makes T usable as a key in unordered_map / unordered_set. Specialising it teaches the standard hash containers about your type.
// reference: https://en.cppreference.com/w/cpp/utility/hash

#include "support/demo.hpp"
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

namespace std {
template <>
struct hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        // boost::hash_combine-style mix; not crypto, just diffusion.
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};
}  // namespace std

int main() {
    demo::title("C++11 std::hash");

    // The library already specialises std::hash for built-ins and std::string.
    demo::value("hash<int>(42) is some size_t",
                std::hash<int>{}(42) != 0);
    demo::value("hash<string> is stable within a process",
                std::hash<std::string>{}("abc") == std::hash<std::string>{}("abc"));

    // With our specialisation in scope, unordered_set<Point> compiles and
    // gives O(1) average lookup.
    std::unordered_set<Point> seen;
    seen.insert({1, 2});
    seen.insert({1, 2});       // same key -> no growth
    seen.insert({3, 4});
    DEMO_ASSERT(seen.size() == 2);
    DEMO_ASSERT(seen.count({1, 2}) == 1);
    DEMO_ASSERT(seen.count({9, 9}) == 0);
    return 0;
}
