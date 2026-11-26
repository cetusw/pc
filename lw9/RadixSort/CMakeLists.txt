cmake_minimum_required(VERSION 4.2)
project(RadixSort)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCL REQUIRED)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
)

FetchContent_MakeAvailable(benchmark)

add_library(radix_sort_lib
        OpenClContext.cpp
        OpenClProgram.cpp
        RadixSorter.cpp
)

target_link_libraries(radix_sort_lib
        PUBLIC OpenCL::OpenCL
)

target_include_directories(radix_sort_lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(RadixSort
        main.cpp
)

target_link_libraries(RadixSort
        PRIVATE radix_sort_lib
)

add_executable(radix_sort_tests
        tests/RadixSorterTest.cpp
)

target_link_libraries(radix_sort_tests
        PRIVATE radix_sort_lib
        PRIVATE GTest::gtest_main
)

add_executable(radix_sort_benchmarks
        benchmarks/RadixSorterBenchmark.cpp
        utils/Utils.h
)

target_link_libraries(radix_sort_benchmarks
        PRIVATE radix_sort_lib
        PRIVATE benchmark::benchmark_main
)

target_compile_definitions(radix_sort_lib
        PUBLIC
        CL_HPP_ENABLE_EXCEPTIONS
        CL_TARGET_OPENCL_VERSION=300
        CL_HPP_TARGET_OPENCL_VERSION=300
)

include(GoogleTest)
gtest_discover_tests(radix_sort_tests)