cmake_minimum_required(VERSION 3.5)
project(SimpleCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Token.cpp src/Lexer.cpp src/Parser.cpp)
add_executable(SimpleCompiler ${SOURCE_FILES})