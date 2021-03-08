#include <memory>
#include <unordered_map>
#include <array>
#include <iostream>
#include <cmath>
#include <initializer_list>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Crosshair.hpp"
#include "Voxel.hpp"
#include "Constants.hpp"
#include "Coordinates.hpp"
#include "ChunkManager.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"

std::vector<glm::ivec3> leftFace    = { glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0) };
std::vector<glm::vec3>  rightFace   = { glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0) };
std::vector<glm::ivec3> bottomFace  = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 0.0, 0.0),  glm::ivec3(0.0, 0.0, 1.0) };
std::vector<glm::ivec3> topFace     = { glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(1.0, 1.0, 1.0) };

enum class FaceType
{
    Left,
    Right,
    Bottom,
    Top
};

void PushIndices(std::vector<unsigned int>& indices)
{
    uint32_t m;
    if (!indices.empty())
        m = indices.back() + 1;
    else
        m = 0;
    indices.insert(indices.end(), { 0 + m, 1 + m, 2 + m, 2 + m, 1 + m, 3 + m } );
}

void PushFace(const VoxelPosition& position, Mesh& mesh, VoxelType type, const std::vector<glm::ivec3>& face)
{
    static float spriteSize = 1.0f / 16.0f;
    float u = ((int)type % 16) * spriteSize;
    float v = ((int)type / 16) * spriteSize;
    mesh.vertices.insert(mesh.vertices.end(), 
    {
        { face.at(0) + position, { u, v + spriteSize } },
        { face.at(1) + position, { u + spriteSize, v + spriteSize } },
        { face.at(2) + position, { u, v } },
        { face.at(3) + position, { u + spriteSize, v } }
    });
    PushIndices(mesh.indices);
}

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 0, 16, 0 });
        
        Skybox skybox("../assets/skybox/", "jpg");

        Crosshair crosshair;

        Texture tex("../assets/textures/atlas.png");
        Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
        shader.SetInt("texture", 0);

        Mesh mesh;

        ChunkManager m;
        m.AddChunk({ 0, 0, 0 });
        auto& ch = m.GetChunk({ 0, 0, 0 });

        for (int z = 0; z < 16; z++)
        {
            for (int y = 0; y < 16; y++)
            {
                for (int x = 0; x < 16; x++)
                {
                    if (ch.QuickGetVoxel({ x, y, z }).type != VoxelType::Air)
                    {
                        if (ch.GetVoxel({ x, y + 1, z }).type == VoxelType::Air)
                            PushFace({ x, y, z }, mesh, ch.QuickGetVoxel({ x, y, z }).type, topFace);
                    }
                }
            }
        }

        mesh.Upload();

        while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
        {
            Engine::GetEventSystem()->Process();

            // timing
            static float deltaTime = 0.0f;
            static float lastFrame = 0.0f;
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera.Update(deltaTime);

            window->Clear();

            mesh.va->Bind();
            shader.Bind();
            shader.SetMat4("u_Projection", camera.GetProjectionMatrix());
            shader.SetMat4("u_View", camera.GetViewMatrix());
            shader.SetMat4("u_Model", glm::mat4(1.0f));
            glDrawElements(GL_TRIANGLES, mesh.ib->GetCount(), GL_UNSIGNED_INT, 0);

            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}