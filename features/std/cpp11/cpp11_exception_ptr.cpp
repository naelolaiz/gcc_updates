// description: std::exception_ptr captures an in-flight exception so it can be transported across threads / async boundaries; std::nested_exception lets a handler chain a new exception while preserving the original.
// reference: https://en.cppreference.com/w/cpp/error/exception_ptr

#include "support/demo.hpp"
#include <exception>
#include <stdexcept>
#include <string>

void rethrow_via_eptr() {
    std::exception_ptr eptr;
    try {
        throw std::runtime_error("boom on producer side");
    } catch (...) {
        eptr = std::current_exception();   // capture, do not unwind
    }
    // ... eptr could be moved across threads, stored in a std::future, etc.
    if (eptr) {
        std::rethrow_exception(eptr);      // re-raise on the consumer side
    }
}

void wrap_with_context() {
    try {
        throw std::runtime_error("inner I/O failure");
    } catch (...) {
        // std::throw_with_nested attaches the current exception to a new one.
        std::throw_with_nested(std::runtime_error("could not open config"));
    }
}

void print_chain(const std::exception& e, int depth = 0) {
    demo::text("level " + std::to_string(depth), e.what());
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& inner) {
        print_chain(inner, depth + 1);
    } catch (...) {}
}

int main() {
    demo::title("C++11 exception_ptr");

    try {
        rethrow_via_eptr();
    } catch (const std::exception& e) {
        demo::text("caught", e.what());
    }

    try {
        wrap_with_context();
    } catch (const std::exception& e) {
        print_chain(e);   // Should print outer then nested inner.
    }
    return 0;
}
