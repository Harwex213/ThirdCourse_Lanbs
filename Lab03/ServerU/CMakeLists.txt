cmake_minimum_required(VERSION 3.20)
project(Lab03_Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_library(error STATIC ../include/error/error.cpp)
add_executable(Lab03_Server main.cpp)

target_link_libraries(Lab03_Server error)
target_link_libraries(Lab03_Server ws2_32)