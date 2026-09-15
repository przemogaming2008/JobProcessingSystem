#include "job_system/worker.h"

#include <cassert>

int main()
{
    ThreadSafeQueue<Job> queue;

    Worker worker(queue);

    worker.start();

    queue.push(
        Job(
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