#include <algorithm>
#include <iostream>
#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"


ChunkManager::ChunkManager(const Camera& camera)
    : m_Camera(camera)
{
    // for test spawn 5 x 5 around camera
    auto spawnPoint = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition())) - ChunkPosition(0, 1, 0);
    for (int z = spawnPoint.z - RenderDistance; z < spawnPoint.z + RenderDistance; ++z)
    {
        for (int x = spawnPoint.x - RenderDistance; x < spawnPoint.x + RenderDistance; ++x)
            AddChunk({ x, spawnPoint.y, z });
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

bool ChunkManager::ChunkOutOfBounds(const ChunkPosition& playerPosition, const ChunkPosition& chunkPosition)
{
    return  std::abs(chunkPosition.x - playerPosition.x) > RenderDistance ||
            std::abs(chunkPosition.y - playerPosition.y) > RenderDistance ||
            std::abs(chunkPosition.z - playerPosition.z) > RenderDistance;
}

void ChunkManager::OnUpdate()
{
    auto playerPos = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition()));

    std::erase_if(m_Chunks, [&](const auto& p)
    {
        return ChunkOutOfBounds(playerPos, p.first);
    });
}