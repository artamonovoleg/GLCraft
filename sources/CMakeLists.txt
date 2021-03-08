cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX)
set (CMAKE_CXX_STANDARD 20)

include_directories(../third_party/glfw/include
                    ../third_party/glm/
                    ../third_party/glad/include
                    ../third_party/stb_image)
                    
set(SOURCES ./Engine.cpp
            ./Window.cpp
            ./EventSystem.cpp
            ./Camera.cpp
            ./Shader.cpp
            ./Skybox.cpp
            ./Crosshair.cpp
            ./SpriteLibrary.cpp
            ./Coordinates.cpp
            ./Chunk.cpp
            ./ChunkManager.cpp
            ./main.cpp)

add_executable(GLCraft ${SOURCES})
target_link_libraries(GLCraft glfw glad stb_image)   