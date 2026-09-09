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

    assert(job.setStatus(JobStatus::Running));
    assert(job.status() == JobStatus::Running);

    job.setResult("42");
    assert(job.result().has_value());
    assert(job.result().value() == "42");

    assert(job.setStatus(JobStatus::Completed));
    assert(job.status() == JobStatus::Completed);

    assert(!job.setStatus(JobStatus::Running));
    assert(job.status() == JobStatus::Completed);


    Job failedJob(
        2,
        JobType::CountPrimes,
        JobPriority::Normal
    );

    assert(failedJob.status() == JobStatus::Queued);

    assert(failedJob.setStatus(JobStatus::Running));
    assert(failedJob.status() == JobStatus::Running);

    failedJob.setError("Invalid input");

    assert(failedJob.error().has_value());
    assert(failedJob.error().value() == "Invalid input");

    assert(failedJob.setStatus(JobStatus::Failed));
    assert(failedJob.status() == JobStatus::Failed);

    assert(!failedJob.setStatus(JobStatus::Completed));
    assert(!failedJob.setStatus(JobStatus::Running));
    assert(failedJob.status() == JobStatus::Failed);

    return 0;
}