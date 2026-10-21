#include "job_system/job_manager.h"

#include <cassert>
#include <chrono>
#include <thread>

int main()
{
    //anulowanie przed uruchomieniem
    {
        JobManager manager(1);

        const auto id = manager.submit(
            JobType::Sleep,
            JobPriority::Normal,
            500
        );

        assert(manager.cancel(id));
        assert(manager.status(id) == JobStatus::Cancelled);
    }

    //anulowanie podczas wykonywania
    {
        JobManager manager(1);

        manager.start();

        const auto id = manager.submit(
            JobType::Sleep,
            JobPriority::Normal,
            500
        );

        bool runningObserved = false;

        for (int i = 0; i < 100; ++i)
        {
            if (manager.status(id) == JobStatus::Running)
            {
                runningObserved = true;
                break;
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(5)
            );
        }

        assert(runningObserved);
        assert(manager.cancel(id));

        manager.stop();

        assert(manager.status(id) == JobStatus::Cancelled);
    }

    //timeout
    {
        JobManager manager(1);

        manager.start();

        const auto id = manager.submit(
            JobType::Sleep,
            JobPriority::Normal,
            500,
            std::chrono::milliseconds(50)
        );

        manager.stop();

        assert(manager.status(id) == JobStatus::TimedOut);
        assert(!manager.result(id).has_value());
    }

    return 0;
}