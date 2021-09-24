cmake_minimum_required(VERSION 3.20)
project(Lab04_Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_library(error STATIC ../include/error/error.cpp)
add_library(connectionCustomizer STATIC connection/connectionCustomizer.cpp)
add_executable(Lab04_Server main.cpp)

target_link_libraries(Lab04_Server error)
target_link_libraries(Lab04_Server connectionCustomizer)
target_link_libraries(Lab04_Server ws2_32)