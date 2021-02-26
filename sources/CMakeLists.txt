cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX)
set (CMAKE_CXX_STANDARD 17)

include_directories(../third_party/glfw/include
                    ../third_party/glm/)
                    
set(SOURCES ./Engine.cpp
            ./EventSystem.cpp
            ./main.cpp)

add_executable(GLCraft ${SOURCES})
target_link_libraries(GLCraft glfw)