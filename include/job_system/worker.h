#pragma once

#include "job_system/job.h"
#include "job_system/job_executor.h"
#include "job_system/thread_safe_queue.h"
#include "job_system/logger.h"

#include <cstddef>
#include <thread>
#include <memory>

class Worker
{
public:
    Worker(
        ThreadSafeQueue<std::shared_ptr<Job>>& queue,
        Logger& logger,
        std::size_t workerId
    );

    void start();
    void join();
    
private:
    void run();

    ThreadSafeQueue<std::shared_ptr<Job>>& queue_;
    JobExecutor executor_;
    std::thread thread_;

    Logger& logger_;
    std::size_t workerId_;
};