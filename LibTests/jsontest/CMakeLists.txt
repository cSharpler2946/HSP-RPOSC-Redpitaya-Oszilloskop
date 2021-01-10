cmake_minimum_required(VERSION 3.17.3)
project(jsontest)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/)

add_executable(jsontest main.cpp)
