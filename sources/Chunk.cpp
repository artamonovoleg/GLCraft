#include "Chunk.hpp"
#include "ChunkManager.hpp"


#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBuffersLayout.hpp"
#include "IndexBuffer.hpp"

Chunk::Chunk(ChunkManager& chunkManager, const VoxelPosition& position)
    : m_ChunkManager(chunkManager), m_Position(position) 
{
    m_Data.fill({ VoxelType::Ground });
}

bool Chunk::PositionInBounds(const VoxelPosition& position) const
{
    return (position.x >= 0 && position.z >= 0 && position.y >= 0 && position.x < ChunkSize && position.y < ChunkSize && position.z < ChunkSize);
}

Voxel Chunk::QuickGetVoxel(const VoxelPosition& position) const
{
    return m_Data.at(position.x + ChunkSize * ChunkSize * position.y + ChunkSize * position.z);
}

Voxel Chunk::GetVoxel(const VoxelPosition& position) const
{
    if (PositionInBounds(position)) return QuickGetVoxel(position);

    return m_ChunkManager.GetVoxel(LocalToGlobalVoxel(m_Position, position));
}

void Chunk::QuickSetVoxel(const VoxelPosition& position, VoxelType type)
{
    m_Data.at(position.x + ChunkSize * ChunkSize * position.y + ChunkSize * position.z).type = type;
}

void Chunk::SetVoxel(const VoxelPosition& position, VoxelType type)
{
    if (PositionInBounds(position)) QuickSetVoxel(position, type);
    m_ChunkManager.SetVoxel(LocalToGlobalVoxel(m_Position, position), type);
}