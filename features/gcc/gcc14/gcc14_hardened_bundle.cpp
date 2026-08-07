// description: GCC 14 introduced -fhardened as an umbrella for production hardening flags; this test proves that stack protection and fortified libc calls are enabled.
// reference: https://gcc.gnu.org/gcc-14/changes.html
// why: Production builds need a maintained hardening baseline without copying a fragile list of flags.
// before: Build systems selected stack protection and _FORTIFY_SOURCE individually.
// pitfall: The bundle is target- and release-dependent, so inspect the enabled flags for a pinned compiler.

#include "support/demo.hpp"

#ifndef __SSP_STRONG__
#error "-fhardened did not enable strong stack protection"
#endif

#if !defined(_FORTIFY_SOURCE) || _FORTIFY_SOURCE < 2
#error "-fhardened did not enable _FORTIFY_SOURCE"
#endif

int main() {
    demo::title("GCC 14 hardened bundle");
    static_assert(__SSP_STRONG__ == 3);
    DEMO_ASSERT(_FORTIFY_SOURCE >= 2);
    return 0;
}
