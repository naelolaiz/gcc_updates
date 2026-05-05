// gcc-test: std=c++17 min-gcc=13 topic=language experimental=false
// description: C++17 standardised three useful attributes: [[nodiscard]], [[maybe_unused]], [[fallthrough]].
// reference: https://en.cppreference.com/w/cpp/language/attributes

#include <cassert>

[[nodiscard]] int parse_required(int x) { return x * 2; }

[[nodiscard("you almost certainly want to check this status")]]
int do_io() { return 0; }

int classify(int x) {
    [[maybe_unused]] int debug_only_var = x * 2;

    switch (x) {
        case 1:
            // intentional fall-through; without [[fallthrough]] -Wimplicit-fallthrough warns.
            [[fallthrough]];
        case 2:
            return 100;
        default:
            return 0;
    }
}

int main() {
    int v = parse_required(7);   // discarding the result would warn
    assert(v == 14);

    assert(do_io() == 0);

    assert(classify(1) == 100);
    assert(classify(2) == 100);
    assert(classify(3) == 0);
    return 0;
}
