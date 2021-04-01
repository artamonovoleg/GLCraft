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
    for (int z = spawnPoint.z - 5; z < spawnPoint.z + 5; ++z)
    {
        for (int x = spawnPoint.x - 5; x < spawnPoint.x + 5; ++x)
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

void ChunkManager::OnUpdate()
{
    auto chunkPos = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition()));

    for (int z = chunkPos.z - RenderDistance; z < chunkPos.z + RenderDistance; ++z)
    {
        for (int y = chunkPos.y - RenderDistance; y < chunkPos.y + RenderDistance; ++y)
        {
            for (int x = chunkPos.x - RenderDistance; x < chunkPos.x + RenderDistance; ++x)
                AddChunk({ x, y, z });
        }
    }
}

bool ChunkManager::InRenderDistance(const ChunkPosition& position) const
{
    auto ppos = ToChunkPosition(ToVoxelPosition(m_Camera.GetPosition()));
    auto length = glm::length(glm::vec3(ppos - position));
    return (length <= RenderDistance);
}