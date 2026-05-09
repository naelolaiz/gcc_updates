// description: emplace_back / emplace forward arguments and construct the element in-place inside the container, avoiding the temporary that push_back requires. std::piecewise_construct lets std::pair / std::map values be constructed in two halves separately.
// reference: https://en.cppreference.com/w/cpp/container/vector/emplace_back

#include "support/demo.hpp"
#include <map>
#include <string>
#include <utility>
#include <vector>

struct Counter {
    static int copies;
    static int moves;
    static int direct;
    int x, y;

    Counter(int a, int b) : x(a), y(b) { ++direct; }
    Counter(const Counter& o) : x(o.x), y(o.y) { ++copies; }
    Counter(Counter&& o) noexcept : x(o.x), y(o.y) { ++moves; }
};
int Counter::copies = 0;
int Counter::moves  = 0;
int Counter::direct = 0;

int main() {
    demo::title("C++11 emplace");

    std::vector<Counter> v;
    v.reserve(4);              // remove reallocation noise from the count
    v.push_back(Counter{1, 2});  // builds a temp, then moves it in: 1 direct + 1 move
    v.emplace_back(3, 4);        // forwards args -> direct construction in slot

    demo::value("direct constructed", Counter::direct);
    demo::value("copy constructed",   Counter::copies);
    demo::value("move constructed",   Counter::moves);
    DEMO_ASSERT(v.size() == 2);

    // For std::map<K, V>, emplace constructs the std::pair in place. To
    // forward args separately to the key and the value, use piecewise_construct.
    std::map<std::string, Counter> m;
    m.emplace(std::piecewise_construct,
              std::forward_as_tuple("origin"),
              std::forward_as_tuple(0, 0));
    DEMO_ASSERT(m.at("origin").x == 0);
    return 0;
}
