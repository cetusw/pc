cmake_minimum_required(VERSION 3.30)
project(Bank)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_executable(Bank main.cpp
        Bank.cpp
        Bank.h
        Actor.cpp
        Actor.h
        actors/Homer.h
        ActorWithBankAccount.cpp
        ActorWithBankAccount.h
        actors/Homer.cpp
        actors/Marge.cpp
        actors/Marge.h
        actors/Bart.cpp
        actors/Bart.h
        actors/Liza.cpp
        actors/Liza.h
        actors/Apu.cpp
        actors/Apu.h
        actors/MrBurns.cpp
        actors/MrBurns.h
        actors/Nelson.cpp
        actors/Nelson.h
        actors/Snake.cpp
        actors/Snake.h
        actors/Smithers.cpp
        actors/Smithers.h
        Simulation.cpp
        Simulation.h
)

add_executable(bank-tests
        test/SimulationTest.cpp
        Actor.cpp
        ActorWithBankAccount.cpp
        Bank.cpp
        Simulation.cpp

        actors/Homer.cpp
        actors/Marge.cpp
        actors/Bart.cpp
        actors/Liza.cpp
        actors/Apu.cpp
        actors/MrBurns.cpp
        actors/Nelson.cpp
        actors/Snake.cpp
        actors/Smithers.cpp
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
