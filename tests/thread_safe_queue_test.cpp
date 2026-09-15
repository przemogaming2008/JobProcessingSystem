#include "job_system/thread_safe_queue.h"

#include <cassert>
#include <thread>

int main()
{
    // basic test
    ThreadSafeQueue<int> queue;

    assert(queue.empty());

    queue.push(42);

    assert(!queue.empty());

    int value = queue.waitAndPop();

    assert(value == 42);
    assert(queue.empty());

    // two thread test
    ThreadSafeQueue<int> threadedQueue;

    int result = 0;

    std::thread worker(
        [&]()
        {
            result = threadedQueue.waitAndPop();
        }
    );

    threadedQueue.push(123);

    worker.join();

    assert(result == 123);
    assert(threadedQueue.empty());

    return 0;
}