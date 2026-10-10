#pragma once

#include "job_system/job.h"
#include "job_system/thread_safe_queue.h"
#include "job_system/worker.h"
#include "job_system/logger.h"

#include <cstddef>
#include <memory>
#include <vector>

class WorkerPool
{
public:
    explicit WorkerPool(std::size_t workerCount);

    void start();
    bool submit(std::shared_ptr<Job> job);
    void stop();

private:
    ThreadSafeQueue<std::shared_ptr<Job>> queue_;
    std::vector<std::unique_ptr<Worker>> workers_;
    Logger logger_;
};