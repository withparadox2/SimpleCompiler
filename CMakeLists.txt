cmake_minimum_required(VERSION 3.5)
project(SimpleCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(SimpleCompiler ${SOURCE_FILES})