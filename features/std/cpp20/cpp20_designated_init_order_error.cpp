// description: C++20 designated initializers must follow declaration order; this negative example asserts that portability rule.
// reference: https://en.cppreference.com/w/cpp/language/aggregate_initialization
// why: Named members improve readability while preserving C++ aggregate initialization order.
// before: Positional braces made it easy to transpose fields with the same type.
// pitfall: Unlike C, standard C++ does not permit designators in arbitrary order.

struct Options {
    int width;
    int height;
};

int main() {
    Options options{.height = 1080, .width = 1920};
    return options.width;
}
