// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind.
// reference: https://en.cppreference.com/w/cpp/container/span

#include "support/demo.hpp"
#include <array>
#include <cassert>
#include <numeric>
#include <span>
#include <vector>

int sum(std::span<const int> data) {
    int s = 0;
    for (int v : data) s += v;
    return s;
}

int main() {
    demo::title("C++20 span");
    int c_array[] = {1, 2, 3, 4, 5};
    std::array<int, 4>  std_array{10, 20, 30, 40};
    std::vector<int>    vec{100, 100, 100};

    // Same function takes any contiguous container or raw array.
    DEMO_ASSERT(sum(c_array)   == 15);
    DEMO_ASSERT(sum(std_array) == 100);
    DEMO_ASSERT(sum(vec)       == 300);

    // Subviews with first/last/subspan.
    std::span<int> s(c_array);
    DEMO_ASSERT(s.size() == 5);
    DEMO_ASSERT(sum(s.first(2)) == 3);
    DEMO_ASSERT(sum(s.last(2))  == 9);
    DEMO_ASSERT(sum(s.subspan(1, 3)) == 9);   // {2,3,4}

    // Static-extent span: size baked into the type.
    std::span<int, 5> static_s(c_array);
    static_assert(static_s.extent == 5);

    return 0;
}
