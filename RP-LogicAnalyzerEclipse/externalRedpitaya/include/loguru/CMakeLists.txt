# File added from myself. Integrates loguru into the CMakeLists framework. Obsolestes call to #include "loguru.cpp"
cmake_minimum_required(VERSION 3.17.3)
project(loguru)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_C_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/../../../gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/../../../gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++)
find_package(Threads REQUIRED)

add_library(loguru loguru.cpp)
target_link_libraries(loguru PRIVATE Threads::Threads)
target_link_libraries(loguru PRIVATE dl)
