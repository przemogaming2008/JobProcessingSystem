#include "job_system/statistics.h"

void Statistics::recordCompleted(
    std::chrono::milliseconds waitTime,
    std::chrono::milliseconds executionTime
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ++completed_;
    ++finishedJobs_;

    totalWaitTime_ += waitTime;
    totalExecutionTime_ += executionTime;
}

void Statistics::recordFailed(
    std::chrono::milliseconds waitTime,
    std::chrono::milliseconds executionTime
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ++failed_;
    ++finishedJobs_;

    totalWaitTime_ += waitTime;
    totalExecutionTime_ += executionTime;
}

void Statistics::recordCancelled(
    std::chrono::milliseconds waitTime,
    std::chrono::milliseconds executionTime
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ++cancelled_;
    ++finishedJobs_;

    totalWaitTime_ += waitTime;
    totalExecutionTime_ += executionTime;
}

void Statistics::recordTimedOut(
    std::chrono::milliseconds waitTime,
    std::chrono::milliseconds executionTime
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ++timedOut_;
    ++finishedJobs_;

    totalWaitTime_ += waitTime;
    totalExecutionTime_ += executionTime;
}

std::size_t Statistics::completed() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return completed_;
}

std::size_t Statistics::failed() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return failed_;
}

std::size_t Statistics::cancelled() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return cancelled_;
}

std::size_t Statistics::timedOut() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return timedOut_;
}

double Statistics::averageWaitTimeMs() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (finishedJobs_ == 0)
    {
        return 0.0;
    }

    return static_cast<double>(totalWaitTime_.count())
        / finishedJobs_;
}

double Statistics::averageExecutionTimeMs() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (finishedJobs_ == 0)
    {
        return 0.0;
    }

    return static_cast<double>(totalExecutionTime_.count())
        / finishedJobs_;
}