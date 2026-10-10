#include "job_system/statistics.h"

#include <cassert>
#include <chrono>

int main()
{
    Statistics statistics;

    assert(statistics.completed() == 0);
    assert(statistics.failed() == 0);
    assert(statistics.cancelled() == 0);
    assert(statistics.timedOut() == 0);

    assert(statistics.averageWaitTimeMs() == 0.0);
    assert(statistics.averageExecutionTimeMs() == 0.0);

    statistics.recordCompleted(
        std::chrono::milliseconds(10),
        std::chrono::milliseconds(100)
    );

    statistics.recordFailed(
        std::chrono::milliseconds(20),
        std::chrono::milliseconds(200)
    );

    statistics.recordCancelled(
        std::chrono::milliseconds(30),
        std::chrono::milliseconds(300)
    );

    statistics.recordTimedOut(
        std::chrono::milliseconds(40),
        std::chrono::milliseconds(400)
    );

    assert(statistics.completed() == 1);
    assert(statistics.failed() == 1);
    assert(statistics.cancelled() == 1);
    assert(statistics.timedOut() == 1);

    assert(statistics.averageWaitTimeMs() == 25.0);
    assert(statistics.averageExecutionTimeMs() == 250.0);

    return 0;
}