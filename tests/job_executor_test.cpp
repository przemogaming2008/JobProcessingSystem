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



    Job completedJob(
    5,
    JobType::CalculateSum,
    JobPriority::Normal,
    5
    );

    executor.execute(completedJob);

    assert(completedJob.status() == JobStatus::Completed);
    assert(completedJob.result().has_value());
    assert(completedJob.result().value() == "15");


    executor.execute(completedJob);

    assert(completedJob.status() == JobStatus::Completed);
    assert(completedJob.result().value() == "15");
    return 0;


    Job invalidSleepJob(
    6,
    JobType::Sleep,
    JobPriority::Low,
    -10
    );

    executor.execute(invalidSleepJob);

    assert(invalidSleepJob.status() == JobStatus::Failed);
    assert(invalidSleepJob.error().has_value());

    Job invalidPrimesJob(
    7,
    JobType::CountPrimes,
    JobPriority::Normal,
    -5
    );

    executor.execute(invalidPrimesJob);

    assert(invalidPrimesJob.status() == JobStatus::Failed);
    assert(invalidPrimesJob.error().has_value());
}