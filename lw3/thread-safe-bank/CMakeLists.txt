cmake_minimum_required(VERSION 3.30)
project(thread-safe-bank)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_executable(Bank main.cpp
        Bank.cpp
        Bank.h
        Actor.cpp
        Actor.h
        Actors/Homer.h
        ActorWithBankAccount.cpp
        ActorWithBankAccount.h
        Actors/Homer.cpp
        Actors/Marge.cpp
        Actors/Marge.h
        Actors/Bart.cpp
        Actors/Bart.h
        Actors/Liza.cpp
        Actors/Liza.h
        Actors/Apu.cpp
        Actors/Apu.h
        Actors/MrBurns.cpp
        Actors/MrBurns.h
        Actors/Nelson.cpp
        Actors/Nelson.h
        Actors/Snake.cpp
        Actors/Snake.h
        Actors/Smithers.cpp
        Actors/Smithers.h
        Simulation.cpp
        Simulation.h
)

add_executable(bank-tests
        Test/SimulationTest.cpp
        Actor.cpp
        ActorWithBankAccount.cpp
        Bank.cpp
        Simulation.cpp

        Actors/Homer.cpp
        Actors/Marge.cpp
        Actors/Bart.cpp
        Actors/Liza.cpp
        Actors/Apu.cpp
        Actors/MrBurns.cpp
        Actors/Nelson.cpp
        Actors/Snake.cpp
        Actors/Smithers.cpp
)

target_link_libraries(bank-tests
        PRIVATE
        GTest::GTest
        GTest::Main
)

target_include_directories(bank-tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
