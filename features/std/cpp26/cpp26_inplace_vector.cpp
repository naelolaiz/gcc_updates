// description: std::inplace_vector is a vector-like contiguous container whose maximum capacity and storage are part of the object, so growth never allocates.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p0843r14.html
// why: It provides vector ergonomics when capacity is bounded and heap allocation is undesirable.
// before: A hand-written size field paired with std::array storage.
// pitfall: Insertion past capacity throws std::bad_alloc rather than growing storage.

#include "support/demo.hpp"
#include <inplace_vector>
#include <numeric>

int main() {
    demo::title("C++26 inplace_vector");
    std::inplace_vector<int, 4> values{1, 2};
    values.push_back(3);
    values.emplace_back(4);

    static_assert(decltype(values)::capacity() == 4);
    DEMO_ASSERT(values.size() == 4);
    DEMO_ASSERT(std::accumulate(values.begin(), values.end(), 0) == 10);
    DEMO_ASSERT(values.data() == &values.front());
    return 0;
}
