cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(third_party)
add_subdirectory(sources)