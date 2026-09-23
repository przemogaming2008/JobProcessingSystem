#include "job_system/worker_pool.h"

int main()
{
    WorkerPool pool(4);

    pool.start();

    for (Job::Id id = 1; id <= 20; ++id)
    {
        pool.submit(
            Job(
                id,
                JobType::CalculateSum,
                JobPriority::Normal,
                100
            )
        );
    }

    pool.stop();

    return 0;
}