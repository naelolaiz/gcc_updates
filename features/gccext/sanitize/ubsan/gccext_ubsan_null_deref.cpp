// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=undefined expect-exit=1
// description: UBSan demo: dereferencing a null pointer. Runs only under -fsanitize=undefined; reports the null deref and aborts.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

[[gnu::noinline]] int read_through(int* p) { return *p; }   // UB if p == nullptr

int main() {
    int* p = nullptr;
    return read_through(p);   // UBSan: load of null pointer
}
