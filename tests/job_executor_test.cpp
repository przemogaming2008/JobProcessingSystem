#include "job_system/job_executor.h"

#include <cassert>
#include <chrono>
#include <thread>

int main()
{
    JobExecutor executor;

    Job job(
        1,
        JobType::CalculateSum,
        JobPriority::Normal,
        10
    );

    executor.execute(job);

    assert(job.status() == JobStatus::Completed);
    assert(job.result().has_value());
    assert(job.result().value() == "55");
    assert(!job.error().has_value());

    Job invalidJob(
        2,
        JobType::CalculateSum,
        JobPriority::Normal,
        -5
    );

    executor.execute(invalidJob);

    assert(invalidJob.status() == JobStatus::Failed);
    assert(invalidJob.error().has_value());


    Job primesJob(
    3,
    JobType::CountPrimes,
    JobPriority::Normal,
    10
    );

    executor.execute(primesJob);

    assert(primesJob.status() == JobStatus::Completed);
    assert(primesJob.result().has_value());
    assert(primesJob.result().value() == "4");


    Job sleepJob(
    4,
    JobType::Sleep,
    JobPriority::Low,
    10
    );

    executor.execute(sleepJob);

    assert(sleepJob.status() == JobStatus::Completed);
    assert(sleepJob.result().has_value());
    assert(sleepJob.result().value() == "Sleep completed");
    return 0;
}