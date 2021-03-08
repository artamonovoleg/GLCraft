#include "ChunkManager.hpp"
#include "Camera.hpp"

ChunkManager::ChunkManager(const Camera& camera)
{
    auto startPoint = ToChunkPosition(GlobalToVoxel(camera.GetPosition()) - glm::ivec3(0, 1, 0));

    for (int z = startPoint.z - 1; z < 1; ++z)
    {
        for (int x = startPoint.x - 1; x < 1; ++x)
            AddChunk({ x, startPoint.y, z });
    }
}

const Chunk& ChunkManager::GetChunk(const VoxelPosition& position)
{
    auto chunkPos = ToChunkPosition(position);
    if (m_ChunkMap.find(chunkPos) == m_ChunkMap.cend())
    {
        static Chunk errorChunk(*this, { 0, 0, 0 });
        // errorChunk.m_Data.fill({ VoxelType::Ground });
        return errorChunk;
    }

    return m_ChunkMap.at(chunkPos);
}

Chunk& ChunkManager::AddChunk(const VoxelPosition& position)
{
    auto it = m_ChunkMap.find(ToChunkPosition(position));
    if (it == m_ChunkMap.cend())
        return m_ChunkMap.emplace(std::make_pair(position, Chunk(*this, position))).first->second;
    return it->second;
}

Voxel ChunkManager::GetVoxel(const VoxelPosition& position)
{
    return GetChunk(position).QuickGetVoxel(GlobalToLocalVoxel(position));
}

void ChunkManager::SetVoxel(const VoxelPosition& position, VoxelType type)
{
    AddChunk(position).QuickSetVoxel(GlobalToLocalVoxel(position), type);
}