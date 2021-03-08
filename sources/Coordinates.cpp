#include "Coordinates.hpp"
#include "Constants.hpp"

VoxelPosition GlobalToVoxel(const GlobalPosition& position)
{
    auto x = static_cast<int>(std::floor(position.x));
    auto y = static_cast<int>(std::floor(position.y));
    auto z = static_cast<int>(std::floor(position.z));
    return { x, y, z };
}

VoxelPosition LocalToGlobalVoxel(const VoxelPosition& chunkPosition, const VoxelPosition& position)
{
    return {chunkPosition.x * ChunkSize + position.x,
            chunkPosition.y * ChunkSize + position.y,
            chunkPosition.z * ChunkSize + position.z};
}

VoxelPosition GlobalToLocalVoxel(const VoxelPosition& position)
{
    return {(ChunkSize + (position.x % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.y % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.z % ChunkSize)) % ChunkSize};
}

VoxelPosition ToChunkPosition(const VoxelPosition& position)
{
    int x = position.x;
    int y = position.y;
    int z = position.z;
    return {
        x < 0 ? ((x - ChunkSize) / ChunkSize) : (x / ChunkSize),
        y < 0 ? ((y - ChunkSize) / ChunkSize) : (y / ChunkSize),
        z < 0 ? ((z - ChunkSize) / ChunkSize) : (z / ChunkSize),
    };
}