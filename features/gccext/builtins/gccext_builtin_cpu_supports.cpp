// description: __builtin_cpu_supports("feature") tests a CPU capability at runtime (after __builtin_cpu_init) -- the manual dispatch mechanism underneath [[gnu::target_clones]]'s automatic IFUNC dispatch.
// reference: https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html
// note: x86-only through GCC 15; on AArch64 use [[gnu::target_clones]] (see gccext_target_clones_aarch64) or glibc getauxval(AT_HWCAP) for runtime dispatch.

#include "support/demo.hpp"

// x86-only builtin, like the target_clones demo in attributes/.
int main() {
    demo::title("GCC __builtin_cpu_supports");

    __builtin_cpu_init();  // must run once before the queries below

    demo::value("sse2", static_cast<bool>(__builtin_cpu_supports("sse2")));
    demo::value("avx2", static_cast<bool>(__builtin_cpu_supports("avx2")));
    demo::value("avx512f", static_cast<bool>(__builtin_cpu_supports("avx512f")));

    // x86-64 guarantees SSE2, so this always holds there.
    DEMO_ASSERT(__builtin_cpu_supports("sse2"));

    // Typical use: pick an implementation once at startup and cache it.
    const char* impl = __builtin_cpu_supports("avx2") ? "avx2 kernel"
                                                      : "scalar kernel";
    demo::text("dispatch", impl);
    return 0;
}
