#include <glm/gtc/noise.hpp>
#include <iostream>
#include "Chunk.hpp"
#include "ChunkManager.hpp"

Chunk::Chunk(const ChunkManager& chunkManager, const VoxelPosition& position)
    : m_ChunkManager(chunkManager), m_Position(position)
{
    glm::vec3 worldGlobal = LocalVoxelToGlobal(m_Position, VoxelPosition(0, 0, 0));

    m_Data.fill(Voxel::Air);
    if (m_Position.y < 0)
    {
        if (m_Position.y == -1)
        {
            for (int z = 0; z < ChunkSize; ++z)
            {
                for (int x = 0; x < ChunkSize; ++x)
                {
                    int height = (glm::perlin(glm::vec3((worldGlobal.x + x) * 0.05f, (worldGlobal.z + z) * 0.05f, 0.0f)) + 1) * ChunkSize;
                    
                    if (height > ChunkSize)
                        height = ChunkSize;

                    for (int y = 0; y < height; ++y)
                    {
                        if (y == height - 1)
                            m_Data.at(PositionToIndex(VoxelPosition(x, y, z))) = Voxel::Grass;
                        else
                            m_Data.at(PositionToIndex(VoxelPosition(x, y, z))) = Voxel::Ground;
                    }
                }
            }
        }
        else
        {
            m_Data.fill(Voxel::Ground);
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