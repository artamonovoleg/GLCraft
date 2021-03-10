#pragma once

#include <array>
#include "Constants.hpp"
#include "Coordinates.hpp"
#include "Voxel.hpp"

class Chunk
{
    private:
        VoxelPosition m_Position;
        std::array<Voxel, ChunkVolume> m_Data;

        static size_t PositionToIndex(const VoxelPosition& position);
    public:
        Chunk(const VoxelPosition& position);

        const VoxelPosition& GetPosition() const;

        Voxel QGetVoxel(const VoxelPosition& position) const;
        void QSetVoxel(const VoxelPosition& position, Voxel voxel);
};