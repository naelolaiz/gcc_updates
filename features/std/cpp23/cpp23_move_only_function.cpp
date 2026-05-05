// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::move_only_function is std::function but for move-only callables (e.g. unique_ptr captures, std::packaged_task-style ownership).
// reference: https://en.cppreference.com/w/cpp/utility/functional/move_only_function

#include <cassert>
#include <functional>
#include <memory>
#include <string>

int main() {
    // Capture a move-only resource -- std::function would refuse this.
    auto p = std::make_unique<std::string>("payload");
    std::move_only_function<std::string()> fn =
        [p = std::move(p)]() -> std::string {
            return *p;
        };

    assert(fn() == "payload");

    // It IS move-only (cannot copy).
    static_assert(!std::is_copy_constructible_v<decltype(fn)>);
    static_assert(std::is_move_constructible_v<decltype(fn)>);

    auto fn2 = std::move(fn);
    assert(fn2() == "payload");
    return 0;
}
