#include "job_system/job_manager.h"

#include <cassert>

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

    manager.stop();

    return 0;
}