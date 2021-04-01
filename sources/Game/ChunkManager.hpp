#pragma once

#include <unordered_map>
#include "Coordinates.hpp"
#include "Voxel.hpp"
#include "Chunk.hpp"

class Camera;

using ChunkMap = std::unordered_map<ChunkPosition, Chunk>;

class ChunkManager
{
    private:
        const Camera& m_Camera;

        ChunkMap m_Chunks;
    public:
        ChunkManager(const Camera& camera);

        Chunk& AddChunk(const ChunkPosition& position);
        const Chunk& GetChunk(const ChunkPosition& position) const;
        
        Voxel GetVoxel(const VoxelPosition& position) const;
        void SetVoxel(const VoxelPosition& position, Voxel voxel);

        /// Take global voxel position and check is chunk exist on position
        bool HasChunk(const ChunkPosition& position) const;

        const ChunkMap& GetChunkMap() const { return m_Chunks; }

        bool InRenderDistance(const ChunkPosition& position) const;
        
        void OnUpdate();
};