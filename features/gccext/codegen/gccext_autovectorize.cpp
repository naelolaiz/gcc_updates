// description: A loop the auto-vectorizer should turn into SIMD at -O3. We can't easily ASSERT vectorization at runtime; use 'g++ -O3 -fopt-info-vec' (or this build) to confirm.
// reference: https://gcc.gnu.org/projects/tree-ssa/vectorization.html

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>

void axpy(float* __restrict__ y, const float* __restrict__ x, float a, int n) {
    // Classic SAXPY: contiguous, no aliasing (__restrict__), independent iterations
    // -> GCC vectorises this with -O3 (often even at -O2 since GCC 12).
    for (int i = 0; i < n; ++i) y[i] += a * x[i];
}

int main() {
    demo::title("GCC extension autovectorize");
    constexpr int N = 1024;
    alignas(64) float x[N];
    alignas(64) float y[N];
    for (int i = 0; i < N; ++i) { x[i] = static_cast<float>(i); y[i] = 1.0f; }

    axpy(y, x, 2.0f, N);
    bool all_lanes_match = true;
    for (int i = 0; i < N; ++i) {
        all_lanes_match = all_lanes_match
            && y[i] == 1.0f + 2.0f * static_cast<float>(i);
    }
    DEMO_ASSERT(all_lanes_match);
    return 0;
}
