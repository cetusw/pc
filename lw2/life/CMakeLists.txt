cmake_minimum_required(VERSION 4.0)
project(life)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(life main.cpp
        FileDescriptor.cpp
        FileDescriptor.h
        Model/Board.cpp
        Model/Board.h
        StepSynchronizer.cpp
        StepSynchronizer.h
        Visualizer.cpp
        Visualizer.h
)

target_link_libraries(life sfml-graphics sfml-window sfml-system)
