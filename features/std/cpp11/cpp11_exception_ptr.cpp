// description: std::exception_ptr captures an in-flight exception so it can be transported across threads / async boundaries; std::nested_exception lets a handler chain a new exception while preserving the original.
// reference: https://en.cppreference.com/w/cpp/error/exception_ptr
// why: An exception thrown in a worker thread or callback can be stored as a value and rethrown on the caller's thread.
// before: Callers mapped exceptions to ad-hoc error codes or lost the original dynamic exception type across boundaries.
// pitfall: std::current_exception() returns a null exception_ptr when called outside an active catch handler.

#include "support/demo.hpp"
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

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

void collect_chain(const std::exception& e, std::vector<std::string>& out, int depth = 0) {
    demo::text("level " + std::to_string(depth), e.what());
    out.emplace_back(e.what());
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& inner) {
        collect_chain(inner, out, depth + 1);
    } catch (...) {}
}

int main() {
    demo::title("C++11 exception_ptr");

    std::string caught_msg;
    try {
        rethrow_via_eptr();
    } catch (const std::exception& e) {
        caught_msg = e.what();
        demo::text("caught", caught_msg);
    }
    DEMO_ASSERT(caught_msg == "boom on producer side");

    std::vector<std::string> chain;
    try {
        wrap_with_context();
    } catch (const std::exception& e) {
        collect_chain(e, chain);   // Should collect outer then nested inner.
    }
    DEMO_ASSERT(chain.size() == 2);
    DEMO_ASSERT(chain[0] == "could not open config");
    DEMO_ASSERT(chain[1] == "inner I/O failure");
    return 0;
}
