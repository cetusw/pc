cmake_minimum_required(VERSION 4.0)
project(BoundedBlockingQueue)

set(CMAKE_CXX_STANDARD 20)

add_executable(BoundedBlockingQueue main.cpp
        BoundedBlockingQueue.h
        Application.cpp
        Application.h
        utils/SignalHandler.h
        utils/SignalHandler.cpp
        workers/Customer.cpp
        workers/Customer.h
        workers/Worker.cpp
        workers/Producer.cpp
        workers/Producer.h
        utils/Rand.h)
