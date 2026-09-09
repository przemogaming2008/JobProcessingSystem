#include "job_system/job_executor.h"

#include <string>
#include <chrono>
#include <thread>

void JobExecutor::execute(Job& job)
{
    if (!job.setStatus(JobStatus::Running))
    {
        return;
    }

    switch (job.type())
    {
        case JobType::CalculateSum:
        {
            const auto n = job.input();

            if (n < 0)
            {
                job.setError("Input cannot be negative");
                job.setStatus(JobStatus::Failed);
                return;
            }

            std::int64_t sum = 0;

            for (std::int64_t i = 1; i <= n; ++i)
            {
                sum += i;
            }

            job.setResult(std::to_string(sum));
            job.setStatus(JobStatus::Completed);
            break;
        }
        case JobType::Sleep:
        {
            const auto milliseconds = job.input();

            if (milliseconds < 0)
            {
                job.setError("Sleep duration cannot be negative");
                job.setStatus(JobStatus::Failed);
                return;
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(milliseconds)
            );

            job.setResult("Sleep completed");
            job.setStatus(JobStatus::Completed);
            break;
        }

        case JobType::CountPrimes:
        {
            const auto n = job.input();

            if (n < 0)
            {
                job.setError("Input cannot be negative");
                job.setStatus(JobStatus::Failed);
                return;
            }

            std::int64_t count = 0;

            for (std::int64_t number = 2; number <= n; ++number)
            {
                bool isPrime = true;

                for (std::int64_t divisor = 2;
                    divisor * divisor <= number;
                    ++divisor)
                {
                    if (number % divisor == 0)
                    {
                        isPrime = false;
                        break;
                    }
                }

                if (isPrime)
                {
                    ++count;
                }
            }

            job.setResult(std::to_string(count));
            job.setStatus(JobStatus::Completed);
            break;
        }

        default:
            job.setError("Unsupported job type");
            job.setStatus(JobStatus::Failed);
            break;
    }
}