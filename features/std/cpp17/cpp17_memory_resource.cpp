// description: <memory_resource> introduces a polymorphic allocator (std::pmr::polymorphic_allocator) backed by a runtime memory_resource pointer; you can swap in a monotonic, pool, or custom resource without changing the container's type.
// reference: https://en.cppreference.com/w/cpp/header/memory_resource

#include "support/demo.hpp"
#include <array>
#include <cstddef>
#include <memory_resource>
#include <string>
#include <vector>

int main() {
    demo::title("C++17 memory_resource (pmr)");

    // monotonic_buffer_resource: ultra-fast bump allocator over a fixed buffer;
    // free() is a no-op, the whole arena is released at scope exit.
    std::array<std::byte, 1024> arena{};
    std::pmr::monotonic_buffer_resource mr(arena.data(), arena.size());

    // pmr containers carry the resource pointer at runtime, not in the type.
    std::pmr::vector<std::pmr::string> strings(&mr);
    strings.emplace_back("alpha");
    strings.emplace_back("bravo");
    strings.emplace_back("charlie with a long enough payload to trigger SBO miss");
    DEMO_ASSERT(strings.size() == 3);
    demo::range("strings", strings);

    // unsynchronized_pool_resource: chains size-class pools on top of an
    // upstream (default = std::pmr::new_delete_resource()). Single-threaded;
    // synchronized_pool_resource has the locking version.
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::vector<int> v(&pool);
    for (int i = 0; i < 100; ++i) v.push_back(i);
    DEMO_ASSERT(v.front() == 0 && v.back() == 99);

    // The three standard upstream resources are observable / swappable.
    DEMO_ASSERT(std::pmr::get_default_resource() != nullptr);
    return 0;
}
