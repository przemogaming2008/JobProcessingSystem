#pragma once

#include <cstdint>
#include <chrono>
#include <optional>
#include <string>
#include <mutex>

enum class JobType
{
    Sleep,
    CalculateSum,
    CountPrimes
};

enum class JobStatus
{
    Queued,
    Running,
    Completed,
    Failed,
    Cancelled,
    TimedOut
};

enum class JobPriority
{
    Low,
    Normal,
    High
};

class Job
{
public:
    using Id = std::uint64_t;
    
    Job(
        Id id,
        JobType type,
        JobPriority priority,
        std::int64_t input
    );

    Id id() const;
    JobType type() const;
    JobStatus status() const;
    JobPriority priority() const;
    std::chrono::system_clock::time_point createdAt() const;

    bool setStatus(JobStatus status);
    void setResult(std::string result);
    void setError(std::string error);

    std::optional<std::string> result() const;
    std::optional<std::string> error() const;

    std::int64_t input() const;

    bool requestCancel();
    bool cancelRequested() const;
private:
    Id id_;
    JobType type_;
    JobStatus status_;
    JobPriority priority_;
    std::chrono::system_clock::time_point created_at_;

    std::optional<std::string> result_;
    std::optional<std::string> error_;

    std::int64_t input_;

    mutable std::mutex mutex_;
    bool cancelRequested_ = false;

};