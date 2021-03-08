#pragma once

#include <array>
#include "Constants.hpp"
#include "Coordinates.hpp"
#include "Voxel.hpp"

class ChunkManager;

class Chunk
{
    private:
        const ChunkManager& m_ChunkManager;

        std::array<Voxel, ChunkVolume> m_Data;

        VoxelPosition m_Position;
    public:
        Chunk(const ChunkManager& chunkManager)
            : m_ChunkManager(chunkManager) {}

        bool PositionInBounds(const VoxelPosition& position) const;

        /// Unsafe function without check is position in bounds. Returns voxel by local chunk coords
        const Voxel& QuickGetVoxel(const VoxelPosition& position) const;

        /// Safe function. If chunk not in bounds return from neighbour chunk
        const Voxel& GetVoxel(const VoxelPosition& position) const;
};