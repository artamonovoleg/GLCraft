#include "ChunkManager.hpp"
#include "Chunk.hpp"

ChunkManager::ChunkManager(const Camera& camera)
    : m_Camera(camera)
{
    for (int z = 0; z < 2; ++z)
    {
        for (int y = 0; y < 2; ++y)
        {
            for (int x = 0; x < 2; ++x)
                m_Chunks.emplace(std::make_pair(VoxelPosition(x, y, z), Chunk({ x, y, z })));
        }
    }
}

const Chunk& ChunkManager::GetChunk(const VoxelPosition& position) const
{
    
}