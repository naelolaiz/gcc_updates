// description: std::exchange(obj, new_value) writes the new value and returns the old one -- the one-liner behind move constructors ('data = std::exchange(other.data, nullptr)') and state flips.
// reference: https://en.cppreference.com/w/cpp/utility/exchange

#include "support/demo.hpp"
#include <cstddef>
#include <string>
#include <utility>

// The canonical use: a move constructor / move assignment that leaves the
// source empty without a separate 'other.data = nullptr;' line per member.
struct Buffer {
    int* data = nullptr;
    std::size_t size = 0;

    Buffer() = default;
    explicit Buffer(std::size_t n) : data(new int[n]{}), size(n) {}
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept
        : data(std::exchange(other.data, nullptr)),
          size(std::exchange(other.size, 0)) {}
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = std::exchange(other.data, nullptr);
            size = std::exchange(other.size, 0);
        }
        return *this;
    }
    ~Buffer() { delete[] data; }
};

int main() {
    demo::title("C++14 exchange");

    Buffer a(4);
    Buffer b(std::move(a));
    DEMO_ASSERT(a.data == nullptr && a.size == 0);
    DEMO_ASSERT(b.data != nullptr && b.size == 4);

    // State flip: read the old value and install the new one in one expression.
    std::string state = "idle";
    std::string previous = std::exchange(state, "running");
    DEMO_ASSERT(previous == "idle" && state == "running");
    demo::text("state", state);

    int counter = 41;
    DEMO_ASSERT(std::exchange(counter, 0) == 41 && counter == 0);
    return 0;
}
