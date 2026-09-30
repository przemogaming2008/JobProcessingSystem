#include "job_system/worker.h"

#include <cassert>
#include <memory>

int main()
{
    ThreadSafeQueue<std::shared_ptr<Job>> queue;

    Worker worker(queue);

    worker.start();

    queue.push(
        std::make_shared<Job>(
            1,
            JobType::CalculateSum,
            JobPriority::Normal,
            10
        )
    );

    queue.close();

    worker.join();

    assert(queue.empty());

    return 0;
}