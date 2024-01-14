cmake_minimum_required(VERSION 3.26)
project(PEA3nowe)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA3nowe main.cpp
        Matrix.cpp
        Matrix.h
        GeneticAlgorithm.cpp
        GeneticAlgorithm.h
)
