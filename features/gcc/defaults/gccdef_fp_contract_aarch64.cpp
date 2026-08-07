// description: On AArch64 the default -ffp-contract=fast needs no extra flags at all: FMA is in the base ISA, so a*b + c compiles to one fused fmadd at -O2 and skips the intermediate rounding -- same default, no -mfma, no CPU probe.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html

#include "support/demo.hpp"
#include <cmath>

// AArch64 twin of gccdef_fp_contract: fmadd is baseline, so unlike x86 no
// -mfma build flag and no __builtin_cpu_supports runtime check are needed.
double mul_add(double a, double b, double c) { return a * b + c; }

int main() {
    demo::title("GCC default -ffp-contract=fast (AArch64)");

    // AArch64 evaluates float/double at their own precision, like x86-64 SSE.
    static_assert(__FLT_EVAL_METHOD__ == 0, "");

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
