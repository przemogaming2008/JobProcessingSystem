#include "job_system/worker_pool.h"

#include <cassert>
#include <chrono>
#include <stdexcept>

int main()
{
    WorkerPool pool(4);

    pool.start();

    const auto start =
        std::chrono::steady_clock::now();

    for (Job::Id id = 1; id <= 4; ++id)
    {
        pool.submit(
            Job(
                id,
                JobType::Sleep,
                JobPriority::Normal,
                200
            )
        );
    }

    pool.stop();

    const auto end =
        std::chrono::steady_clock::now();

    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start
        );

    assert(duration.count() < 600);


    bool exceptionThrown = false;

    try
    {
        WorkerPool invalidPool(0);
    }
    catch (const std::invalid_argument&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    
    return 0;
}