#pragma once

#include <unordered_map>
#include "Coordinates.hpp"
#include "Voxel.hpp"

class Camera;
class Chunk;

using ChunkMap = std::unordered_map<VoxelPosition, Chunk>;

class ChunkManager
{
    private:
        const Camera& m_Camera;

        ChunkMap m_Chunks;
    public:
        ChunkManager(const Camera& camera);

        const ChunkMap& GetChunkMap() const { return m_Chunks; }
};