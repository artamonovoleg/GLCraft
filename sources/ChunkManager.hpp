#pragma once

#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "Chunk.hpp"

class ChunkManager
{
    private:
        std::unordered_map<VoxelPosition, Chunk> m_Chunks;
    public:
        const Chunk& GetChunk(const VoxelPosition& position) const
        {
            return m_Chunks.at(position);
        }

        const Voxel& GetVoxel(const VoxelPosition& position) const
        {
            return GetChunk(position).QuickGetVoxel(position);
        }
};