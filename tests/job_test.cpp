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

    return 0;
}