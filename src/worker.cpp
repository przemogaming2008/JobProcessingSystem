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
    while (true)
    {
        auto job = queue_.waitAndPop();

        if (!job.has_value())
        {
            break;
        }

        executor_.execute(job.value());
    }
}
void Worker::join()
{
    if (thread_.joinable())
    {
        thread_.join();
    }
}