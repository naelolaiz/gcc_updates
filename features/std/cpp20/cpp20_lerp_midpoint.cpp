// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::lerp and std::midpoint give numerically-careful linear interpolation and midpoint, including for integers.
// reference: https://en.cppreference.com/w/cpp/numeric/lerp

#include <cassert>
#include <cmath>
#include <cstdint>
#include <numeric>

int main() {
    assert(std::lerp(0.0, 10.0, 0.5) == 5.0);
    assert(std::lerp(2.0, 4.0, 0.25) == 2.5);

    // midpoint for floats: avoids overflow that plagues (a+b)/2.
    assert(std::midpoint(2.0, 4.0) == 3.0);

    // midpoint for integers: also overflow-safe -- (INT_MAX + 0)/2 would be fine,
    // but (INT_MAX + INT_MAX)/2 in plain int is UB; midpoint handles it.
    assert(std::midpoint<int>(INT32_MAX, INT32_MAX - 4) == INT32_MAX - 2);

    // midpoint for pointers: pointer halfway between two array indices.
    int arr[10]{};
    int* mid = std::midpoint(arr + 0, arr + 10);
    assert(mid == arr + 5);
    return 0;
}
