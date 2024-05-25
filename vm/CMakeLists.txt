cmake_minimum_required(VERSION 3.16)
project(pvm)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)

add_executable(pvm main.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp
    object/hiObject.cpp
    object/hiInteger.cpp
    object/hiString.cpp
    object/hiList.cpp
    object/hiDict.cpp
    object/arrayList.cpp
    object/typeObject.cpp
    object/klass.cpp
    runtime/stringTable.cpp
    runtime/universe.cpp
    runtime/interpreter.cpp
    runtime/frameObject.cpp
    runtime/functionObject.cpp
    runtime/cellObject.cpp
    memory/heap.cpp
    util/map.cpp
    )

include_directories(./)
