cmake_minimum_required(VERSION 3.15)

project(CRNLCH)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(tests)