cmake_minimum_required(VERSION 3.20)
project(Lab02_Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_library(error STATIC ../include/error/error.cpp)

add_executable(Lab02_Client main.cpp)
target_link_libraries(Lab02_Client ws2_32)
target_link_libraries(Lab02_Client error)
