#pragma once

#include "job_system/job.h"

class JobExecutor
{
public:
    void execute(Job& job);
};