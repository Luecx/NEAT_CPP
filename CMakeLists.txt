cmake_minimum_required(VERSION 3.17)
project(NEAT)

set(CMAKE_CXX_STANDARD 20)

add_executable(NEAT main.cpp
        src/Gene.cpp
        src/Gene.h
        src/types.h
        src/Genome.cpp
        src/Genome.h
        src/distance.h
        src/config.h
        src/System.h
        src/crossover.h
        src/distance.cpp
        src/crossover.cpp
        src/mutate.h
        src/mutate.cpp
        src/calculator.h
        src/activation.h
        src/activation.cpp)