// description: allocator_traits::allocate_at_least requests N objects and reports the actual allocation count so containers can exploit allocator-provided extra capacity.
// reference: https://gcc.gnu.org/gcc-16/changes.html
// why: An allocator may cheaply provide a larger block that a container can use as capacity.
// before: allocate(N) communicated only the requested size, hiding usable extra storage.
// pitfall: Deallocation must use the returned count, not merely the requested count.

#include "support/demo.hpp"
#include <memory>

int main() {
    demo::title("C++23 allocate_at_least");
    std::allocator<int> allocator;
    auto allocation = std::allocator_traits<decltype(allocator)>::allocate_at_least(
        allocator, 8);
    DEMO_ASSERT(allocation.ptr != nullptr);
    DEMO_ASSERT(allocation.count >= 8);
    std::allocator_traits<decltype(allocator)>::deallocate(
        allocator, allocation.ptr, allocation.count);
    return 0;
}
