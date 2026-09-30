#include "job_system/job_manager.h"

JobManager::JobManager(std::size_t workerCount)
    : workerPool_(workerCount)
{
}

Job::Id JobManager::submit(
    JobType type,
    JobPriority priority,
    std::int64_t input
)
{
    const Job::Id id = nextId_++;

    workerPool_.submit(
        Job(
            id,
            type,
            priority,
            input
        )
    );

    return id;
}

void JobManager::start()
{
    workerPool_.start();
}

void JobManager::stop()
{
    workerPool_.stop();
}