#include "Chunk.hpp"
#include "ChunkManager.hpp"

Chunk::Chunk(const ChunkManager& chunkManager, const VoxelPosition& position)
    : m_ChunkManager(chunkManager), m_Position(position)
{
    for (int z = 0; z < ChunkSize; ++z)
    {
        for (int y = 0; y < ChunkSize; ++y)
        {
            for (int x = 0; x < ChunkSize; ++x)
                QSetVoxel({ x, y, z }, Voxel::Glass);
        }
    }
    QSetVoxel({ 1, 1, 1 }, Voxel::Sand);
}

size_t Chunk::PositionToIndex(const VoxelPosition& position)
{
    return (position.x + ChunkSize * ChunkSize * position.y + ChunkSize * position.z);
}

const VoxelPosition& Chunk::GetPosition() const { return m_Position; }

Voxel Chunk::QGetVoxel(const VoxelPosition& position) const
{
    return m_Data.at(PositionToIndex(position));
}

void Chunk::QSetVoxel(const VoxelPosition& position, Voxel voxel)
{
    m_Data.at(PositionToIndex(position)) = voxel;
}