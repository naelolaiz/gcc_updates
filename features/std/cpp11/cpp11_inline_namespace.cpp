// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: An 'inline namespace' is transparently visible to its parent -- the standard mechanism for ABI versioning.
// reference: https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces

#include <cassert>

namespace lib {
    inline namespace v2 {
        int api() { return 2; }
    }
    namespace v1 {
        int api() { return 1; }
    }
}

int main() {
    // 'lib::api()' resolves to v2's because v2 is the inline namespace.
    assert(lib::api() == 2);
    // Older callers can still pin to v1 explicitly.
    assert(lib::v1::api() == 1);
    // v2 is also reachable explicitly.
    assert(lib::v2::api() == 2);
    return 0;
}
