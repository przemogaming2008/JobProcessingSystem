#include "job_system/job.h"
#include <cassert>
#include <stdexcept>

int main()
{
    Job job(
        1,
        JobType::CalculateSum,
        JobPriority::High,
        100
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
        JobPriority::Normal,
        100
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



    bool exceptionThrown = false;

    try
    {
        Job invalidJob(
            0,
            JobType::Sleep,
            JobPriority::Low,
            100
        );
    }
    catch (const std::invalid_argument&)
    {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    {
        Job job(
            100,
            JobType::CalculateSum,
            JobPriority::Normal,
            10
        );

        assert(job.setStatus(JobStatus::Running));

        assert(job.requestCancel());

        const bool completed = job.complete("55");

        assert(!completed);
        assert(job.status() == JobStatus::Cancelled);
        assert(!job.result().has_value());
    }


    {
        Job job(
            101,
            JobType::CalculateSum,
            JobPriority::Normal,
            10
        );

        assert(job.setStatus(JobStatus::Running));

        assert(job.complete("55"));

        assert(job.status() == JobStatus::Completed);
        assert(job.result().value() == "55");

        assert(!job.requestCancel());
    }

    {
        Job job(
            102,
            JobType::CalculateSum,
            JobPriority::Normal,
            10
        );

        job.setTimeout(std::chrono::milliseconds(0));

        assert(job.setStatus(JobStatus::Running));

        const bool completed = job.complete("55");

        assert(!completed);
        assert(job.status() == JobStatus::TimedOut);
        assert(!job.result().has_value());
    }
    return 0;
}