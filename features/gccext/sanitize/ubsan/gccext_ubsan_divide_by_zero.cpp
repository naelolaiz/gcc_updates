// description: UndefinedBehaviorSanitizer diagnoses integer division by zero before the machine instruction traps.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// why: UBSan names the language-level fault at its source instead of exposing only a later signal.
// before: Debugging began from a platform-dependent arithmetic trap or unexplained termination.
// pitfall: Volatile keeps the zero runtime-visible; this program intentionally has undefined behavior.

int main() {
    volatile int zero = 0;
    return 42 / zero;
}
