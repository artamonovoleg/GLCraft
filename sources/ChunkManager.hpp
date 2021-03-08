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
        ChunkManager()
        {
            m_Chunks.emplace(std::make_pair(VoxelPosition{ 0, 0, 0 }, Chunk(*this, { 0, 0, 0 })));
        }

        Chunk* begin() { return &m_Chunks.begin()->second; }
        Chunk* end() { return &m_Chunks.end()->second; }

        const Chunk& GetChunk(const VoxelPosition& position)
        {
            auto chunkPos = ToChunkPosition(position);
            if (m_Chunks.find(chunkPos) == m_Chunks.end())
            {
                static Chunk error(*this, { 0, 0, 0 });
                return error;
            }
            return m_Chunks.at(chunkPos);
        }

        Chunk& AddChunk(const VoxelPosition& position)
        {
            auto it = m_Chunks.find(position);
            if (it != m_Chunks.end())
                return m_Chunks.emplace(std::make_pair(position, Chunk(*this, position))).first->second;
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