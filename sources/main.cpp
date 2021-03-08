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

std::vector<glm::ivec3> leftFace    = { glm::vec3(0.0, 0.0, 0.0),  glm::vec3(0.0, 0.0, 1.0),  glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0) };
std::vector<glm::ivec3> rightFace   = { glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0) };
std::vector<glm::ivec3> bottomFace  = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 1.0) };
std::vector<glm::ivec3> topFace     = { glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(1.0, 1.0, 1.0) };
std::vector<glm::ivec3> frontFace   = { glm::ivec3(0.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 1.0) };
std::vector<glm::ivec3> backFace    = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 0.0) };

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

class MeshBuilder
{
    private:
        std::unordered_map<VoxelPosition, Mesh> m_Meshes;

        void GenerateChunkMesh(const Chunk& chunk, Mesh& mesh)
        {
            for (int z = 0; z < ChunkSize; z++)
            {
                for (int y = 0; y < ChunkSize; y++)
                {
                    for (int x = 0; x < ChunkSize; x++)
                    {
                        if (chunk.QuickGetVoxel({ x, y, z }).type != VoxelType::Air)
                        {
                            auto position = glm::ivec3(x, y, z) + chunk.GetPosition() * 16;
                            // if (chunk.GetVoxel({ x, y + 1, z }).type == VoxelType::Air)
                                PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, topFace);
                            // if (chunk.GetVoxel({ x, y - 1, z }).type == VoxelType::Air)
                                PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, bottomFace);
                            // if (chunk.GetVoxel({ x, y, z - 1 }).type == VoxelType::Air)
                                PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, backFace);
                            // if (chunk.GetVoxel({ x, y, z + 1 }).type == VoxelType::Air)
                                PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, frontFace);
                            // if (chunk.GetVoxel({ x - 1, y, z }).type == VoxelType::Air)
                                PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, leftFace);
                            // if (chunk.GetVoxel({ x + 1, y, z }).type == VoxelType::Air)
                                PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, rightFace);
                        }
                    }
                }
            }
            mesh.Upload();
        }
    public:
        MeshBuilder(ChunkManager& manager)
        {
            for (auto& postochunk : manager.GetChunkMap())
            {
                auto& pos = postochunk.first;
                auto& chunk = postochunk.second;
                m_Meshes.emplace(std::make_pair(pos, Mesh{}));
                GenerateChunkMesh(chunk, m_Meshes.at(pos));
            }
        }

        void UpdateChunk(const Chunk& chunk)
        {
            auto& updateMesh = m_Meshes.at(chunk.GetPosition());
            updateMesh.vertices.clear();
            updateMesh.indices.clear();
            GenerateChunkMesh(chunk, updateMesh);
            updateMesh.Upload();
        }

        const std::unordered_map<VoxelPosition, Mesh>& GetMeshes() const
        {
            return m_Meshes;
        }
};

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
        m.AddChunk({ 1, 0, 0 });
        m.GetChunkMap().at({ 0, 0, 0 }).QuickSetVoxel({ 1, 15, 1 }, VoxelType::Sand);

        MeshBuilder mb(m);
        
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

            shader.Bind();
            shader.SetMat4("u_Projection", camera.GetProjectionMatrix());
            shader.SetMat4("u_View", camera.GetViewMatrix());
            shader.SetMat4("u_Model", glm::mat4(1.0f));
            
            for (const auto& mesh : mb.GetMeshes())
            {
                mesh.second.va->Bind();
                glDrawElements(GL_TRIANGLES, mesh.second.ib->GetCount(), GL_UNSIGNED_INT, 0);
            }

            if (keyboard.GetKeyDown(GLFW_KEY_F))
            {
                m.GetChunkMap().at({ 0, 0, 0 }).QuickSetVoxel({ 1, 15, 1 }, VoxelType::Air);
                mb.UpdateChunk(m.GetChunk({ 0, 0, 0 }));
            }

            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}