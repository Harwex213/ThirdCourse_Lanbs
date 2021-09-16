cmake_minimum_required(VERSION 3.20)
project(Lab02_Client)

set(CMAKE_CXX_STANDARD 17)

add_library(error STATIC ../include/error/error.cpp)

add_executable(Lab02_Client main.cpp)
target_link_libraries(Lab02_Client ws2_32)
target_link_libraries(Lab02_Client error)
