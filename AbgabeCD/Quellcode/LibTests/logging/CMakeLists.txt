cmake_minimum_required(VERSION 3.17.3)
project(loggingtest)

set(CMAKE_CXX_STANDARD 14)


include_directories(lib/)
add_subdirectory(lib/loguru)

add_executable(loggingtest main.cpp)
target_link_libraries(loggingtest PRIVATE loguru)