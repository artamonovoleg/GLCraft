#include "ChunkManager.hpp"
#include "Chunk.hpp"

ChunkManager::ChunkManager(const Camera& camera)
    : m_Camera(camera)
{
    // for (int z = 0; z < 2; ++z)
    // {
    //     for (int y = 0; y < 2; ++y)
    //     {
    //         for (int x = 0; x < 2; ++x)
                m_Chunks.emplace(std::make_pair(VoxelPosition(0, 0, 0), Chunk(*this, { 0, 0, 0 })));
    //     }
    // }
}

bool ChunkManager::HasChunk(const VoxelPosition& position) const
{
    return (m_Chunks.find(ToChunkPosition(position)) != m_Chunks.cend());
}

Chunk& ChunkManager::AddChunk(const VoxelPosition& position)
{
    if (m_Chunks.find(position) != m_Chunks.cend()) return m_Chunks.at(position);
    return m_Chunks.emplace(std::make_pair(position, Chunk(*this, position))).first->second;
}

const Chunk& ChunkManager::GetChunk(const VoxelPosition& position) const
{
    auto chunkPosition = ToChunkPosition(position);
    if (m_Chunks.find(chunkPosition) == m_Chunks.cend())
    {
        static Chunk errorChunk(*this, { 0, 0, 0 });
        return errorChunk;
    }

    return m_Chunks.at(chunkPosition);
}