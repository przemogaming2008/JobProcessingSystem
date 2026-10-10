#pragma once

#include <chrono>
#include <cstddef>
#include <mutex>

class Statistics
{
public:
    void recordCompleted(
        std::chrono::milliseconds waitTime,
        std::chrono::milliseconds executionTime
    );

    void recordFailed(
        std::chrono::milliseconds waitTime,
        std::chrono::milliseconds executionTime
    );

    void recordCancelled(
        std::chrono::milliseconds waitTime,
        std::chrono::milliseconds executionTime
    );

    void recordTimedOut(
        std::chrono::milliseconds waitTime,
        std::chrono::milliseconds executionTime
    );

    std::size_t completed() const;
    std::size_t failed() const;
    std::size_t cancelled() const;
    std::size_t timedOut() const;

    double averageWaitTimeMs() const;
    double averageExecutionTimeMs() const;

private:
    void recordTimes(
        std::chrono::milliseconds waitTime,
        std::chrono::milliseconds executionTime
    );

    mutable std::mutex mutex_;

    std::size_t completed_ = 0;
    std::size_t failed_ = 0;
    std::size_t cancelled_ = 0;
    std::size_t timedOut_ = 0;

    std::size_t finishedJobs_ = 0;

    std::chrono::milliseconds totalWaitTime_{0};
    std::chrono::milliseconds totalExecutionTime_{0};
};