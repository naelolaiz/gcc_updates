// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: '= delete' removes a special member function; '= default' asks the compiler to write the default version.
// reference: https://en.cppreference.com/w/cpp/language/function

#include "support/demo.hpp"
#include <cassert>
#include <utility>

// Non-copyable, default-movable.
struct UniqueResource {
    int* p = nullptr;

    UniqueResource() = default;             // default ctor
    explicit UniqueResource(int v) : p(new int(v)) {}
    ~UniqueResource() { delete p; }

    UniqueResource(const UniqueResource&)            = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    UniqueResource(UniqueResource&& other) noexcept : p(other.p) { other.p = nullptr; }
    UniqueResource& operator=(UniqueResource&& other) noexcept {
        delete p; p = other.p; other.p = nullptr; return *this;
    }
};

int main() {
    demo::title("C++11 delete default");
    UniqueResource a(42);
    UniqueResource b = std::move(a);
    DEMO_ASSERT(b.p && *b.p == 42);
    DEMO_ASSERT(a.p == nullptr);

    // UniqueResource c(b);   // would not compile -- copy is deleted
    return 0;
}
