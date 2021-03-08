#pragma once

#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "Chunk.hpp"

using ChunkMap = std::unordered_map<VoxelPosition, Chunk>;

class ChunkManager
{
    private:
        ChunkMap m_ChunkMap;
    public:
        ChunkManager()
        {
        }

        ChunkMap& GetChunkMap() { return m_ChunkMap;}

        const Chunk& GetChunk(const VoxelPosition& position)
        {
            auto chunkPos = ToChunkPosition(position);
            if (m_ChunkMap.find(chunkPos) == m_ChunkMap.end())
            {
                return m_ChunkMap.begin()->second;
            }
            return m_ChunkMap.at(chunkPos);
        }

        Chunk& AddChunk(const VoxelPosition& position)
        {
            auto it = m_ChunkMap.find(position);
            if (it == m_ChunkMap.end())
                return m_ChunkMap.emplace(std::make_pair(position, Chunk(*this, position))).first->second;
            return it->second;
        }

        Voxel GetVoxel(const VoxelPosition& position)
        {
            return GetChunk(position).QuickGetVoxel(GlobalToLocalVoxel(position));
        }

        void SetVoxel(const VoxelPosition& position, VoxelType type)
        {
            AddChunk(position).QuickSetVoxel(GlobalToLocalVoxel(position), type);
        }
};