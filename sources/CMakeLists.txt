cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX)
set (CMAKE_CXX_STANDARD 20)
add_compile_options(-O3 -g)

include_directories(../third_party/glfw/include
                    ../third_party/glm/
                    ../third_party/glad/include
                    ../third_party/stb_image
                    ./Engine
                    ./Game)
                    
set(SOURCES ./Engine/Engine.cpp
            ./Engine/Window.cpp
            ./Engine/EventSystem.cpp
            ./Engine/Keyboard.cpp
            ./Engine/Mouse.cpp
            ./Engine/Shader.cpp
            ./Game/Camera.cpp
            ./Game/Skybox.cpp
            ./Game/Crosshair.cpp
            ./Game/Mesh.cpp
            ./Game/Voxel.cpp
            ./Game/Coordinates.cpp
            ./Game/Chunk.cpp
            ./Game/ChunkManager.cpp
            ./Game/MeshBuilder.cpp
            ./main.cpp)

add_executable(GLCraft ${SOURCES})
target_link_libraries(GLCraft glfw glad stb_image)   