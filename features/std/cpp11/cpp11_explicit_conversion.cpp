// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'explicit operator T()' allows static_cast<T>(obj) but blocks implicit conversion (e.g. accidental bool conversion).
// reference: https://en.cppreference.com/w/cpp/language/cast_operator

#include <cassert>
#include <type_traits>

struct Handle {
    int fd;
    explicit operator bool() const { return fd >= 0; }
    explicit operator int()  const { return fd; }
};

int main() {
    Handle good{3};
    Handle bad{-1};

    // Boolean context (if/while/...) DOES use explicit operator bool.
    if (good) { assert(true); } else { assert(false); }
    if (!bad) { assert(true); } else { assert(false); }

    // Implicit conversion is forbidden, so this is rejected:
    // int x = good;        // would not compile
    int x = static_cast<int>(good);
    assert(x == 3);

    // No accidental sneak through chained conversions.
    static_assert(!std::is_convertible<Handle, int>::value, "");
    return 0;
}
