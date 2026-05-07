// gcc-test: std=c++17 min-gcc=13 topic=gcc-codegen experimental=false extra-flags=-O3,-fopt-info-vec=/dev/null
// description: A loop the auto-vectorizer should turn into SIMD at -O3. We can't easily ASSERT vectorization at runtime; use 'g++ -O3 -fopt-info-vec' (or this build) to confirm.
// reference: https://gcc.gnu.org/projects/tree-ssa/vectorization.html

#include <cassert>
#include <cstdint>

void axpy(float* __restrict__ y, const float* __restrict__ x, float a, int n) {
    // Classic SAXPY: contiguous, no aliasing (__restrict__), independent iterations
    // -> GCC vectorises this with -O3 (often even at -O2 since GCC 12).
    for (int i = 0; i < n; ++i) y[i] += a * x[i];
}

int main() {
    constexpr int N = 1024;
    alignas(64) float x[N];
    alignas(64) float y[N];
    for (int i = 0; i < N; ++i) { x[i] = static_cast<float>(i); y[i] = 1.0f; }

    axpy(y, x, 2.0f, N);
    for (int i = 0; i < N; ++i) {
        assert(y[i] == 1.0f + 2.0f * static_cast<float>(i));
    }
    return 0;
}
