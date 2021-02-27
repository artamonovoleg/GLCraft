project(stb_image)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

set(srcs stb_image.cpp)

include_directories(.)

add_library(stb_image STATIC ${srcs})