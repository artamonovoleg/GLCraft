cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX C)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# glad
include_directories(./glad/include)
add_library(glad STATIC ./glad/src/glad.c)
#

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(stb_image)