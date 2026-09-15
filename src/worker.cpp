#include "job_system/worker.h"

Worker::Worker(ThreadSafeQueue<Job>& queue)
    : queue_(queue)
{
}

void Worker::start()
{
    thread_ = std::thread(
        [this]()
        {
            run();
        }
    );
}

void Worker::run()
{
    Job job = queue_.waitAndPop();
    executor_.execute(job);
}

void Worker::join()
{
    if (thread_.joinable())
    {
        thread_.join();
    }
}