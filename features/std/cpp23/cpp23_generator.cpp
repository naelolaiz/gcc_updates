// description: std::generator is the standard library's coroutine generator -- a range-compatible co_yield producer.
// reference: https://en.cppreference.com/w/cpp/coroutine/generator

#include "support/demo.hpp"
#include <cassert>
#include <generator>
#include <ranges>
#include <vector>

std::generator<int> primes_up_to(int n) {
    for (int i = 2; i <= n; ++i) {
        bool prime = true;
        for (int d = 2; d * d <= i; ++d) {
            if (i % d == 0) { prime = false; break; }
        }
        if (prime) co_yield i;
    }
}

std::generator<int> count_from(int start) {
    while (true) co_yield start++;
}

int main() {
    demo::title("C++23 generator");
    auto v = primes_up_to(20)
           | std::ranges::to<std::vector<int>>();
    DEMO_ASSERT((v == std::vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));

    // Infinite generators compose with views::take.
    auto first5 = count_from(100)
                | std::views::take(5)
                | std::ranges::to<std::vector<int>>();
    DEMO_ASSERT((first5 == std::vector<int>{100, 101, 102, 103, 104}));
    return 0;
}
