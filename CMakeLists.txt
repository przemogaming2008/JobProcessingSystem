cmake_minimum_required(VERSION 3.20)

project(JobProcessingSystem
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(job_system
    app/main.cpp
)

enable_testing()

add_executable(smoke_test
    tests/smoke_test.cpp
)

add_test(
    NAME smoke_test
    COMMAND smoke_test
)