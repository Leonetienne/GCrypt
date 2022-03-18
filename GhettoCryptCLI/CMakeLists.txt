cmake_minimum_required(VERSION 3.16)
project(gecrypt)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB GhettoCrypt ../GhettoCrypt/*.cpp)

add_executable(gecrypt

        ${GhettoCrypt}

        CommandlineInterface.cpp
        Hazelnupp.cpp
        StringTools.cpp
        main.cpp
)
