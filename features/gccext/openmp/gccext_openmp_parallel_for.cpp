// gcc-test: std=c++17 min-gcc=13 topic=gcc-openmp experimental=false extra-flags=-fopenmp skip-sanitizer=thread
// description: OpenMP #pragma omp parallel for parallelises a loop across threads via libgomp; -fopenmp both compiles and links.
// reference: https://gcc.gnu.org/onlinedocs/libgomp/

#include <cassert>
#include <vector>

int main() {
    constexpr int N = 1000;
    std::vector<int> v(N);

    // Each iteration is independent; OpenMP splits 0..N across worker threads.
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        v[i] = i * 2;
    }
    for (int i = 0; i < N; ++i) assert(v[i] == i * 2);

    // Reduction clause -- safe parallel accumulate.
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; ++i) sum += v[i];
    // sum of (0,2,4,...,1998) = 2 * 999*1000/2 = 999000
    assert(sum == 999000);
    return 0;
}
