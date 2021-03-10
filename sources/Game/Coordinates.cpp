#include "Coordinates.hpp"
#include "Constants.hpp"

VoxelPosition ToChunkPosition(const VoxelPosition& position)
{
    return {(position.x < 0) ? (position.x - ChunkSize) / ChunkSize : position.x / ChunkSize,
            (position.y < 0) ? (position.y - ChunkSize) / ChunkSize : position.y / ChunkSize,
            (position.z < 0) ? (position.z - ChunkSize) / ChunkSize : position.z / ChunkSize };
}

VoxelPosition GlobalVoxelToLocal(const VoxelPosition& position)
{
    return {(ChunkSize + (position.x % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.y % ChunkSize)) % ChunkSize,
            (ChunkSize + (position.z % ChunkSize)) % ChunkSize };
}

VoxelPosition LocalVoxelToGlobal(const VoxelPosition& chunkPosition, const VoxelPosition& voxelPosition)
{
    return {chunkPosition.x * ChunkSize + voxelPosition.x,
            chunkPosition.y * ChunkSize + voxelPosition.y,
            chunkPosition.z * ChunkSize + voxelPosition.z };
}