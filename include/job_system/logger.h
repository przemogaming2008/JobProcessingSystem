#pragma once

#include "job_system/job.h"

#include <cstddef>
#include <mutex>
#include <ostream>
#include <string>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class Logger
{
public:
    explicit Logger(std::ostream& output);

    void log(
        LogLevel level,
        std::size_t workerId,
        Job::Id jobId,
        const std::string& message
    );

private:
    std::string levelToString(LogLevel level) const;

    std::ostream& output_;
    std::mutex mutex_;
};