cmake_minimum_required(VERSION 4.2)
project(Stop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)

add_library(stop_lib
        StopToken.cpp
        StopToken.h
        StopSource.cpp
        StopSource.h
)

add_executable(stop
        main.cpp
)

target_link_libraries(stop
        PRIVATE
        stop_lib
)

add_executable(stop_tests
        tests/SingleThreadStopTest.cpp
        tests/MultipleThreadsStopTest.cpp
)

target_link_libraries(stop_tests
        PRIVATE
        stop_lib
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(stop_tests)
