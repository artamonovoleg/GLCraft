#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <fstream>
#include "Engine.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureCubemap.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Crosshair.hpp"
#include "Voxel.hpp"
#include "Constants.hpp"
#include "Chunk.hpp"
#include "Mesh.hpp"
#include "ChunkManager.hpp"
#include "Coordinates.hpp"

class ChunkMeshBuilder
{
    private:
        const ChunkManager& m_ChunkManager;
        VoxelDataManager m_VoxelDataManager;

        std::unordered_map<VoxelPosition, Mesh> m_Meshes;
        void BuildMesh(Mesh& mesh, const Chunk& chunk);
        void PushIndices(std::vector<unsigned int>& indices);
        void PushFace(Mesh& mesh, const VoxelPosition& position, Voxel voxel, Face face);
    public:
        ChunkMeshBuilder(const ChunkManager& chunkManager);
        
        void UpdateChunk(const Chunk& chunk)
        {
            auto& mesh = m_Meshes.at(chunk.GetPosition());
            mesh.vertices.clear();
            mesh.indices.clear();
            BuildMesh(mesh, chunk);
            mesh.Load();
        }

        const std::unordered_map<VoxelPosition, Mesh>& GetMeshes() const { return m_Meshes; }
};

ChunkMeshBuilder::ChunkMeshBuilder(const ChunkManager& chunkManager)
    : m_ChunkManager(chunkManager)
{
    for (const auto& [position, chunk] : chunkManager.GetChunkMap())
    {
        auto& mesh = m_Meshes.emplace(std::make_pair(position, Mesh{})).first->second;
        BuildMesh(mesh, chunk);
        mesh.Load();
    }
}

void ChunkMeshBuilder::PushIndices(std::vector<unsigned int>& indices)
{
    uint32_t m;
    if (!indices.empty())
        m = indices.back() + 1;
    else
        m = 0;
    indices.insert(indices.end(), { 0 + m, 1 + m, 2 + m, 2 + m, 1 + m, 3 + m } );
}

void ChunkMeshBuilder::PushFace(Mesh& mesh, const VoxelPosition& position, Voxel voxel, Face face)
{
    auto& vertices  = m_VoxelDataManager.GetVertices(face);
    auto [u, v]     = m_VoxelDataManager.GetUV(voxel, face);

    mesh.vertices.insert(mesh.vertices.end(),
    {
        { vertices.at(0) + position, { u, v + UVSize } },
        { vertices.at(1) + position, { u + UVSize, v + UVSize } },
        { vertices.at(2) + position, { u, v } },
        { vertices.at(3) + position, { u + UVSize, v } }
    });

    PushIndices(mesh.indices);
}

void ChunkMeshBuilder::BuildMesh(Mesh& mesh, const Chunk& chunk)
{
    for (int z = 0; z < ChunkSize; ++z)
    {
        for (int y = 0; y < ChunkSize; ++y)
        {
            for (int x = 0; x < ChunkSize; ++x)
            {
                auto& position = chunk.GetPosition();
                auto voxel = chunk.QGetVoxel({ x, y, z });
                if (voxel != Voxel::Air)
                {
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x - 1, y, z))) == Voxel::Air
                        || m_VoxelDataManager.IsTransparent(m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x - 1, y, z)))) && !m_VoxelDataManager.IsTransparent(voxel))
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Left);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x + 1, y, z))) == Voxel::Air
                        || m_VoxelDataManager.IsTransparent(m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x + 1, y, z)))) && !m_VoxelDataManager.IsTransparent(voxel))
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Right);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z + 1))) == Voxel::Air
                        || m_VoxelDataManager.IsTransparent(m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z + 1)))) && !m_VoxelDataManager.IsTransparent(voxel))
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Front);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z - 1))) == Voxel::Air
                        || m_VoxelDataManager.IsTransparent(m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z - 1)))) && !m_VoxelDataManager.IsTransparent(voxel))
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Back);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y - 1, z))) == Voxel::Air
                        || m_VoxelDataManager.IsTransparent(m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y - 1, z)))) && !m_VoxelDataManager.IsTransparent(voxel))
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Bottom);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y + 1, z))) == Voxel::Air
                        || m_VoxelDataManager.IsTransparent(m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y + 1, z)))) && !m_VoxelDataManager.IsTransparent(voxel))
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Top);
                }
            }
        }
    }
}

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });
    {
        auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
        auto& mouse = Engine::GetEventSystem()->GetMouse();
        auto& window = Engine::GetWindow();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / static_cast<float>(window->GetHeight()), 0.1f, 30.0f);

        Camera camera({ 0, 0, 0 });
        
        Skybox skybox("../assets/skybox/", "jpg");

        Crosshair crosshair;

        Texture tex("../assets/textures/atlas.png");
        Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
        shader.SetInt("texture", 0);

        Mesh mesh;
        ChunkManager manager(camera);
        ChunkMeshBuilder meshBuilder(manager);

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

            for (const auto& mesh : meshBuilder.GetMeshes())
                mesh.second.Draw();
            
            if (keyboard.GetKeyDown(GLFW_KEY_F))
            {
                manager.SetVoxel({ 2, 2, 2 }, Voxel::Grass);
                manager.SetVoxel({ 0, 0, 0 }, Voxel::Grass);
                meshBuilder.UpdateChunk(manager.GetChunk({ 0, 0, 0 }));
                manager.SetVoxel({ 3, 3, 3 }, Voxel::Grass);
                manager.SetVoxel({ 5, 5, 5 }, Voxel::Grass);
                meshBuilder.UpdateChunk(manager.GetChunk({ 3, 3, 3 }));
            }
            crosshair.Draw();
            skybox.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            window->SwapBuffers();
        }
    }

    Engine::Shutdown();
    return 0;
}