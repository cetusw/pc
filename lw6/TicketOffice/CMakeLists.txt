cmake_minimum_required(VERSION 4.2)
project(TicketOffice)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)

FetchContent_MakeAvailable(benchmark)

add_library(ticket_office
        lock-free/TicketOffice.cpp
        lock-free/TicketOffice.h
        lock-based/TicketOfficeWithLock.cpp
        lock-based/TicketOfficeWithLock.h
        utils/Rand.h
)

target_include_directories(ticket_office PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(ticket_office_tests
        tests/SingleThreadTicketOfficeTest.cpp
        tests/MultipleThreadsTicketOfficeTest.cpp
)

target_link_libraries(ticket_office_tests
        PRIVATE
        ticket_office
        GTest::gtest
        GTest::gtest_main
)

add_executable(ticket_office_benchmark
        benchmarks/TicketOfficeBenchmark.cpp
)

target_link_libraries(ticket_office_benchmark
        PRIVATE
        ticket_office
        benchmark::benchmark
)

include(GoogleTest)

gtest_discover_tests(ticket_office_tests)

