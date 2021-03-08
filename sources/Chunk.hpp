#pragma once

#include <array>
#include <memory>
#include <vector>
#include "Constants.hpp"
#include "Coordinates.hpp"
#include "Voxel.hpp"
#include "Vertex.hpp"

class ChunkManager;

class VertexArray;
class VertexBuffer;
class IndexBuffer;

class Chunk
{
    private:
        friend class ChunkManager;
        
        ChunkManager& m_ChunkManager;

        std::array<Voxel, ChunkVolume> m_Data;

        VoxelPosition m_Position;

        mutable bool m_OutDate = false;
    public:
        Chunk(ChunkManager& chunkManager, const VoxelPosition& position);
        const VoxelPosition& GetPosition() const { return m_Position; }

        bool PositionInBounds(const VoxelPosition& position) const;

        /// Unsafe function without check is position in bounds. Returns voxel by local chunk coords
        Voxel QuickGetVoxel(const VoxelPosition& position) const;

        /// Safe function. If position not in bounds return from neighbour chunk
        Voxel GetVoxel(const VoxelPosition& position) const;

        /// Unsafe function without check is position in bounds. Set voxel by local chunk coords
        void QuickSetVoxel(const VoxelPosition& position, VoxelType type);

        /// Safe function. If position not in bounds return from neighbour chunk
        void SetVoxel(const VoxelPosition& position, VoxelType type);

        bool Outdate() const { return m_OutDate; }
        void Updated() const { m_OutDate = false; }
};