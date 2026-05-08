// description: std::lerp and std::midpoint give numerically-careful linear interpolation and midpoint, including for integers.
// reference: https://en.cppreference.com/w/cpp/numeric/lerp

#include "support/demo.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <numeric>

int main() {
    demo::title("C++20 lerp midpoint");
    DEMO_ASSERT(std::lerp(0.0, 10.0, 0.5) == 5.0);
    DEMO_ASSERT(std::lerp(2.0, 4.0, 0.25) == 2.5);

    // midpoint for floats: avoids overflow that plagues (a+b)/2.
    DEMO_ASSERT(std::midpoint(2.0, 4.0) == 3.0);

    // midpoint for integers: also overflow-safe -- (INT_MAX + 0)/2 would be fine,
    // but (INT_MAX + INT_MAX)/2 in plain int is UB; midpoint handles it.
    DEMO_ASSERT(std::midpoint<int>(INT32_MAX, INT32_MAX - 4) == INT32_MAX - 2);

    // midpoint for pointers: pointer halfway between two array indices.
    int arr[10]{};
    int* mid = std::midpoint(arr + 0, arr + 10);
    DEMO_ASSERT(mid == arr + 5);
    return 0;
}
