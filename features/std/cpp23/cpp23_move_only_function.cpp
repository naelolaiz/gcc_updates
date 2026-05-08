// description: std::move_only_function is std::function but for move-only callables (e.g. unique_ptr captures, std::packaged_task-style ownership).
// reference: https://en.cppreference.com/w/cpp/utility/functional/move_only_function

#include "support/demo.hpp"
#include <cassert>
#include <functional>
#include <memory>
#include <string>

int main() {
    demo::title("C++23 move only function");
    // Capture a move-only resource -- std::function would refuse this.
    auto p = std::make_unique<std::string>("payload");
    std::move_only_function<std::string()> fn =
        [p = std::move(p)]() -> std::string {
            return *p;
        };

    DEMO_ASSERT(fn() == "payload");

    // It IS move-only (cannot copy).
    static_assert(!std::is_copy_constructible_v<decltype(fn)>);
    static_assert(std::is_move_constructible_v<decltype(fn)>);

    auto fn2 = std::move(fn);
    DEMO_ASSERT(fn2() == "payload");
    return 0;
}
