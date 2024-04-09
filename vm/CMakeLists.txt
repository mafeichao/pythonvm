cmake_minimum_required(VERSION 3.16)
project(pvm)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)

add_executable(pvm main.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp
    object/hiInteger.cpp
    object/hiString.cpp
    object/hiList.cpp
    object/arrayList.cpp
    runtime/universe.cpp
    runtime/interpreter.cpp
    util/map.cpp
    )

include_directories(./)
