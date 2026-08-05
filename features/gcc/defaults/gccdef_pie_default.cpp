// description: Position-independent executables are a TOOLCHAIN default, not a language one: upstream gcc:N builds default to non-PIE while distro packages (like the Debian g++-16 CI uses) enable -fPIE by default -- __PIE__/__PIC__ tell you which world you're in.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html

#include "support/demo.hpp"

int main() {
    demo::title("GCC PIE/PIC defaults are packaging-dependent");

#ifdef __PIE__
    demo::value("__PIE__", static_cast<int>(__PIE__));
    constexpr bool pie = true;
#else
    demo::text("__PIE__", "(not defined -- non-PIE toolchain default)");
    constexpr bool pie = false;
#endif
#ifdef __PIC__
    demo::value("__PIC__", static_cast<int>(__PIC__));
    constexpr bool pic = true;
#else
    demo::text("__PIC__", "(not defined)");
    constexpr bool pic = false;
#endif

    // PIE implies PIC; the reverse doesn't hold. This is the only invariant
    // that survives packaging differences -- the raw values differ between
    // the official gcc:N images (non-PIE) and distro compilers (PIE).
    DEMO_ASSERT(!pie || pic);
    demo::text("note", "same source, same flags -- the toolchain build decides");
    return 0;
}
