// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false extra-flags=-ltbb
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
#if defined(__SANITIZE_THREAD__)
    // oneTBB's parallel backend trips TSan inside the runtime; keep this
    // example in the TSan lane by using the standard sequential policy there.
    auto sum_par = std::reduce(std::execution::seq, v.begin(), v.end(), 0);
#else
    auto sum_par = std::reduce(std::execution::par, v.begin(), v.end(), 0);
#endif
    assert(sum_seq == 500500);
    assert(sum_red == 500500);
    assert(sum_par == 500500);

    // transform_reduce: map-reduce in one pass.
#if defined(__SANITIZE_THREAD__)
    auto sum_squares = std::transform_reduce(
        std::execution::seq, v.begin(), v.end(), 0,
        std::plus<>{}, [](int x) { return x * x; });
#else
    auto sum_squares = std::transform_reduce(
        std::execution::par, v.begin(), v.end(), 0,
        std::plus<>{}, [](int x) { return x * x; });
#endif
    assert(sum_squares == 333833500);    // 1^2 + 2^2 + ... + 1000^2

    // exclusive_scan: prefix sums, but the i-th output excludes v[i].
    std::vector<int> small{1, 2, 3, 4, 5};
    std::vector<int> ps(small.size());
    std::exclusive_scan(small.begin(), small.end(), ps.begin(), 0);
    assert((ps == std::vector<int>{0, 1, 3, 6, 10}));
    return 0;
}
