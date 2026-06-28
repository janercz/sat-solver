cmake_minimum_required(VERSION 3.10)

project(SatSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

include_directories(include)

add_executable(solver
    src/main.cpp
    src/Parser.cpp
    src/Solver.cpp
)