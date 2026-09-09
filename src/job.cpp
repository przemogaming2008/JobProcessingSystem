#include "job_system/job.h"
#include <utility>
#include <stdexcept>

Job::Job(Id id, JobType type, JobPriority priority)
    : id_(id),
      type_(type),
      status_(JobStatus::Queued),
      priority_(priority),
      created_at_(std::chrono::system_clock::now())
{
    if (id == 0)
    {
        throw std::invalid_argument("Job ID cannot be zero");
    }
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

bool Job::setStatus(JobStatus newStatus)
{
    bool allowed = false;

    switch (status_)
    {
        case JobStatus::Queued:
            allowed =
                newStatus == JobStatus::Running ||
                newStatus == JobStatus::Cancelled ||
                newStatus == JobStatus::TimedOut;
            break;

        case JobStatus::Running:
            allowed =
                newStatus == JobStatus::Completed ||
                newStatus == JobStatus::Failed ||
                newStatus == JobStatus::Cancelled ||
                newStatus == JobStatus::TimedOut;
            break;

        case JobStatus::Completed:
        case JobStatus::Failed:
        case JobStatus::Cancelled:
        case JobStatus::TimedOut:
            allowed = false;
            break;
    }

    if (!allowed)
    {
        return false;
    }

    status_ = newStatus;
    return true;
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