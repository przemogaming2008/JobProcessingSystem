#include "job_system/worker_pool.h"
#include <utility>
#include <stdexcept>

WorkerPool::WorkerPool(std::size_t workerCount)
{
    if (workerCount == 0)
    {
        throw std::invalid_argument(
            "WorkerPool must contain at least one worker"
        );
    }

    workers_.reserve(workerCount);

    for (std::size_t i = 0; i < workerCount; ++i)
    {
        workers_.push_back(
            std::make_unique<Worker>(queue_)
        );
    }
}

void WorkerPool::start()
{
    for (auto& worker : workers_)
    {
        worker->start();
    }
}

void WorkerPool::submit(Job job)
{
    queue_.push(std::move(job));
}

void WorkerPool::stop()
{
    queue_.close();

    for (auto& worker : workers_)
    {
        worker->join();
    }
}