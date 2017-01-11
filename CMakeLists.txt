cmake_minimum_required(VERSION 3.5)
project(SimpleCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SOURCE_FILES src/main.cpp
        src/parser/Token.cpp
        src/parser/Lexer.cpp
        src/util/names.cpp
        src/util/error.cpp
        src/parser/Parser.cpp
        src/tree/tree.cpp
        src/tree/pretty.cpp
        src/code/symbol.cpp
        src/code/type.cpp
        src/comp/enter.cpp
        src/jvm/ClassReader.cpp
        src/comp/env.cpp
        src/code/Symtab.cpp
        src/code/scope.cpp
        src/comp/attr.cpp
        src/util/tools.cpp)
add_executable(SimpleCompiler ${SOURCE_FILES})