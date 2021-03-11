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
        VoxelPosition m_Position;
        std::array<Voxel, ChunkVolume> m_Data;

        mutable bool m_WasModified = false;

        static size_t PositionToIndex(const VoxelPosition& position);
        static bool PositionInBounds(const VoxelPosition& position);
    public:
        Chunk(const ChunkManager& chunkManager, const VoxelPosition& position);

        bool Modified() const
        { 
            auto res = m_WasModified;
            m_WasModified = false;
            return res; 
        }

        const VoxelPosition& GetPosition() const;

        Voxel QGetVoxel(const VoxelPosition& position) const;
        void QSetVoxel(const VoxelPosition& position, Voxel voxel);

        Voxel GetVoxel(const VoxelPosition& position) const;
};