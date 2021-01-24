# File added from myself. Integrates loguru into the CMakeLists framework. Obsolestes call to #include "loguru.cpp"
cmake_minimum_required(VERSION 3.17.3)
project(loguru)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(loguru loguru.cpp)
target_link_libraries(loguru PRIVATE Threads::Threads)
target_link_libraries(loguru PRIVATE dl)