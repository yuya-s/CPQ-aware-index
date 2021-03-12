cmake_minimum_required(VERSION 3.7)
project(construct_cpqx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x -O3")

set(SOURCE_FILES
        graph.cpp
        graph.h
        index.cpp
        index.h
        main.cpp
        kbisimulation.cpp
        kbisimulation.h
        utility.h)

add_executable(construct_cpqx ${SOURCE_FILES})
