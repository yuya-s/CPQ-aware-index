cmake_minimum_required(VERSION 3.7)
project(construct_wastructuralindex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x -O3")

set(SOURCE_FILES
        graph.cpp
        graph.h
        index.cpp
        index.h
        main.cpp
        utility.h
        workload.cpp
        workload.h
        workloadaware_transequivalent.cpp
        workloadaware_transequivalent.h)

add_executable(construct_wastructuralindex ${SOURCE_FILES})