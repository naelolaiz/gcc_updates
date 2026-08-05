// description: std::bind_front(f, args...) binds the FIRST arguments of a callable -- the modern replacement for std::bind's placeholder dance, ideal for member-function callbacks.
// reference: https://en.cppreference.com/w/cpp/utility/functional/bind_front

#include "support/demo.hpp"
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

struct Logger {
    std::string prefix;
    std::string format(const std::string& msg) const { return prefix + msg; }
};

int main() {
    demo::title("C++20 bind_front");

    // Bind the leading arguments; the remaining ones stay open.
    auto add = [](int a, int b, int c) { return a + b + c; };
    auto add_100 = std::bind_front(add, 100);
    DEMO_ASSERT(add_100(1, 2) == 103);

    // The everyday win: member function + object in one line -- compare the
    // C++11 spelling std::bind(&Logger::format, log, std::placeholders::_1).
    // bind_front can't silently reorder or drop arguments like bind could.
    Logger log{"[app] "};
    auto fmt = std::bind_front(&Logger::format, log);
    DEMO_ASSERT(fmt("started") == "[app] started");
    demo::text("fmt(\"started\")", fmt("started"));

    // Fits anywhere a predicate is expected.
    std::vector<int> v{1, 5, 3, 9};
    auto less_than = [](int limit, int x) { return x < limit; };
    auto below_4 = std::count_if(v.begin(), v.end(), std::bind_front(less_than, 4));
    DEMO_ASSERT(below_4 == 2);
    return 0;
}
