#include <algorithm>
#include <iostream>
#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

ChunkManager::ChunkManager(const Camera& camera)
    : m_Camera(camera)
{
    AddAroundCamera();
}

void ChunkManager::AddAroundCamera()
{
    auto start = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition()));
    for (int z = start.z - 3; z < start.z + 3; ++z)
    {
        for (int y = start.y - 1; y < start.y; ++y)
        {
            for (int x = start.x - 3; x < start.x + 3; ++x)
                m_Chunks.emplace(std::make_pair(VoxelPosition(x, y, z), Chunk(*this, { x, y, z })));
        }
    }
}

Chunk& ChunkManager::AddChunk(const ChunkPosition& position)
{
    if (m_Chunks.find(position) != m_Chunks.cend()) return m_Chunks.at(position);
    return m_Chunks.emplace(std::make_pair(position, Chunk(*this, position))).first->second;
}

const Chunk& ChunkManager::GetChunk(const ChunkPosition& position) const
{
    if (m_Chunks.find(position) == m_Chunks.cend())
    {
        static Chunk errorChunk(*this, { 0, 0, 0 });
        return errorChunk;
    }

    return m_Chunks.at(position);
}

Voxel ChunkManager::GetVoxel(const VoxelPosition& position) const
{
    auto it = m_Chunks.find(ToChunkPosition(position));
    if (it == m_Chunks.cend())
        return Voxel::Air;
    else
        return it->second.QGetVoxel(GlobalVoxelToLocal(position));
}

void ChunkManager::SetVoxel(const VoxelPosition& position, Voxel voxel)
{
    auto chunkPosition = ToChunkPosition(position);
    auto it = m_Chunks.find(chunkPosition);
    auto local = GlobalVoxelToLocal(position);
    if (it != m_Chunks.cend()) 
        it->second.QSetVoxel(local, voxel);
    else 
        AddChunk(chunkPosition).QSetVoxel(local, voxel);
}

bool ChunkManager::HasChunk(const ChunkPosition& position) const
{
    return m_Chunks.find(position) != m_Chunks.cend();
}

void ChunkManager::RemoveOld()
{
    auto it = std::erase_if(m_Chunks, [&](const auto& p) 
    {
        auto position = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition()));
        bool res = (std::abs(position.x - p.second.GetPosition().x) > 3) || (std::abs(position.z - p.second.GetPosition().z) > 3);
        return res;
    });
}

void ChunkManager::Process()
{
    auto position = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition()));
    if (!HasChunk(position * 3) || !HasChunk(position * -3))
    {
        AddAroundCamera();
        RemoveOld();
    }
}