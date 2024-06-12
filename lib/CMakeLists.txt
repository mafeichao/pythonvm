cmake_minimum_required(VERSION 3.16)

project(stdlib)

set(SRCFILES
    builtins.py
    math.py
    compilelib.py
    )

set(LIBFILES
    builtins.pyc
    math.pyc
)

add_custom_command(OUTPUT ${LIBFILES}
    POST_BUILD
    DEPENDS ${SRCFILES}
    COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/compilelib.py
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(PyLibs ALL
    DEPENDS ${LIBFILES}
)

INCLUDE_DIRECTORIES(./ ../vm/)

# libmath
ADD_LIBRARY(math SHARED math.cpp)
TARGET_LINK_LIBRARIES(math hivm)

