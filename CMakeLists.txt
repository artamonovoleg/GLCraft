cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX)
add_compile_options(-O3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(third_party)
add_subdirectory(sources)