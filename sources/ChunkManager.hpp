#pragma once

#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "Chunk.hpp"

using ChunkMap = std::unordered_map<VoxelPosition, Chunk>;

class Camera;

class ChunkManager
{
    private:
        ChunkMap m_ChunkMap;
    public:
        ChunkManager(const Camera& camera);

        ChunkMap& GetChunkMap() { return m_ChunkMap;}

        const Chunk& GetChunk(const VoxelPosition& position);

        Chunk& AddChunk(const VoxelPosition& position);
        Voxel GetVoxel(const VoxelPosition& position);
        void SetVoxel(const VoxelPosition& position, VoxelType type);
};