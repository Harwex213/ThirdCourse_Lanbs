cmake_minimum_required(VERSION 3.20)
project(ClientNPt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_library(error STATIC ../include/error/error.cpp)
add_executable(ClientNPt main.cpp)

target_link_libraries(ClientNPt error)
target_link_libraries(ClientNPt ws2_32)