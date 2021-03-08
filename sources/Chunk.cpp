#include "Chunk.hpp"
#include "ChunkManager.hpp"

bool Chunk::PositionInBounds(const VoxelPosition& position) const
{
    return (position.x >= 0 && position.z >= 0 && position.y >= 0 && position.x < ChunkSize && position.y < ChunkSize && position.z < ChunkSize);
}

const Voxel& Chunk::QuickGetVoxel(const VoxelPosition& position) const
{
    return m_Data.at(position.x + ChunkSize * ChunkSize * position.y + ChunkSize * position.z);
}

const Voxel& Chunk::GetVoxel(const VoxelPosition& position) const
{
    if (PositionInBounds(position)) return QuickGetVoxel(position);

    return m_ChunkManager.GetVoxel(LocalToGlobalVoxel(m_Position, position));
}