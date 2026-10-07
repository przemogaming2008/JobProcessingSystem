#include "job_system/thread_safe_queue.h"
#include "job_system/job.h"

#include <atomic>
#include <cassert>
#include <thread>
#include <vector>
#include <memory>

int main()
{
    ThreadSafeQueue<int> queue;

    assert(queue.empty());

    queue.push(42);

    assert(!queue.empty());

    auto value = queue.waitAndPop();

    assert(value.has_value());
    assert(value.value() == 42);

    assert(value == 42);
    assert(queue.empty());


    ThreadSafeQueue<int> threadedQueue;

    int result = 0;

    std::thread worker(
        [&]()
        {
            auto value = threadedQueue.waitAndPop();

            assert(value.has_value());
            result = value.value();
        }
    );

    threadedQueue.push(123);

    worker.join();

    assert(result == 123);
    assert(threadedQueue.empty());


    ThreadSafeQueue<int> stressQueue;

    constexpr int producerCount = 4;
    constexpr int consumerCount = 4;
    constexpr int itemsPerProducer = 250;

    constexpr int totalItems =
        producerCount * itemsPerProducer;

    std::atomic<long long> sum = 0;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;


    for (int i = 0; i < consumerCount; ++i)
    {
        consumers.emplace_back(
            [&]()
            {
                for (int j = 0;
                     j < totalItems / consumerCount;
                     ++j)
                {
                    auto value = stressQueue.waitAndPop();

                    assert(value.has_value());
                    sum += value.value();
                }
            }
        );
    }

    for (int producer = 0;
         producer < producerCount;
         ++producer)
    {
        producers.emplace_back(
            [&, producer]()
            {
                const int start =
                    producer * itemsPerProducer + 1;

                const int end =
                    start + itemsPerProducer;

                for (int currentValue = start;
                     currentValue < end;
                     ++currentValue)
                {
                    stressQueue.push(currentValue);
                }
            }
        );
    }

    for (auto& producer : producers)
    {
        producer.join();
    }

    for (auto& consumer : consumers)
    {
        consumer.join();
    }

    assert(stressQueue.empty());

    assert(sum == 500500);


    {
        ThreadSafeQueue<std::shared_ptr<Job>> queue(
            [](const std::shared_ptr<Job>& job)
            {
                if (job->priority() == JobPriority::High)
                {
                    return 2;
                }

                if (job->priority() == JobPriority::Low)
                {
                    return 0;
                }

                return 1;
            }
        );

        queue.push(std::make_shared<Job>(
            1,
            JobType::Sleep,
            JobPriority::Low,
            1
        ));

        queue.push(std::make_shared<Job>(
            2,
            JobType::Sleep,
            JobPriority::High,
            1
        ));

        queue.push(std::make_shared<Job>(
            3,
            JobType::Sleep,
            JobPriority::High,
            1
        ));

        queue.push(std::make_shared<Job>(
            4,
            JobType::Sleep,
            JobPriority::Normal,
            1
        ));

        auto first = queue.waitAndPop();
        auto second = queue.waitAndPop();
        auto third = queue.waitAndPop();
        auto fourth = queue.waitAndPop();

        assert(first.value()->id() == 2);
        assert(second.value()->id() == 3);
        assert(third.value()->id() == 4);
        assert(fourth.value()->id() == 1);
    }
    return 0;
}