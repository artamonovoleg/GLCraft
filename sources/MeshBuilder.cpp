#include "MeshBuilder.hpp"
#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include "VoxelData.hpp"

namespace 
{
    VoxelData m_VoxelData;
    
    std::vector<glm::ivec3> leftFace    = { glm::vec3(0.0, 0.0, 0.0),  glm::vec3(0.0, 0.0, 1.0),  glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0) };
    std::vector<glm::ivec3> rightFace   = { glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0) };
    std::vector<glm::ivec3> bottomFace  = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 1.0) };
    std::vector<glm::ivec3> topFace     = { glm::ivec3(0.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(1.0, 1.0, 1.0) };
    std::vector<glm::ivec3> frontFace   = { glm::ivec3(0.0, 0.0, 1.0), glm::ivec3(1.0, 0.0, 1.0), glm::ivec3(0.0, 1.0, 1.0), glm::ivec3(1.0, 1.0, 1.0) };
    std::vector<glm::ivec3> backFace    = { glm::ivec3(1.0, 0.0, 0.0), glm::ivec3(0.0, 0.0, 0.0), glm::ivec3(1.0, 1.0, 0.0), glm::ivec3(0.0, 1.0, 0.0) };
}

MeshBuilder::MeshBuilder(ChunkManager& manager)
    : m_Manager(manager)
{
    for (auto& [pos, chunk] : manager.GetChunkMap())
    {
        m_Meshes.emplace(std::make_pair(pos, Mesh{}));
        GenerateChunkMesh(chunk, m_Meshes.at(pos));
    }
}

void MeshBuilder::UpdateChunk(const Chunk& chunk)
{
    if (m_Meshes.find(chunk.GetPosition()) != m_Meshes.cend())
    {
        auto& updateMesh = m_Meshes.at(chunk.GetPosition());
        updateMesh.vertices.clear();
        updateMesh.indices.clear();
        GenerateChunkMesh(chunk, updateMesh);
        updateMesh.Upload();
        chunk.Updated();
    }
}

void MeshBuilder::Update()
{
    for (auto& [pos, chunk] : m_Manager.GetChunkMap())
    {
        if (chunk.Outdate())
            UpdateChunk(chunk);
    }
}

const std::unordered_map<VoxelPosition, Mesh>& MeshBuilder::GetMeshes() const
{
    return m_Meshes;
}

void MeshBuilder::PushIndices(std::vector<unsigned int>& indices)
{
    uint32_t m;
    if (!indices.empty())
        m = indices.back() + 1;
    else
        m = 0;
    indices.insert(indices.end(), { 0 + m, 1 + m, 2 + m, 2 + m, 1 + m, 3 + m } );
}

void MeshBuilder::PushFace(const VoxelPosition& position, Mesh& mesh, VoxelType type, const std::vector<glm::ivec3>& face)
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

void MeshBuilder::GenerateChunkMesh(const Chunk& chunk, Mesh& mesh)
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
                    if (chunk.GetVoxel({ x, y + 1, z }).type == VoxelType::Air || m_VoxelData.IsTransparent(chunk.GetVoxel({ x, y + 1, z })))
                        PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, topFace);
                    if (chunk.GetVoxel({ x, y - 1, z }).type == VoxelType::Air)
                        PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, bottomFace);
                    if (chunk.GetVoxel({ x, y, z - 1 }).type == VoxelType::Air)
                        PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, backFace);
                    if (chunk.GetVoxel({ x, y, z + 1 }).type == VoxelType::Air)
                        PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, frontFace);
                    if (chunk.GetVoxel({ x - 1, y, z }).type == VoxelType::Air)
                        PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, leftFace);
                    if (chunk.GetVoxel({ x + 1, y, z }).type == VoxelType::Air)
                        PushFace(position, mesh, chunk.QuickGetVoxel({ x, y, z }).type, rightFace);
                }
            }
        }
    }
    mesh.Upload();
}