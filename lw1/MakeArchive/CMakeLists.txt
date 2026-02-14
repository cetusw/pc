cmake_minimum_required(VERSION 4.0)
project(make-archive)

set(CMAKE_CXX_STANDARD 20)

add_executable(make-archive main.cpp
        Process.cpp
        Process.h
        ICompressor.h
        SequentialCompressor.cpp
        SequentialCompressor.h
        FileUtils.h
        ParallelCompressor.cpp
        ParallelCompressor.h
        Archiver.cpp
        Archiver.h
        Application.cpp
        Application.h)
