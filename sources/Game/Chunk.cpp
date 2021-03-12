#include "Chunk.hpp"
#include "ChunkManager.hpp"

Chunk::Chunk(const ChunkManager& chunkManager, const VoxelPosition& position)
    : m_ChunkManager(chunkManager), m_Position(position)
{
    m_Data.fill(Voxel::Air);
    if (m_Position.y < 0)
    {
        for (int z = 0; z < ChunkSize; ++z)
        {
            for (int y = 0; y < ChunkSize; ++y)
            {
                for (int x = 0; x < ChunkSize; ++x)
                    QSetVoxel({ x, y, z }, Voxel::Grass);
            }
        }
    }
}

size_t Chunk::PositionToIndex(const VoxelPosition& position)
{
    return (position.x + ChunkSize * ChunkSize * position.y + ChunkSize * position.z);
}

bool Chunk::PositionInBounds(const VoxelPosition& position)
{
    return (position.x >= 0 && position.y >= 0 && position.z >= 0 && position.x < ChunkSize && position.y < ChunkSize && position.z < ChunkSize);
}

const VoxelPosition& Chunk::GetPosition() const { return m_Position; }

Voxel Chunk::QGetVoxel(const VoxelPosition& position) const
{
    return m_Data.at(PositionToIndex(position));
}

void Chunk::QSetVoxel(const VoxelPosition& position, Voxel voxel)
{
    if (voxel == Voxel::Air)
    {
        if (position.x == 0)
            m_ChunkManager.GetChunk(m_Position - ChunkPosition(1, 0, 0)).Modified(true);
        if (position.x == ChunkSize - 1)
            m_ChunkManager.GetChunk(m_Position - ChunkPosition(-1, 0, 0)).Modified(true);
        if (position.z == 0)
            m_ChunkManager.GetChunk(m_Position - ChunkPosition(0, 0, 1)).Modified(true);
        if (position.z == ChunkSize - 1)
            m_ChunkManager.GetChunk(m_Position - ChunkPosition(0, 0, -1)).Modified(true);
        if (position.y == 0)
            m_ChunkManager.GetChunk(m_Position - ChunkPosition(0, 0, 1)).Modified(true);
        if (position.y == ChunkSize - 1)
            m_ChunkManager.GetChunk(m_Position - ChunkPosition(0, 0, -1)).Modified(true);
    }

    m_Data.at(PositionToIndex(position)) = voxel;
    Modified(true);
}

Voxel Chunk::GetVoxel(const VoxelPosition& position) const
{
    if (PositionInBounds(position)) return QGetVoxel(position);
    return m_ChunkManager.GetVoxel(LocalVoxelToGlobal(m_Position, position));
}