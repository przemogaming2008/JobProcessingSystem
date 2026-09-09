#include "job_system/job.h"

#include <cassert>

int main()
{
    Job job(
        1,
        JobType::CalculateSum,
        JobPriority::High
    );

    assert(job.id() == 1);
    assert(job.type() == JobType::CalculateSum);
    assert(job.status() == JobStatus::Queued);
    assert(job.priority() == JobPriority::High);

    job.setStatus(JobStatus::Running);
    assert(job.status() == JobStatus::Running);

    job.setResult("42");
    assert(job.result().has_value());
    assert(job.result().value() == "42");

    job.setStatus(JobStatus::Completed);
    assert(job.status() == JobStatus::Completed);

    Job failedJob(
        2,
        JobType::CountPrimes,
        JobPriority::Normal
    );

    failedJob.setError("Invalid input");
    failedJob.setStatus(JobStatus::Failed);

    assert(failedJob.error().has_value());
    assert(failedJob.error().value() == "Invalid input");
    assert(failedJob.status() == JobStatus::Failed);

    return 0;
}