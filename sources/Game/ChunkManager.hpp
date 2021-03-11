#pragma once

#include <unordered_map>
#include "Coordinates.hpp"
#include "Voxel.hpp"
#include "Chunk.hpp"

class Camera;

using ChunkMap = std::unordered_map<VoxelPosition, Chunk>;

class ChunkManager
{
    private:
        const Camera& m_Camera;

        ChunkMap m_Chunks;
    public:
        ChunkManager(const Camera& camera);

        Chunk& AddChunk(const VoxelPosition& position);
        const Chunk& GetChunk(const VoxelPosition& position) const;
        
        Voxel GetVoxel(const VoxelPosition& position) const;
        void SetVoxel(const VoxelPosition& position, Voxel voxel);

        /// Take global voxel position and check is chunk exist on position
        bool HasChunk(const VoxelPosition& position) const;

        void Process();

        const ChunkMap& GetChunkMap() const { return m_Chunks; }
};