set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.16)
project(pythonvm)

add_subdirectory(parser)
add_subdirectory(vm)
add_subdirectory(lib)
add_subdirectory(test)

