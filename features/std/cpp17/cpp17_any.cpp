// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::any is a heterogenous box that holds any copyable type; std::any_cast extracts.
// reference: https://en.cppreference.com/w/cpp/utility/any

#include <any>
#include <cassert>
#include <string>

int main() {
    std::any a = 42;
    assert(a.has_value());
    assert(a.type() == typeid(int));
    assert(std::any_cast<int>(a) == 42);

    a = std::string("hello");
    assert(a.type() == typeid(std::string));
    assert(std::any_cast<std::string>(a) == "hello");

    // any_cast on a wrong type throws.
    bool threw = false;
    try { (void)std::any_cast<int>(a); } catch (const std::bad_any_cast&) { threw = true; }
    assert(threw);

    // Pointer form returns nullptr on mismatch instead of throwing.
    assert(std::any_cast<int>(&a) == nullptr);
    assert(std::any_cast<std::string>(&a) != nullptr);

    a.reset();
    assert(!a.has_value());
    return 0;
}
