// description: std::scoped_lock<Ms...> is a variadic, deadlock-free RAII lock for any number of mutexes; one declaration replaces the C++11 std::lock + std::lock_guard pair.
// reference: https://en.cppreference.com/w/cpp/thread/scoped_lock

#include "support/demo.hpp"
#include <mutex>
#include <thread>
#include <vector>

struct Account {
    std::mutex m;
    int balance;
};

void transfer(Account& from, Account& to, int amount) {
    // C++11 needed: std::lock(from.m, to.m) and then two std::lock_guard
    // adopts. C++17's scoped_lock does the deadlock-avoidance algorithm AND
    // the RAII unlock in one line.
    std::scoped_lock lk(from.m, to.m);
    from.balance -= amount;
    to.balance   += amount;
}

int main() {
    demo::title("C++17 scoped_lock");

    Account a{{}, 1000}, b{{}, 500};

    // Concurrent transfers in opposite directions: classic deadlock setup
    // with naive lock ordering. scoped_lock makes both threads safe.
    std::vector<std::thread> threads;
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back(transfer, std::ref(a), std::ref(b), 1);
        threads.emplace_back(transfer, std::ref(b), std::ref(a), 1);
    }
    for (auto& t : threads) t.join();

    DEMO_ASSERT(a.balance + b.balance == 1500);  // total invariant preserved
    demo::value("a balance", a.balance);
    demo::value("b balance", b.balance);

    // Single-mutex use: scoped_lock with one arg behaves like lock_guard.
    std::mutex m;
    {
        std::scoped_lock single(m);
        // critical section
    }
    return 0;
}
