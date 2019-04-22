cmake_minimum_required(VERSION 3.10)
project(hua_to_modify)

set(CMAKE_CXX_STANDARD 11)

add_executable(hua_to_modify main.cpp problem_spec.cpp problem_spec.h reading_utilities.cpp reading_utilities.h split_option_finder.cpp split_option_finder.h reach_table.cpp reach_table.h convertor.cpp convertor.h solution.h io.h util.h util.cpp)