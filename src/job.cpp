#include "job_system/job.h"
#include <utility>

Job::Job(Id id, JobType type, JobPriority priority)
    : id_(id),
      type_(type),
      status_(JobStatus::Queued),
      priority_(priority),
      created_at_(std::chrono::system_clock::now())
{
}

Job::Id Job::id() const
{
    return id_;
}

JobType Job::type() const
{
    return type_;
}

JobStatus Job::status() const
{
    return status_;
}

JobPriority Job::priority() const
{
    return priority_;
}

std::chrono::system_clock::time_point Job::createdAt() const
{
    return created_at_;
}

void Job::setStatus(JobStatus status)
{
    status_ = status;
}

void Job::setResult(std::string result)
{
    result_ = std::move(result);
}

void Job::setError(std::string error)
{
    error_ = std::move(error);
}

const std::optional<std::string>& Job::result() const
{
    return result_;
}

const std::optional<std::string>& Job::error() const
{
    return error_;
}