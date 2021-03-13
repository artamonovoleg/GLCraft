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
                auto position = LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(x, y, z));
                auto voxel = chunk.QGetVoxel({ x, y, z });
                auto neighbours = GetNeighbourVoxels(chunk, { x, y, z });
                if (voxel != Voxel::Air)
                {
                    if (ShouldPushFace(neighbours, voxel, Face::Left))
                        PushFace(mesh, position, voxel, Face::Left);
                    if (ShouldPushFace(neighbours, voxel, Face::Right))
                        PushFace(mesh, position, voxel, Face::Right);
                    if (ShouldPushFace(neighbours, voxel, Face::Front))
                        PushFace(mesh, position, voxel, Face::Front);
                    if (ShouldPushFace(neighbours, voxel, Face::Back))
                        PushFace(mesh, position, voxel, Face::Back);
                    if (ShouldPushFace(neighbours, voxel, Face::Bottom))
                        PushFace(mesh, position, voxel, Face::Bottom);
                    if (ShouldPushFace(neighbours, voxel, Face::Top))
                        PushFace(mesh, position, voxel, Face::Top);
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
        if (chunk.Modified(false))
        {
            auto& mesh = m_Meshes.find(chunk.GetPosition())->second;
            mesh.vertices.clear();
            mesh.indices.clear();
            BuildMesh(mesh, chunk);
            mesh.Load();
        }
    }
}

std::array<Voxel, 6> MeshBuilder::GetNeighbourVoxels(const Chunk& chunk, const VoxelPosition& position) const
{
    return std::array<Voxel, 6>
    {
        m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(position.x - 1, position.y, position.z))),
        m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(position.x + 1, position.y, position.z))),
        m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(position.x, position.y - 1, position.z))),
        m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(position.x, position.y + 1, position.z))),
        m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(position.x, position.y, position.z + 1))),
        m_ChunkManager.GetVoxel(LocalVoxelToGlobal(chunk.GetPosition(), VoxelPosition(position.x, position.y, position.z - 1)))
    };
}

bool MeshBuilder::ShouldPushFace(const std::array<Voxel, 6>& neighbours, Voxel voxel, Face face) const
{
    return neighbours.at(static_cast<int>(face)) == Voxel::Air
            || m_VoxelDataManager.IsTransparent(neighbours.at(static_cast<int>(face)));
}