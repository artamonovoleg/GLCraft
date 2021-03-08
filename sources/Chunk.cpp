#include "Chunk.hpp"

bool Chunk::PositionInBounds(const VoxelPosition& position) const
{
    return (position.x >= 0 && position.z >= 0 && position.y >= 0 && position.x < ChunkSide && position.y < ChunkHeight && position.z < ChunkSide);
}

const Voxel& Chunk::QuickGetVoxel(const VoxelPosition& position) const
{
    return m_Data.at(position.x + ChunkSide * ChunkSide * position.y + ChunkSide * position.z);
}

const Voxel& Chunk::GetVoxel(const VoxelPosition& position) const
{
    if (PositionInBounds(position)) return QuickGetVoxel(position);

    return m_ChunkManager.GetVoxel(LocalToGlobalVoxel(m_Position, position));
}