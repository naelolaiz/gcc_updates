// gcc-test: std=c++20 min-gcc=14 topic=ranges experimental=false
// description: std::ranges versions of classic algorithms accept full ranges + projections directly.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges

#include "support/demo.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;
};

int main() {
    demo::title("C++20 ranges algorithms");
    std::vector<Person> v{
        {"alice", 30}, {"bob", 25}, {"carol", 40}, {"dan", 22}};

    // Sort by .age via projection -- no custom comparator boilerplate.
    std::ranges::sort(v, std::less{}, &Person::age);
    DEMO_ASSERT(v.front().name == "dan");
    DEMO_ASSERT(v.back().name == "carol");

    // Find with projection: search by .name without lambdas.
    auto it = std::ranges::find(v, std::string("alice"), &Person::name);
    DEMO_ASSERT(it != v.end());
    DEMO_ASSERT(it->age == 30);

    // count_if with projection.
    auto adults = std::ranges::count_if(v, [](int a) { return a >= 25; }, &Person::age);
    DEMO_ASSERT(adults == 3);

    return 0;
}
