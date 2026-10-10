#include "job_system/worker.h"

Worker::Worker(
    ThreadSafeQueue<std::shared_ptr<Job>>& queue,
    Logger& logger,
    std::size_t workerId
)
    : queue_(queue),
      logger_(logger),
      workerId_(workerId)
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
        
        logger_.log(
            LogLevel::Info,
            workerId_,
            job.value()->id(),
            "START"
        );

        executor_.execute(*job.value());

        std::string statusText;

        switch (job.value()->status())
        {
            case JobStatus::Completed:
                statusText = "COMPLETED";
                break;

            case JobStatus::Failed:
                statusText = "FAILED";
                break;

            case JobStatus::Cancelled:
                statusText = "CANCELLED";
                break;

            case JobStatus::TimedOut:
                statusText = "TIMED_OUT";
                break;

            case JobStatus::Running:
                statusText = "RUNNING";
                break;

            case JobStatus::Queued:
                statusText = "QUEUED";
                break;
        }

        logger_.log(
            LogLevel::Info,
            workerId_,
            job.value()->id(),
            "status=" + statusText
        );
    }
}
void Worker::join()
{
    if (thread_.joinable())
    {
        thread_.join();
    }
}