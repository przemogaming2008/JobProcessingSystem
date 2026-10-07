#include "job_system/job_manager.h"

#include <stdexcept>

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

    auto job = std::make_shared<Job>(
        id,
        type,
        priority,
        input
    );

    jobs_.emplace(id, job);

    workerPool_.submit(job);

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

JobStatus JobManager::status(Job::Id id) const
{
    const auto it = jobs_.find(id);

    if (it == jobs_.end())
    {
        throw std::out_of_range("Job not found");
    }

    return it->second->status();
}

std::optional<std::string> JobManager::result(Job::Id id) const
{
    const auto it = jobs_.find(id);

    if (it == jobs_.end())
    {
        throw std::out_of_range("Job not found");
    }

    return it->second->result();
}

std::optional<std::string> JobManager::error(Job::Id id) const
{
    const auto it = jobs_.find(id);

    if (it == jobs_.end())
    {
        throw std::out_of_range("Job not found");
    }

    return it->second->error();
}

bool JobManager::cancel(Job::Id id)
{
    auto it = jobs_.find(id);

    if (it == jobs_.end())
    {
        return false;
    }

    return it->second->requestCancel();
}