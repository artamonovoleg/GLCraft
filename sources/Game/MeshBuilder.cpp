#include "MeshBuilder.hpp"
#include "Constants.hpp"
#include "ChunkManager.hpp"

MeshBuilder::MeshBuilder(const ChunkManager& chunkManager)
    : m_ChunkManager(chunkManager)
{
    for (const auto& [position, chunk] : chunkManager.GetChunkMap())
    {
        auto& mesh = m_Meshes.emplace(std::make_pair(position, Mesh{})).first->second;
        BuildMesh(mesh, chunk);
        mesh.Load();
    }
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

void MeshBuilder::PushFace(Mesh& mesh, const VoxelPosition& position, Voxel voxel, Face face)
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

void MeshBuilder::BuildMesh(Mesh& mesh, const Chunk& chunk)
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
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x - 1, y, z))) == Voxel::Air)
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Left);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x + 1, y, z))) == Voxel::Air)
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Right);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z + 1))) == Voxel::Air)
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Front);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z - 1))) == Voxel::Air)
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Back);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y - 1, z))) == Voxel::Air)
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Bottom);
                    if (m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y + 1, z))) == Voxel::Air)
                        PushFace(mesh, VoxelPosition(x, y, z) + position * ChunkSize, voxel, Face::Top);
                }
            }
        }
    }
}

void MeshBuilder::Process()
{
    for (const auto& [position, chunk] : m_ChunkManager.GetChunkMap())
    {
        if (m_Meshes.find(chunk.GetPosition()) == m_Meshes.end())
        {
            auto& mesh = m_Meshes.emplace(std::make_pair(position, Mesh{})).first->second;
            BuildMesh(mesh, chunk);
            mesh.Load();
        }
        else
        if (chunk.Modified())
        {
            auto& mesh = m_Meshes.find(chunk.GetPosition())->second;
            mesh.vertices.clear();
            mesh.indices.clear();
            BuildMesh(mesh, chunk);
            mesh.Load();
        }
    }
}