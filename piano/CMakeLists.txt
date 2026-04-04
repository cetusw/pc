cmake_minimum_required(VERSION 4.0)
project(piano)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS window system graphics REQUIRED)

add_executable(piano main.cpp
        SineWaveGenerator.cpp
        SineWaveGenerator.h
        Piano.cpp
        Piano.h
        Player.cpp
        Player.h
        Device.cpp
        Device.h
        KeyboardController.cpp
        KeyboardController.h
        lib/miniaudio.c
        lib/miniaudio.h
)

target_link_libraries(piano
        sfml-window
        sfml-system
        sfml-graphics
        m
        dl
        pthread
)
