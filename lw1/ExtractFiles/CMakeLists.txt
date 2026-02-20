cmake_minimum_required(VERSION 4.0)
project(extract-files)

set(CMAKE_CXX_STANDARD 20)

add_executable(extract-files main.cpp
        Extractor/Extractor.cpp
        Decompressor/ParallelDecompressor.cpp
        Decompressor/SequentialDecompressor.cpp
        Process/Process.cpp
        Application.cpp
        Process/Pipe.cpp
        Process/Pipe.h)
