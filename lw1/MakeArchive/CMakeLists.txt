cmake_minimum_required(VERSION 4.0)
project(make-archive)

set(CMAKE_CXX_STANDARD 20)

add_executable(make-archive main.cpp
        Process/Process.cpp
        Process/Process.h
        Compressor/ICompressor.h
        Compressor/SequentialCompressor.cpp
        Compressor/SequentialCompressor.h
        Utils/FileUtils.h
        Compressor/ParallelCompressor.cpp
        Compressor/ParallelCompressor.h
        Archiver/Archiver.cpp
        Archiver/Archiver.h
        Application.cpp
        Application.h)
