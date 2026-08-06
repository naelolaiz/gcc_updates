// description: AddressSanitizer's use-after-scope instrumentation catches a pointer used after the local object it referred to left scope.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// why: A pointer can remain non-null even though the stack object behind it is already dead.
// before: The dangling access could appear to work until stack layout or optimization changed.
// pitfall: This file deliberately has undefined behavior and is valid only in the ASan lane.

[[gnu::noinline]] int read(const int* pointer) {
    return *pointer;
}

int main() {
    const int* escaped = nullptr;
    {
        int local = 42;
        escaped = &local;
    }
    return read(escaped);
}
