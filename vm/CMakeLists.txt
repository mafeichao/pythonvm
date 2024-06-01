cmake_minimum_required(VERSION 3.16)
project(pvm)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)

ADD_EXECUTABLE(pvm main.cpp)

set(SOURCE_FILE
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
    runtime/module.cpp
    runtime/traceback.cpp
    runtime/generator.cpp
    memory/heap.cpp
    memory/oopClosure.cpp
    util/map.cpp
    util/stack.cpp
    util/handles.cpp
    util/vector.cpp
    )

INCLUDE_DIRECTORIES(./)

if (APPLE)
    message(STATUS "cmake for apple")
    add_definitions(-DHIVM_APPLE)
endif()

# libhivm
ADD_LIBRARY(hivm SHARED ${SOURCE_FILE})
TARGET_LINK_LIBRARIES(hivm ${CMAKE_DL_LIBS})

# libmath
#ADD_LIBRARY(math SHARED lib/math.cpp)
#TARGET_LINK_LIBRARIES(math hivm)

TARGET_LINK_LIBRARIES(pvm hivm)
