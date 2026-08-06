// description: C++26 classifies an ordinary uninitialized read as erroneous behavior; GCC diagnoses this example and CI requires that diagnostic.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2795r5.html
// why: A compile-time warning exposes an indeterminate read before it becomes runtime corruption.
// before: The same source was usually discussed only as undefined behavior or caught dynamically.
// pitfall: The C++26 wording distinguishes erroneous values from genuinely undefined cases.

int read_uninitialized() {
    int value;
    return value;
}

int main() {
    return read_uninitialized();
}
