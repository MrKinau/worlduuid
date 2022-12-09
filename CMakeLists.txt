cmake_minimum_required(VERSION 3.22)
project(worlduuid C)

set(CMAKE_C_STANDARD 99)

add_executable(worlduuid main.c)

find_path(LIBUUID_INCLUDE_DIR uuid.h PATH_SUFFIXES uuid)
find_library(LIBUUID_LIBRARY libuuid.a)
include_directories(${LIBUUID_INCLUDE_DIR})
target_link_libraries(worlduuid PUBLIC ${LIBUUID_LIBRARY})