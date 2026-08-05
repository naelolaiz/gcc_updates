// description: std::make_unique<T>(args...) completes the C++11 smart-pointer story: no naked 'new', exception-safe as a function argument, plus an array form make_unique<T[]>(n).
// reference: https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique

#include "support/demo.hpp"
#include <memory>
#include <string>
#include <utility>

struct Widget {
    std::string name;
    int size;
    Widget(std::string n, int s) : name(std::move(n)), size(s) {}
};

int main() {
    demo::title("C++14 make_unique");

    // Arguments are forwarded to T's constructor; 'new' never appears.
    // (C++11 shipped make_shared but forgot make_unique.)
    auto w = std::make_unique<Widget>("gear", 3);
    DEMO_ASSERT(w->name == "gear" && w->size == 3);

    // Array form: value-initialises n elements and calls delete[] correctly.
    auto buf = std::make_unique<int[]>(5);
    DEMO_ASSERT(buf[0] == 0 && buf[4] == 0);
    buf[2] = 7;
    DEMO_ASSERT(buf[2] == 7);

    // Ownership still transfers by move, exactly like a raw unique_ptr.
    std::unique_ptr<Widget> owner = std::move(w);
    DEMO_ASSERT(!w && owner->size == 3);
    demo::text("owner->name", owner->name);
    return 0;
}
