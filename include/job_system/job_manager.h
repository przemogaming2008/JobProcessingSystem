#pragma once

#include "job_system/job.h"
#include "job_system/worker_pool.h"

#include <cstddef>
#include <cstdint>

class JobManager
{
public:
    explicit JobManager(std::size_t workerCount);

    Job::Id submit(
        JobType type,
        JobPriority priority,
        std::int64_t input
    );

    void start();
    void stop();

private:
    WorkerPool workerPool_;
    Job::Id nextId_ = 1;
};