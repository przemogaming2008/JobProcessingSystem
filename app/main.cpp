#include "job_system/job_manager.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

std::string statusToString(JobStatus status)
{
    switch (status)
    {
        case JobStatus::Queued:
            return "QUEUED";

        case JobStatus::Running:
            return "RUNNING";

        case JobStatus::Completed:
            return "COMPLETED";

        case JobStatus::Failed:
            return "FAILED";

        case JobStatus::Cancelled:
            return "CANCELLED";

        case JobStatus::TimedOut:
            return "TIMED_OUT";
    }

    return "UNKNOWN";
}

int main()
{
    JobManager manager(4);
    manager.start();

    bool running = true;

    while (running)
    {
        std::cout << "\n=== JobProcessingSystem ===\n";
        std::cout << "1. Submit job\n";
        std::cout << "2. Check status\n";
        std::cout << "3. Get result\n";
        std::cout << "4. Cancel job\n";
        std::cout << "5. Exit\n";
        std::cout << "Choice: ";

        int choice = 0;
        std::cin >> choice;

        switch (choice)
        {
            case 1:
            {
                std::cout << "\nJob type:\n";
                std::cout << "1. CalculateSum\n";
                std::cout << "2. Sleep\n";
                std::cout << "3. CountPrimes\n";
                std::cout << "Choice: ";

                int typeChoice = 0;
                std::cin >> typeChoice;

                JobType type;

                if (typeChoice == 1)
                {
                    type = JobType::CalculateSum;
                }
                else if (typeChoice == 2)
                {
                    type = JobType::Sleep;
                }
                else if (typeChoice == 3)
                {
                    type = JobType::CountPrimes;
                }
                else
                {
                    std::cout << "Invalid job type\n";
                    break;
                }

                std::cout << "\nPriority:\n";
                std::cout << "1. Low\n";
                std::cout << "2. Normal\n";
                std::cout << "3. High\n";
                std::cout << "Choice: ";

                int priorityChoice = 0;
                std::cin >> priorityChoice;

                JobPriority priority;

                if (priorityChoice == 1)
                {
                    priority = JobPriority::Low;
                }
                else if (priorityChoice == 2)
                {
                    priority = JobPriority::Normal;
                }
                else if (priorityChoice == 3)
                {
                    priority = JobPriority::High;
                }
                else
                {
                    std::cout << "Invalid priority\n";
                    break;
                }

                std::cout << "Input value: ";

                std::int64_t input = 0;
                std::cin >> input;

                try
                {
                    const auto id = manager.submit(
                        type,
                        priority,
                        input
                    );

                    std::cout << "Job submitted. ID = "
                              << id << '\n';
                }
                catch (const std::exception& exception)
                {
                    std::cout << "Submit failed: "
                              << exception.what() << '\n';
                }

                break;
            }

            case 2:
            {
                std::cout << "Job ID: ";

                Job::Id id = 0;
                std::cin >> id;

                try
                {
                    const auto status = manager.status(id);

                    std::cout << "Status: "
                              << statusToString(status)
                              << '\n';
                }
                catch (const std::out_of_range&)
                {
                    std::cout << "Job not found\n";
                }

                break;
            }

            case 3:
            {
                std::cout << "Job ID: ";

                Job::Id id = 0;
                std::cin >> id;

                try
                {
                    const auto result = manager.result(id);
                    const auto error = manager.error(id);

                    if (result.has_value())
                    {
                        std::cout << "Result: "
                                  << result.value()
                                  << '\n';
                    }
                    else if (error.has_value())
                    {
                        std::cout << "Error: "
                                  << error.value()
                                  << '\n';
                    }
                    else
                    {
                        std::cout
                            << "Result is not available yet\n";
                    }
                }
                catch (const std::out_of_range&)
                {
                    std::cout << "Job not found\n";
                }

                break;
            }

            case 4:
            {
                std::cout << "Job ID: ";

                Job::Id id = 0;
                std::cin >> id;

                if (manager.cancel(id))
                {
                    std::cout
                        << "Cancellation requested\n";
                }
                else
                {
                    std::cout
                        << "Job cannot be cancelled or does not exist\n";
                }

                break;
            }

            case 5:
            {
                std::cout << "Shutting down...\n";

                running = false;
                break;
            }

            default:
            {
                std::cout << "Invalid choice\n";
                break;
            }
        }
    }

    manager.stop();

    std::cout << "JobProcessingSystem stopped\n";

    return 0;
}