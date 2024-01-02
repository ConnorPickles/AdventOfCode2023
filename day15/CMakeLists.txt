cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)
set(CMAKE_CXX_STANDARD 17)
set(CAMKE_CXX_STANDARD_REQUIRED True)

add_executable(aoc src/aoc.cpp ../shared/input.hpp ../shared/printing.hpp)
set_target_properties(aoc PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
