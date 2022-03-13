cmake_minimum_required(VERSION 3.16)
project(GhettoCryptCLI)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB GhettoCrypt ../GhettoCrypt/*.cpp)

add_executable(GhettoCryptCLI
        
        ${GhettoCrypt}

        CommandLineInterface.cpp
        Hazelnupp.cpp
        StringTools.cpp
        main.cpp
)
