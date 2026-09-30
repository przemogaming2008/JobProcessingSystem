#include "job_system/job_manager.h"

#include <cassert>
#include <chrono>
#include <thread>

int main()
{
    JobManager manager(2);

    manager.start();

    const auto id1 = manager.submit(
        JobType::CalculateSum,
        JobPriority::Normal,
        10
    );

    const auto id2 = manager.submit(
        JobType::CountPrimes,
        JobPriority::High,
        100
    );

    assert(id1 == 1);
    assert(id2 == 2);
    assert(id1 != id2);


    const auto sleepId = manager.submit(
        JobType::Sleep,
        JobPriority::Normal,
        300
    );


    bool observedRunning = false;

    for (int i = 0; i < 100; ++i)
    {
        if (manager.status(sleepId) == JobStatus::Running)
        {
            observedRunning = true;
            break;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(5)
        );
    }

    assert(observedRunning);

    manager.stop();

    assert(manager.status(id1) == JobStatus::Completed);
    assert(manager.status(id2) == JobStatus::Completed);

    const auto result1 = manager.result(id1);
    const auto result2 = manager.result(id2);

    assert(result1.has_value());
    assert(result1.value() == "55");

    assert(result2.has_value());

    assert(result2.value() == "25");

    assert(!manager.error(id1).has_value());
    assert(!manager.error(id2).has_value());

    bool resultExceptionThrown = false;

    try
    {
        manager.result(999);
    }
    catch (const std::out_of_range&)
    {
        resultExceptionThrown = true;
    }

    assert(resultExceptionThrown);

    assert(manager.status(id1) == JobStatus::Completed);
    assert(manager.status(id2) == JobStatus::Completed);

    bool exceptionThrown = false;

    try
    {
        manager.status(999);
    }
    catch (const std::out_of_range&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    return 0;
}