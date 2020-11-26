cmake_minimum_required(VERSION 3.6)
project(test)

set(CMAKE_CXX_STANDARD 14)

include_directories(test /usr/include/glib-2.0)
#Fedora
include_directories(test /usr/lib64/glib-2.0/include/)
#Ubuntu/Mint
include_directories(test /usr/lib/x86_64-linux-gnu/glib-2.0/include)

find_library(Lglib glib-2.0)
find_library(Lsigrokdec sigrokdecode)

add_executable(test main.cpp)
target_link_libraries(test ${Lglib})
target_link_libraries(test ${Lsigrokdec})