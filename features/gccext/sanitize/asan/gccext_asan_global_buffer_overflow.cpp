// description: AddressSanitizer detects an out-of-bounds write past a global array and identifies the global-buffer-overflow class.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// why: Global-array overruns can silently corrupt adjacent process state without instrumentation.
// before: The defect depended on a crash, a debugger, or a difficult manual bounds audit.
// pitfall: This file deliberately has undefined behavior and is valid only in the ASan lane.

int global_values[4]{};

// The CI sanitize lane combines UBSan and ASan; UBSan's array-bounds check
// would report this store first and halt before ASan's global-buffer-overflow
// report (the diagnostic this test asserts) ever appears. Opting this one
// function out of UBSan keeps the catch on the ASan side.
__attribute__((no_sanitize("undefined")))
void overflow_write(int index) { global_values[index] = 42; }

int main(int argc, char**) {
    const int index = argc > 0 ? 4 : 0;
    overflow_write(index);
    return global_values[0];
}
