cmake_minimum_required(VERSION 3.20)
project(Lab02_Server)

set(CMAKE_CXX_STANDARD 17)

add_library(error STATIC ../include/error/error.cpp)

add_executable(Lab02_Server main.cpp)
target_link_libraries(Lab02_Server ws2_32)
target_link_libraries(Lab02_Server error)

