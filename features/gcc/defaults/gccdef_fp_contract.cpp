// description: -ffp-contract=fast is GCC's default in every release (13-16) and every dialect, even strict -std=c++NN: a*b+c may compile to one fused multiply-add, so results change when -march gains FMA -- not because you asked for fast-math.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html

#include "support/demo.hpp"
#include <cmath>

// Compiled with -mfma (see CMakeLists.txt): under the DEFAULT
// -ffp-contract=fast GCC folds a*b + c into a single fused multiply-add,
// skipping the intermediate rounding of a*b.
double mul_add(double a, double b, double c) { return a * b + c; }

int main() {
    demo::title("GCC default -ffp-contract=fast");

    // x86-64 uses SSE math: no x87 excess precision by default either.
    static_assert(__FLT_EVAL_METHOD__ == 0, "");

    if (!__builtin_cpu_supports("fma")) {
        demo::text("skip", "CPU lacks FMA; the contracted code path can't run");
        return 0;
    }

    // Chosen so fused and unfused answers differ: a*b rounds to exactly 1.0
    // in double, so the two-rounding result of (a*b) + c is 0, while a single
    // fused rounding keeps the 2^-54 term. volatile keeps the values out of
    // the compiler's reach -- constant folding always uses two roundings and
    // would hide the contraction entirely.
    volatile double va = 1.0 + 0x1p-27;
    volatile double vb = 1.0 - 0x1p-27;
    volatile double vc = -1.0;
    const double a = va, b = vb, c = vc;

    const double contracted = mul_add(a, b, c);
    const double fused = std::fma(a, b, c);
    demo::value("a*b + c", contracted);
    demo::value("std::fma(a, b, c)", fused);

    DEMO_ASSERT(fused == -0x1p-54);    // the single-rounding answer
    DEMO_ASSERT(contracted == fused);  // the default contraction kicked in

    // When bit-reproducible two-rounding results matter, build with
    // -ffp-contract=off -- or write std::fma explicitly where you WANT fusing.
    return 0;
}
