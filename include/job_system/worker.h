#pragma once

#include "job_system/job.h"
#include "job_system/job_executor.h"
#include "job_system/thread_safe_queue.h"

#include <thread>
#include <memory>

class Worker
{
public:
    explicit Worker(
        ThreadSafeQueue<std::shared_ptr<Job>>& queue
    );

    void start();
    void join();
    
private:
    void run();

    ThreadSafeQueue<std::shared_ptr<Job>>& queue_;
    JobExecutor executor_;
    std::thread thread_;
};