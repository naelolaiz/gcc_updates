// gcc-test: std=c++20 min-gcc=14 topic=threading experimental=false
// description: std::binary_semaphore = counting_semaphore<1>; classic single-slot signalling primitive.
// reference: https://en.cppreference.com/w/cpp/thread/counting_semaphore

#include <atomic>
#include <cassert>
#include <semaphore>
#include <thread>
#include <vector>

int main() {
    // Producer/consumer ping-pong: 1 producer hands off N values via two
    // binary semaphores. No mutex needed -- the semaphores order the access.
    std::binary_semaphore slot_empty(1);   // 1 = ready to be filled
    std::binary_semaphore slot_full(0);    // 0 = nothing to consume yet

    int slot = -1;
    constexpr int N = 50;
    std::vector<int> received;
    received.reserve(N);

    std::jthread consumer([&] {
        for (int i = 0; i < N; ++i) {
            slot_full.acquire();
            received.push_back(slot);
            slot_empty.release();
        }
    });

    for (int i = 0; i < N; ++i) {
        slot_empty.acquire();
        slot = i * i;
        slot_full.release();
    }

    consumer.join();
    assert(received.size() == N);
    for (int i = 0; i < N; ++i) assert(received[i] == i * i);
    return 0;
}
