cmake_minimum_required(VERSION 3.16)
project(galang_parser)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)

add_executable(parser main.cpp
    lexer.cpp
    parser.cpp
    ast.cpp
    visitor.cpp
    codegen.cpp)
