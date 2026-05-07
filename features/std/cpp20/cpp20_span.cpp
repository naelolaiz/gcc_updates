// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind.
// reference: https://en.cppreference.com/w/cpp/container/span

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
    int c_array[] = {1, 2, 3, 4, 5};
    std::array<int, 4>  std_array{10, 20, 30, 40};
    std::vector<int>    vec{100, 100, 100};

    // Same function takes any contiguous container or raw array.
    assert(sum(c_array)   == 15);
    assert(sum(std_array) == 100);
    assert(sum(vec)       == 300);

    // Subviews with first/last/subspan.
    std::span<int> s(c_array);
    assert(s.size() == 5);
    assert(sum(s.first(2)) == 3);
    assert(sum(s.last(2))  == 9);
    assert(sum(s.subspan(1, 3)) == 9);   // {2,3,4}

    // Static-extent span: size baked into the type.
    std::span<int, 5> static_s(c_array);
    static_assert(static_s.extent == 5);

    return 0;
}
