// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=true extra-flags=-ltbb
// description: <numeric> gained reduce/transform_reduce/inclusive_scan/exclusive_scan; <execution> adds parallel policies.
// reference: https://en.cppreference.com/w/cpp/algorithm/reduce
// note: Parallel STL on libstdc++ uses TBB at link time -- hence -ltbb.

#include <algorithm>
#include <cassert>
#include <execution>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 1);   // 1..1000

    // std::accumulate is sequential, left-fold; std::reduce can re-associate (and parallelise).
    auto sum_seq = std::accumulate(v.begin(), v.end(), 0);
    auto sum_red = std::reduce(v.begin(), v.end(), 0);
    auto sum_par = std::reduce(std::execution::par, v.begin(), v.end(), 0);
    assert(sum_seq == 500500);
    assert(sum_red == 500500);
    assert(sum_par == 500500);

    // transform_reduce: map-reduce in one pass.
    auto sum_squares = std::transform_reduce(
        std::execution::par, v.begin(), v.end(), 0,
        std::plus<>{}, [](int x) { return x * x; });
    assert(sum_squares == 333833500);    // 1^2 + 2^2 + ... + 1000^2

    // exclusive_scan: prefix sums, but the i-th output excludes v[i].
    std::vector<int> ps(5);
    std::exclusive_scan(std::vector<int>{1, 2, 3, 4, 5}.begin(),
                        std::vector<int>{1, 2, 3, 4, 5}.end(),
                        ps.begin(), 0);
    assert((ps == std::vector<int>{0, 1, 3, 6, 10}));
    return 0;
}
