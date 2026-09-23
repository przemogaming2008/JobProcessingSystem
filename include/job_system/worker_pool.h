#pragma once

#include "job_system/job.h"
#include "job_system/thread_safe_queue.h"
#include "job_system/worker.h"

#include <cstddef>
#include <memory>
#include <vector>

class WorkerPool
{
public:
    explicit WorkerPool(std::size_t workerCount);

    void start();
    void submit(Job job);
    void stop();

private:
    ThreadSafeQueue<Job> queue_;
    std::vector<std::unique_ptr<Worker>> workers_;
};