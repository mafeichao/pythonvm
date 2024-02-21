cmake_minimum_required(VERSION 3.16)
project(pvm)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)

add_executable(pvm main.cpp
    code/codeObject.cpp
    object/hiString.cpp
    object/arrayList.cpp
    binaryFileParser.cpp
    runtime/interpreter.cpp
    )

include_directories(./)
