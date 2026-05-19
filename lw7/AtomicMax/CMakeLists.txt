cmake_minimum_required(VERSION 4.2)
project(AtomicMax)

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

add_library(atomic-max
        INTERFACE
)
target_include_directories(atomic-max
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(atomic-max-tests
        tests/SingleThreadAtomicMaxTest.cpp
        tests/MultiThreadsAtomicMaxTest.cpp
)
target_link_libraries(atomic-max-tests
        PRIVATE
        atomic-max
        GTest::gtest_main
)

add_executable(atomic-max-benchmarks
        benchmarks/AtomicMaxBenchmark.cpp
)
target_link_libraries(atomic-max-benchmarks
        PRIVATE
        atomic-max
        benchmark::benchmark
)

include(GoogleTest)
gtest_discover_tests(atomic-max-tests)
