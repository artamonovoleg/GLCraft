#include "Coordinates.hpp"
#include "Constants.hpp"

VoxelPosition ToVoxelPosition(const glm::vec3& position)
{
    auto x = position.x;
    auto y = position.y;
    auto z = position.z;
    return { static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)), static_cast<int>(std::floor(z)) };
}

ChunkPosition ToChunkPosition(const VoxelPosition& position)
{
    int x = position.x;
    int y = position.y;
    int z = position.z;
    return {
        x < 0 ? ((++x - ChunkSize) / ChunkSize) : (x / ChunkSize),
        y < 0 ? ((++y - ChunkSize) / ChunkSize) : (y / ChunkSize),
        z < 0 ? ((++z - ChunkSize) / ChunkSize) : (z / ChunkSize),
    };
}

VoxelPosition GlobalVoxelToLocal(const VoxelPosition& position)
{
    return {(ChunkSize + (position.x % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.y % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.z % ChunkSize)) % ChunkSize };
}

VoxelPosition LocalVoxelToGlobal(const ChunkPosition& chunkPosition, const VoxelPosition& voxelPosition)
{
    return {chunkPosition.x * ChunkSize + voxelPosition.x,
            chunkPosition.y * ChunkSize + voxelPosition.y,
            chunkPosition.z * ChunkSize + voxelPosition.z };
}