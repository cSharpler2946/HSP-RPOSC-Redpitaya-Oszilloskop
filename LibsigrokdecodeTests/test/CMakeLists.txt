cmake_minimum_required(VERSION 3.17.3)
project(test)

set(CMAKE_CXX_STANDARD 14)

#TIP: Use locate <x.h> to find paths
include_directories(test /usr/include/glib-2.0)
#Fedora
include_directories(test /usr/lib64/glib-2.0/include/)
#Ubuntu/Mint
include_directories(test /usr/lib/x86_64-linux-gnu/glib-2.0/include)

find_library(Lglib glib-2.0)
find_library(Lsigrokdec sigrokdecode)

find_package (Python3 REQUIRED COMPONENTS Development)
include_directories(test ${Python3_INCLUDE_DIRS})
message(STATUS "Python 3 found? ${Python3_FOUND}")

add_executable(test main.cpp)
target_link_libraries(test ${Lglib})
target_link_libraries(test ${Lsigrokdec})
target_link_libraries(test ${Python3_LIBRARIES})
