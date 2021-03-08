#include "Coordinates.hpp"
#include "Constants.hpp"

glm::ivec3 LocalToGlobalVoxel(const VoxelPosition& chunkPosition, const VoxelPosition& position)
{
    return {chunkPosition.x * ChunkSize + position.x,
            chunkPosition.y * ChunkSize + position.y,
            chunkPosition.z * ChunkSize + position.z};
}

glm::ivec3 GlobalToLocalVoxel(const VoxelPosition& position)
{
    return {(ChunkSize + (position.x % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.y % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.z % ChunkSize)) % ChunkSize};
}

glm::ivec3 ToChunkPosition(const VoxelPosition& position)
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