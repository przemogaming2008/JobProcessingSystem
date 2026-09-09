cmake_minimum_required(VERSION 3.20)

project(JobProcessingSystem
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(job_system_lib
    src/job.cpp
    src/job_executor.cpp
)

target_include_directories(job_system_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(job_system
    app/main.cpp
)

target_link_libraries(job_system
    PRIVATE
        job_system_lib
)

enable_testing()

add_executable(smoke_test
    tests/smoke_test.cpp
)

add_test(
    NAME smoke_test
    COMMAND smoke_test
)

add_executable(job_test
    tests/job_test.cpp
)

target_link_libraries(job_test
    PRIVATE
        job_system_lib
)

add_test(
    NAME job_test
    COMMAND job_test
)

add_executable(job_executor_test
    tests/job_executor_test.cpp
)

target_link_libraries(job_executor_test
    PRIVATE
        job_system_lib
)

add_test(
    NAME job_executor_test
    COMMAND job_executor_test
)