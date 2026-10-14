#include "job_system/thread_safe_queue.h"

#include <algorithm>
#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

int main()
{
    ThreadSafeQueue<int> queue;

    constexpr int producerCount = 4;
    constexpr int consumerCount = 4;
    constexpr int jobsPerProducer = 5000;
    constexpr int totalJobs = producerCount * jobsPerProducer;

    std::vector<int> consumed;
    consumed.reserve(totalJobs);

    std::mutex consumedMutex;

    std::vector<std::thread> consumers;

    for (int i = 0; i < consumerCount; ++i)
    {
        consumers.emplace_back(
            [&]()
            {
                while (true)
                {
                    auto value = queue.waitAndPop();

                    if (!value.has_value())
                    {
                        break;
                    }

                    std::lock_guard<std::mutex> lock(consumedMutex);
                    consumed.push_back(value.value());
                }
            }
        );
    }

    std::vector<std::thread> producers;

    for (int producer = 0; producer < producerCount; ++producer)
    {
        producers.emplace_back(
            [&, producer]()
            {
                const int firstId = producer * jobsPerProducer;

                for (int i = 0; i < jobsPerProducer; ++i)
                {
                    const int id = firstId + i;

                    const bool pushed = queue.push(id);
                    assert(pushed);
                }
            }
        );
    }

    for (auto& producer : producers)
    {
        producer.join();
    }

    queue.close();

    for (auto& consumer : consumers)
    {
        consumer.join();
    }

    assert(consumed.size() == totalJobs);

    std::sort(consumed.begin(), consumed.end());

    for (int id = 0; id < totalJobs; ++id)
    {
        assert(consumed[id] == id);
    }

    return 0;
}