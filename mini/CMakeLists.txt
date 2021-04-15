cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
project(controllerhf)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++)
set(CMAKE_CXX_FLAGS "-fPIC -Os -s")
set(LDFLAGS "-Wl,--whole-archive,--no-as-needed")
file(GLOB analyser_src "src/*.cpp")

# Search libraries only under *target* paths.
set(CMAKE_FIND_ROOT_PATH src)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/redpitaya
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/rp_sdk
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/rp_sdk/libjson
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/arm-glib-2.0-include
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/glib-2.0
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/libsigrokdecode
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/python3.5
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/nlohmann
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/include/loguru
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

add_library(controllerhf SHARED ${analyser_src})
set_target_properties(controllerhf PROPERTIES PREFIX "")
target_link_libraries(controllerhf ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/librp.a
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/libglib-2.0.so
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/libpython3.5m.so
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/libsigrokdecode.so
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/arm-linux-gnueabihf/libpcre.so
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/arm-linux-gnueabihf/libexpat.so
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/arm-linux-gnueabihf/libz.so
        ${CMAKE_CURRENT_SOURCE_DIR}/externalRedpitaya/lib/libloguru.so
        ${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/arm-none-linux-gnueabihf/libc/lib/libpthread.so.0
        ${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/arm-none-linux-gnueabihf/libc/lib/libdl.so.2
        )