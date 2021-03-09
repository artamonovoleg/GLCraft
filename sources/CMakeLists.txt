cmake_minimum_required(VERSION 3.16)
project(GLCraft CXX)
set (CMAKE_CXX_STANDARD 17)
add_compile_options(-g)

include_directories(../third_party/glfw/include
                    ../third_party/glm/
                    ../third_party/glad/include
                    ../third_party/stb_image
                    ./Engine
                    ./Game)
                    
set(SOURCES ./Engine/Engine.cpp
            ./Engine/Window.cpp
            ./Engine/EventSystem.cpp
            ./Engine/Shader.cpp
            ./Game/Camera.cpp
            ./Game/Skybox.cpp
            ./Game/Crosshair.cpp
            ./Game/Mesh.cpp
            ./Game/Voxel.cpp
            ./main.cpp)

add_executable(GLCraft ${SOURCES})
target_link_libraries(GLCraft glfw glad stb_image)   